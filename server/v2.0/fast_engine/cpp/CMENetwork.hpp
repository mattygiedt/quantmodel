#ifndef QUICKFAST_CME_NETWORK_H
#define QUICKFAST_CME_NETWORK_H

class CMENetwork;
class CMEProduct;
class CMEChannel;
class CMEConnection;

class CMEProduct
{
    public:

        CMEProduct( const CMEChannel * const ch, const XMLNode& product_node ) :
            channel( ch )
        {
            code = product_node.getAttribute( "code" );

            const XMLNode group_node = product_node.getChildNode( "group" );
            group_code = group_node.getAttribute( "code" );

            if( group_node.nChildNode( "subchannel" ) == 1 )
            {
                group_subchannel = group_node.getChildNode( "subchannel" ).getText();
            }
            else
            {
                group_subchannel = "";
            }
        }

        ~CMEProduct(){}

        inline const CMEChannel* getChannel() const { return channel; }
        inline std::string getProductCode() const { return code; }
        inline std::string getGroupCode() const { return group_code; }
        inline std::string getGroupSubchannel() const { return group_subchannel; }

    private:
        std::string code, group_code, group_subchannel;

        const CMEChannel * const channel;

        CMEProduct (const CMEProduct &);
        CMEProduct & operator= (const CMEProduct &);
};

class CMEConnection
{
    public:

        CMEConnection( const CMEChannel * const ch, const XMLNode& connection_node ) :
            channel( ch )
        {
            last_msg_seq_num = 0L;
            
            if( connection_node.isAttributeSet( "id" ) )
            {
                id = connection_node.getAttribute( "id" );
            }
            else 
            {
                std::cout << "CONNECTION NODE HAS NO ID" << std::endl;
                return;
            }
            
            std::cout << "\t  loading connection: " << id << std::endl;
            
            if( connection_node.nChildNode( "type" ) == 1 )  
            {            
                const XMLNode type_node = connection_node.getChildNode( "type" );
                
                if( type_node.isAttributeSet( "feed-type" ) )
                {
                    feed_type = type_node.getAttribute( "feed-type" );
                }
                else
                {
                    std::cout << "CONNECTION TYPE HAS NO FEED-TYPE" << std::endl;
                }
                
                is_incremental_feed = false;
                feed_name = type_node.getText();        
            }
            else
            {
                std::cout << "CONNECTION NODE HAS NO TYPE" << std::endl;
                return;
            }
           
            if( connection_node.nChildNode( "protocol" ) == 1 &&
                connection_node.getChildNode( "protocol" ).nText() == 1 )
            {
                protocol = connection_node.getChildNode( "protocol" ).getText();
            }
            else
            {
                protocol = "";
            }

            if( connection_node.nChildNode( "ip" ) == 1 &&
                connection_node.getChildNode( "ip" ).nText() == 1 )
            {
                ip = connection_node.getChildNode( "ip" ).getText();
            }
            else
            {
                ip = "";
            }

            if( connection_node.nChildNode( "host-ip" ) == 1 &&
                connection_node.getChildNode( "host-ip" ).nText() == 1)
            {
                host_ip = connection_node.getChildNode( "host-ip" ).getText();
            }
            else
            {
                host_ip = "";
            }

            if( connection_node.nChildNode( "port" ) == 1 && 
                connection_node.getChildNode( "port" ).nText() == 1 )
            {
                port = connection_node.getChildNode( "port" ).getText();
            }
            else
            {
                port = "";
            }

            if( connection_node.nChildNode( "feed" ) == 1 &&
                connection_node.getChildNode( "feed" ).nText() == 1 )
            {
                feed = connection_node.getChildNode( "feed" ).getText();
            }
            else
            {
                feed = "";
            }
        }

        ~CMEConnection(){}

        inline const CMEChannel* getChannel() const { return channel; }
        inline std::string getConnectionId() const { return id; }
        inline std::string getConnectionIP() const { return ip; }
        inline std::string getConnectionProtocol() const { return protocol; }
        inline std::string getConnectionHostIP() const { return host_ip; }
        inline std::string getConnectionPort() const { return port; }
        inline std::string getConnectionFeed() const { return feed; }
        inline std::string getConnectionFeedType() const { return feed_type; }
        inline std::string getConnectionFeedName() const { return feed_name; }
        inline const int getFd() const { return fd; }
        inline void setFd( const int i ) { fd = i; }
        inline void getLastMsgSeqNum( long& seq_num ) const { seq_num = last_msg_seq_num; }
        inline void setLastMsgSeqNum( const long& seq_num ) { last_msg_seq_num = seq_num; }
        inline const bool isIncrementalFeed() const { return is_incremental_feed; }
        inline const bool getLastMsgSeqNum( long& next_seq_num, long& seq_num ) const
        {
            if( next_seq_num <= last_msg_seq_num ) return false;
            seq_num = last_msg_seq_num;
            return true;
        }

    private:

        std::string id, ip, protocol, host_ip, port, feed, feed_type, feed_name, msg_data;
        int fd;
        long last_msg_seq_num;
        const CMEChannel * const channel;
        bool is_incremental_feed;

        CMEConnection (const CMEConnection &);
        CMEConnection & operator= (const CMEConnection &);
};

class CMEChannel
{
    public:

        CMEChannel( const XMLNode& channel_node )
        {
            id = channel_node.getAttribute( "id" );
            label = channel_node.getAttribute( "label" );

            cout << "\tcreating channel: " << label << endl;

            const XMLNode products_node = channel_node.getChildNode( "products" );
            const XMLNode connections_node = channel_node.getChildNode( "connections" );

            //
            //  Iterate over each product
            //

            for( int i=0; i<products_node.nChildNode( "product" ); ++i )
            {
                CMEProduct *cme_product = new CMEProduct( this,
                    products_node.getChildNode( "product", i ) );

                product_map.insert(
                    std::pair< const std::string , CMEProduct * >
                        ( cme_product->getProductCode(), cme_product ) );
            }

            cout << "\t added products..." << endl;

            //
            //  Iterate over each connection
            //

            for( int i=0; i<connections_node.nChildNode( "connection" ); ++i )
            {
                CMEConnection *cme_connection = new CMEConnection( this,
                    connections_node.getChildNode( "connection", i ) );

                connection_map.insert(
                    std::pair< const std::string , CMEConnection * >
                        ( cme_connection->getConnectionId(), cme_connection ) );
            }

            cout << "\t added connections..." << endl;

            //
            //  Set the incremental connection pair
            //

            if(! setIncrementalConnectionPair() )
            {
                fprintf( stderr, "Invalid incremental connection pair." );
                exit( 1 );
            }
        }

        ~CMEChannel()
        {
            std::map< const std::string , CMEProduct * >::const_iterator product_iter;
            std::map< const std::string , CMEConnection * >::const_iterator connection_iter;

            product_iter = product_map.begin();
            while( product_iter != product_map.end() )
            {
                delete product_iter->second;
                ++product_iter;
            }

            connection_iter = connection_map.begin();
            while( connection_iter != connection_map.end() )
            {
                delete connection_iter->second;
                ++connection_iter;
            }
        }

        inline std::string& getChannelId() { return id; }
        inline const std::string getChannelId() const { return id; }
        inline std::string& getChannelLabel() { return label; }
        inline const std::string getChannelLabel() const { return label; }

        CMEProduct* getCMEProduct( const std::string& code ) const
        {
            std::map< const std::string , CMEProduct * >::const_iterator product_iter;

            product_iter = product_map.find( code );
            if( product_iter != product_map.end() )
            {
                return product_iter->second;
            }
            else
            {
                return NULL;
            }
        }

        void getCMEProductCodes( std::vector< std::string >& vec ) const
        {
            std::map< const std::string , CMEProduct * >::const_iterator product_iter;

            product_iter = product_map.begin();
            while( product_iter != product_map.end() )
            {
                vec.push_back( product_iter->first );
                ++product_iter;
            }
        }

        CMEConnection* getCMEConnection( const std::string& id ) const
        {
            std::map< const std::string , CMEConnection * >::const_iterator connection_iter;

            connection_iter = connection_map.find( id );
            if( connection_iter != connection_map.end() )
            {
                return connection_iter->second;
            }
            else
            {
                return NULL;
            }
        }

        void getCMEConnectionIds( std::vector< std::string >& vec ) const
        {
            std::map< const std::string , CMEConnection * >::const_iterator connection_iter;

            connection_iter = connection_map.begin();
            while( connection_iter != connection_map.end() )
            {
                vec.push_back( connection_iter->first );
                ++connection_iter;
            }
        }

        inline void getIncrementalConnectionPair( CMEConnection **pair_arr ) const
        {
            pair_arr[ 0 ] = incremental_pair[ 0 ]; pair_arr[ 1 ] = incremental_pair[ 1 ];
        }

        inline void setLastMsgSeqNum( const long& seq_num ) const
        {
            incremental_pair[ 0 ]->setLastMsgSeqNum( seq_num );
            incremental_pair[ 1 ]->setLastMsgSeqNum( seq_num );
        }

    private:
        std::string id, label;
        CMEConnection * incremental_pair[ 2 ];
        std::map< const std::string , CMEProduct * > product_map;
        std::map< const std::string , CMEConnection * > connection_map;

        CMEChannel (const CMEChannel &);
        CMEChannel & operator= (const CMEChannel &);

        bool setIncrementalConnectionPair()
        {
            std::map< const std::string , CMEConnection * >::const_iterator connection_iter;

            incremental_pair[ 0 ] = incremental_pair[ 1 ] = NULL;
            connection_iter = connection_map.begin();

            while( connection_iter != connection_map.end() )
            {
                if( connection_iter->second->getConnectionFeedType() == "I" &&
                    connection_iter->second->getConnectionFeed() == "A" )
                {
                    incremental_pair[ 0 ] = connection_iter->second;
                }

                if( connection_iter->second->getConnectionFeedType() == "I" &&
                    connection_iter->second->getConnectionFeed() == "B" )
                {
                    incremental_pair[ 1 ] = connection_iter->second;
                }

                ++connection_iter;
            }

            return incremental_pair[ 0 ] != NULL && incremental_pair[ 1 ] != NULL;
        }

};


class CMENetwork
{
    public:
        CMENetwork( const XMLNode& network_node )
        {
            std::cout << " loading file: " << network_node.getAttribute( "file" ) << endl;

            XMLNode network_config =
                XMLNode::openFileHelper(
                    network_node.getAttribute( "file" ) );

            if( network_config.isDeclaration( ) )
            {
                network_config = network_config.getChildNode( 0 );
            }

            std::cout << "  channel count: " << network_config.nChildNode( "channel" ) << endl;

            //
            //  Load each channel into the channel map
            //

            for( int i=0; i<network_config.nChildNode( "channel" ); i++ )
            {
                CMEChannel *channel = new CMEChannel(
                    network_config.getChildNode( "channel", i ) );

                channel_map.insert(
                    std::pair< const std::string , CMEChannel * >(
                        channel->getChannelId(), channel ) );

                std::cout << "  loaded channel " << channel->getChannelId()
                          << ": " << channel->getChannelLabel() << endl;
            }
        }

        ~CMENetwork()
        {
            std::map< const std::string , CMEChannel * >::iterator channel_iter;

            channel_iter = channel_map.begin();

            while( channel_iter != channel_map.end() )
            {
                delete channel_iter->second;
                ++channel_iter;
            }
        }

        CMEChannel* getCMEChannel( const std::string& id ) const
        {
            std::map< const std::string , CMEChannel * >::const_iterator channel_iter;

            channel_iter = channel_map.find( id );
            if( channel_iter != channel_map.end() )
            {
                return channel_iter->second;
            }
            else
            {
                return NULL;
            }
        }

        void getCMEChannelIds( std::vector< std::string >& vec ) const
        {
            std::map< const std::string , CMEChannel * >::const_iterator channel_iter;

            channel_iter = channel_map.begin();
            while( channel_iter != channel_map.end() )
            {
                vec.push_back( channel_iter->first );
                ++channel_iter;
            }
        }

        inline size_t count() const { return channel_map.size(); }

    private:
        std::map< const std::string , CMEChannel * > channel_map;

        CMENetwork (const CMENetwork &);
        CMENetwork & operator= (const CMENetwork &);
};

#endif // QUICKFAST_CME_NETWORK_H
