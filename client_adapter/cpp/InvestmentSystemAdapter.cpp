#include <iostream>
#include <string>
#include <list>
#include <map>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <quantmodel.hpp>

void initLogging( XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

void initNetwork( const XMLNode& config, zmq::context_t& ctx, std::map< const std::string , zmq::socket_t* >& socket_map, log4cpp::Category* log )
{
    zmq::socket_t *s = NULL;
    const XMLNode network_node = config.getChildNode( "network" );

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

    if( network_node.isAttributeSet( "invsys_callforward" ) )
    {
        log->info( " connecting invsys callforward: %s",
            network_node.getAttribute( "invsys_callforward" ) );
        s = new zmq::socket_t( ctx, ZMQ_REQ );
        s->connect( network_node.getAttribute( "invsys_callforward" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "invsys_callforward", s ) );
    }
    else
    {
        log->fatal( "invsys_callforward not set." );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "invsys_broadcast" ) )
    {
        log->info( " connecting invsys broadcast: %s",
            network_node.getAttribute( "invsys_broadcast" ) );
        s = new zmq::socket_t( ctx, ZMQ_PULL );
        s->connect( network_node.getAttribute( "invsys_broadcast" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "invsys_broadcast", s ) );
    }
    else
    {
        log->error( "invsys_broadcast address not set" );
        exit( 1 );
    }
}

int main( int argc, char *argv[] )
{
    int rc;
    log4cpp::Category* log;
    zmq::context_t ctx( 1 );

    std::map< const std::string , zmq::socket_t* > socket_map;

    if( argc == 1 )
    {
        cerr << " usage: InvestmentSystemAdapter [config.xml]" << endl;
        exit( 1 );
    }

    //
    //  Load the configuration file
    //

    cout << "Loading configuration file: " << argv[ 1 ] << endl;
    XMLNode config = XMLNode::openFileHelper( argv[ 1 ] );
    //XMLNode config = XMLNode::openFileHelper( "../config/investmentSystemAdapter.xml" );

    //
    //  Setup log4cpp
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "InvestmentSystemAdapter" );

    //
    //  Create the internal (ZMQ) network connections
    //

    initNetwork( config, ctx, socket_map, log );

    //
    //  Setup the poll item array
    //

    zmq::pollitem_t items [ ] =
    {
        { (*socket_map[ "internal_callforward" ]), 0, ZMQ_POLLIN, 0 },
        { (*socket_map[ "invsys_broadcast" ]), 0, ZMQ_POLLIN, 0 }
    };

    while( ( rc = zmq::poll( items, 2, -1 ) ) > 0 )
    {
        if( items[ 0 ].revents & ZMQ_POLLIN )
        {
            //
            //  Received internal callforward.
            //

            quantmodel::InvestmentSystemRequest request_msg;
            quantmodel::InvestmentSystemResponse response_msg;

            //
            //  Receive message
            //

            if(! quantmodel::ZMQMessage::recv( &request_msg, socket_map[ "internal_callforward" ] ) )
            {
                log->fatal( "UNABLE TO RECV ON internal_callforward SOCKET!!!" );
                exit( 1 );
            }

            if( request_msg.type() == quantmodel::InvestmentSystemRequest::COMMAND )
            {
                log->info( "RECV COMMAND:" );

                //
                //  Send RPC request to investment system
                //

                quantmodel::ZMQMessage::send( &request_msg, socket_map[ "invsys_callforward" ] );

                //
                //  Recv RPC response from investment system
                //

                zmq::pollitem_t rpc_poll [ ] = { { (*socket_map[ "invsys_callforward" ]), 0, ZMQ_POLLIN, 0 } };
                rc = zmq::poll( rpc_poll, 1, 2 * 1000 * 1000 );

                if( rc > 0 )
                {
                    quantmodel::ZMQMessage::recv( &response_msg, socket_map[ "invsys_callforward" ] );
                }
                else
                {
                    log->warn( "RPC TIMEOUT TO invsys_callforward" );
                    response_msg.set_request_id( request_msg.request_id() );
                    quantmodel::TimeStamp::getMessageId( *response_msg.mutable_response_id( ) );
                    quantmodel::TimeStamp::getUTCTime( *response_msg.mutable_timestamp( ) );
                    response_msg.set_type( quantmodel::InvestmentSystemResponse::ERROR );

                    //
                    //  We now need to reset the socket
                    //

                    socket_map[ "invsys_callforward" ]->close();
                    delete socket_map[ "invsys_callforward" ];

                    zmq::socket_t *s = NULL;
                    XMLNode network_node = config.getChildNode( "network" );

                    log->info( " re-connecting invsys callforward: %s",
                        network_node.getAttribute( "invsys_callforward" ) );
                    s = new zmq::socket_t( ctx, ZMQ_REQ );
                    s->connect( network_node.getAttribute( "invsys_callforward" ) );
                    socket_map.insert( std::pair< const std::string ,
                        zmq::socket_t* >( "invsys_callforward", s ) );
                }
            }
            else if( request_msg.type() == quantmodel::InvestmentSystemRequest::HEARTBEAT )
            {
                log->info( "HEARTBEAT: %s", request_msg.ShortDebugString().c_str() );

                //
                //  Reply to heartbeat
                //

                response_msg.set_request_id( request_msg.request_id() );
                quantmodel::TimeStamp::getMessageId( *response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( *response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::InvestmentSystemResponse::ACKNOWLEDGEMENT );
            }
            else
            {
                log->info( "UNKNOWN: %s", request_msg.ShortDebugString().c_str() );

                //
                //  Unknown investment system request type
                //

                response_msg.set_request_id( request_msg.request_id() );
                quantmodel::TimeStamp::getMessageId( *response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( *response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::InvestmentSystemResponse::ERROR );
            }

            //
            //  Send RPC response back to proxy
            //

            quantmodel::ZMQMessage::send( &response_msg, socket_map[ "internal_callforward" ] );
        }
        else if( items[ 1 ].revents & ZMQ_POLLIN )
        {
            //
            //  Received internal broadcast.
            //

            zmq::message_t broadcast;

            //
            //  Send the broadcast to the proxy.
            //

            socket_map[ "invsys_broadcast" ]->recv( &broadcast );
            socket_map[ "internal_broadcast" ]->send( broadcast );
        }
    }

    log->fatal( "ZMQ POLL returns: %d", rc );
    exit( 1 );
}
