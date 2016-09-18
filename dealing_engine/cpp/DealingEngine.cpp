#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <zmq.hpp>

#include <gmpxx.h>

#include <quantmodel.hpp>

#include "ActiveSet.hpp"
#include "DealingEngineParameters.hpp"
#include "RequestHandler.hpp"

void initLogging( XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

void initNetwork( const XMLNode& config, zmq::context_t& ctx, std::map< const std::string , zmq::socket_t* >& socket_map, log4cpp::Category* log )
{
    zmq::socket_t* s = NULL;
    const XMLNode network_node = config.getChildNode( "network" );

    if( network_node.isAttributeSet( "client_request" ) )
    {
        log->info( " client_request network address: %s", network_node.getAttribute( "client_request" ) );
        s = new zmq::socket_t( ctx, ZMQ_REP );
        s->bind( network_node.getAttribute( "client_request" ) );
        socket_map.insert( pair< const std::string , zmq::socket_t* >( "client_request", s ) );
    }
    else
    {
        log->fatal( " client_request network address NOT SET." );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "fix_request" ) )
    {
        log->info( " fix_request network address: %s", network_node.getAttribute( "fix_request" ) );
        s = new zmq::socket_t( ctx, ZMQ_REP );
        s->bind( network_node.getAttribute( "fix_request" ) );
        socket_map.insert( pair< const std::string , zmq::socket_t* >( "fix_request", s ) );
    }
    else
    {
        log->fatal( " fix_request network address NOT SET." );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "dpe_callforward" ) )
    {
        log->info( " connecting to persistence engine callforward: %s", network_node.getAttribute( "dpe_callforward" ) );
        s = new zmq::socket_t( ctx, ZMQ_REQ );
        s->connect( network_node.getAttribute( "dpe_callforward" ) );
        socket_map.insert( pair< const std::string , zmq::socket_t* >( "dpe_callforward", s ) );
    }
    else
    {
        log->fatal( " dpe_callforward network address NOT SET." );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "fix_callforward" ) )
    {
        log->info( " connecting to fix engine callforward: %s", network_node.getAttribute( "fix_callforward" ) );
        s = new zmq::socket_t( ctx, ZMQ_REQ );
        s->connect( network_node.getAttribute( "fix_callforward" ) );
        socket_map.insert( pair< const std::string , zmq::socket_t* >( "fix_callforward", s ) );
    }
    else
    {
        log->warn( " fix_callforward network address NOT SET." );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "database_callforward" ) )
    {
        log->info( " connecting to database engine callforward: %s", network_node.getAttribute( "database_callforward" ) );
        s = new zmq::socket_t( ctx, ZMQ_REQ );
        s->connect( network_node.getAttribute( "database_callforward" ) );
        socket_map.insert( pair< const std::string , zmq::socket_t* >( "database_callforward", s ) );
    }
    else
    {
        log->fatal( " database_callforward network address NOT SET." );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "broadcast" ) )
    {
        log->info( " connecting to broadcast: %s", network_node.getAttribute( "broadcast" ) );
        s = new zmq::socket_t( ctx, ZMQ_PUB );
        s->bind( network_node.getAttribute( "broadcast" ) );
        socket_map.insert( pair< const std::string , zmq::socket_t* >( "broadcast", s ) );
    }
    else
    {
        log->fatal( " broadcast network address NOT SET." );
        exit( 1 );
    }
}

int main( int argc, char *argv[] )
{
    log4cpp::Category* log;
    std::string requestor;
    std::map< const std::string, zmq::socket_t* > socket_map;

    dealing::ActiveSet active_set;
    dealing::DealingEngineParameters parameters;
    dealing::RequestHandler handler( &active_set, &parameters );

    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;
    quantmodel::DealingEngineBroadcast broadcast_msg;
    quantmodel::DatabaseEngineRequest database_msg;

    //
    //  Load the XML configuration
    //

    std::cout << "Loading configuration file: " << argv[ 1 ] << std::endl;
    XMLNode config = XMLNode::openFileHelper( argv[ 1 ] );
    //XMLNode config = XMLNode::openFileHelper( "../config/dealingEngine.xml" );

    //
    //  Setup log4cpp
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "DealingEngine" );
    log->warn( "Starting QuantModel DealingEngine v1.0" );

    handler.initLogging();
    parameters.initLogging();
    active_set.initLogging();

    //
    //  Setup ZMQ
    //

    zmq::context_t ctx ( 1 );
    zmq::socket_t *request_socket = NULL;
    initNetwork( config, ctx, socket_map, log );
    handler.initNetwork( socket_map );

    //
    //  Load the dealing engine parameters
    //

    parameters.loadParameters( socket_map[ "database_callforward" ] );
    log->info( "Loaded dealing engine parameters." );

    //
    //  Load the static data using a dummy request
    //

    request_msg.set_type( quantmodel::DealingEngineRequest::RELOAD_STATIC_DATA );
    handler.reloadStaticData( request_msg, response_msg, broadcast_msg, database_msg );

    //
    //  Load the active set
    //

    active_set.loadOrders( socket_map[ "database_callforward" ] );
    active_set.loadReleases( socket_map[ "database_callforward" ] );
    active_set.loadExecutions( socket_map[ "database_callforward" ] );
    active_set.loadOvernightPositions( socket_map[ "database_callforward" ] );

    log->info( "Loaded active set." );

    //
    //  Poll on fix_request and client_request
    //

    zmq::pollitem_t items [ ] =
    {
        { (*socket_map[ "client_request" ]), 0, ZMQ_POLLIN, 0 },
        { (*socket_map[ "fix_request" ]), 0, ZMQ_POLLIN, 0 }
    };

    log->info( "DealingEngine initialized, waiting for requests." );

    while( 1 )
    {
        //
        //  Poll for next incoming message from a client or fix engine
        //

        const int rc = zmq::poll( items, 2, -1 );

        //
        //  Clear our messages
        //

        request_msg.Clear();
        response_msg.Clear();
        database_msg.Clear();
        broadcast_msg.Clear();

        if( rc == 0 )
        {
            //
            //  ERROR!!!
            //

            log->error( "zmq_poll returned 0" );
            continue;
        }
        else if( rc == -1 )
        {
            //
            //  ERROR!!!
            //

            log->fatal( "Unable to poll rc = %d", rc );
            exit( 1 );
        }
        else if( items[ 0 ].revents & ZMQ_POLLIN )
        {
            if(! quantmodel::ZMQMessage::recv( &request_msg, socket_map[ "client_request" ] ) )
            {
                log->fatal( "UNABLE TO RECV ON client_request SOCKET!!!" );
                exit( 1 );
            }

            requestor = "CLIENT REQUEST";
            request_socket = socket_map[ "client_request" ];
        }
        else if( items[ 1 ].revents & ZMQ_POLLIN )
        {
            if(! quantmodel::ZMQMessage::recv( &request_msg, socket_map[ "fix_request" ] ) )
            {
                log->fatal( "UNABLE TO RECV ON fix_request SOCKET!!!" );
                exit( 1 );
            }

            requestor = "FIX REQUEST";
            request_socket = socket_map[ "fix_request" ];
        }
        else
        {
            log->warn( "ZMQ_POLL fired invalid ZMQ_POLLIN signal." );
            continue;
        }

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " %s --> %s", requestor.c_str(), request_msg.ShortDebugString().c_str() );
        }

        //
        //  Service the request based on the request type
        //

        switch( request_msg.type() )
        {
            case quantmodel::DealingEngineRequest::EXECUTE_SIGNAL:

                handler.executeSignal( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::EXECUTE_DRAFT_ORDER:

                handler.executeDraftOrder( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::CREATE_DRAFT_ORDER:

                handler.createDraftOrder( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::CREATE_ORDER:

                handler.createOrder( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::RELEASE_ORDER:

                handler.releaseOrder( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::UPDATE_ORDER:

                handler.updateOrder( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::CANCEL_ORDER:

                handler.cancelOrder( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::CREATE_RELEASE:

                handler.createRelease( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::VALIDATE_RELEASE:

                handler.validateRelease( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::CANCEL_RELEASE:

                handler.cancelRelease( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::REPLACE_RELEASE:

                handler.replaceRelease( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::RELEASE_REJECTED:

                handler.releaseRejected( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::CANCEL_REJECTED:

                handler.cancelRejected( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::RELEASE_RESTATED:

                handler.releaseRestated( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::RELEASE_CANCELED:

                handler.releaseCanceled( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::RELEASE_REPLACED:

                handler.releaseReplaced( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::CREATE_EXECUTION:

                handler.createExecution( request_msg, response_msg, broadcast_msg, database_msg );
                break;

             case quantmodel::DealingEngineRequest::EXECUTION_CANCELED:

                handler.executionCanceled( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::ACTIVE_SET:

                handler.getActiveSet( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::BROADCAST_DATA:

                handler.broadcastActiveSet( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::QUERY_ORDER:

                handler.queryOrder( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::QUERY_RELEASE:

                handler.queryRelease( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::QUERY_EXECUTION:

                handler.queryExecution( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::ROLLOVER:

                handler.rollover( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::CURRENT_STATUS:

                handler.currentStatus( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::UPDATE_STATUS:

                handler.updateStatus( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::MARKETDATA_SUBSCRIBE:

                handler.marketDataSubscribe( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::MARKETDATA_UNSUBSCRIBE:

                handler.marketDataUnsubscribe( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::FIXENGINE_LOGIN:

                handler.fixEngineLogin( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::FIXENGINE_LOGOUT:

                handler.fixEngineLogout( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::LOGOUT_FIXENGINE:

                handler.logoutFixEngine( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::LOGIN_FIXENGINE:

                handler.loginFixEngine( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            case quantmodel::DealingEngineRequest::RELOAD_STATIC_DATA:

                handler.reloadStaticData( request_msg, response_msg, broadcast_msg, database_msg );
                break;

            default:

                log->error( "UNKNOWN DEALING ENGINE REQUEST: " + request_msg.ShortDebugString() );
                break;
        }

        //
        //  Create the audit event(s)
        //

        handler.createAuditEvent( request_msg, response_msg, database_msg );

        //
        //  Send the RPC response
        //

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " <-- " + response_msg.ShortDebugString() );
        }

        quantmodel::ZMQMessage::send( &response_msg, request_socket );

        //
        //  Send the broadcast
        //

        if( broadcast_msg.has_broadcast_id() )
        {
            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " <-- " + broadcast_msg.ShortDebugString() );
            }

            quantmodel::ZMQMessage::send( &broadcast_msg, socket_map[ "broadcast" ] );
        }

        //
        //  Send the persistence engine request
        //

        if( database_msg.query_size() > 0 )
        {
            quantmodel::TimeStamp::getMessageId( database_msg.mutable_request_id( ) );
            quantmodel::TimeStamp::getUTCTime( database_msg.mutable_timestamp( ) );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " <-- " + database_msg.ShortDebugString() );
            }

            quantmodel::ZMQMessage::sendRPC( &database_msg, socket_map[ "dpe_callforward" ] );
        }
    }

    exit( 0 );
}

