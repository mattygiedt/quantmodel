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

#include <Application/QuickFAST.h>
#include <Messages/Message.h>
#include <Messages/Field.h>
#include <Messages/FieldIdentity.h>
#include <Messages/Sequence.h>
#include <Messages/Group.h>

#include <Codecs/Decoder.h>
#include <Codecs/DataSource.h>
#include <Codecs/GenericMessageBuilder.h>
#include <Codecs/MessageConsumer.h>
#include <Messages/Sequence.h>

#include <quantmodel.hpp>

#include "CMENetwork.hpp"
#include "CMEDataSource.hpp"
#include "CMEValueMessageBuilder.hpp"

static inline bool is_digit( const char c )
{
    return ( c >= '0' ) && ( c <= '9' );
}

class MessageFormatter
{
    public:
    std::ostream & out_;
    size_t indent_;

    MessageFormatter(std::ostream & out)
      : out_(out)
      , indent_(0)
    {
    }

    ~MessageFormatter()
    {
    }

    void
    formatMessage(const QuickFAST::Messages::Message & message)
    {
      if(!message.getApplicationType().empty())
      {
        out_ << " Type: ";
        if(!message.getApplicationTypeNs().empty())
        {
          out_ << message.getApplicationTypeNs() << '.';
        }
        out_ << message.getApplicationType() << ' ';
      }
      for( QuickFAST::Messages::Message::const_iterator it = message.begin();
        it != message.end();
        ++it)
      {
        const QuickFAST::Messages::FieldIdentityCPtr & identity = it->getIdentity();
        const QuickFAST::Messages::FieldCPtr & field = it->getField();
        QuickFAST::ValueType::Type type = field->getType();
        if(type == QuickFAST::ValueType::SEQUENCE)
        {
          formatSequence(identity, field);
        }
        else if(type == QuickFAST::ValueType::GROUP)
        {
          formatGroup(identity, field);
        }
        else
        {
          out_ << ' ' << identity->name() << '[' << identity->id() << "]=";
          displayFieldValue(field);
        }
      }
      newline();
    }

    void
    newline()
    {
      out_ << std::endl << std::string(indent_ * 2, ' ');
    }

    void
    formatSequence(
      const QuickFAST::Messages::FieldIdentityCPtr & identity,
      const QuickFAST::Messages::FieldCPtr & field)
    {
      QuickFAST::Messages::SequenceCPtr sequence = field->toSequence();
      size_t count = sequence->size();
      newline();
      out_ << ' ' << identity->name() << '[' << identity->id() << "]=";

      const QuickFAST::Messages::FieldIdentityCPtr lengthIdentity = sequence->getLengthIdentity();

      out_ << "Sequence: " << lengthIdentity->name() << '[' << lengthIdentity->id() << ']' << " = " << count << " {";

      size_t entryCount = 0;
      ++indent_;
      for(QuickFAST::Messages::Sequence::const_iterator it = sequence->begin();
        it != sequence->end();
        ++it)
      {
        newline();
        out_ << '[' << entryCount++ << "]: ";

        QuickFAST::Messages::FieldSetCPtr fieldSet = *it;
        for(QuickFAST::Messages::FieldSet::const_iterator fsit = fieldSet->begin();
          fsit != fieldSet->end();
          ++fsit)
        {
          // todo: refactor with message decoding
          const QuickFAST::Messages::FieldIdentityCPtr & identity = fsit->getIdentity();
          const QuickFAST::Messages::FieldCPtr & field = fsit->getField();
          QuickFAST::ValueType::Type type = field->getType();
          if(type == QuickFAST::ValueType::SEQUENCE)
          {
            formatSequence(identity, field);
          }
          else if(type == QuickFAST::ValueType::GROUP)
          {
            formatGroup(identity, field);
          }
          else
          {
            out_ << ' ' << identity->name() << '[' << identity->id() << "]=";
            displayFieldValue(field);
          }
        }
      }
      newline();
      out_ << '}';
      --indent_;
      newline();
    }

    void
    formatGroup(
      const QuickFAST::Messages::FieldIdentityCPtr & identity,
      const QuickFAST::Messages::FieldCPtr & field)
    {
      QuickFAST::Messages::GroupCPtr group = field->toGroup();
      newline();
      out_ << ' ' << identity->name() << '[' << identity->id() << "] Group {"
        << group->getApplicationTypeNs() << ':' << group->getApplicationType()
        << "}= {";
      ++indent_;
      newline();

      for(QuickFAST::Messages::FieldSet::const_iterator fsit = group->begin();
        fsit != group->end();
        ++fsit)
      {
        const QuickFAST::Messages::FieldIdentityCPtr & identity = fsit->getIdentity();
        const QuickFAST::Messages::FieldCPtr & field = fsit->getField();
        QuickFAST::ValueType::Type type = field->getType();
        if(type == QuickFAST::ValueType::SEQUENCE)
        {
          formatSequence(identity, field);
        }
        else if(type == QuickFAST::ValueType::GROUP)
        {
          formatGroup(identity, field);
        }
        else
        {
          out_ << ' ' << identity->name() << '[' << identity->id() << "]=";
          displayFieldValue(field);
        }
      }
      newline();
      out_ << '}';
      --indent_;
      newline();
    }

    void
    displayFieldValue(const QuickFAST::Messages::FieldCPtr & field)
    {
      switch(field->getType())
      {
      case QuickFAST::ValueType::EXPONENT:
      case QuickFAST::ValueType::INT32:
        out_ << field->toInt32();
        break;
      case QuickFAST::ValueType::LENGTH:
      case QuickFAST::ValueType::UINT32:
        out_ << field->toUInt32();
        break;
      case QuickFAST::ValueType::MANTISSA:
      case QuickFAST::ValueType::INT64:
        out_ << field->toInt64();
        break;
      case QuickFAST::ValueType::UINT64:
        out_ << field->toUInt64();
        break;
      case QuickFAST::ValueType::DECIMAL:
        {
          QuickFAST::Decimal d = field->toDecimal();
          std::ostringstream ostr;
          //ostr << field->toDecimal().getMantissa() << " e" << field->toDecimal().getExponent();
          ostr << d.getMantissa() << " E" << (int32_t)d.getExponent();
          out_ << ostr.str();
          break;
        }
      case QuickFAST::ValueType::ASCII:
        {
          out_ << field->toAscii();
          break;
        }
      case QuickFAST::ValueType::UTF8:
        {
          out_ << field->toUtf8();
          break;
        }
      case QuickFAST::ValueType::BYTEVECTOR:
        {
          // todo: we probably should hex dump this
          out_ << field->toByteVector();
          break;
        }
      case QuickFAST::ValueType::SEQUENCE:
        {
          out_ << "sequence";
          break;
        }
      case QuickFAST::ValueType::GROUP:
        {
          static const std::string sts("group");
          out_ << "group";
          break;
        }
      default:
        {
          out_ << "unknown field type";
          break;
        }
      }
    }  
};

bool openCMEConnection( const char* interface, CMEConnection *connection )
{
    int fd, ret, opts;
    struct ip_mreq mreq;
    struct sockaddr_in sock;

    memset( &mreq, 0, sizeof( mreq ) );
    memset( &sock, 0, sizeof( sock ) );

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
            return false;
        }
    }
    else
    {
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
            close( fd );
            return false;
        }

        //
        //  Bind to UDP port on router
        //

        ret = ::bind( fd, (const sockaddr*)&sock, sizeof( sock ) );

        if( ret < 0 )
        {
            close( fd );
            return false;
        }

        //
        //  Set the multicast address
        //

        mreq.imr_multiaddr.s_addr = inet_addr( connection->getConnectionIP().c_str( ) );

        if( NULL == interface )
        {
            mreq.imr_interface.s_addr = htonl( INADDR_ANY );
        }
        else
        {
            mreq.imr_interface.s_addr = inet_addr( interface );
        }

        ret = setsockopt( fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof( mreq ) );
        if( ret < 0 )
        {
            close( fd );
            return false;
        }
    }
    else
    {
        //
        //  Connect to hostname
        //

        if( connect( fd, (const sockaddr*)&sock, sizeof( sock ) ) < 0 )
        {
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
        return false;
    }

    opts = opts | O_NONBLOCK;

    if( fcntl( fd, F_SETFL, opts ) < 0 )
    {
        return false;
    }

    //
    //  Assign the file descriptor to the connection
    //

    connection->setFd( fd );
    return true;
}

class LocalDataSource : public QuickFAST::Codecs::DataSource
{
    public:

        LocalDataSource()
        {
            first = false;
        }

        void setBuffer( unsigned char * msg_buf, const int msg_len )
        {
            first = true;
            reset();
            end = msg_len;
            buf = msg_buf;
        }


        bool getBuffer( const unsigned char *& msg_buf, size_t & msg_len )
        {
            if(! first )
            {
                return false;
            }

            first = false;
            msg_buf = buf;
            msg_len = end;
            return true;
        }

    private:

        unsigned char * buf;
        size_t end;
        bool first;
};

/*
class ImpliedMessageReader : public QuickFAST::Codecs::MessageConsumer
{
    public:

            ImpliedMessageReader( )
            {
                trade_count = implied_trade_count = 0;
                formatter = new MessageFormatter( std::cout );
                quoteConditionIdentity = new QuickFAST::Messages::FieldIdentity( "QuoteCondition" );
            }

            ~ImpliedMessageReader( )
            {
                delete quoteConditionIdentity;
                delete formatter;
            }

            bool consumeMessage( QuickFAST::Messages::Message &msg )
            {
                //
                //  Get the MessageType field
                //

                msg.getField( "MessageType", message_type_field );

                //
                //  Currently we only support incremental updates
                //

                if( (int)message_type_field->toAscii()[ 0 ] == 88 )
                {
                    //
                    //  Process all MDEntries in the message
                    //

                    msg.getField( "MDEntries", entry_field );
                    sequence = entry_field->toSequence();

                    for( sequence_iter = sequence->begin(); sequence_iter != sequence->end(); ++sequence_iter )
                    {
                        field_set = *sequence_iter;
                        field_set->getField( "MDEntryType", entry_type_field );

                        //if( entry_type_field->toAscii() == "2" )
                        //{
                            trade_count += 1;

                            if( field_set->isPresent( *quoteConditionIdentity ) )
                            {
                                field_set->getField( "QuoteCondition", quote_condition_field );

                                if( quote_condition_field->toAscii() == "K" )
                                {
                                    implied_trade_count += 1;
                                    formatter->formatMessage( msg );
                                }
                            }
                        //}
                    }
                }

                return true;
            }

            long getTradeCount() { return trade_count; }
            long getImpliedTradeCount() { return implied_trade_count; }

            bool wantLog( unsigned short ) { return true; }
            bool logMessage( unsigned short , const std::string& ) { return true; }
            bool reportDecodingError( const std::string& ) { return true; }
            bool reportCommunicationError( const std::string& ) { return true; }
            void decodingStarted(){}
            void decodingStopped(){}

    private:

        long trade_count;
        long implied_trade_count;
        ImpliedMessageReader (const ImpliedMessageReader &);
        ImpliedMessageReader & operator= (const ImpliedMessageReader &);

        QuickFAST::Messages::MessageFormatter *formatter;
        QuickFAST::Messages::FieldIdentity *quoteConditionIdentity;
        QuickFAST::Messages::Sequence::const_iterator sequence_iter;

        QuickFAST::Messages::FieldCPtr entry_field;
        QuickFAST::Messages::FieldCPtr entry_type_field;
        QuickFAST::Messages::FieldCPtr message_type_field;
        QuickFAST::Messages::FieldCPtr quote_condition_field;

        QuickFAST::Messages::FieldSetCPtr field_set;
        QuickFAST::Messages::SequenceCPtr sequence;
};
*/


class MessageReader : public QuickFAST::Codecs::MessageConsumer
{
    public:

            MessageReader( )
            {
                formatter = new MessageFormatter( std::cout );
            }

            ~MessageReader( )
            {
                delete formatter;
            }

            bool consumeMessage( QuickFAST::Messages::Message &msg )
            {
                formatter->formatMessage( msg );
                return true;
            }

            bool wantLog( unsigned short ) { return true; }
            bool logMessage( unsigned short , const std::string& ) { return true; }
            bool reportDecodingError( const std::string& ) { return true; }
            bool reportCommunicationError( const std::string& ) { return true; }
            void decodingStarted(){}
            void decodingStopped(){}

    private:

        MessageReader (const MessageReader &);
        MessageReader & operator= (const MessageReader &);
        MessageFormatter* formatter;
};

class NOPMessageReader : public QuickFAST::Codecs::MessageConsumer
{
    public:

            NOPMessageReader( ) { }
            ~NOPMessageReader( ) { }

            bool consumeMessage( QuickFAST::Messages::Message &msg )
            {
                return true;
            }

            bool wantLog( unsigned short ) { return true; }
            bool logMessage( unsigned short , const std::string& ) { return true; }
            bool reportDecodingError( const std::string& ) { return true; }
            bool reportCommunicationError( const std::string& ) { return true; }
            void decodingStarted(){}
            void decodingStopped(){}

    private:

        NOPMessageReader (const NOPMessageReader &);
        NOPMessageReader & operator= (const NOPMessageReader &);
};

int executeFileMessageReader( char *sourceFile, char *templateFile )
{        
    long len;
    char buf[ 2048 ];
    char len_buf[ 8 ];

    std::fstream infile;
    MessageReader reader;
    LocalDataSource source;

    struct timeval kernel_time;
    const size_t sizeof_kernel_time = sizeof( kernel_time );

    std::cout << "FIX/FAST Message Reader: input="
              << sourceFile << ", template=" << templateFile << std::endl;

    std::cout << "sizeof len: " << sizeof( len ) << std::endl;
    std::cout << "sizeof timeval: " << sizeof( timeval ) << std::endl;

    //
    //  Create the QuickFAST IOC objects
    //

    std::ifstream templates ( templateFile );
    QuickFAST::Codecs::XMLTemplateParser parser;
    QuickFAST::Codecs::Decoder decoder( parser.parse( templates ) );
    QuickFAST::Codecs::GenericMessageBuilder genericBuilder( reader );
    fast_engine::CMEValueMessageBuilder valueBuilder;

    //
    //  Open the FIX/FAST market data file
    //

    infile.open( sourceFile, std::fstream::in | std::ifstream::binary );

    while( infile.good() )
    {
        len = 0;

        memset( len_buf, 0, 8 );
        infile.read( len_buf, 8 );
        memcpy( &len, len_buf, 8 );

        if( len == 0 )
        {
            std::cerr << "Invalid length" << std::endl;
            exit( 1 );
        }

        memset( buf, 0, 2048 );
        infile.read( buf, len );
        memcpy( &kernel_time, buf, sizeof_kernel_time );

        //
        //  print the kernel_time
        //
/*
        printf( "[%08d] MSG_LEN: %ld, SO_TIMESTAMP: %ld.%06ld\n",
            ++count,
            len,
            (long)kernel_time.tv_sec,
            (long)kernel_time.tv_usec );
*/
        //
        //  Skip the kernel time and CME's preamble when decoding the FAST message
        //
        //
        //source.setBuffer( (unsigned char*)buf + 5 + sizeof_kernel_time, len - 5 - sizeof_kernel_time );

        try
        {
            //
            //  Decode with QuickFIX GenericMessageBuilder and
            //  our custom ValueMessageBuilder.
            //

            //std::cout << std::endl << "=============================" << std::endl;

            //source.setBuffer( (unsigned char*)buf + 5 + sizeof_kernel_time, len - 5 - sizeof_kernel_time );
            //decoder.reset();
            //decoder.decodeMessage( source, genericBuilder );
            
            //std::cout << buf[ 5 + sizeof_kernel_time ] << std::endl;

            source.setBuffer( (unsigned char*)buf + 5 + sizeof_kernel_time, len - 5 - sizeof_kernel_time );
            decoder.reset();
            decoder.decodeMessage( source, genericBuilder );
            
            source.setBuffer( (unsigned char*)buf + 5 + sizeof_kernel_time, len - 5 - sizeof_kernel_time );
            //source.setEcho( std::cout, QuickFAST::Codecs::DataSource::RAW, true, true );
            decoder.reset();
            decoder.decodeMessage( source, valueBuilder );
            
            quantmodel::FastEngineBroadcast fast_engine_broadcast;
            valueBuilder.getFastEngineBroadcast( fast_engine_broadcast );
            std::cout << fast_engine_broadcast.DebugString() << std::endl;

            //std::cout << std::endl << "=============================" << std::endl;
        }
        catch( std::exception& e )
        {
            std::cerr << "decoder.decodeMessage exception: " << e.what() << std::endl;
        }
    }

    //std::cout << "Total trades: " << reader.getTradeCount() << std::endl;
    //std::cout << "Total implied trades: " << reader.getImpliedTradeCount() << std::endl;

    return 0;
}

int executeNetworkMessageReader( char *configFile )
{
    std::vector< CMEConnection* > secdef_fd_vec;
    std::map< const int , CMEConnection* > fd_map;
    std::vector< CMEConnection* >::const_iterator fd_iter;

    const XMLNode& config = XMLNode::openFileHelper( configFile );

    //
    //  Load the FAST templates
    //

    const XMLNode template_node = config.getChildNode( "template" );

    std::ifstream templates ( template_node.getAttribute( "file" ) );

    if(! templates.good() )
    {
        std::cout << "ERROR: Can't open CME template file: " << template_node.getAttribute( "file" ) << std::endl;
        return 1;
    }

    std::cout << "templates loaded from: " << template_node.getAttribute( "file" ) << std::endl;
    
    //
    //  Create the QuickFAST IOC objects
    //

    MessageReader reader;
    fast_engine::CMEDataSource source;
    QuickFAST::Codecs::XMLTemplateParser parser;
    QuickFAST::Codecs::Decoder decoder( parser.parse( templates ) );
    QuickFAST::Codecs::GenericMessageBuilder builder( reader );

    //
    //  Set the interface IP
    //

    const char* interface;
    if( config.isAttributeSet( "interface" ) )
    {
        std::cout << "using network interface: " << config.getAttribute( "interface" ) << std::endl;
        interface = config.getAttribute( "interface" );
    }
    else
    {
        std::cout << "using default network interface" << std::endl;
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

    std::cout << "loaded " << channel_vec.size() << " channels from: " << channels_node.getAttribute( "file" ) << std::endl;
    
    for( channel_iter = channel_vec.begin();
         channel_iter != channel_vec.end();
         ++channel_iter )
    {
        CMEChannel* channel = network.getCMEChannel( (*channel_iter) );
        std::cout << " " << channel->getChannelId() << " : " << channel->getChannelLabel() << std::endl;
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
                    std::cout << "  opening channel " << channel->getChannelId() 
                              << ": " << channel->getChannelLabel() << std::endl;
                    
                    std::vector< std::string > string_vec;
                    std::vector< std::string >::const_iterator string_iter;

                    channel->getCMEProductCodes( string_vec );

                    for( string_iter = string_vec.begin();
                         string_iter != string_vec.end();
                         ++string_iter )
                    {
                        std::cout << "   product: " << channel->getCMEProduct( *string_iter )->getProductCode() << std::endl;
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

                        if( conn->getConnectionProtocol().compare( "UDP/IP" ) != 0 )
                        {
                            continue;
                        }

                        if( openCMEConnection( interface, conn ) )
                        {
                            std::cout << "     socket open: " 
                                      << conn->getConnectionIP() << ": "
                                      << conn->getConnectionPort() << ", on interface: "
                                      << interface << std::endl;
                                      
                            secdef_fd_vec.push_back( conn );

                            fd_map.insert(
                                std::pair< const int , CMEConnection * >
                                    ( conn->getFd(), conn ) );
                        }
                        else
                        {
                            std::cout << "COULD NOT OPEN CONNECTION TO: " 
                                      << conn->getConnectionIP() << ": "
                                      << conn->getConnectionPort() << ", on interface: "
                                      << interface << std::endl;
                            return 1;
                        }
                    }
                }
            }
        }
    }

    //
    //  SELECT 
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
            FD_ZERO( &fdset );

            for( fd_iter = secdef_fd_vec.begin(); fd_iter != secdef_fd_vec.end(); ++fd_iter )
            {
                FD_SET( (*fd_iter)->getFd(), &fdset );
            }

            if( ( ret = select( secdef_fd_vec.back()->getFd() + 1, &fdset, NULL, NULL, NULL ) ) == -1 )
            {
                std::cout << "select error: " << ret << std::endl;
                return 1;
            }
            else
            {
                for( fd_iter = secdef_fd_vec.begin(); fd_iter != secdef_fd_vec.end(); ++fd_iter )
                {
                    if( FD_ISSET( (*fd_iter)->getFd(), &fdset ) )
                    {
                        if( ( ret = recvfrom( (*fd_iter)->getFd(), buf, 2047, 0, (struct sockaddr *)&their_addr, &addr_len ) ) == -1 )
                        {
                            std::cout << "recvfrom error: -1" << std::endl;
                            return 1;
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

int main( int argc, char *argv[] )
{
    if( strcmp( argv[ 1 ], "FILE" ) == 0 )
    {
        return executeFileMessageReader( argv[ 2 ], argv[ 3 ] );
    }
    else if( strcmp( argv[ 1 ], "NETWORK" ) == 0 )
    {
        return executeNetworkMessageReader( argv[ 2 ] );
    }
    else
    {
        std::cout << "Unknown message reader type: " << argv[ 1 ] << " (FILE|NETWORK)" << std::endl;
        return 1;
    }
    
    return 0;
}

int main2( int argc, char *argv[] )
{
    long len;
    char buf[ 2048 ];
    char len_buf[ 8 ];

    std::fstream infile;
    std::fstream outfile;

    LocalDataSource source;
    NOPMessageReader reader;

    const size_t sizeof_long = sizeof( long );
    const size_t sizeof_kernel_time = sizeof( struct timeval );

    std::cout << "FIX/FAST Message Length Converter: template="
              << argv[ 1 ] << " input=" << argv[ 2 ] << ", output="
              << argv[ 3 ] << std::endl;

    //
    //  Create the QuickFAST IOC objects
    //

    std::ifstream templates ( argv[ 1 ] );
    QuickFAST::Codecs::XMLTemplateParser parser;
    QuickFAST::Codecs::Decoder decoder( parser.parse( templates ) );
    QuickFAST::Codecs::GenericMessageBuilder builder( reader );

    //
    //  Create the file streams
    //

    infile.open( argv[ 2 ], std::fstream::in | std::ifstream::binary );
    outfile.open( argv[ 3 ], std::fstream::out | std::fstream::trunc | std::ifstream::binary );

    while( infile.good() )
    {
        len = 0;

        //
        //  [ 2 ][ 16 ][ 5 ][ FIX/FAST ] --> SUBCHANNEL at index 22
        //  [ 3 ][ 16 ][ 5 ][ FIX/FAST ] --> SUBCHANNEL at index 23
        //

        const long pos = infile.tellg();
        memset( buf, 0, 64 );
        infile.read( buf, 64 );
        infile.seekg( pos );

        memset( len_buf, 0, 8 );

        if( (int)buf[ 23 ] == 0 ||
            (int)buf[ 23 ] == 1 ||
            (int)buf[ 23 ] == 2 )
        {
            infile.read( len_buf, 3 );
        }
        else if( (int)buf[ 22 ] == 0 ||
                 (int)buf[ 22 ] == 1 ||
                 (int)buf[ 22 ] == 2 )
        {
            infile.read( len_buf, 2 );
        }
        else
        {
            printf( " 0: '%d', 1: '%d', 2: '%d', 3: '%d'\n",
                (int)buf[ 0 ],
                (int)buf[ 1 ],
                (int)buf[ 2 ],
                (int)buf[ 3 ] );

            printf( " error at file_pos %ld", pos );

            infile.close();
            outfile.close();
            exit( 1 );
        }

        len = atol( len_buf );
        memset( buf, 0, 2048 );
        infile.read( buf, len );

        //
        //  Skip the kernel time and CME's preamble when decoding the FAST message
        //

        source.setBuffer( (unsigned char*)buf + 5 + sizeof_kernel_time, len - 5 - sizeof_kernel_time );

        try
        {
            decoder.reset();
            decoder.decodeMessage( source, builder );
        }
        catch( std::exception& e )
        {
            std::cerr << "decoder.decodeMessage exception: " << e.what() << std::endl;

            infile.close();
            outfile.close();
            exit( 1 );
        }

        //
        //  Write the length as a long, and the buffer to the outfile
        //

        outfile.write( (const char*)&len, sizeof_long );
        outfile.write( (const char*)buf, len );
    }

    infile.close();
    outfile.close();

    printf( "Length conversion complete." );
    return 0;
}