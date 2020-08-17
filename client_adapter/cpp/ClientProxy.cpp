#include <iostream>
#include <string>
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

    //
    //  The sockets used by external clients
    //

    if( network_node.isAttributeSet( "external_callforward" ) )
    {
        log->info( " binding external callforward: %s", network_node.getAttribute( "external_callforward" ) );
        s = new zmq::socket_t( ctx, ZMQ_XREP );
        s->bind( network_node.getAttribute( "external_callforward" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "external_callforward", s ) );
    }
    else
    {
        log->fatal( " INVALID CONFIGURATION: no external_callforward attribute." );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "external_broadcast" ) )
    {
        log->info( " binding external broadcast: %s", network_node.getAttribute( "external_broadcast" ) );
        s = new zmq::socket_t( ctx, ZMQ_PUB );
        s->bind( network_node.getAttribute( "external_broadcast" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "external_broadcast", s ) );
    }
    else
    {
        log->fatal( " INVALID CONFIGURATION: no external_broadcast attribute." );
        exit( 1 );
    }

    //
    //  The sockets used to connect with the (internal) ClientAdapter
    //

    if( network_node.isAttributeSet( "internal_callforward" ) )
    {
        log->info( " binding internal callforward: %s", network_node.getAttribute( "internal_callforward" ) );
        s = new zmq::socket_t( ctx, ZMQ_XREQ );
        s->bind( network_node.getAttribute( "internal_callforward" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "internal_callforward", s ) );
    }
    else
    {
        log->fatal( " INVALID CONFIGURATION: no internal_callforward attribute." );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "internal_broadcast" ) )
    {
        log->info( " binding internal broadcast: %s", network_node.getAttribute( "internal_broadcast" ) );
        s = new zmq::socket_t( ctx, ZMQ_PULL );
        //s = new zmq::socket_t( ctx, ZMQ_PAIR );
        s->bind( network_node.getAttribute( "internal_broadcast" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "internal_broadcast", s ) );
    }
    else
    {
        log->fatal( " INVALID CONFIGURATION: no internal_broadcast attribute." );
        exit( 1 );
    }
}

int main( int argc, char *argv[] )
{
    int rc;
    int64_t more;
    size_t more_size;
    time_t nexttime, currtime;
    log4cpp::Category* log;
    std::string msg_data, session_id;
    zmq::context_t ctx( 1 );

    quantmodel::ClientAdapterRequest request_msg;
    quantmodel::ClientAdapterResponse response_msg;

    std::map< const std::string , zmq::socket_t* > socket_map;

    //if( argc == 1 )
    //{
    //    cerr << " usage: ClientAdapter [config.xml]" << endl;
    //    exit( 1 );
    //}
    //
    ////
    ////  Load the configuration file
    ////
    //

    cout << "Loading configuration file: " << argv[ 1 ] << endl;
    XMLNode config = XMLNode::openFileHelper( argv[ 1 ] );
    //XMLNode config = XMLNode::openFileHelper( "../config/clientProxy.xml" );

    //
    //  Setup log4cpp
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "ClientProxy" );

    //
    //  Create the session_id and internal (ZMQ) network connections
    //

    session_id = "HEARTBEAT";
    initNetwork( config, ctx, socket_map, log );

    //
    //  Initialize the heartbeat variable (seconds), send one immediately
    //

    nexttime = 0;
    const int heartbeat = atoi( config.getAttribute( "heartbeat" ) );

    //
    //  Setup the poll item array
    //

    more_size = sizeof( int64_t );
    zmq::pollitem_t items [ ] =
    {
        { *socket_map[ "external_callforward" ], 0, ZMQ_POLLIN, 0 },
        { *socket_map[ "internal_broadcast" ], 0, ZMQ_POLLIN, 0 }
    };

    //
    //  Poll on internal / external sockets forever
    //

    log->info( "Client proxy initialized, waiting for requests." );

    while( 1 )
    {
        rc = zmq::poll( items, 2, 5 * 1000 * 1000 ); // Max delay is 5 seconds

        if( rc == -1 )
        {
            log->fatal( "Unable to poll rc = %d", rc );
            exit( 1 );
        }

        if( rc > 0 )
        {
            if( items[ 0 ].revents == ZMQ_POLLIN )
            {
                while( 1 )
                {
                    zmq::message_t message;
                    socket_map[ "external_callforward" ]->recv( &message );
                    socket_map[ "external_callforward" ]->getsockopt( ZMQ_RCVMORE, &more, &more_size );
                    socket_map[ "internal_callforward" ]->send( message, more ? ZMQ_SNDMORE : 0 );
                    if( !more ) { break; }
                }

                while( 1 )
                {
                    zmq::message_t message;
                    socket_map[ "internal_callforward" ]->recv( &message );
                    socket_map[ "internal_callforward" ]->getsockopt( ZMQ_RCVMORE, &more, &more_size );
                    socket_map[ "external_callforward" ]->send( message, more ? ZMQ_SNDMORE : 0 );
                    if( !more ) { break; }
                }
            }
            else if( items[ 1 ].revents == ZMQ_POLLIN )
            {
                zmq::message_t message;
                socket_map[ "internal_broadcast" ]->recv( &message );
                socket_map[ "external_broadcast" ]->send( message );
            }
        }

        //
        //  Test for heartbeat
        //

        time( &currtime );

        if( currtime >= nexttime )
        {
            nexttime = currtime + heartbeat;

            //
            //  Send a heartbeat client request to the client adapter
            //

            msg_data.clear();
            request_msg.Clear();
            response_msg.Clear();

            quantmodel::TimeStamp::getMessageId( *request_msg.mutable_request_id( ) );
            quantmodel::TimeStamp::getUTCTime( *request_msg.mutable_timestamp( ) );
            request_msg.set_session_id( session_id );
            request_msg.set_type( quantmodel::ClientAdapterRequest::HEARTBEAT );

            request_msg.SerializeToString( &msg_data );

            //
            //  We need to send a multi-part message because we're XREP
            //

            zmq::message_t header;
            zmq::message_t heartbeat ( msg_data.size() );
            memcpy( heartbeat.data(), msg_data.data(), msg_data.size() );
            socket_map[ "internal_callforward" ]->send( header, ZMQ_SNDMORE );
            socket_map[ "internal_callforward" ]->send( heartbeat, 0 );

            //
            //  Receive the multi-part response
            //

            while( 1 )
            {
                zmq::message_t message;
                socket_map[ "internal_callforward" ]->recv( &message );
                socket_map[ "internal_callforward" ]->getsockopt( ZMQ_RCVMORE, &more, &more_size );
                if( !more ) { break; }
            }
        }
    }

    exit( 0 );
}
