#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <quantmodel.hpp>

#include <Application/QuickFAST.h>
#include <Codecs/Decoder.h>
#include <Codecs/GenericMessageBuilder.h>

#include <CMENetwork.hpp>
#include <CMEDataSource.hpp>
#include <CMEMessageConsumer.hpp>
#include <CMEValueMessageBuilder.hpp>
#include <CMENetworkConnection.h>

#define LOG_DELAY 29;

struct thread_args
{
    const XMLNode* config;
    zmq::context_t* ctx;
};

static inline void getLong( long& l, const unsigned char * const b )
{
    l = 0L;
    l |= b[0] & 0xFF; l <<= 8;
    l |= b[1] & 0xFF; l <<= 8;
    l |= b[2] & 0xFF; l <<= 8;
    l |= b[3] & 0xFF;
}

/*

2012.07.28 -- Bifurcating the generation of market data from the persistence.

static void* collector( void* args )
{
    int rc;
    //time_t now;
    //struct tm *timeinfo;
    std::fstream file;
    log4cpp::Category* log;
    std::string utc_day, last_utc_day, filename, header;

    thread_args* pargs = (struct thread_args *) args;
    log = &log4cpp::Category::getInstance( "collector" );

    //
    //  Load the configured channels to create the filename header
    //

    header = "";

    const XMLNode channels_node = pargs->config->getChildNode( "channels" );
    for( int i=0; i<channels_node.nChildNode( "channel" ); ++i )
    {
        const XMLNode channel_node = channels_node.getChildNode( "channel", i );
        header.append( channel_node.getAttribute( "id" ) );
        header.append( "." );
    }

    //
    //  Set the data directory & open the file
    //

    if( pargs->config->nChildNode( "data" ) == 0 )
    {
        log->fatal( "invalid configuration: missing <data /> node." );
        exit( 0 );
    }

    const XMLNode& data_node = pargs->config->getChildNode( "data" );
    const std::string data_dir = data_node.getAttribute( "dir" );
    quantmodel::TimeStamp::getUTCDay( utc_day );
    last_utc_day = utc_day;

    filename.assign( data_dir )
            .append( "/" )
            .append( header )
            .append( utc_day )
            .append( ".dat" );

    log->info( "opening file: " + filename );
    file.open( filename.c_str(), ios::out | ios::app | ios::binary );

    if( file.fail() )
    {
        log->fatal( "error opening file: " + filename );
        exit( 1 );
    }

    //
    //  Create the zmq inproc receiver
    //

    log->info( "connecting to: inproc://mktdata" );
    zmq::context_t *inproc_ctx = pargs->ctx;
    zmq::socket_t mktdata ( *inproc_ctx, ZMQ_PULL );
    mktdata.connect( "inproc://mktdata" );

    //
    //  Recv forever, until we receive a zero length message, then shutdown
    //

    while( 1 )
    {
        zmq::message_t message;

        if( ( rc = mktdata.recv( &message ) ) == -1 )
        {
            log->fatal( "ZMQ recv error: %d", rc );
            exit( 1 );
        }

        if( likely( message.size() > 0 ) )
        {
            file << message.size();
            file.write( (const char*)message.data(), message.size() );
        }
        else
        {
            log->fatal( "SHUTDOWN" );
            exit( 0 );
        }
    }
}

static void* shutdown( void* args )
{
    log4cpp::Category* log;
    log = &log4cpp::Category::getInstance( "shutdown" );
    thread_args* pargs = (struct thread_args *) args;

    while( true )
    {
        sleep( 60 );

        const time_t now = time( NULL );
        struct tm *timeinfo = gmtime( &now );

        if( unlikely( timeinfo->tm_wday == 6 ) )
        {
            log->fatal( "---- SHUTDOWN INITIATED ----" );

            zmq::context_t *inproc_ctx = pargs->ctx;
            zmq::socket_t shutdown ( *inproc_ctx, ZMQ_PUSH );
            shutdown.connect( "inproc://mktdata" );
            zmq::message_t msg ( 0 );
            shutdown.send( msg );
        }
    }

    return NULL;
}
*/

static void* shutdown( void* args )
{
    log4cpp::Category* log;
    log = &log4cpp::Category::getInstance( "shutdown" );

    while( true )
    {
        sleep( 60 );

        const time_t now = time( NULL );
        struct tm *timeinfo = gmtime( &now );

        if( unlikely( timeinfo->tm_wday == 6 ) )
        {
            log->fatal( "---- SHUTDOWN INITIATED ----" );
            log->fatal( "SHUTDOWN COMPLETE" );
            exit( 0 );
        }
    }

    return NULL;
}

void initLogging( const XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

int main( int argc, char *argv[] )
{
    std::string ts;
    log4cpp::Category* log;
    double recv_tm, send_tm;
    struct timespec timer_1, timer_2;
    time_t next_log_time, curr_log_time;
    std::vector< CMEConnection* > secdef_fd_vec, incremental_fd_vec;
    std::map< const int , CMEConnection* > fd_map;
    std::vector< CMEConnection* >::const_iterator fd_iter;
    quantmodel::FastEngineBroadcast fast_engine_broadcast;
    
    bool debug_messages;
    bool use_value_message_builder;
    
    next_log_time = time( &curr_log_time ) + LOG_DELAY;
    recv_tm = send_tm = 0.0;

    XMLNode config = XMLNode::openFileHelper( argv[ 1 ] );
    //XMLNode config = XMLNode::openFileHelper( "../config/channel.30.xml" );

    //
    //  Setup log4cpp
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "CMEFastEngine" );
    log->warn( "Starting QuantModel CME FIX/FAST Engine v1.0" );

    //
    //  Set debug_messages and use_value_message_builder flags
    //

    if( config.isAttributeSet( "use_value_message_builder" ) )
    {
        use_value_message_builder = strcmp( "true", config.getAttribute( "use_value_message_builder" ) ) == 0;
    }
    else
    {
        use_value_message_builder = false;
    }
    
    if( config.isAttributeSet( "debug_messages" ) )
    {
        debug_messages = strcmp( "true", config.getAttribute( "debug_messages" ) ) == 0;
    }
    else
    {
        debug_messages = false;
    }

    if( use_value_message_builder == true )
    {
        log->info( "use_value_message_builder: true" );
    }
    else
    {
        log->info( "use_value_message_builder: false" );
    }
    
    if( debug_messages == true )
    {
        log->info( "debug_messages: true" );
    }
    else
    {
        log->info( "debug_messages: false" );
    }

    //
    //  Initialize the ZMQ broadcast socket
    //

    fast_engine_broadcast.Clear();
    const XMLNode network_node = config.getChildNode( "network" );
    zmq::context_t ctx( 1 );
    zmq::socket_t broadcast ( ctx, ZMQ_PUB );

    if( network_node.isAttributeSet( "broadcast" ) )
    {
        log->info( "binding ZMQ broadcast to: %s", network_node.getAttribute( "broadcast" ) );

        try
        {
            broadcast.bind( network_node.getAttribute( "broadcast" ) );
        }
        catch( std::exception& e )
        {
            log->error( " bind exception: %s", e.what() );
            exit( 1 );
        }
    }
    else
    {
        log->warn( "broadcast address not set." );
    }

    //
    //  Initialize the ZMQ multicast socket
    //

    if( network_node.isAttributeSet( "multicast" ) )
    {
        log->info( "binding ZMQ multicast to: %s", network_node.getAttribute( "multicast" ) );

        try
        {
            broadcast.bind( network_node.getAttribute( "multicast" ) );
        }
        catch( std::exception& e )
        {
            log->error( " bind exception: %s", e.what() );
            exit( 1 );
        }
    }
    else
    {
        log->warn( "multicast address not set." );
    }

    //
    //  Load the FAST templates.
    //

    const XMLNode template_node = config.getChildNode( "template" );

    std::ifstream templates ( template_node.getAttribute( "file" ) );

    if(! templates.good() )
    {
        log->fatal( "ERROR: Can't open CME template file: %s", template_node.getAttribute( "file" ) );
        return 1;
    }

    log->info( "templates loaded from: %s", template_node.getAttribute( "file" ) );

    //
    //  Create the QuickFAST wrappers
    //

    fast_engine::CMEDataSource source;
    fast_engine::CMEMessageConsumer consumer;
    fast_engine::CMEValueMessageBuilder valueBuilder;

    //
    //  Create the QuickFAST IOC objects
    //

    QuickFAST::Codecs::XMLTemplateParser parser;
    QuickFAST::Codecs::Decoder decoder( parser.parse( templates ) );
    QuickFAST::Codecs::GenericMessageBuilder builder( consumer );

    //
    //  Set the interface IP address
    //

    const char* interface;
    if( config.isAttributeSet( "interface" ) )
    {
        log->info( "using network interface: %s", config.getAttribute( "interface" ) );
        interface = config.getAttribute( "interface" );
    }
    else
    {
        log->info( "using default network interface" );
        interface = NULL;
    }

    //
    //  Load the CME Network channels
    //

    std::vector< std::string > channel_vec;
    std::vector< std::string >::const_iterator channel_iter;
    const XMLNode channels_node = config.getChildNode( "channels" );

    CMENetwork network ( channels_node );
    network.getCMEChannelIds( channel_vec );

    log->info( "loaded %ld channels from: %s", channel_vec.size(), channels_node.getAttribute( "file" ) );
    for( channel_iter = channel_vec.begin();
         channel_iter != channel_vec.end();
         ++channel_iter )
    {
        CMEChannel* channel = network.getCMEChannel( (*channel_iter) );
        log->info( " %s: %s", channel->getChannelId().c_str(), channel->getChannelLabel().c_str() );
    }

    //
    //  Iterate through each network channel
    //

    for( channel_iter = channel_vec.begin();
         channel_iter != channel_vec.end();
         ++channel_iter )
    {
        CMEChannel* channel = network.getCMEChannel( (*channel_iter) );

        //
        //  Iterate through each channels node, try and match against this
        //  channel id.
        //

        for( int i=0; i<channels_node.nChildNode( "channel" ); ++i )
        {
            const XMLNode channel_node = channels_node.getChildNode( "channel", i );
            if( channel->getChannelId().compare( channel_node.getAttribute( "id" ) ) == 0 )
            {
                if( strcmp( "true", channel_node.getAttribute( "active" ) ) == 0 )
                {
                    log->info( "  opening channel %s: %s",
                        channel->getChannelId().c_str(),
                        channel->getChannelLabel().c_str() );

                    std::vector< std::string > string_vec;
                    std::vector< std::string >::const_iterator string_iter;

                    channel->getCMEProductCodes( string_vec );

                    for( string_iter = string_vec.begin();
                         string_iter != string_vec.end();
                         ++string_iter )
                    {
                        log->info( "   product: %s", channel->getCMEProduct( *string_iter )->getProductCode().c_str() );
                    }

                    string_vec.clear();
                    channel->getCMEConnectionIds( string_vec );

                    for( string_iter = string_vec.begin();
                         string_iter != string_vec.end();
                         ++string_iter )
                    {
                        CMEConnection *conn =
                            channel->getCMEConnection(
                                (*string_iter) );

                        //
                        //  2011.03.05 -- now that we load our security
                        //  definitions from the XML file, we no longer need
                        //  to listen to the instrument definition channel(s).
                        //
                        //  The functionality will remain in place, but we now
                        //  ONLY connect to the incremental feeds.
                        //

                        if( conn->getConnectionFeedType().compare( "I" ) != 0 )
                        {
                            continue;
                        }

                        //
                        //  Open connection with SO_TIMESTAMP disabled.
                        //

                        if( openCMEConnection( interface, conn, log, false ) )
                        {
                            if( conn->getConnectionFeedType().compare( "N" ) == 0 )
                            {
                                secdef_fd_vec.push_back( conn );
                            }
                            else if( conn->getConnectionFeedType().compare( "I" ) == 0 )
                            {
                                incremental_fd_vec.push_back( conn );
                            }
                            else
                            {
                                log->warn( "UNKNOWN CONNECTION FEED-TYPE %s %s",
                                    conn->getChannel()->getChannelLabel().c_str(),
                                    conn->getConnectionFeedType().c_str() );

                                continue;
                            }

                            fd_map.insert(
                                std::pair< const int , CMEConnection * >
                                    ( conn->getFd(), conn ) );
                        }
                        else
                        {
                            log->fatal( "Unable to open CME Market Data Interface." );
                            exit( 1 );
                        }
                    }
                }
                else
                {
                    log->info( "   skipping channel %s: %s",
                        channel->getChannelId().c_str(),
                        channel->getChannelLabel().c_str() );
                }
            }
        }
    }

    //
    //  Start the shutdown thread
    //

    log->info( "starting shutdown thread..." );

    pthread_t shutdown_thread;
    if( pthread_create( &shutdown_thread, NULL, shutdown, NULL ) )
    {
        log->fatal( "Failed to create shutdown thread." );
        exit( 1 );
    }

    sleep( 1 );

/*
    //
    //  Start the market data collector thread
    //
    //      2012.07.28 -- this is now handled by CMEFastDataCollector
    //

    log->info( "starting marketdata collector thread..." );

    pthread_t collector_thread;
    if( pthread_create( &collector_thread, NULL, collector, &args ) )
    {
        log->fatal( "Failed to create marketdata collector thread." );
        exit( 1 );
    }

    sleep( 1 );
*/


/*
    //
    //  EDGE TRIGGERED EPOLL + RECVMMSG -- INCREMENTAL SECTION
    //
    //      DOES NOT PULL FROM SECOND SFD!!!
    //

    if( incremental_fd_vec.size() > 0 )
    {
        CMEConnection *conn;
        const int batch_size = 16;
        const int buf_size = 2048;

        char buf[ batch_size ][ buf_size ];
        struct iovec iovec[ batch_size ][ 1 ];
        struct sockaddr addr[ batch_size ];
        struct mmsghdr datagrams[ batch_size ];

        long next_seq_num, last_seq_num;
        static struct epoll_event ev;
        struct epoll_event events[ incremental_fd_vec.size() ];
        int i, ret, sfd, epfd, nfds;
        unsigned long int msg_count, pkt_count, total_msg_count;

        msg_count = pkt_count = total_msg_count = 0;
        const size_t incremental_fd_vec_size = incremental_fd_vec.size();

        clock_getres( CLOCK_PROCESS_CPUTIME_ID, &timer_1 );

        log->info( "incremental backend messaging subsystem: EDGE TRIGGERED EPOLL + RECVMMSG" );
        log->info( " CLOCK_PROCESS_CPUTIME_ID resolution: %ld ns", timer_1.tv_nsec );
        log->info( " incremental_fd_vec.size(): %ld", incremental_fd_vec.size() );

        //
        //  Set the epoll parameters
        //

        epfd = epoll_create( incremental_fd_vec.size() );

        //
        //  Create the epfd
        //

        for( fd_iter = incremental_fd_vec.begin(); fd_iter != incremental_fd_vec.end(); ++fd_iter )
        {
            log->info( "  adding incremental fd[ %d ] to epfd", (*fd_iter)->getFd() );

            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = (*fd_iter)->getFd();

            if( epoll_ctl( epfd, EPOLL_CTL_ADD, (*fd_iter)->getFd(), &ev ) == -1 )
            {
                log->fatal( "epoll_ctl EPOLL_CTL_ADD error:" );
                exit( 1 );
            }
        }

        //
        //  Initialize the iovec and datagrams array
        //

        log->info( " recvmmsg batch size: %d", batch_size );

        for( i = 0; i < batch_size; ++i )
        {
            iovec[ i ][ 0 ].iov_base = buf[ i ];
            iovec[ i ][ 0 ].iov_len  = sizeof( buf[ i ] );
            datagrams[ i ].msg_hdr.msg_iov = iovec[ i ];
            datagrams[ i ].msg_hdr.msg_iovlen = 1;
            datagrams[ i ].msg_hdr.msg_name = &addr[ i ];
            datagrams[ i ].msg_hdr.msg_namelen = sizeof( addr[ i ] );
        }

        log->info( "CMEFastEngine configuration complete." );

        while( 1 )
        {
            //
            //  epoll and wait forever until signalled
            //

            nfds = epoll_wait( epfd, events, incremental_fd_vec_size, -1 );

            if( unlikely( nfds < 0 && errno != EINTR ) )
            {
                log->fatal( "epoll_wait error: %s", strerror( errno ) );
                exit( 1 );
            }

            //
            //  Loop through each signalled file descriptor
            //

            for( i=0; i<nfds; ++i )
            {
                msg_count = 0;
                consumer.reset();
                sfd = events[ i ].data.fd;
                conn = fd_map[ sfd ];

                std::cout << "sfd: " << sfd << std::endl;

                clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &timer_1 );

                //
                //  Call new recvmmsg API allowing the kernel to transfer up
                //  to batch_size UDP messages in one go. Continue to do this until
                //  all messages have been consumed. (edge triggered)
                //

                while( ( ret = recvmmsg( sfd, datagrams, batch_size, MSG_DONTWAIT, 0 ) ) > 0 )
                {
                    pkt_count += ret;

                    //
                    //  Loop through each datagram message
                    //

                    for( i = 0; i < ret; ++i )
                    {
                        unsigned char *dgram = (unsigned char*)datagrams[ i ].msg_hdr.msg_iov->iov_base;
                        const unsigned int dgram_len = datagrams[ i ].msg_len;

                        if( dgram_len >= 5 )
                        {
                            //
                            //  Get the preamble number for this incremental message
                            //

                            next_seq_num = 0L;
                            next_seq_num |= dgram[ 0 ] & 0xFF; next_seq_num <<= 8;
                            next_seq_num |= dgram[ 1 ] & 0xFF; next_seq_num <<= 8;
                            next_seq_num |= dgram[ 2 ] & 0xFF; next_seq_num <<= 8;
                            next_seq_num |= dgram[ 3 ] & 0xFF;

                            //log->warn( "recv: %s %ld", conn->getConnectionId().c_str(), next_seq_num );

                            //
                            //  Get the last preamble number for this connection and
                            //  at the same time check to see if we need to process it.
                            //

                            if( conn->getLastMsgSeqNum( next_seq_num, last_seq_num ) )
                            {
                                //log->warn( "handl: %ld", next_seq_num );
                                //
                                //  Increment our msg_count
                                //

                                ++msg_count;

                                //
                                //  Skip the CME's preamble when decoding
                                //  the FAST message
                                //

                                decoder.reset();
                                source.setBuffer( dgram + 5, dgram_len - 5 );

                                try
                                {
                                    //
                                    //  Decode the FIX/FAST message
                                    //

                                    if( likely( use_value_message_builder == true ) )
                                    {
                                        decoder.decodeMessage( source, valueBuilder );
                                        valueBuilder.getFastEngineBroadcast( fast_engine_broadcast );
                                    }
                                    else
                                    {
                                        decoder.decodeMessage( source, builder );
                                    }

                                    //
                                    //  Set the last preamble sequence number for both
                                    //  incremental connection feeds
                                    //

                                    conn->getChannel()->setLastMsgSeqNum( next_seq_num );

                                    //
                                    //  Ensure that we're only one sequence number
                                    //  away from the last_seq_num for this channel.
                                    //

                                    if( unlikely( next_seq_num != last_seq_num + 1 ) )
                                    {
                                        if( last_seq_num != 0 )
                                        {
                                            log->warn( "POSSIBLE MESSAGE LOSS: next_seq_num %ld : last_seq_num: %ld, feed: %s %s",
                                                next_seq_num, last_seq_num,
                                                conn->getChannel()->getChannelLabel().c_str(),
                                                conn->getConnectionId().c_str() );
                                        }
                                    }
                                }
                                catch( std::exception& e )
                                {
                                    log->error( "decoder.decodeMessage exception: %s", e.what() );
                                    log->error( " seq_num %ld : feed: %s %s", next_seq_num,
                                        conn->getChannel()->getChannelLabel().c_str(),
                                        conn->getConnectionId().c_str() );
                                }

                                //
                                //  Send the fast message to the market data collector
                                //

                                //zmq::message_t msg ( dgram_len );
                                //memcpy( msg.data(), dgram, dgram_len );
                                //mktdata.send( msg );
                            }
                            else
                            {
                                std::cout << "duplicate seq_num: " << next_seq_num << std::endl;
                            }
                        }
                    }
                }

                clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &timer_2 );
                recv_tm += quantmodel::TimeStamp::nano_diff( timer_2, timer_1 );

                //if( unlikely( errno != EAGAIN || errno != EWOULDBLOCK ) )
                //{
                //    perror( "recvmmsg" );
                //    log->fatal( "recvmmsg error: %s", strerror( errno ) );
                //    exit( 1 );
                //}

                //
                //  Broadcast all book changes held in the message consumer
                //

                if( msg_count > 0 )
                {
                    total_msg_count += msg_count;

                    //log->info( " recv nano_diff: %ld", quantmodel::TimeStamp::nano_diff( timer_2, timer_1 ) );

                    if( likely( use_value_message_builder == true ) )
                    {
                        //std::cout << fast_engine_broadcast.timestamp()
                        //          << ": " << fast_engine_broadcast.cme_market_data_size() << std::endl;

                        if( likely( fast_engine_broadcast.cme_market_data_size() > 0 ) )
                        {
                            //
                            //  Publish the fast engine broadcast
                            //

                            quantmodel::ZMQMessage::send( &fast_engine_broadcast, &broadcast );
                        }

                        fast_engine_broadcast.Clear();
                    }
                    else
                    {
                        //
                        //  broadcast the messages stored in the message consumer
                        //

                        consumer.broadcast( &broadcast );
                    }

                    clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &timer_1 );
                    send_tm += quantmodel::TimeStamp::nano_diff( timer_1, timer_2 );

                    //log->info( " send nano_diff: %ld", quantmodel::TimeStamp::nano_diff( timer_1, timer_2 ) );
                }
            }

            //
            //  Log message statistics, check for shutdown
            //

            if( unlikely( next_log_time < time( &curr_log_time ) ) )
            {
                log->info( "pkt: %d, msg: %d, recv: %.2f, send: %.2f",
                    pkt_count,
                    total_msg_count,
                    ( recv_tm / (double)pkt_count ),
                    ( send_tm / (double)total_msg_count ) );

                next_log_time = curr_log_time + LOG_DELAY;
            }
        }
    }
*/

    //
    //  EDGE TRIGGERED EPOLL + RECVFROM -- INCREMENTAL SECTION
    //

    if( incremental_fd_vec.size() > 0 )
    {
        CMEConnection *conn;
        unsigned char buf[ 2048 ];

        long next_seq_num, last_seq_num;
        static struct epoll_event ev;
        struct epoll_event events[ incremental_fd_vec.size() ];

        struct msghdr msg;
        struct iovec iov;
        struct sockaddr_in sender_addr;

        int i, ret, sfd, epfd, nfds;
        unsigned long int msg_count, pkt_count, snd_count, total_msg_count;

        clock_getres( CLOCK_PROCESS_CPUTIME_ID, &timer_1 );
        log->info( "CLOCK_PROCESS_CPUTIME_ID resolution: %ld ns", timer_1.tv_nsec );
        log->info( "incremental backend messaging subsystem: EDGE TRIGGERED EPOLL" );
        log->info( " incremental_fd_vec.size(): %ld", incremental_fd_vec.size() );

        //
        //  Set the msghdr struct for use in recvmsg
        //

        msg.msg_name = &sender_addr;
        msg.msg_namelen = sizeof( sender_addr );
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_iov->iov_base = buf;
        msg.msg_iov->iov_len = 2048;
        msg.msg_control = 0;
        msg.msg_controllen = 0;
        msg.msg_flags = 0;

        //
        //  Set the epoll parameters
        //

        epfd = epoll_create( incremental_fd_vec.size() );

        //
        //  Create the epfd
        //

        for( fd_iter = incremental_fd_vec.begin(); fd_iter != incremental_fd_vec.end(); ++fd_iter )
        {
            log->info( " adding incremental fd[ %d ] to epfd", (*fd_iter)->getFd() );

            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = (*fd_iter)->getFd();

            if( epoll_ctl( epfd, EPOLL_CTL_ADD, (*fd_iter)->getFd(), &ev ) == -1 )
            {
                log->fatal( "epoll_ctl EPOLL_CTL_ADD error:" );
                exit( 1 );
            }
        }

        msg_count = pkt_count = snd_count = total_msg_count = 0;
        const size_t incremental_fd_vec_size = incremental_fd_vec.size();
        log->info( "CMEFastEngine configuration complete." );

        while( 1 )
        {
            msg_count = 0;

            if( use_value_message_builder == false )
            {
                consumer.reset();
            }

            //
            //  EPOLL SYSTEM CALL
            //

            nfds = epoll_wait( epfd, events, incremental_fd_vec_size, -1 );

            if( nfds < 0 )
            {
                log->fatal( "epoll_wait error:" );
                exit( 1 );
            }

            clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &timer_1 );

            for( i=0; i<nfds; ++i )
            {
                sfd = events[ i ].data.fd;
                conn = fd_map[ sfd ];

                //std::cout << "sfd: " << sfd << std::endl;

                while( ( ret = recvfrom( sfd, buf, 2048, MSG_DONTWAIT, NULL, NULL ) ) != -1 )
                //while( ( ret = recvmsg( sfd, &msg, MSG_DONTWAIT ) ) != -1 )
                {
                    ++pkt_count;

                    if( likely( ret > 5 ) )
                    {
                        //
                        //  Get the preamble number for this incremental message
                        //

                        next_seq_num = 0L;
                        next_seq_num |= buf[ 0 ] & 0xFF; next_seq_num <<= 8;
                        next_seq_num |= buf[ 1 ] & 0xFF; next_seq_num <<= 8;
                        next_seq_num |= buf[ 2 ] & 0xFF; next_seq_num <<= 8;
                        next_seq_num |= buf[ 3 ] & 0xFF;

                        //log->warn( "recv: %s %ld", conn->getConnectionId().c_str(), next_seq_num );

                        //
                        //  Get the last preamble number for this connection and
                        //  at the same time check to see if we need to process it.
                        //

                        if( conn->getLastMsgSeqNum( next_seq_num, last_seq_num ) )
                        {
                            //log->warn( "handl: %ld", next_seq_num );
                            //
                            //  Increment our msg_count
                            //

                            ++msg_count;

                            //
                            //  Skip the CME's preamble when decoding
                            //  the FAST message
                            //

                            decoder.reset();
                            source.setBuffer( buf + 5, ret - 5 );

                            try
                            {
                                //
                                //  Decode the FIX/FAST message
                                //

                                if( likely( use_value_message_builder == true ) )
                                {
                                    decoder.decodeMessage( source, valueBuilder );
                                }
                                else
                                {
                                    decoder.decodeMessage( source, builder );
                                }

                                //
                                //  Set the last preamble sequence number for both
                                //  incremental connection feeds
                                //

                                conn->getChannel()->setLastMsgSeqNum( next_seq_num );

                                //
                                //  Ensure that we're only one sequence number
                                //  away from the last_seq_num for this channel.
                                //

                                if( unlikely( next_seq_num != last_seq_num + 1 ) )
                                {
                                    if( last_seq_num != 0 )
                                    {
                                        log->warn( "POSSIBLE MESSAGE LOSS: next_seq_num %ld : last_seq_num: %ld, feed: %s %s",
                                            next_seq_num, last_seq_num,
                                            conn->getChannel()->getChannelLabel().c_str(),
                                            conn->getConnectionId().c_str() );
                                    }
                                }
                            }
                            catch( std::exception& e )
                            {
                                log->error( "decoder.decodeMessage exception: %s", e.what() );
                                log->error( " seq_num %ld : feed: %s %s", next_seq_num,
                                    conn->getChannel()->getChannelLabel().c_str(),
                                    conn->getConnectionId().c_str() );
                            }
                        }
                        //else
                        //{
                        //    std::cout << "duplicate seq_num: " << next_seq_num << std::endl;
                        //}
                    }
                }
            }

            clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &timer_2 );
            recv_tm += quantmodel::TimeStamp::nano_diff( timer_2, timer_1 );

            //
            //  Broadcast all book changes held in the message consumer
            //

            if( msg_count > 0 )
            {
                ++snd_count;
                total_msg_count += msg_count;

                //log->info( " recv nano_diff: %ld", quantmodel::TimeStamp::nano_diff( timer_2, timer_1 ) );

                if( likely( use_value_message_builder == true && 
                            valueBuilder.getFastEngineBroadcast( fast_engine_broadcast ) ) )
                {
                    //std::cout << fast_engine_broadcast.timestamp()
                    //          << ": " << fast_engine_broadcast.cme_market_data_size() << std::endl;

                    //
                    //  Publish the fast engine broadcast
                    //

                    quantmodel::ZMQMessage::send( &fast_engine_broadcast, &broadcast );
                    
                    if( debug_messages == true )
                    {
                        log->info( fast_engine_broadcast.DebugString() );
                    }
                    
                    fast_engine_broadcast.Clear();
                }
                else
                {
                    //
                    //  broadcast the messages stored in the message consumer
                    //

                    consumer.broadcast( &broadcast );
                }

                clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &timer_1 );
                send_tm += quantmodel::TimeStamp::nano_diff( timer_1, timer_2 );

                //log->info( " send nano_diff: %ld", quantmodel::TimeStamp::nano_diff( timer_1, timer_2 ) );
            }

            //
            //  Log message statistics
            //

            if( unlikely( next_log_time < time( &curr_log_time ) ) )
            {
                next_log_time = curr_log_time + LOG_DELAY;

                if( pkt_count > 0 && snd_count > 0 )
                {
                    log->info( "pkt: %d, msg: %d, recv: %.2f, send: %.2f",
                        pkt_count,
                        total_msg_count,
                        ( recv_tm / (double)pkt_count ),
                        ( send_tm / (double)snd_count ) );
                }
            }
        }
    }

    return 0;
}

/*
    //
    //  SELECT
    //

    if( fd_vec.size() > 0 )
    {
        //
        //  Setup our select file descriptor set
        //

        int ret;
        char buf[ 2048 ];
        socklen_t addr_len;
        static fd_set fdset;
        struct sockaddr_storage their_addr;

        addr_len = sizeof( their_addr );

        while( 1 )
        {
            FD_ZERO( &fdset );

            for( fd_iter = fd_vec.begin(); fd_iter != fd_vec.end(); ++fd_iter )
            {
                FD_SET( (*fd_iter)->getFd(), &fdset );
            }

            if( ( ret = select( fd_vec.back()->getFd() + 1, &fdset, NULL, NULL, NULL ) ) == -1 )
            {
                printf( "select error: %d", ret );
                exit( 1 );
            }
            else
            {
                printf( " select returns: %d\n", ret );

                for( fd_iter = fd_vec.begin(); fd_iter != fd_vec.end(); ++fd_iter )
                {
                    if( FD_ISSET( (*fd_iter)->getFd(), &fdset ) )
                    {
                        printf( " fd [ %d ] selected\n", (*fd_iter)->getFd() );

                        if( ( ret = recvfrom( (*fd_iter)->getFd(), buf, 2047, 0,
                            (struct sockaddr *)&their_addr, &addr_len ) ) == -1 )
                        {
                            perror( "recvfrom" );
                            exit( 1 );
                        }

                        printf( "  recvfrom: %d bytes\n", ret );
                    }
                }
            }
        }
    }
*/

/*
    //
    //  ZMQ POLL
    //

    if( fd_vec.size() > 0 )
    {
        //
        //  Create a ZMQ pollset from the fd_vec
        //

        int ret;
        zmq_pollitem_t *items = new zmq_pollitem_t [ fd_vec.size() ];

        for( size_t pos = 0; pos < fd_vec.size(); pos++ )
        {
            items[ pos ].socket = NULL;
            items[ pos ].fd = fd_vec[ pos ]->getFd();
            items[ pos ].events = ZMQ_POLLIN;
        }

        //
        //  Poll for market data events indefinitely
        //

        while( 1 )
        {
            const int rc = zmq_poll( items, fd_vec.size(), -1 );

            printf( " zmq_poll returns: %d\n", rc );

            if( rc >= 0 )
            {
                fprintf( stderr, "zmq_poll returned %d\n", rc );
                break;
            }

            for( size_t i = 0; i < fd_vec.size(); i++ )
            {
                if( items[ i ].revents & ZMQ_POLLIN )
                {
                    ret = fd_vec[ i ]->recvMsg();

                    if( ret > 0 )
                    {
                        printf( " read %d bytes from channel %s\n",
                            ret, fd_vec[ i ]->getConnectionFeedName().c_str() );

                        fd_vec[ i ]->getMsgData().clear();
                    }
                    else if( ret == 0  )
                    {

                    }
                    else
                    {

                    }
                }
            }
        }
    }
*/
