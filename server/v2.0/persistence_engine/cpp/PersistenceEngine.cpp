#include <sys/time.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <deque>
#include <map>
#include <assert.h>

#include "zlib.h"

#include <mysql++.h>
#include <pthread.h>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <zmq.hpp>

#include <gmpxx.h>

#include <quantmodel.hpp>

#define CHUNK 16384

using namespace std;

static bool PERSISTING = true;
static pthread_mutex_t queue_mutex;
static pthread_t de_log_thread;
static std::deque< std::vector< std::string > > queue;

bool validateTxFiles( const std::string&, const XMLNode& );
bool persist( mysqlpp::Connection* , const XMLNode& , const char*, log4cpp::Category* );
static void* persist_thread( void* );
mysqlpp::Connection* initDatabase( const XMLNode& );

bool filenameSortPredicate( const std::string& s1, const std::string& s2 )
{
  return s1 < s2;
}

int compress( FILE *source, FILE *dest ) // http://www.zlib.net/zpipe.c
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}

void persistDeltaFile(
    const std::string& dir,
    const XMLNode& config,
    const std::string& delta_filename,
    const std::string& logfile_name,
    const std::string& loggedfile_name,
    log4cpp::Category* log )
{
    char ch;
    std::string logfile_line, delta_logged_filename;
    FILE *logfile, *loggedfile, *delta_file, *delta_logged_file;

    delta_file = fopen( delta_filename.c_str(), "r" );

    if(! delta_file )
    {
        log->fatal( "Cannot open delta file for read: " + delta_filename );
        exit( 1 );
    }

    log->info( "Persisting delta file for logfile: %s", logfile_name.c_str() );

    struct stat delta_file_stat;
    fstat( fileno( delta_file ), &delta_file_stat );

    if( delta_file_stat.st_size != 0 )
    {
        //
        //  Create the delta.logged file
        //

        delta_logged_filename = dir + "/" + "delta.logged";
        delta_logged_file = fopen( delta_logged_filename.c_str(), "w" );

        //
        //  Create a database connection
        //

        mysqlpp::Connection* conn = initDatabase( config );

        if(! conn->connected() )
        {
            log->fatal( "Unable to connect to database." );
            fclose( delta_file );
            exit( 1 );
        }

        //
        //  Read each line of the delta file and persist it
        //

        while(! feof( delta_file ) )
        {
            logfile_line.clear();

            do
            {
                ch = fgetc( delta_file );
                if(! feof( delta_file ) ) logfile_line.push_back( ch );

                if( ch == '\n' )
                    break;
            }
            while( ch != EOF );

            if( feof( delta_file ) && logfile_line.size() == 0 )
            {
                continue;
            }

            if( persist( conn, config, logfile_line.c_str(), log ) )
            {
                //
                //  Log the delta transaction to the delta.logged file
                //

                fprintf( delta_logged_file, "%s", logfile_line.c_str() );
                fflush( delta_logged_file );
            }
            else
            {
                log->fatal( "Database error on delta input: %s", logfile_line.c_str() );

                conn->disconnect();
                delete conn;
                fclose( delta_file );
                fclose( delta_logged_file );
                exit( 1 );
            }
        }

        conn->disconnect();
        delete conn;

        fclose( delta_file );
        fclose( delta_logged_file );

        log->warn( "Successfully recovered %s from delta file.", logfile_name.c_str() );

        //
        //  Create a new logged file as a copy of log file
        //

        logfile = fopen( logfile_name.c_str(), "r" );
        loggedfile = fopen( loggedfile_name.c_str(), "w" );

        while(! feof( logfile ) )
        {
            ch = fgetc( logfile );
            if(! feof( logfile ) ) fputc( ch, loggedfile );
        }

        fclose( logfile );
        fclose( loggedfile );
    }
    else
    {
        log->fatal( "delta file: %s is empty.", delta_filename.c_str() );
        exit( 1 );
    }
}

bool validateTxFiles( const std::string& dir, const XMLNode& config, log4cpp::Category* log )
{
    char ch;
    unsigned int row_id = 0;
    size_t found;
    std::string filename;
    std::vector< std::string > log_files;
    struct dirent* dir_entry;
    DIR* log_dir;
    FILE *logfile, *loggedfile;
    std::string logfile_line, loggedfile_line;

    log_dir = opendir( dir.c_str() );

    while( ( dir_entry = readdir( log_dir ) ) )
    {
        filename.clear();
        filename.append( dir_entry->d_name );

        found = filename.find( ".log" );

        if( found != std::string::npos &&
            found + 4 == filename.length() )
        {
            log_files.push_back( dir + "/" + filename );
        }
    }

    closedir( log_dir );

    //
    //  Sort the log files, oldest to newest
    //

    std::sort( log_files.begin(), log_files.end(), filenameSortPredicate );

    for( std::vector<std::string>::iterator iter = log_files.begin();
         iter != log_files.end(); ++iter )
    {
        log->info( " validating file: " + (*iter) );
    }

    for( std::vector<std::string>::iterator iter = log_files.begin();
         iter != log_files.end(); ++iter )
    {
        const std::string logfile_name = (*iter);
        const std::string loggedfile_name = logfile_name + "ged";

        log->info( " validating file: " + logfile_name + " --> " + loggedfile_name );

        logfile = fopen( logfile_name.c_str(), "r" );
        row_id = 0;

        if( logfile )
        {
            //
            //  Try to open the LOGGED file.
            //

            loggedfile = fopen( loggedfile_name.c_str(), "r" );

            if( loggedfile )
            {
                //
                //  Are the two file sizes the same?
                //

                struct stat logfile_stat, loggedfile_stat;
                fstat( fileno( logfile ), &logfile_stat );
                fstat( fileno( loggedfile ), &loggedfile_stat );

                if( logfile_stat.st_size != loggedfile_stat.st_size )
                {
                    //
                    //  Uh-oh.
                    //

                    log->warn( "%s size %ld != %ld size %s",
                        logfile_name.c_str(), logfile_stat.st_size,
                        loggedfile_stat.st_size, loggedfile_name.c_str() );

                    if( logfile_stat.st_size < loggedfile_stat.st_size )
                    {
                        //
                        //  Fatal problem.
                        //

                        log->fatal( logfile_name + " has a smaller filesize than " + loggedfile_name );
                        fclose( loggedfile );
                        fclose( logfile );
                        exit( 1 );
                    }
                    else
                    {
                        //
                        //  Open the DELTA file for write
                        //

                        std::string delta_filename = dir + "/" + "delta.xml";
                        FILE *delta_file = fopen( delta_filename.c_str(), "w" );

                        if(! delta_file )
                        {
                            log->fatal( "Cannot open delta file for write: " + delta_filename );
                            exit( 1 );
                        }

                        //
                        //  Read a line from the log file, ensure that the same
                        //  line exists in the logged file.
                        //

                        while(! feof( logfile ) )
                        {
                            row_id++;
                            logfile_line.clear();
                            loggedfile_line.clear();

                            //
                            //  Read from the logfile until we see a '\n' or EOF
                            //

                            do
                            {
                                ch = fgetc( logfile );
                                if(! feof( logfile ) ) logfile_line.push_back( ch );

                                if( ch == '\n' )
                                    break;
                            }
                            while( ch != EOF );

                            if( ch == EOF )
                            {
                                continue;
                            }

                            //
                            //  Does logfile_line contain a line?
                            //

                            if( logfile_line.find( '\n' ) == string::npos )
                            {
                                //
                                //  Fatal problem.
                                //

                                log->fatal( "line %d of %s is invalid: '%s'",
                                    row_id,
                                    logfile_name.c_str(),
                                    logfile_line.c_str() );

                                fclose( loggedfile );
                                fclose( logfile );
                                fclose( delta_file );

                                exit( 1 );
                            }

                            //
                            //  We now have the following options:
                            //
                            //      1. loggedfile is at EOF,
                            //      2. loggedfile has the same line,
                            //      3. loggedfile has a different line.
                            //

                            if( feof( loggedfile ) )
                            {
                                //
                                //  Simply append to delta file
                                //

                                fprintf( delta_file, "%s", logfile_line.c_str() );
                                fflush( delta_file );
                            }
                            else
                            {
                                //
                                //  Read from the loggedfile until we see a '\n' or EOF
                                //

                                do
                                {
                                    ch = fgetc( loggedfile );
                                    if(! feof( loggedfile ) ) loggedfile_line.push_back( ch );

                                    if( ch == '\n' )
                                        break;
                                }
                                while( ch != EOF );

                                if( feof( loggedfile ) && loggedfile_line.size() == 0 )
                                {
                                    //
                                    //  Simply append to delta file
                                    //

                                    fprintf( delta_file, "%s", logfile_line.c_str() );
                                    fflush( delta_file );

                                    continue;
                                }

                                //
                                //  Ensure that the logfile_line == loggedfile_line
                                //

                                if( logfile_line.compare( loggedfile_line ) != 0 )
                                {
                                    //
                                    //  Fatal problem.
                                    //

                                    log->fatal( "line %d of %s is not the same as line %d of %s\n %s: '%s' %s: '%s'",
                                        row_id,
                                        logfile_name.c_str(),
                                        row_id,
                                        loggedfile_name.c_str(),
                                        logfile_name.c_str(),
                                        logfile_line.c_str(),
                                        loggedfile_name.c_str(),
                                        loggedfile_line.c_str() );

                                    fclose( loggedfile );
                                    fclose( logfile );
                                    fclose( delta_file );

                                    exit( 1 );
                                }
                            }
                        }

                        fclose( logfile );
                        fclose( loggedfile );
                        fclose( delta_file );

                        //
                        //  The delta file now contains any transaction file
                        //  requests that did not get replicated to the LOGGED
                        //  file.
                        //

                        persistDeltaFile( dir, config, delta_filename, logfile_name, loggedfile_name, log );
                    }
                }
                else
                {
                    log->info( logfile_name + " has same size as " + loggedfile_name );

                    fclose( logfile );
                    fclose( loggedfile );
                }
            }
            else
            {
                struct stat logfile_stat;
                fstat( fileno( logfile ), &logfile_stat );

                log->info( " cannot open LOGGED file, assuming it was never created: " + loggedfile_name );
                log->info( " reading %ld bytes from %s", logfile_stat.st_size, logfile_name.c_str() );

                //
                //  Open the DELTA file for write
                //

                std::string delta_filename = dir + "/" + "delta.xml";
                FILE *delta_file = fopen( delta_filename.c_str(), "w" );

                if(! delta_file )
                {
                    log->fatal( "Cannot open delta file for write: " + delta_filename );
                    exit( 1 );
                }

                while(! feof( logfile ) )
                {
                    row_id++;
                    logfile_line.clear();

                    //
                    //  Read from the logfile until we see a '\n' or EOF
                    //

                    do
                    {
                        ch = fgetc( logfile );
                        if(! feof( logfile ) ) logfile_line.push_back( ch );

                        if( ch == '\n' )
                            break;
                    }
                    while( ch != EOF );

                    if( feof( logfile ) && logfile_line.size() == 0 )
                    {
                        continue;
                    }

                    //
                    //  Does logfile_line contain a line?
                    //

                    if( logfile_line.find( '\n' ) == string::npos )
                    {
                        //
                        //  Fatal problem.
                        //

                        log->fatal( "line %d of %s is invalid: '%s'",
                            row_id,
                            logfile_name.c_str(),
                            logfile_line.c_str() );

                        fclose( logfile );
                        fclose( delta_file );

                        exit( 1 );
                    }

                    //
                    //  Append to delta
                    //

                    fprintf( delta_file, "%s", logfile_line.c_str() );
                    fflush( delta_file );

                }

                fclose( logfile );
                fclose( delta_file );

                //
                //  The delta file now contains any transaction file requests
                //  that did not get replicated to the LOGGED file.
                //

                persistDeltaFile( dir, config, delta_filename, logfile_name, loggedfile_name, log );
            }
        }
        else
        {
            log->fatal( " cannot open LOG file: " + logfile_name );
            exit( 1 );
        }
    }

    log->info( "transaction log data is valid, compressing files." );

    std::string ts;
    quantmodel::TimeStamp::getLocalTime( ts );

    for( std::vector<std::string>::iterator iter = log_files.begin();
         iter != log_files.end(); ++iter )
    {
        int ret;
        const std::string logfile_cmd = "gzip -fS ." + ts + ".gz " + (*iter);
        const std::string loggedfile_cmd = "gzip -fS ." + ts + ".gz " + (*iter) + "ged";

        //
        //  GZip the logfile, then the loggedfile
        //

        ret = system( logfile_cmd.c_str() );
        log->warn( " '%s' returns: %d", logfile_cmd.c_str(), ret );

        ret = system( loggedfile_cmd.c_str() );
        log->warn( " '%s' returns: %d", loggedfile_cmd.c_str(), ret );
    }

    return true;
}

bool persist( mysqlpp::Connection* conn, const XMLNode& config, const char *sql, log4cpp::Category* log )
{
    //
    //  Ensure database connection
    //

    if( NULL == conn )
    {
        return false;
    }

    if( !conn->connected() && !conn->ping() )
    {
        log->warn( "Reconnecting to database..." );
        conn->disconnect();
        delete conn;
        conn = initDatabase( config );

        if(! conn->connected() )
        {
            log->fatal( "Unable to connect to database." );
            return false;
        }
    }

    if( unlikely( log->isDebugEnabled() ) )
        log->debug( " EXECUTING SQL: %s", sql );

    //
    //  Create a new transaction
    //

    mysqlpp::Transaction trans( (*conn) );
    mysqlpp::Query query = conn->query( sql );
    const mysqlpp::SimpleResult res = query.execute();

    if(! res )
    {
        trans.rollback();
        log->fatal( "EXEC query: %s FAILED: %s", sql, conn->error() );
        return false;
    }

    if( unlikely( log->isDebugEnabled() ) )
        log->debug( " SQL OK." );

    //
    //  Commit the transaction
    //

    trans.commit();

    return true;
}

mysqlpp::Connection* initDatabase( const XMLNode& config )
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

static void* persist_thread( void* args )
{
    log4cpp::Category* log;
    bool sleep_flag = false;
    std::string filename;
    XMLNode* config = (XMLNode*)args;
    const XMLNode tx_log_dir_node = config->getChildNode( "tx_log_dir" );
    const std::string dir ( tx_log_dir_node.getAttribute( "dir" ) );

    log = &log4cpp::Category::getInstance( "PersistenceThread" );
    mysqlpp::Connection* conn = initDatabase( (*config) );

    if(! conn->connected() )
    {
        log->fatal( "Unable to connect to database." );
        PERSISTING = false;
        pthread_exit( NULL );
    }

    std::vector< std::string >::iterator sql_iter;
    std::vector< std::vector< std::string > > requests;
    std::vector< std::vector< std::string > >::iterator request_iter;
    std::deque< std::vector< std::string > >::iterator queue_iter;

    while( 1 )
    {
        //
        //  Poll every second, until we get something to do.
        //

        pthread_mutex_lock( &queue_mutex );
        sleep_flag = queue.empty();

        if( sleep_flag )
        {
            pthread_mutex_unlock( &queue_mutex );
            sleep( 1 );
            continue;
        }

        //
        //  Queue must now contain SQL!!!
        //

        queue_iter = queue.begin();

        while( queue_iter != queue.end() )
        {
            requests.push_back( (*queue_iter++) );
        }

        queue.clear();
        pthread_mutex_unlock( &queue_mutex );

        //
        //  Should never happen, but just in case...
        //

        if( requests.size() == 0 )
        {
            continue;
        }

        //
        //  Loop through each request vector
        //

        for( request_iter = requests.begin(); request_iter != requests.end(); ++request_iter )
        {
            if( unlikely( log->isDebugEnabled() ) )
            {
                for( sql_iter = (*request_iter).begin(); sql_iter != (*request_iter).end(); ++sql_iter )
                {
                    log->debug( " REQVEC: %s", (*sql_iter).c_str() );
                }
            }

            //
            //  Ensure valid request vector
            //

            if( (*request_iter).front() == "SHUTDOWN" )
            {
                log->fatal( "SHUTDOWN persistence_thread" );
                PERSISTING = false;
                pthread_exit( NULL );
            }

            if( (*request_iter).size() < 2 )
            {
                log->fatal( "Invalid request vector size: %ld", (*request_iter).size() );
                PERSISTING = false;
                pthread_exit( NULL );
            }

            //
            //  Build the filename, open the file
            //

            filename.assign( (*request_iter).front() );
            filename.append( "ged" );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( "LOGGED: %s", filename.c_str() );
            }

            FILE *file = fopen( filename.c_str(), "a" );

            if(! file )
            {
                log->fatal( "Cannot open file for append: " + filename );
                PERSISTING = false;
                pthread_exit( NULL );
            }

            //
            //  Loop through each query string and persist it
            //

            //for( uint i=1; i<(*request_iter).size(); i++ )
            for( sql_iter = (*request_iter).begin()+1; sql_iter != (*request_iter).end(); ++sql_iter )
            {
                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( "PERSIST: %s", (*sql_iter).c_str() );
                }

                //
                //  Persist the query_str
                //

                if(! persist( conn, (*config), (*sql_iter).c_str(), log ) )
                {
                    log->fatal( "Database error, persistence thread exiting..." );

                    fclose( file );
                    PERSISTING = false;
                    pthread_exit( NULL );
                }

                //
                //  We were able to execute all of the queries. Add the query_iter
                //  to logged file.
                //

                fprintf( file, "%s\n", (*sql_iter).c_str() );

                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( "LOGGED: %s", (*sql_iter).c_str() );
                }
            }

            //
            //  Close the file.
            //

            fclose( file );

            //
            //  Clear the request iterator vector
            //

            (*request_iter).clear();
        }

        //
        //  Clear the requests vector
        //

        requests.clear();
    }

    PERSISTING = false;
    pthread_exit( NULL );
}

int main( int argc, char *argv[] )
{
    std::string filename;
    log4cpp::Category* log;
    zmq::context_t ctx( 1 );

    quantmodel::DatabaseEngineRequest request_msg;
    quantmodel::DatabaseEngineResponse response_msg;

    if( argc == 1 )
    {
        cerr << " usage: PersistenceEngine [config.xml]" << endl;
        exit( 1 );
    }

    //
    //  Load the configuration file
    //

    cout << "Loading configuration file: " << argv[ 1 ] << endl;
    XMLNode config = XMLNode::openFileHelper( argv[ 1 ] );

    //
    //  Initialize log4cpp logger
    //

    const XMLNode logging_node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( logging_node.getAttribute( "properties" ) );
    log = &log4cpp::Category::getInstance( "PersistenceEngine" );

    //
    //  What is the directory that we're watching?
    //

    const XMLNode tx_log_dir_node = config.getChildNode( "tx_log_dir" );
    const std::string dir ( tx_log_dir_node.getAttribute( "dir" ) );

    //
    //  Ensure that our transactions agree
    //

    if(! validateTxFiles( dir, config, log ) )
    {
        log->warn( "INVALID TX FILES." );
    }

    //
    //  Initialize PTHREAD mutex
    //

    pthread_mutex_init( &queue_mutex, NULL );

    //
    //  Start de_log_thread thread
    //

    if( pthread_create( &de_log_thread, NULL, persist_thread, (void *)&config ) )
    {
        log->fatal( "Failed to create DE LOG persistence thread." );
        exit( 1 );
    }

    //
    //  Initialize ZMQ REP socket
    //

    zmq::socket_t* socket = NULL;
    const XMLNode network_node = config.getChildNode( "network" );

    log->info( " persistence engine callforward network address: %s",
        network_node.getAttribute( "callforward" ) );

    socket = new zmq::socket_t( ctx, ZMQ_REP );
    socket->bind( network_node.getAttribute( "callforward" ) );

    //
    //  Ready for business!
    //

    log->info( "initialization complete." );

    while( 1 )
    {
        //
        //  Receive a callforward request
        //

        if(! quantmodel::ZMQMessage::recv( &request_msg, socket ) )
        {
            log->fatal( "UNABLE TO RECV ON callforward SOCKET!!!" );
            exit( 1 );
        }

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " --> " + request_msg.ShortDebugString() );
        }

        if( request_msg.request_id().compare( "LOGIN" ) == 0 )
        {
            //
            //  Ack to sender
            //

            response_msg.Clear();
            response_msg.set_request_id( "LOGIN" );
            quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );

            quantmodel::ZMQMessage::send( &response_msg, socket );
            continue;
        }

        if( request_msg.request_id().compare( "SHUTDOWN" ) == 0 )
        {
            //
            //  Ack to sender
            //

            if( PERSISTING )
            {
                std::vector< std::string > vec;
                vec.push_back( "SHUTDOWN" );
                pthread_mutex_lock( &queue_mutex );
                queue.push_back( vec );
                pthread_mutex_unlock( &queue_mutex );
                sleep( 2 );
            }

            response_msg.Clear();
            response_msg.set_request_id( "SHUTDOWN" );
            quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
            quantmodel::ZMQMessage::send( &response_msg, socket );
            sleep( 1 );
            exit( 0 );
        }

        //
        //  Log the data that we received to the filesystem. Build the filename
        //

        filename.assign( dir ).append( "/" );
        quantmodel::TimeStamp::getUTCDay( filename, false );
        filename.append( ".log" );

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( "filename: " + filename );
        }

        FILE *file = fopen( filename.c_str(), "a" );

        if(! file )
        {
            log->fatal( "Cannot open file for append: " + filename );
            exit( 1 );
        }

        for( int i=0; i<request_msg.query_size(); ++i )
        {
            const quantmodel::DatabaseEngineRequest::Query& query_req = request_msg.query( i );

            if( fprintf( file, "%s\n", query_req.query_str().c_str() ) < 0 )
            {
                log->fatal( "Cannot append to file: " + filename + " --> " + query_req.query_str() );
                exit( 1 );
            }

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( "   LOG: %s", query_req.query_str().c_str() );
            }
        }

        if( fclose( file ) == EOF )
        {
            log->fatal( "Cannot close file: " + filename );
            exit( 1 );
        }

        //
        //  add query strings to deque if the persistence thread is running.
        //

        if( PERSISTING )
        {
            std::vector< std::string > vec;
            vec.push_back( filename );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( "   VEC: %s", filename.c_str() );
            }

            for( int i=0; i<request_msg.query_size(); ++i )
            {
                const quantmodel::DatabaseEngineRequest::Query& query_req = request_msg.query( i );
                vec.push_back( query_req.query_str() );

                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( "   VEC: %s", query_req.query_str().c_str() );
                }
            }

            pthread_mutex_lock( &queue_mutex );
            queue.push_back( vec );
            pthread_mutex_unlock( &queue_mutex );
        }
        else
        {
            log->warn( "PERSISTENCE THREAD HAS FAILED." );
        }

        //
        //  Ack to sender
        //

        response_msg.Clear();
        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " <-- " + response_msg.ShortDebugString() );
        }

        quantmodel::ZMQMessage::send( &response_msg, socket );
    }

    exit( 0 );
}
