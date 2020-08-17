#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

#include <pthread.h>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <zmq.hpp>

#include <gmpxx.h>

#include <mysql++.h>

#include <quantmodel.hpp>

using namespace std;

struct db_thread_args
{
    int id;
    XMLNode* config;
};

static log4cpp::Category* log = NULL;
static zmq::context_t ctx( 1 );

bool initStaticData( XMLNode& );
mysqlpp::Connection* initDatabase( XMLNode& );

static void* db_thread( void* args )
{
    db_thread_args* pargs = (struct db_thread_args *) args;

    quantmodel::DatabaseEngineRequest request_msg;
    quantmodel::DatabaseEngineResponse response_msg;

    std::string msg_data;
    mysqlpp::Connection* conn;
    zmq::socket_t* socket;

    //
    //  Initialize the database connection
    //

    conn = initDatabase( (*pargs->config) );

    if(! conn->connected() )
    {
        log->fatal( "Unable to connect to database." );
        pthread_exit( NULL );
    }

    //
    //  Initialize our REP socket, connect to inproc
    //

    socket = new zmq::socket_t( ctx, ZMQ_REP );
    socket->connect( "inproc://db_workers" );

    //
    //  Listen for incoming data request messages
    //

    while( 1 )
    {
        //
        //  Receive the request message
        //

        if(! quantmodel::ZMQMessage::recv( &request_msg, socket ) )
        {
            log->fatal( "UNABLE TO RECV ON callforward SOCKET!!!" );
            pthread_exit( NULL );
        }

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( "[ %d ] --> %s", pargs->id, request_msg.ShortDebugString().c_str() );
        }

        if( request_msg.request_id().compare( "SHUTDOWN" ) == 0 )
        {
            //
            //  Ack to sender
            //

            response_msg.Clear();
            response_msg.set_request_id( "SHUTDOWN" );
            quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
            quantmodel::ZMQMessage::send( &response_msg, socket );
            sleep( 1 );
            exit( 0 );
        }

        //
        //  Ensure database connection is live
        //

        if( !conn->connected() && !conn->ping() )
        {
            log->warn( "Reconnecting to database..." );
            conn->disconnect();
            delete conn;
            conn = initDatabase( (*pargs->config) );

            if(! conn->connected() )
            {
                log->fatal( "Unable to connect to database." );
                pthread_exit( NULL );
            }
        }

        //
        //  Clear the response message
        //

        response_msg.Clear();

        //
        //  Create a new transaction
        //

        bool commit_trans = true;
        mysqlpp::Transaction trans( (*conn) );

        //
        //  Loop through each database query
        //

        for( int i=0; i<request_msg.query_size(); ++i )
        {
            if(! commit_trans )
            {
                break;
            }

            const quantmodel::DatabaseEngineRequest::Query& query_req = request_msg.query( i );

            //
            //  Test for a static data refresh request
            //

            if( query_req.query_id().compare( "RELOAD_STATIC_DATA" ) == 0 )
            {
                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( "[ %d ] RELOAD_STATIC_DATA", pargs->id );
                }

                initStaticData( (*pargs->config) );
                continue;
            }

            //
            //  Not a static data refresh...
            //

            quantmodel::DatabaseEngineResponse::ResultSet* rs = response_msg.add_result_set( );

            rs->set_query_id( query_req.query_id() );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( "[ %d ] EXECUTING SQL: %s", pargs->id, query_req.query_str().c_str() );
            }

            mysqlpp::Query query = conn->query( query_req.query_str() );

            if( query_req.type() == quantmodel::DatabaseEngineRequest::UPDATE )
            {
                //
                //  Execute via Query::execute()
                //

                const mysqlpp::SimpleResult res = query.execute();

                if( res )
                {
                    quantmodel::DatabaseEngineResponse::ResultSet::Row *row = rs->add_row();
                    quantmodel::DatabaseEngineResponse::ResultSet::Row::Column *column = row->add_column();

                    row->set_row_id( 1 );
                    column->set_column_id( 1 );
                    column->set_name( "info" );
                    column->set_value( res.info() );
                }
                else
                {
                    log->error( "Query FAILED %s: ", query_req.query_str().c_str() );
                    commit_trans = false;
                    break;
                }
            }
            else
            {
                //
                //  Execute via Query::use()
                //

                const mysqlpp::UseQueryResult res = query.use();

                if( res )
                {
                    int row_id = 1;
                    while( const mysqlpp::Row db_row = res.fetch_row() )
                    {
                        quantmodel::DatabaseEngineResponse::ResultSet::Row *row = rs->add_row();
                        row->set_row_id( row_id++ );

                        for( size_t i=0; i<res.num_fields(); ++i )
                        {
                            quantmodel::DatabaseEngineResponse::ResultSet::Row::Column *column = row->add_column();

                            column->set_column_id( i+1 );
                            column->set_name( res.field_name( i ) );
                            column->set_value( db_row[ i ] );
                        }
                    }
                }
                else
                {
                    log->error( "Query FAILED %s: ", query_req.query_str().c_str() );
                    commit_trans = false;
                    break;
                }
            }

            //
            //  Drain the query as MySQL returns two
            //  result sets for each stored procedure...
            //

            while( query.more_results() )
            {
                query.store_next();
            }
        }

        //
        //  We've completed all SQL commands, commit or rollback
        //

        if( commit_trans )
        {
            trans.commit();
        }
        else
        {
            trans.rollback();
        }

        //
        //  Send the response back to the client
        //

        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " <-- " + response_msg.ShortDebugString() );
        }

        quantmodel::ZMQMessage::send( &response_msg, socket );
    }

    pthread_exit( NULL );
}

void initLogging( XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

mysqlpp::Connection* initDatabase( XMLNode& config )
{
    mysqlpp::Connection* conn = new mysqlpp::Connection( false );
    conn->set_option( new mysqlpp::MultiStatementsOption( true ) );
    conn->set_option( new mysqlpp::ReconnectOption( true ) );

    const XMLNode node = config.getChildNode( "database" );

    cout << " connecting to mysql: " << endl
         << "  db: " << node.getAttribute( "db" ) << endl
         << "  server: " << node.getAttribute( "server" ) << endl
         << "  username: " << node.getAttribute( "username" ) << endl
         << "  password: " << node.getAttribute( "password" ) << endl;

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

bool initStaticData( XMLNode& config )
{
    /*

    05.09.2011 -- MEMCACHED NO LONGER SUPPORTED.

    std::string key, val;
    quantmodel::MemcachedConnection memcache;
    mysqlpp::Connection* conn = new mysqlpp::Connection( false );

    log->info( "Loading static data: " );

    const XMLNode node = config.getChildNodeByPath( "staticData/database" );

    cout << " connecting to mysql: " << endl
         << "  db: " << node.getAttribute( "db" ) << endl
         << "  server: " << node.getAttribute( "server" ) << endl
         << "  username: " << node.getAttribute( "username" ) << endl
         << "  password: " << node.getAttribute( "password" ) << endl;

    if( conn->connect(
        node.getAttribute( "db" ),
        node.getAttribute( "server" ),
        node.getAttribute( "username" ),
        node.getAttribute( "password" ) ) )
    {
        //
        //  For each child, create the select statement from the table name
        //  and load the Memcache server.
        //

        for( int i=0; i<node.nChildNode( "table" ); ++i )
        {
            const XMLNode table = node.getChildNode( "table", i );

            const std::string table_name ( table.getAttribute( "name" ) );
            const std::string pk ( table.getAttribute( "key" ) );

            std::string stmt( "select * from " );
            stmt.append( table_name );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " static data query: %s", stmt.c_str() );
            }

            mysqlpp::Query query = conn->query( stmt );
            const mysqlpp::UseQueryResult res = query.use();

            if( res )
            {
                while( const mysqlpp::Row db_row = res.fetch_row() )
                {
                    quantmodel::DatabaseEngineResponse::ResultSet::Row row;

                    row.set_row_id( 0 );

                    for( size_t i=0; i<res.num_fields(); ++i )
                    {
                        quantmodel::DatabaseEngineResponse::ResultSet::Row::Column *column = row.add_column();

                        column->set_column_id( i+1 );
                        column->set_name( res.field_name( i ) );
                        column->set_value( db_row[ i ] );
                    }

                    key.assign( table_name );
                    key.append( "::" )
                       .append( pk )
                       .append( "::" )
                       .append( db_row[ pk.c_str() ] );

                    log->info( " adding memcached key --> " + key );

                    row.SerializeToString( &val );

                    if(! memcache.put( key, val ) )
                    {
                        log->error( "could not cache: %s --> %s", key.c_str(), val.c_str() );

                        conn->disconnect();
                        delete conn;

                        return false;
                    }
                }
            }
        }

        conn->disconnect();
        delete conn;
    }
    else
    {
        log->error( "CANNOT CONNECT TO DATABASE!!!" );
        delete conn;
        return false;
    }

    //
    //  Load any static keys
    //

    for( int i=0; i<config.nChildNode( "staticData" ); ++i )
    {
        const XMLNode static_data_node = config.getChildNode( "staticData", i );

        for( int j=0; j<static_data_node.nChildNode( "static" ); ++j )
        {
            const XMLNode static_node = static_data_node.getChildNode( "static", j );

            key.assign( static_node.getAttribute( "key" ) );
            val.assign( static_node.getAttribute( "value" ) );

            log->debug( "STATIC " + key + std::string( " --> " ) + val );

            if(! memcache.put( key, val ) )
            {
                log->error( "could not cache: " + key +
                    std::string( " --> " ) + val );

                return false;
            }
        }
    }
    */

    return true;
}

int main( int argc, char *argv[] )
{
    //if( argc == 1 )
    //{
    //    cerr << " usage: DatabaseEngine [config.xml]" << endl;
    //    exit( 1 );
    //}
    //
    ////
    ////  Load the configuration file
    ////
    //
    //cout << "Loading configuration file: " << argv[ 1 ] << endl;
    //XMLNode config = XMLNode::openFileHelper( argv[ 1 ] );

    cout << "Loading configuration file: ../config/databaseEngine.xml" << endl;
    XMLNode config = XMLNode::openFileHelper( "../config/databaseEngine.xml" );

    //
    //  Setup log4cpp
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "DatabaseEngine" );

    //
    //  Load the static data
    //

    if(! initStaticData( config ) )
    {
        cerr << "Load static data failure." << endl;
        exit( 1 );
    }

    //
    //  Create the XREP socket
    //

    const XMLNode network = config.getChildNode( "network" );
    log->info( " Binding XREP to %s", network.getAttribute( "callforward" ) );

    zmq::socket_t* callforward = new zmq::socket_t( ctx, ZMQ_XREP );
    callforward->bind( network.getAttribute( "callforward" ) );

    //
    //  Create the XREQ inproc socket
    //

    const XMLNode node = config.getChildNode( "network" );
    zmq::socket_t* inproc = new zmq::socket_t( ctx, ZMQ_XREQ );
    inproc->bind( "inproc://db_workers" );

    //
    //  Start our threads
    //

    const int thread_count = atoi( config.getAttribute( "threads" ) );
    pthread_t pthread_arr[ thread_count ];
    struct db_thread_args args[ thread_count ];

    for( int i=0; i<thread_count; ++i )
    {
        log->info( " starting REP db_thread %d: ", i );

        args[ i ].id = i;
        args[ i ].config = &config;

        if( pthread_create( &pthread_arr[ i ], NULL, db_thread, &args[ i ] ) )
        {
            log->fatal( "Failed to create db_thread." );
            exit( 1 );
        }

        sleep( 1 );
    }

    //
    //  Setup the poll item array
    //

    zmq_pollitem_t items [ 2 ];
    items[ 0 ].socket = (*callforward);
    items[ 0 ].events = ZMQ_POLLIN;
    items[ 1 ].socket = (*inproc);
    items[ 1 ].events = ZMQ_POLLIN;

    log->info( "Database engine initialized, waiting for requests." );

    //
    //  Poll on callforward / inproc forever
    //

    int64_t more;
    size_t more_size = sizeof( int64_t );

    while( 1 )
    {
        zmq::message_t message;
        const int rc = zmq_poll( &items[ 0 ], 2, -1 );

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
        else
        {
            if( items[ 0 ].revents == ZMQ_POLLIN )
            {
                while( 1 )
                {
                    callforward->recv( &message );
                    callforward->getsockopt( ZMQ_RCVMORE, &more, &more_size );
                    inproc->send( message, more ? ZMQ_SNDMORE : 0 );

                    if( !more )
                    {
                        break;
                    }
                }
            }

            if( items[ 1 ].revents == ZMQ_POLLIN )
            {
                while( 1 )
                {
                    inproc->recv( &message );
                    inproc->getsockopt( ZMQ_RCVMORE, &more, &more_size );
                    callforward->send( message, more ? ZMQ_SNDMORE : 0 );

                    if( !more )
                    {
                        break;
                    }
                }
            }
        }
    }

    exit( 0 );
}
