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

#include <quantmodel.hpp>

#include "FIXApplication.hpp"
#include "GlobexOrder.hpp"
#include "TradingTechnologiesOrder.hpp"
#include "TradingTechnologiesPrice.hpp"

static void* shutdown( void* args )
{
    while( true )
    {
        sleep( 60 );

        const time_t now = time( NULL );
        struct tm *timeinfo = gmtime( &now );
        if( unlikely( timeinfo->tm_wday == 6 ) )
        {
            exit( 0 );
        }
    }
}

void initLogging( const XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

void initNetwork( const XMLNode& config, zmq::context_t& ctx, std::map< const std::string , zmq::socket_t* >& socket_map, log4cpp::Category* log )
{
    zmq::socket_t* s = NULL;
    const XMLNode network_node = config.getChildNode( "network" );

    if( network_node.isAttributeSet( "callforward" ) )
    {
        log->info( " connecting to callforward: %s", network_node.getAttribute( "callforward" ) );
        s = new zmq::socket_t( ctx, ZMQ_REP );
        s->bind( network_node.getAttribute( "callforward" ) );
        socket_map.insert( pair< const std::string , zmq::socket_t* >( "callforward", s ) );
    }
    else
    {
        log->fatal( " callforward network address NOT SET." );
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

    if( network_node.isAttributeSet( "persistence_callforward" ) )
    {
        log->info( " connecting to persistence engine callforward: %s", network_node.getAttribute( "persistence_callforward" ) );
        s = new zmq::socket_t( ctx, ZMQ_REQ );
        s->connect( network_node.getAttribute( "persistence_callforward" ) );
        socket_map.insert( pair< const std::string , zmq::socket_t* >( "persistence_callforward", s ) );
    }
    else
    {
        log->fatal( " persistence_callforward network address NOT SET." );
        exit( 1 );
    }
}

int main( int argc, char *argv[] )
{
    log4cpp::Category* log;
    fix_engine::FIXApplication *application = NULL;
    std::map< const std::string, zmq::socket_t* > socket_map;
    std::map< const std::string, fix_engine::FIXApplication* > fix_application_map;
    std::map< const std::string, fix_engine::FIXApplication* >::const_iterator fix_application_iter;

    quantmodel::FixEngineRequest request_msg;
    quantmodel::FixEngineResponse response_msg;
    quantmodel::DatabaseEngineRequest database_msg;

    //
    //  Load the XML configuration
    //

    //cout << "Loading configuration file: " << argv[ 1 ] << endl;
    //const XMLNode config = XMLNode::openFileHelper( argv[ 1 ] );
    const XMLNode config = XMLNode::openFileHelper( "../config/fixEngine.xml" );

    //
    //  Setup log4cpp
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "FixEngine" );
    log->warn( "Starting QuantModel FixEngine v1.0" );

    //
    //  Start the Saturday (GMT) shutdown thread
    //

    pthread_t shutdown_thread;
    if( pthread_create( &shutdown_thread, NULL, shutdown, NULL ) )
    {
        log->fatal( "Failed to create Saturday shutdown thread." );
        exit( 1 );
    }

    //
    //  Setup ZMQ
    //

    zmq::context_t ctx ( 1 );
    initNetwork( config, ctx, socket_map, log );

    //
    //  Load the FIX applications
    //

    for( int i=0; i<config.nChildNode( "fix" ); ++i )
    {
        const XMLNode fix_node = config.getChildNode( "fix", i );
        application = NULL;

        log->info( " loading: %s ", fix_node.getAttribute( "fix_engine_id" ) );

        if( strcmp( "true", fix_node.getAttribute( "is_active" ) ) != 0 )
        {
            log->warn( "FIX ENGINE IS INACTIVE: %s", fix_node.getAttribute( "fix_engine_id" ) );
            continue;
        }
        else
        {
            if( strcmp( "TRADING_TECHNOLOGIES_ORDER", fix_node.getAttribute( "application_type" ) ) == 0 )
            {
                application = new fix_engine::TradingTechnologiesOrder(
                    fix_node, config.getChildNode( "network" ),
                    socket_map[ "persistence_callforward" ] );
            }
            else if( strcmp( "TRADING_TECHNOLOGIES_PRICE", fix_node.getAttribute( "application_type" ) ) == 0 )
            {
                application = new fix_engine::TradingTechnologiesPrice(
                    fix_node, config.getChildNode( "network" ) );
            }
            else if( strcmp( "GLOBEX_ORDER", fix_node.getAttribute( "application_type" ) ) == 0 )
            {
                application = new fix_engine::GlobexOrder(
                    fix_node, config.getChildNode( "network" ),
                    socket_map[ "persistence_callforward" ] );
            }
            else
            {
                log->error( "Unknown application_type: %s", fix_node.getAttribute( "application_type" ) );
                continue;
            }

            //
            //  Load the client order ids
            //

            application->loadClientOrderIdMap( socket_map[ "database_callforward" ] );

            //
            //  Start the FIX connection
            //

            application->start();

            //
            //  Add the application to the fix application map
            //

            log->info( " adding session: %s --> %s",
                application->getSessionID().toString().c_str(),
                fix_node.getAttribute( "application_type" ) );

            fix_application_map.insert(
                std::pair< const std::string , fix_engine::FIXApplication* >(
                    application->getSessionID().toString() , application ) );
        }
    }

    log->info( "FixEngine initialized, waiting for requests." );

    while( 1 )
    {
        if(! quantmodel::ZMQMessage::recv( &request_msg, socket_map[ "callforward" ] ) )
        {
            log->fatal( "UNABLE TO RECV ON callforward SOCKET!!!" );
            exit( 1 );
        }

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " --> " + request_msg.ShortDebugString() );
        }

        //
        //  reset the response message
        //

        database_msg.Clear();
        response_msg.Clear();
        response_msg.set_request_id( request_msg.request_id() );

        //
        //  Find the FIXApplication based on session_id
        //

        fix_application_iter = fix_application_map.find( request_msg.session_id() );

        if( fix_application_iter != fix_application_map.end( ) )
        {
            if( fix_application_iter->second->isLoggedOn( ) ||
                request_msg.type() == quantmodel::FixEngineRequest::FIXENGINE_LOGIN )
            {
                fix_application_iter->second->handleMessage(
                    request_msg, response_msg, database_msg );
            }
            else
            {
                //
                //  REJECT
                //

                response_msg.set_type( quantmodel::FixEngineResponse::REJECT );
                response_msg.set_error_msg( "FIX ENGINE NOT LOGGED ON" );
            }
        }
        else
        {
            //
            //  REJECT
            //

            response_msg.set_type( quantmodel::FixEngineResponse::REJECT );
            response_msg.set_error_msg( "UNKNOWN FIX ENGINE" );
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

            quantmodel::ZMQMessage::sendRPC( &database_msg, socket_map[ "persistence_callforward" ] );
        }

        //
        //  RPC to sender
        //

        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " <-- " + response_msg.ShortDebugString() );
        }

        quantmodel::ZMQMessage::send( &response_msg, socket_map[ "callforward" ] );
    }

    exit( 0 );
}

