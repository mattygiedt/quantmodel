#include <iostream>
#include <string>
#include <list>
#include <map>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

void initLogging( XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

void initNetwork( const XMLNode& config, zmq::context_t& ctx, std::map< const std::string , zmq::socket_t* >& socket_map, log4cpp::Category* log )
{
    zmq::socket_t *s = NULL;
    const XMLNode network_node = config.getChildNode( "network" );

    if( network_node.isAttributeSet( "dealing_callforward" ) )
    {
        log->info( " connecting to dealing callforward: %s",
            network_node.getAttribute( "dealing_callforward" ) );
        s = new zmq::socket_t( ctx, ZMQ_REQ );
        s->connect( network_node.getAttribute( "dealing_callforward" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "dealing_callforward", s ) );
    }
    else
    {
        log->fatal( "dealing_callforward not set." );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "database_callforward" ) )
    {
        log->info( " connecting to database callforward: %s",
            network_node.getAttribute( "database_callforward" ) );
        s = new zmq::socket_t( ctx, ZMQ_REQ );
        s->connect( network_node.getAttribute( "database_callforward" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "database_callforward", s ) );
    }
    else
    {
        log->fatal( "database_callforward not set." );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "internal_callforward" ) )
    {
        log->info( " connecting internal callforward: %s",
            network_node.getAttribute( "internal_callforward" ) );
        s = new zmq::socket_t( ctx, ZMQ_REP );
        s->connect( network_node.getAttribute( "internal_callforward" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "internal_callforward", s ) );
    }
    else
    {
        log->fatal( "internal_callforward not set." );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "internal_broadcast" ) )
    {
        log->info( " connecting internal broadcast: %s",
            network_node.getAttribute( "internal_broadcast" ) );
        s = new zmq::socket_t( ctx, ZMQ_PUSH );
        s->connect( network_node.getAttribute( "internal_broadcast" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "internal_broadcast", s ) );
    }
    else
    {
        log->error( "internal_broadcast address not set" );
        exit( 1 );
    }

    //
    //  Connect to the dealing / market data broadcasts
    //

    if( network_node.isAttributeSet( "dealing_broadcast" ) )
    {
        log->info( " connecting to dealing_broadcast: %s",
            network_node.getAttribute( "dealing_broadcast" ) );
        s = new zmq::socket_t( ctx, ZMQ_SUB );
        s->setsockopt( ZMQ_SUBSCRIBE, "", 0 );
        s->connect( network_node.getAttribute( "dealing_broadcast" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "dealing_broadcast", s ) );
    }
    else
    {
        log->error( " dealing_broadcast network address not set" );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "marketdata_broadcast" ) )
    {
        log->info( " connecting to marketdata_broadcast: %s",
            network_node.getAttribute( "marketdata_broadcast" ) );
        s = new zmq::socket_t( ctx, ZMQ_SUB );
        s->setsockopt( ZMQ_SUBSCRIBE, "", 0 );
        s->connect( network_node.getAttribute( "marketdata_broadcast" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "marketdata_broadcast", s ) );
    }
    else
    {
        log->error( " marketdata_broadcast network address not set" );
        exit( 1 );
    }
}

int main( int argc, char *argv[] )
{
    int rc;
    std::string msg_data;
    log4cpp::Category* log;
    zmq::context_t ctx( 1 );

    std::map< const std::string , zmq::socket_t* > socket_map;

    quantmodel::ClientAdapterRequest request_msg;
    quantmodel::ClientAdapterResponse response_msg;
    quantmodel::HeartbeatData heartbeat_data;
    quantmodel::DealingEngineBroadcast dealing_broadcast;
    quantmodel::MarketDataBroadcast marketdata_broadcast;
    quantmodel::ClientAdapterBroadcast broadcast_msg;

    if( argc == 1 )
    {
        cerr << " usage: ClientAdapter [config.xml]" << endl;
        exit( 1 );
    }

    //
    //  Load the configuration file
    //

    cout << "Loading configuration file: " << argv[ 1 ] << endl;
    XMLNode config = XMLNode::openFileHelper( argv[ 1 ] );
    //XMLNode config = XMLNode::openFileHelper( "../config/clientAdapter.xml" );

    //
    //  Setup log4cpp
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "ClientAdapter" );

    //
    //  Create the internal (ZMQ) network connections
    //

    initNetwork( config, ctx, socket_map, log );

    //
    //  Create the request handler
    //

    client_adapter::RequestHandler handler ( socket_map );
    handler.initLogging();

    //
    //  Setup the poll item array
    //

    zmq::pollitem_t items [ ] =
    {
        { (*socket_map[ "internal_callforward" ]), 0, ZMQ_POLLIN, 0 },
        { (*socket_map[ "dealing_broadcast" ]), 0, ZMQ_POLLIN, 0 },
        { (*socket_map[ "marketdata_broadcast" ]), 0, ZMQ_POLLIN, 0 }
    };

    while( ( rc = zmq::poll( items, 3, -1 ) ) > 0 )
    {
        if( items[ 0 ].revents & ZMQ_POLLIN )
        {
            msg_data.clear();
            if(! quantmodel::ZMQMessage::recv( msg_data, socket_map[ "internal_callforward" ] ) )
            {
                log->fatal( "UNABLE TO RECV ON internal_callforward SOCKET!!!" );
                exit( 1 );
            }

            //
            //  Try to parse the request into a protobuf
            //

            request_msg.Clear();
            response_msg.Clear();

            if(! request_msg.ParseFromString( msg_data ) )
            {
                log->error( "Received %ld bytes of invalid data from socket_proxy.", msg_data.size() );

                //
                //  Could not create a request message from the msg_data.
                //

                response_msg.set_request_id( "INVALID" );
                response_msg.set_session_id( "INVALID" );
                response_msg.set_error_msg( "INVALID REQUEST_MSG" );
                response_msg.set_type( quantmodel::ClientAdapterResponse::ERROR );
                quantmodel::TimeStamp::getMessageId( *response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( *response_msg.mutable_timestamp( ) );

                if(! quantmodel::ZMQMessage::send( &response_msg, socket_map[ "internal_callforward" ] ) )
                {
                    log->fatal( "UNABLE TO SEND ON internal_callforward SOCKET!!!" );
                    exit( 1 );
                }

                continue;
            }

            //
            //  Log all incoming callforward requests
            //

            log->info( " CALLFORWARD --> " + request_msg.ShortDebugString() );

            //
            //  Service the request based on the request type
            //

            switch( request_msg.type() )
            {
                case quantmodel::ClientAdapterRequest::HEARTBEAT:

                    handler.heartbeat( request_msg, response_msg );
                    break;

                case quantmodel::ClientAdapterRequest::DATABASE:

                    handler.databaseRequest( request_msg, response_msg );
                    break;

                case quantmodel::ClientAdapterRequest::DEALING:

                    handler.dealingRequest( request_msg, response_msg );
                    break;

                case quantmodel::ClientAdapterRequest::SHUTDOWN:

                    log->warn( "Initiating ClientAdapter Shutdown" );
                    response_msg.set_request_id( request_msg.request_id() );
                    response_msg.set_session_id( request_msg.session_id() );
                    quantmodel::TimeStamp::getMessageId( *response_msg.mutable_response_id( ) );
                    quantmodel::TimeStamp::getUTCTime( *response_msg.mutable_timestamp( ) );
                    quantmodel::ZMQMessage::send( &response_msg, socket_map[ "internal_callforward" ] );
                    sleep( 1 );
                    exit( 0 );
            }

            //
            //  Send the response back to the client
            //

            response_msg.set_request_id( request_msg.request_id() );
            response_msg.set_session_id( request_msg.session_id() );
            quantmodel::TimeStamp::getMessageId( *response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( *response_msg.mutable_timestamp( ) );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " <-- " + response_msg.ShortDebugString() );
            }

            if(! quantmodel::ZMQMessage::send( &response_msg, socket_map[ "internal_callforward" ] ) )
            {
                log->fatal( "UNABLE TO SEND ON internal_callforward SOCKET!!!" );
                exit( 1 );
            }
        }
        else if( items[ 1 ].revents & ZMQ_POLLIN )
        {
            broadcast_msg.Clear();
            dealing_broadcast.Clear();
            if(! quantmodel::ZMQMessage::recv( &dealing_broadcast, socket_map[ "dealing_broadcast" ] ) )
            {
                log->fatal( "UNABLE TO RECV ON dealing SOCKET!!!" );
                exit( 1 );
            }

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " --> " + dealing_broadcast.ShortDebugString() );
            }

            //
            //  Copy the dealing broadcast to the client adapter broadcast_msg
            //

            broadcast_msg.set_type( quantmodel::ClientAdapterBroadcast::DEALING );
            broadcast_msg.mutable_dealing_broadcast()->CopyFrom( dealing_broadcast );

            //
            //  Set the message id and send
            //

            quantmodel::TimeStamp::getMessageId( *broadcast_msg.mutable_broadcast_id( ) );
            quantmodel::TimeStamp::getUTCTime( *broadcast_msg.mutable_timestamp( ) );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " <-- " + broadcast_msg.ShortDebugString() );
            }

            if(! quantmodel::ZMQMessage::send( &broadcast_msg, socket_map[ "internal_broadcast" ] ) )
            {
                log->fatal( "UNABLE TO SEND ON broadcast SOCKET!!!" );
                exit( 1 );
            }
        }
        else if( items[ 2 ].revents & ZMQ_POLLIN )
        {
            broadcast_msg.Clear();
            marketdata_broadcast.Clear();
            if(! quantmodel::ZMQMessage::recv( &marketdata_broadcast, socket_map[ "marketdata_broadcast" ] ) )
            {
                log->fatal( "UNABLE TO RECV ON marketdata SOCKET!!!" );
                exit( 1 );
            }

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " --> " + marketdata_broadcast.ShortDebugString() );
            }

            //
            //  Copy the marketdata broadcast to the client adapter broadcast_msg
            //

            broadcast_msg.set_type( quantmodel::ClientAdapterBroadcast::MARKETDATA );
            broadcast_msg.mutable_marketdata_broadcast()->CopyFrom( marketdata_broadcast );

            //
            //  Set the message id and send
            //

            quantmodel::TimeStamp::getMessageId( *broadcast_msg.mutable_broadcast_id( ) );
            quantmodel::TimeStamp::getUTCTime( *broadcast_msg.mutable_timestamp( ) );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " <-- " + broadcast_msg.ShortDebugString() );
            }

            if(! quantmodel::ZMQMessage::send( &broadcast_msg, socket_map[ "internal_broadcast" ] ) )
            {
                log->fatal( "UNABLE TO SEND ON broadcast SOCKET!!!" );
                exit( 1 );
            }
        }
    }

    log->fatal( "ZMQ POLL returns: %d", rc );
    exit( 1 );
}
