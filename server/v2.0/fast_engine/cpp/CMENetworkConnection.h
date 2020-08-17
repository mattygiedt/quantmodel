


bool openCMEConnection( const char* interface, CMEConnection *connection, log4cpp::Category* log, bool use_so_timestamp = false )
{
    socklen_t len;
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

    if( ( connection->getConnectionFeedType().compare( "N" ) == 0 ||
          connection->getConnectionFeedType().compare( "I" ) == 0 ) == false )
    {
        return false;
    }

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

        if( use_so_timestamp == true )
        {
            //
            //  Have kernel populate packet arrival time with SO_TIMESTAMP.
            //

            opts = TRUE;
            ret = setsockopt( fd, SOL_SOCKET, SO_TIMESTAMP, &opts, sizeof( opts ) );
            if( ret < 0 )
            {
                log->error( "setsockopt - SO_TIMESTAMP error: %s", strerror( errno ) );
                close( fd );
                return false;
            }
            else
            {
                log->info( " setsockopt - SO_TIMESTAMP: ret=%d", ret );
            }
        }

        //
        //  Print out the SO_RCVBUF / SO_SNDBUF lengths
        //

        opts = 1;
        ret = getsockopt( fd, SOL_SOCKET, SO_RCVBUF, &opts, &len );
        if( ret == 0 )
        {
            log->info( " SO_RCVBUF size: %d", opts );
        }

        opts = 1;
        ret = getsockopt( fd, SOL_SOCKET, SO_SNDBUF, &opts, &len );
        if( ret == 0 )
        {
            log->info( " SO_SNDBUF size: %d", opts );
        }
    }
    else
    {
        //
        //  Connect to hostname
        //

        if( connect( fd, (const sockaddr*)&sock, sizeof( sock ) ) < 0 )
        {
            log->error( "connect error: %s", strerror( errno ) );
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
        log->error( "fcntl( fd, GETFL ): %d", opts );
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