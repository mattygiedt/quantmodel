#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>

#include <mysql++.h>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <quantmodel.hpp>

using namespace std;

void initLogging( const XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

mysqlpp::Connection* initDatabase( const XMLNode& config, log4cpp::Category* log )
{
    mysqlpp::Connection* conn = new mysqlpp::Connection( false );
    conn->set_option( new mysqlpp::MultiStatementsOption( true ) );
    conn->set_option( new mysqlpp::ReconnectOption( true ) );

    const XMLNode& node = config.getChildNode( "database" );

    log->info( " connecting to mysql: db %s, server %s, username %s, password %s",
         node.getAttribute( "db" ),
         node.getAttribute( "server" ),
         node.getAttribute( "username" ),
         node.getAttribute( "password" ) );

    if( conn->connect(
        node.getAttribute( "db" ),
        node.getAttribute( "server" ),
        node.getAttribute( "username" ),
        node.getAttribute( "password" ) ) )
    {
        return conn;
    }

    return NULL;
}

void initNetwork( const XMLNode& config, zmq::socket_t *& s, log4cpp::Category* log )
{
    zmq::context_t *ctx = new zmq::context_t( 1 );

    //
    //  Find the <network /> node
    //

    const XMLNode network_node = config.getChildNode( "network" );

    if(! network_node.isAttributeSet( "broadcast_price" ) )
    {
        log->fatal( "broadcast_price address is not set" );
        exit( 1 );
    }

    s = new zmq::socket_t( *ctx, ZMQ_SUB );
    s->setsockopt( ZMQ_SUBSCRIBE, "", 0 );
    s->connect( network_node.getAttribute( "broadcast_price" ) );
}

int main( int argc, char *argv[] )
{
    zmq::socket_t *socket;
    log4cpp::Category *log;
    mysqlpp::Connection* conn;
    std::string ts, query_str;

    quantmodel::MarketDataBroadcast broadcast_msg;

    //
    //  Load the XML configuration
    //

    //cout << "Loading configuration file: " << argv[ 1 ] << endl;
    const XMLNode& config = XMLNode::openFileHelper( argv[ 1 ] );
    //const XMLNode& config = XMLNode::openFileHelper( "../config/lastPriceEngine.xml" );

    //
    //  Setup log4cpp
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "LastPriceEngine" );
    log->warn( "Starting QuantModel LastPrice Engine v1.0" );

    //
    //  Connect to the database
    //

    conn = initDatabase( config, log );

    if( NULL == conn )
    {
        log->fatal( "Could not connect to database." );
        exit( 1 );
    }

    //
    //  Setup ZMQ
    //

    initNetwork( config, socket, log );

    log->info( "LastPrice Engine initialized, waiting for market data" );

    while( 1 )
    {
        broadcast_msg.Clear();

        //
        //  Receive a market data price broadcast
        //

        if(! quantmodel::ZMQMessage::recv( &broadcast_msg, socket ) )
        {
            log->fatal( "UNABLE TO RECV ON broadcast SOCKET!!!" );
            exit( 1 );
        }

        if( broadcast_msg.type() == quantmodel::MarketDataBroadcast::PRICE )
        {
            //
            //  Ensure database connection is live
            //

            if( !conn->connected() && !conn->ping() )
            {
                log->warn( "Reconnecting to database..." );
                conn->disconnect();
                delete conn;
                conn = initDatabase( config, log );

                if(! conn->connected() )
                {
                    log->fatal( "Unable to connect to database." );
                    exit( 1 );
                }
            }

            quantmodel::TimeStamp::getUTCTime( ts );

            bool commit_trans = true;
            mysqlpp::Transaction trans( (*conn) );

            for( int i=0; i<broadcast_msg.price_data_size(); ++i )
            {
                const quantmodel::MarketDataPriceData& price_data = broadcast_msg.price_data( i );

                //
                //  Create the update statement
                //

                query_str.assign( "UPDATE instrument SET last_prc = '" )
                         .append( price_data.last_prc() )
                         .append( "', updated_datetime = '" )
                         .append( ts )
                         .append( "', updated_by = 'LASTPRC_ENGINE' WHERE instrument_id = " )
                         .append( price_data.instrument_id() );

                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( query_str );
                }

                //
                //  Create and execute the query
                //

                mysqlpp::Query query = conn->query( query_str );

                if(! query.execute() )
                {
                    log->error( "FAILED: " + query_str );
                    commit_trans = false;
                    break;
                }
            }

            //
            //  Commit the transaction if we were able to execute
            //

            if( commit_trans )
            {
                trans.commit();
            }
            else
            {
                trans.rollback();
            }
        }
    }
}
