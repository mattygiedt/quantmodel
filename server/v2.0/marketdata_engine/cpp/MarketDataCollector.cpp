#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>

#include "zlib.h"

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <quantmodel.hpp>

using namespace std;

struct gzip_thread_args
{
    std::string file;
    log4cpp::Category *log;
};

static void* gzip_thread( void* args )
{
    gzip_thread_args* pargs = (struct gzip_thread_args *) args;

    std::string command = "gzip ";
    command.append( pargs->file );
    pargs->log->warn( " gzip_thread command: " + command );

    //
    //  execute gzip via system
    //

    int ret = system( command.c_str() );
    pargs->log->warn( " system call returns: %d", ret );

    //
    //  Terminate the thread
    //

    pthread_exit( NULL );
}

void initLogging( const XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

void initNetwork( const XMLNode& config, zmq::socket_t *& s, log4cpp::Category* log )
{
    zmq::context_t *ctx = new zmq::context_t( 1 );
    s = new zmq::socket_t( *ctx, ZMQ_SUB );
    s->setsockopt( ZMQ_SUBSCRIBE, "", 0 );

    //
    //  Connect to the listed market data providers
    //

    const XMLNode network_node = config.getChildNode( "network" );
    for( int i=0; i<network_node.nChildNode( "provider" ); i++ )
    {
        log->warn( " subscribing to market data provider: %s",
            network_node.getChildNode( "provider", i ).getAttribute( "address" ) );

        s->connect( network_node.getChildNode( "provider", i ).getAttribute( "address" ) );
    }
}

int main( int argc, char *argv[] )
{
    int rc;
    std::fstream file;
    zmq::socket_t *socket;
    log4cpp::Category *log;
    std::string utc_day, last_utc_day, log_dir, filename;

    quantmodel::FastEngineBroadcast fast_engine_broadcast;

    //
    //  Load the XML configuration
    //

    //cout << "Loading configuration file: " << argv[ 1 ] << endl;
    const XMLNode& config = XMLNode::openFileHelper( argv[ 1 ] );
    //const XMLNode& config = XMLNode::openFileHelper( "../config/marketDataCollector.xml" );

    //
    //  Setup log4cpp
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "MarketDataCollector" );
    log->warn( "Starting QuantModel MarketDataCollector v1.0" );

    //
    //  Set the data directory & open the files
    //

    const XMLNode& data_node = config.getChildNode( "data" );
    log_dir = data_node.getAttribute( "dir" );
    quantmodel::TimeStamp::getUTCDay( utc_day );
    last_utc_day = utc_day;

    filename.assign( log_dir )
            .append( "/" )
            .append( utc_day )
            .append( ".proto" );

    log->info( "opening file: " + filename );
    file.open( filename.c_str(), ios::out | ios::app | ios::binary );

    if( file.fail() )
    {
        log->fatal( "error opening file: " + filename );
        exit( 1 );
    }

    //
    //  Setup ZMQ
    //

    initNetwork( config, socket, log );

    zmq::pollitem_t items [ ] = { { (*socket), 0, ZMQ_POLLIN, 0 } };

    log->info( "Market Data Collector initialized, waiting for market data" );

    while( ( rc = zmq::poll( items, 1, 1000 * 1000 * 10 ) ) >= 0 )
    {
        //
        //  Do we need to close + open log file?
        //

        quantmodel::TimeStamp::getUTCDay( utc_day );

        if( utc_day != last_utc_day )
        {
            log->warn( "rolling log file to: " + utc_day );

            //
            //  Close the log file
            //

            last_utc_day = utc_day;
            file.close();

            //
            //  Create a new pthread to compress the saved data
            //

            pthread_t pthread;
            struct gzip_thread_args args;
            args.file.assign( filename );
            args.log = log;

            //
            //  Create the new data file
            //

            filename.assign( log_dir )
                    .append( "/" )
                    .append( utc_day )
                    .append( ".proto" );

            log->info( "opening file: " + filename );
            file.open( filename.c_str(), ios::out | ios::app | ios::binary );

            if( file.fail() )
            {
                log->fatal( "error opening file: " + filename );
                exit( 1 );
            }

            //
            //  Start the gzip thread
            //

            log->info( "starting gzip_thread" );
            if( pthread_create( &pthread, NULL, gzip_thread, &args ) )
            {
                log->error( "unable to start gzip_thread" );
            }
        }

        if( rc == 0 )
        {
            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( "poll timeout." );
            }

            continue;
        }

        //
        //  Receive a fast engine broadcast
        //

        fast_engine_broadcast.Clear();

        if(! quantmodel::ZMQMessage::recv( &fast_engine_broadcast, socket ) )
        {
            log->fatal( "UNABLE TO RECV ON broadcast SOCKET!!!" );
            exit( 1 );
        }

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( "%d: %s", fast_engine_broadcast.ByteSize(),
                fast_engine_broadcast.ShortDebugString().c_str() );
        }

        file << fast_engine_broadcast.ByteSize();
        if(! fast_engine_broadcast.SerializeToOstream( &file ) )
        {
            log->fatal( "unable to persist message." );
            exit( 1 );
        }
    }

    log->fatal( "Unable to poll rc = %d", rc );
    exit( 1 );
}
