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
#include <CMESettlementConsumer.hpp>

static inline void getLong( long& l, const unsigned char *b )
{
    l = 0L;
    l |= b[0] & 0xFF; l <<= 8;
    l |= b[1] & 0xFF; l <<= 8;
    l |= b[2] & 0xFF; l <<= 8;
    l |= b[3] & 0xFF;
}

bool openCMEConnection( const char* interface, CMEConnection *connection, log4cpp::Category* log )
{
    int fd, ret, opts;
    struct ip_mreq mreq;
    struct sockaddr_in sock;

    memset( &mreq, 0, sizeof( mreq ) );
    memset( &sock, 0, sizeof( sock ) );

    log->info( "   attempting %s %s connection to: %s:%s",
        connection->getConnectionProtocol().c_str(),
        connection->getConnectionFeedName().c_str(),
        connection->getConnectionIP().c_str(),
        connection->getConnectionPort().c_str() );

    if( connection->getConnectionProtocol().compare( "TCP/IP" ) == 0 )
    {
        //
        //  FOR RIGHT NOW, DON'T OPEN ANY TCP/IP PORTS!!!
        //

        return false;

        //if( ( fd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP ) ) == -1 )
        //{
        //    fprintf( stderr, "unable to create TCP socket file descriptor\n" );
        //    return false;
        //}
    }
    else if( connection->getConnectionProtocol().compare( "UDP/IP" ) == 0 )
    {
        if( ( fd = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) == -1 )
        {
            log->warn( "unable to create UDP socket file descriptor" );
            return false;
        }
    }
    else
    {
        log->warn( "unknown connection protocol: %s",
            connection->getConnectionProtocol().c_str() );

        return false;
    }

    //
    //  Set the network address and bind
    //

    sock.sin_family = AF_INET;
    sock.sin_port = htons( atoi( connection->getConnectionPort().c_str( ) ) );
    sock.sin_addr.s_addr = inet_addr( connection->getConnectionIP().c_str( ) );

    if( connection->getConnectionProtocol().compare( "UDP/IP" ) == 0 )
    {
        //
        //  Allow multiple listeners on the same broadcast address
        //
        //      http://alas.matf.bg.ac.rs/manuals/lspe/snode=115.html
        //

        opts = TRUE;
        ret = setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof( opts ) );
        if( ret < 0 )
        {
            log->error( "setsockopt - SO_REUSEADDR error: %s", strerror( errno ) );
            close( fd );
            return false;
        }
        else
        {
            log->info( " setsockopt - SO_REUSEADDR: ret=%d", ret );
        }

        //
        //  Bind to UDP port on router
        //

        ret = ::bind( fd, (const sockaddr*)&sock, sizeof( sock ) );

        if( ret < 0 )
        {
             log->error( "bind error: %s", strerror( errno ) );
             close( fd );
             return false;
        }
        else
        {
            log->info( " bind %s %s, ret=%d",
                connection->getConnectionIP().c_str( ),
                connection->getConnectionPort().c_str( ),
                ret );
        }

        //
        //  Set the multicast address
        //

        log->info( " mreq.imr_multiaddr.s_addr = %s", connection->getConnectionIP().c_str( ) );
        mreq.imr_multiaddr.s_addr = inet_addr( connection->getConnectionIP().c_str( ) );

        if( NULL == interface )
        {
            log->info( " setsockopt - IP_ADD_MEMBERSHIP: INADDR_ANY" );
            mreq.imr_interface.s_addr = htonl( INADDR_ANY );
        }
        else
        {
            log->info( " setsockopt - IP_ADD_MEMBERSHIP: %s", interface );
            mreq.imr_interface.s_addr = inet_addr( interface );
        }

        ret = setsockopt( fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof( mreq ) );
        if( ret < 0 )
        {
            log->error( "setsockopt - IP_ADD_MEMBERSHIP error: %s", strerror( errno ) );
            close( fd );
            return false;
        }
        else
        {
            log->info( " setsockopt - IP_ADD_MEMBERSHIP: ret=%d", ret );
        }
    }
    else
    {
        //
        //  Connect to hostname
        //

        if( connect( fd, (const sockaddr*)&sock, sizeof( sock ) ) < 0 )
        {
             log->error( "connect" );
             close( fd );
             return false;
        }
    }

    //
    //  Set nonblocking after successful bind
    //

    opts = fcntl( fd, F_GETFL );

    if( opts < 0 )
    {
        log->error( "fcntl( fd, GETFL )" );
        return false;
    }

    opts = opts | O_NONBLOCK;

    if( fcntl( fd, F_SETFL, opts ) < 0 )
    {
        log->error( "fcntl( fd, SETFL, opts )" );
        return false;
    }

    //
    //  Assign the file descriptor to the connection
    //

    log->info( "   file descriptor [ %d ] bound to: %s:%s",
        fd,
        connection->getConnectionIP().c_str(),
        connection->getConnectionPort().c_str() );

    connection->setFd( fd );
    return true;
}

void initLogging( const XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

void initNetwork( const XMLNode& config, zmq::socket_t*& socket, log4cpp::Category* log )
{
    zmq::context_t *data_ctx = new zmq::context_t( 1 );
    const XMLNode& network_node = config.getChildNode( "network" );

    if( network_node.isAttributeSet( "database_callforward" ) )
    {
        log->info( " connecting to database engine callforward: %s", network_node.getAttribute( "database_callforward" ) );
        socket = new zmq::socket_t( *data_ctx, ZMQ_REQ );
        socket->connect( network_node.getAttribute( "database_callforward" ) );
    }
    else
    {
        log->fatal( "database engine callforward is not set" );
        exit( 1 );
    }
}

int main( int argc, char *argv[] )
{
    time_t stop;
    std::string ts;
    zmq::socket_t *socket;
    log4cpp::Category* log;
    std::vector< CMEConnection* > secdef_fd_vec;
    std::map< const int , CMEConnection* > fd_map;
    std::vector< CMEConnection* >::const_iterator fd_iter;

    const XMLNode& config = XMLNode::openFileHelper( argv[ 1 ] );

    //
    //  Setup log4cpp
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "CMESettlementEngine" );
    log->warn( "Starting QuantModel CME FIX/FAST Settlement Engine v1.0" );

    //
    //  Setup ZMQ
    //

    socket = NULL;
    initNetwork( config, socket, log );

    //
    //  Load the FAST templates
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
    fast_engine::CMESettlementConsumer consumer( socket, log );

    //
    //  Load the instruments
    //

    consumer.loadInstrumentMap( config );

    //
    //  Create the QuickFAST IOC objects
    //

    QuickFAST::Codecs::XMLTemplateParser parser;
    QuickFAST::Codecs::Decoder decoder( parser.parse( templates ) );
    QuickFAST::Codecs::GenericMessageBuilder builder( consumer );

    //
    //  Set the interface IP
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
                        //  Connect to the instrument definition channel(s).
                        //

                        if( conn->getConnectionFeedType().compare( "N" ) != 0 )
                        {
                            continue;
                        }

                        if( openCMEConnection( interface, conn, log ) )
                        {
                            secdef_fd_vec.push_back( conn );

                            fd_map.insert(
                                std::pair< const int , CMEConnection * >
                                    ( conn->getFd(), conn ) );
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
    //  Calculate the stop time
    //

    time( &stop );
    stop = stop + 120;

    //
    //  SELECT -- SECURITY DEFINITION SECTION
    //

    if( secdef_fd_vec.size() > 0 )
    {
        //
        //  Setup our select file descriptor set
        //

        int ret;
        unsigned char buf[ 2048 ];
        socklen_t addr_len;
        static fd_set fdset;

        struct sockaddr_storage their_addr;

        addr_len = sizeof( their_addr );

        while( 1 )
        {
            if( stop < time( NULL ) )
            {
                log->warn( "QuantModel CME FIX/FAST Settlement Engine exiting." );
                exit( 0 );
            }

            FD_ZERO( &fdset );

            for( fd_iter = secdef_fd_vec.begin(); fd_iter != secdef_fd_vec.end(); ++fd_iter )
            {
                FD_SET( (*fd_iter)->getFd(), &fdset );
            }

            if( ( ret = select( secdef_fd_vec.back()->getFd() + 1, &fdset, NULL, NULL, NULL ) ) == -1 )
            {
                log->fatal( "select error: %d", ret );
                exit( 1 );
            }
            else
            {
                for( fd_iter = secdef_fd_vec.begin(); fd_iter != secdef_fd_vec.end(); ++fd_iter )
                {
                    if( FD_ISSET( (*fd_iter)->getFd(), &fdset ) )
                    {
                        if( ( ret = recvfrom( (*fd_iter)->getFd(), buf, 2047, 0, (struct sockaddr *)&their_addr, &addr_len ) ) == -1 )
                        {
                            log->fatal( "recvfrom" );
                            exit( 1 );
                        }

                        //
                        //  Skip the CME's preamble when decoding the FAST message
                        //

                        source.setBuffer( buf + 5, ret - 5 );
                        decoder.reset();
                        decoder.decodeMessage( source, builder );
                    }
                }
            }
        }
    }

    return 0;
}
