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

#include <xmlParser.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

void initLogging( const XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

bool openMDP3Connection( const char* port, const char* addr, const char* interface, int& fd )
{
    int ret, opts;
    struct ip_mreq mreq;
    struct sockaddr_in sock;
    
    fd = 0;
    memset( &mreq, 0, sizeof( mreq ) );
    memset( &sock, 0, sizeof( sock ) );
    
    if( ( fd = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) == -1 )
    {
        std::cout << "ERROR: cannot create fd" << std::endl;
        return false;
    }

    //
    //  Set the network address and bind
    //

    sock.sin_family = AF_INET;
    sock.sin_port = htons( atoi( port ) );
    sock.sin_addr.s_addr = inet_addr( addr );

    //
    //  Allow multiple listeners on the same broadcast address
    //
    //      http://alas.matf.bg.ac.rs/manuals/lspe/snode=115.html
    //

    opts = TRUE;
    ret = setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof( opts ) );
    if( ret < 0 )
    {
        std::cout << "ERROR: cannot reuse to address " << addr << ", ret=" << ret << std::endl;
        close( fd );
        return false;
    }

    //
    //  Bind to UDP port on router
    //

    ret = ::bind( fd, (const sockaddr*)&sock, sizeof( sock ) );

    if( ret < 0 )
    {
        std::cout << "ERROR: cannot bind to address " << addr << ", ret=" << ret << std::endl;
        close( fd );
        return false;
    }

    //
    //  Set the multicast address
    //

    mreq.imr_multiaddr.s_addr = inet_addr( addr );

    if( NULL == interface )
    {
        mreq.imr_interface.s_addr = htonl( INADDR_ANY );
    }
    else
    {
        mreq.imr_interface.s_addr = inet_addr( interface );
    }
    
    //
    //  Join the multicast group
    //
    
    ret = setsockopt( fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof( mreq ) );
    
    if( ret < 0 )
    {
        std::cout << "ERROR: cannot join multicast group, ret=" << ret << std::endl;
        close( fd );
        return false;
    }

    return true;
}

int main( int argc, char *argv[] )
{
    //typedef boost::iostreams::basic_array_source<char> Device;
    //const XMLNode& root = XMLNode::openFileHelper( argv[ 1 ] );
    //const XMLNode& types_node = root.getChildNode( "types" );
    //
    //for( int i=0; i<types_node.nChildNode( "type" ); ++i )
    //{
    //    const XMLNode& type_node = types_node.getChildNode( "type", i );
    //}

    int fd, ret, pos;
    char buf[ 2048 ];
    socklen_t addr_len;
    struct sockaddr_storage their_addr;

    typedef boost::iostreams::basic_array_source<char> MDP3Source;
    const XMLNode& root = XMLNode::openFileHelper( argv[ 1 ] );
    const XMLNode& connection_node = root.getChildNode( "connection" );

    if( openMDP3Connection( 
        connection_node.getAttribute( "port" ),
        connection_node.getAttribute( "address" ),
        connection_node.getAttribute( "interface" ),
        fd ) == false )
    {
        std::cout << "ERROR: cannot open mdp3 connection to: " 
              << connection_node.getAttribute( "address" ) << ":"
              << connection_node.getAttribute( "port" ) << " using interface: " 
              << connection_node.getAttribute( "interface" ) << std::endl;

        return 1;
    }

    addr_len = sizeof( their_addr );

    std::cout << "opened mdp3 connection to: " 
              << connection_node.getAttribute( "address" ) << ":"
              << connection_node.getAttribute( "port" ) << " using interface: " 
              << connection_node.getAttribute( "interface" ) << std::endl;

    while( 1 )
    {
        if( ( ret = recvfrom( fd, buf, 2047, 0, (struct sockaddr *)&their_addr, &addr_len ) ) == -1 )
        {
            std::cout << "recvfrom error: -1" << std::endl;
            close( fd );
            return 1;
        }

        std::cout << "read " << ret << " bytes." << std::endl;
        boost::iostreams::stream<MDP3Source> source( buf, ret );

        //
        //  The MDP3 - Binary Packet Header - first 12 bytes of the message
        //

        uint32_t msgSeqNum;
        uint64_t sendingTime;

        source.read((char*)&msgSeqNum, sizeof(msgSeqNum));
        source.read((char*)&sendingTime, sizeof(sendingTime));
        pos = ( sizeof(msgSeqNum) + sizeof(sendingTime) );

        std::cout << " pos: " << pos 
                  << " msgSeqNum " << msgSeqNum 
                  << ", sendingTime " << sendingTime 
                  << std::endl;

        //
        //  MDP3 - Message Header - each message in the packet starts 
        //  with Binary message header that consists of Binary Size and SBE header
        //
        
        while( pos < ret )
        {
            uint16_t msgSize;
            uint16_t blockLength;
            uint16_t templateId;
            uint8_t version;
            uint8_t reserved;
            
            source.read((char*)&msgSize, sizeof(msgSize));
            source.read((char*)&blockLength, sizeof(blockLength));
            source.read((char*)&templateId, sizeof(templateId));
            source.read((char*)&version, sizeof(version));
            source.read((char*)&reserved, sizeof(reserved));
            pos += ( sizeof(msgSize) + 
                     sizeof(blockLength) + 
                     sizeof(templateId) + 
                     sizeof(version) + 
                     sizeof(reserved) );
            
            std::cout << "  pos: " << pos 
                      << " msgSize " << msgSize 
                      << ", blockLength " << blockLength 
                      << ", templateId " << templateId 
                      << ", version " << static_cast<int16_t>(version)
                      << ", reserved " << static_cast<int16_t>(reserved)
                      << std::endl;
            
            //
            //  msgSize contains length of entire message, including 
            //  binary header (6 bytes) in number of bytes.
            //
            
            if( templateId == 5 )
            {
                //  
                //  <ns2:message name="MDIncrementalRefreshVolume" id="5" description="MDIncrementalRefreshVolume" blockLength="9" semanticType="X">
                //      <field name="TransactTime" id="60" type="uInt64" description="Start of event processing time in number of nanoseconds since Unix epoch" offset="0" semanticType="UTCTimestamp"/>
                //      <field name="MatchEventIndicator" id="5799" type="MatchEventIndicator" description="Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event" offset="8" semanticType="MultipleCharValue"/>
                //      <group name="NoMDEntries" id="268" description="Number of entries in Market Data message" blockLength="13" dimensionType="groupSize">
                //          <field name="MDUpdateAction" id="279" type="MDUpdateAction" description="Market Data update action" offset="0" semanticType="int"/>
                //          <field name="MDEntryType" id="269" type="MDEntryTypeVol" description="Electronic Volume entry provides cumulative session trade volume updated with the event " semanticType="char"/>
                //          <field name="SecurityID" id="48" type="Int32" description="Security ID" offset="1" semanticType="int"/>
                //          <field name="RptSeq" id="83" type="Int32" description="Market Data entry sequence number per instrument update" offset="5" semanticType="int"/>
                //          <field name="MDEntrySize" id="271" type="Int32" description="Cumulative traded volume  " offset="9" semanticType="Qty"/>
                //      </group>
                //  </ns2:message>
                //  
                
                uint64_t transactTime;
                uint8_t matchEventIndicator;
                uint16_t blockLength;
                uint8_t numInGroup;

                source.read((char*)&transactTime, sizeof(transactTime));
                source.read((char*)&matchEventIndicator, sizeof(matchEventIndicator));
                source.read((char*)&blockLength, sizeof(blockLength));
                source.read((char*)&numInGroup, sizeof(numInGroup));
                pos += ( sizeof(transactTime) + 
                         sizeof(matchEventIndicator) + 
                         sizeof(blockLength) + 
                         sizeof(numInGroup) );

                std::cout << "   pos: " << pos 
                          << " transactTime " << transactTime 
                          << ", matchEventIndicator " << static_cast<int16_t>(matchEventIndicator)
                          << ", blockLength " << blockLength 
                          << ", numInGroup " << static_cast<int16_t>(numInGroup)
                          << std::endl;
                
                for( uint8_t i=0; i<numInGroup; ++i )
                {
                    int8_t mdUpdateAction;
                    char mdEntryType = 'e';
                    int32_t securityId;
                    int32_t rptSeq;
                    int32_t mdEntrySize;
                    
                    source.read((char*)&mdUpdateAction, sizeof(mdUpdateAction));
                    source.read((char*)&securityId, sizeof(securityId));
                    source.read((char*)&rptSeq, sizeof(rptSeq));
                    source.read((char*)&mdEntrySize, sizeof(mdEntrySize));
                    
                    pos += ( sizeof(mdUpdateAction) + 
                             sizeof(securityId) + 
                             sizeof(rptSeq) + 
                             sizeof(mdEntrySize) );
                    
                    std::cout << "    pos: " << pos 
                              << " mdEntry " << i
                              << ", mdUpdateAction " << static_cast<int16_t>(mdUpdateAction)
                              << ", mdEntryType " << mdEntryType
                              << ", securityId " << securityId 
                              << ", rptSeq " << rptSeq 
                              << ", mdEntrySize " << mdEntrySize
                              << std::endl;
                }
            }
            else if( templateId == 20 )
            {
                //  
                //  <ns2:message name="MDIncrementalRefreshBook" id="20" description="MDIncrementalRefreshBook" blockLength="9" semanticType="X">
                //      <field name="TransactTime" id="60" type="uInt64" description="Start of event processing time in number of nanoseconds since Unix epoch" offset="0" semanticType="UTCTimestamp"/>
                //      <field name="MatchEventIndicator" id="5799" type="MatchEventIndicator" description="Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event" offset="8" semanticType="MultipleCharValue"/>
                //      <group name="NoMDEntries" id="268" description="Number of entries in Market Data message" blockLength="27" dimensionType="groupSize">
                //          <field name="MDUpdateAction" id="279" type="MDUpdateAction" description=" Market Data update action" offset="0" semanticType="int"/>
                //          <field name="MDEntryType" id="269" type="MDEntryTypeBook" description="Market Data entry type  " offset="1" semanticType="char"/>
                //          <field name="SecurityID" id="48" type="Int32" description="Security ID" offset="2" semanticType="int"/>
                //          <field name="RptSeq" id="83" type="Int32" description="Market Data entry sequence number per instrument update" offset="6" semanticType="int"/>
                //          <field name="MDPriceLevel" id="1023" type="uInt8" description="Aggregate book level" offset="10" semanticType="int"/>
                //          <field name="MDEntryPx" id="270" type="PRICENULL" description="Market Data entry price" offset="11" semanticType="Price"/>
                //          <field name="MDEntrySize" id="271" type="Int32NULL" description="Market Data entry quantity" offset="19" semanticType="Qty"/>
                //          <field name="NumberOfOrders" id="346" type="Int32NULL" description="In Book entry - aggregate number of orders at given price level" offset="23" semanticType="int"/>
                //      </group>
                //  </ns2:message>
                //

                uint64_t transactTime;
                uint8_t matchEventIndicator;
                uint16_t blockLength;
                uint8_t numInGroup;

                source.read((char*)&transactTime, sizeof(transactTime));
                source.read((char*)&matchEventIndicator, sizeof(matchEventIndicator));
                source.read((char*)&blockLength, sizeof(blockLength));
                source.read((char*)&numInGroup, sizeof(numInGroup));
                pos += ( sizeof(transactTime) + 
                         sizeof(matchEventIndicator) + 
                         sizeof(blockLength) + 
                         sizeof(numInGroup) );

                std::cout << "   pos: " << pos 
                          << " transactTime " << transactTime 
                          << ", matchEventIndicator " << static_cast<int16_t>(matchEventIndicator)
                          << ", blockLength " << blockLength 
                          << ", numInGroup " << static_cast<int16_t>(numInGroup)
                          << std::endl;
                
                for( uint8_t i=0; i<numInGroup; ++i )
                {
                    int8_t mdUpdateAction;
                    char mdEntryType;
                    int32_t securityId;
                    int32_t rptSeq;
                    uint8_t mdPriceLevel;
                    int64_t mdEntryPx;
                    int32_t mdEntrySize;
                    int32_t numberOfOrders;
                    
                    source.read((char*)&mdUpdateAction, sizeof(mdUpdateAction));
                    source.read((char*)&mdEntryType, sizeof(mdEntryType));
                    source.read((char*)&securityId, sizeof(securityId));
                    source.read((char*)&rptSeq, sizeof(rptSeq));
                    source.read((char*)&mdPriceLevel, sizeof(mdPriceLevel));
                    source.read((char*)&mdEntryPx, sizeof(mdEntryPx));
                    source.read((char*)&mdEntrySize, sizeof(mdEntrySize));
                    source.read((char*)&numberOfOrders, sizeof(numberOfOrders));
                    pos += ( sizeof(mdUpdateAction) + 
                             sizeof(mdEntryType) + 
                             sizeof(securityId) + 
                             sizeof(rptSeq) + 
                             sizeof(mdPriceLevel) + 
                             sizeof(mdEntryPx) + 
                             sizeof(mdEntrySize) + 
                             sizeof(numberOfOrders) );
                    
                    std::cout << "    pos: " << pos 
                              << " mdEntry " << static_cast<int16_t>(i)
                              << ", mdUpdateAction " << static_cast<int16_t>(mdUpdateAction)
                              << ", mdEntryType " << mdEntryType
                              << ", securityId " << securityId 
                              << ", rptSeq " << rptSeq 
                              << ", mdPriceLevel " << static_cast<int16_t>(mdPriceLevel)
                              << ", mdEntryPx " << mdEntryPx
                              << ", mdEntrySize " << mdEntrySize
                              << ", numberOfOrders " << numberOfOrders
                              << std::endl;
                }
            }
            else if( templateId == 23 )
            {
                //  
                //  <ns2:message name="MDIncrementalRefreshTrade" id="23" description="MDIncrementalRefreshTrade" blockLength="9" semanticType="X">
                //      <field name="TransactTime" id="60" type="uInt64" description="Start of event processing time in number of nanoseconds since Unix epoch" offset="0" semanticType="UTCTimestamp"/>
                //      <field name="MatchEventIndicator" id="5799" type="MatchEventIndicator" description="Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event" offset="8" semanticType="MultipleCharValue"/>
                //      <group name="NoMDEntries" id="268" description="Number of entries in Market Data message" blockLength="30" dimensionType="groupSize">
                //          <field name="MDUpdateAction" id="279" type="MDUpdateAction" description="Market Data update action" offset="0" semanticType="int"/>
                //          <field name="MDEntryType" id="269" type="MDEntryTypeTrade" description="Market Data entry type" semanticType="char"/>
                //          <field name="SecurityID" id="48" type="Int32" description="Security ID" offset="1" semanticType="int"/>
                //          <field name="RptSeq" id="83" type="Int32" description="Market Data entry sequence number per instrument update" offset="5" semanticType="int"/>
                //          <field name="MDEntryPx" id="270" type="PRICE" description="Trade price" offset="9" semanticType="Price"/>
                //          <field name="MDEntrySize" id="271" type="Int32" description="Trade quantity" offset="17" semanticType="Qty"/>
                //          <field name="NumberOfOrders" id="346" type="Int32NULL" description="Total number of real orders per instrument that participated in the trade" offset="21" semanticType="int"/>
                //          <field name="TradeID" id="1003" type="Int32" description="Unique Trade ID per instrument and Trading Date" offset="25" semanticType="int"/>
                //          <field name="AggressorSide" id="5797" type="AggressorSide" description="Indicates aggressor side in the trade, if value is 0 then there is no aggressor" offset="29" semanticType="int"/>
                //      </group>
                //  </ns2:message>
                //

                uint64_t transactTime;
                uint8_t matchEventIndicator;
                uint16_t blockLength;
                uint8_t numInGroup;

                source.read((char*)&transactTime, sizeof(transactTime));
                source.read((char*)&matchEventIndicator, sizeof(matchEventIndicator));
                source.read((char*)&blockLength, sizeof(blockLength));
                source.read((char*)&numInGroup, sizeof(numInGroup));
                pos += ( sizeof(transactTime) + 
                         sizeof(matchEventIndicator) + 
                         sizeof(blockLength) + 
                         sizeof(numInGroup) );

                std::cout << "   pos: " << pos 
                          << " transactTime " << transactTime 
                          << ", matchEventIndicator " << static_cast<int16_t>(matchEventIndicator)
                          << ", blockLength " << blockLength 
                          << ", numInGroup " << static_cast<int16_t>(numInGroup)
                          << std::endl;
                
                for( uint8_t i=0; i<numInGroup; ++i )
                {
                    int8_t mdUpdateAction;
                    char mdEntryType = '2';
                    int32_t securityId;
                    int32_t rptSeq;
                    int64_t mdEntryPx;
                    int32_t mdEntrySize;
                    int32_t numberOfOrders;
                    int32_t tradeId;
                    uint8_t aggressorSide;
                    
                    source.read((char*)&mdUpdateAction, sizeof(mdUpdateAction));
                    source.read((char*)&securityId, sizeof(securityId));
                    source.read((char*)&rptSeq, sizeof(rptSeq));
                    source.read((char*)&mdEntryPx, sizeof(mdEntryPx));
                    source.read((char*)&mdEntrySize, sizeof(mdEntrySize));
                    source.read((char*)&numberOfOrders, sizeof(numberOfOrders));
                    source.read((char*)&tradeId, sizeof(tradeId));
                    source.read((char*)&aggressorSide, sizeof(aggressorSide));
                    pos += ( sizeof(mdUpdateAction) + 
                             sizeof(securityId) + 
                             sizeof(rptSeq) + 
                             sizeof(mdEntryPx) + 
                             sizeof(mdEntrySize) + 
                             sizeof(numberOfOrders) +
                             sizeof(tradeId) + 
                             sizeof(aggressorSide) );
                    
                    std::cout << "    pos: " << pos 
                              << " mdEntry " << static_cast<int16_t>(i)
                              << ", mdUpdateAction " << static_cast<int16_t>(mdUpdateAction)
                              << ", mdEntryType " << mdEntryType
                              << ", securityId " << securityId 
                              << ", rptSeq " << rptSeq 
                              << ", mdEntryPx " << mdEntryPx
                              << ", mdEntrySize " << mdEntrySize
                              << ", numberOfOrders " << numberOfOrders
                              << ", tradeId " << tradeId
                              << ", aggressorSide " << static_cast<int16_t>(aggressorSide)
                              << std::endl;
                }
            }
            else
            {
                //
                //  Advance past the un-handled message. Note that we've already read the entire message header, so we need to back that out.
                //
                 
                boost::iostreams::seek( source, ( msgSize - ( sizeof(msgSize) + sizeof(blockLength) + sizeof(templateId) + sizeof(version) + sizeof(reserved) ) ), std::ios_base::cur );
                pos += ( msgSize - ( sizeof(msgSize) + sizeof(blockLength) + sizeof(templateId) + sizeof(version) + sizeof(reserved) ) );
            }
        }
    }
    
    return 0;
}
