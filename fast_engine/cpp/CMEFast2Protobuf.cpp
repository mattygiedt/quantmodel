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

#include <CMEDataSource.hpp>
#include <CMEMessageConsumer.hpp>

static inline void getLong( long& l, const unsigned char *b )
{
    l = 0L;
    l |= b[0] & 0xFF; l <<= 8;
    l |= b[1] & 0xFF; l <<= 8;
    l |= b[2] & 0xFF; l <<= 8;
    l |= b[3] & 0xFF;
}

void initLogging( const XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

int main( int argc, char *argv[] )
{
    long len;
    time_t tm;
    char buf[ 2048 ];
    char timebuf[ 16 ];
    log4cpp::Category* log;

    const size_t sizeof_long = sizeof( long );
    const size_t sizeof_kernel_time = sizeof( struct timeval );

    std::fstream infile, outfile;
    std::string currtime, filename;

    quantmodel::FastEngineBroadcast orig, persist;

    const XMLNode& config = XMLNode::openFileHelper( argv[ 1 ] );

    //
    //  Setup log4cpp
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "CMEFast2Protobuf" );

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

    log->info( " templates loaded from: %s", template_node.getAttribute( "file" ) );

    //
    //  Create the QuickFAST wrappers
    //

    fast_engine::CMEDataSource source;
    fast_engine::CMEMessageConsumer consumer;

    //
    //  Create the QuickFAST IOC objects
    //

    QuickFAST::Codecs::XMLTemplateParser parser;
    QuickFAST::Codecs::Decoder decoder( parser.parse( templates ) );
    QuickFAST::Codecs::GenericMessageBuilder builder( consumer );

    for( int i=0; i<config.nChildNode( "data" ); ++i )
    {
        //
        //  Get the data node
        //

        const XMLNode data_node = config.getChildNode( "data", i );

        //
        //  New parameter added for kernel_time struct placed before the
        //  fix/fast message
        //

        const bool has_timestamp = strcmp( "true", data_node.getAttribute( "has_timestamp" ) ) == 0;

        log->warn( "=====================" );
        log->warn( " infile: '%s'", data_node.getAttribute( "infile" ) );
        log->warn( " outdir: '%s'", data_node.getAttribute( "outdir" ) );
        log->warn( " tmstmp: '%s'", data_node.getAttribute( "has_timestamp" ) );
        log->warn( " sec_id: '%s'", data_node.getAttribute( "sec_id" ) );

        //
        //  Open the input file & assign the security id
        //

        infile.open( data_node.getAttribute( "infile" ), std::fstream::in | std::ifstream::binary );

        if(! infile.good() )
        {
            log->fatal( "ERROR: Can't open input file: %s", data_node.getAttribute( "infile" ) );
            return 1;
        }

        tm = 0;
        currtime = "";
        const unsigned int security_id = atoi( data_node.getAttribute( "sec_id" ) );

        //
        //  While there is data to read...
        //

        while( infile.good() )
        {
            len = 0;
            orig.Clear();
            decoder.reset();
            consumer.reset();

            if( has_timestamp )
            {
                //
                //  Skip the kernel time and CME's preamble
                //  when decoding the FAST message. Also note the
                //  new message length format.
                //

                infile.read( (char*)&len, sizeof_long );
                infile.read( buf, len );
                source.setBuffer( (unsigned char*)buf + 5 + sizeof_kernel_time, len - 5 - sizeof_kernel_time );
            }
            else
            {
                //
                //  Skip the CME's preamble when decoding the FAST message
                //

                infile >> len;
                infile.read( buf, len );
                source.setBuffer( (unsigned char*)buf + 5, len - 5 );
            }

            try
            {
                decoder.decodeMessage( source, builder );
            }
            catch( std::exception& e )
            {
                if( has_timestamp )
                {
                    log->error( "decoder.decodeMessage exception: %s, msg_len: %ld", e.what(), ( len - 5 - sizeof_kernel_time ) );
                }
                else
                {
                    log->error( "decoder.decodeMessage exception: %s, msg_len: %ld", e.what(), ( len - 5 ) );
                }

                continue;
            }

            if( consumer.getBroadcastMessage( orig ) )
            {
                //
                //  Init the message to persist
                //

                persist.Clear();
                persist.set_timestamp( orig.timestamp() );
                persist.set_broadcast_type( orig.broadcast_type() );

                //
                //  Loop through the cme market data messages...
                //

                for( int i=0; i<orig.cme_market_data_size(); ++i )
                {
                    const quantmodel::CMEMarketData& cme_market_data = orig.cme_market_data( i );

                    if( security_id == cme_market_data.security_id() )
                    {
                        //
                        //  Copy the cme market data to the broadcast to persist
                        //

                        persist.add_cme_market_data()->CopyFrom( cme_market_data );
                    }
                }

                if( persist.cme_market_data_size() > 0 )
                {
                    log->info( persist.ShortDebugString() );

                    //
                    //  Do we need to open a new proto file?
                    //

                    tm = atol( persist.timestamp().c_str() );
                    strftime( timebuf, 16, "%Y%m%d", gmtime( &tm ) );

                    if( currtime != timebuf )
                    {
                        currtime = timebuf;

                        //
                        //  Close (if open) existing output file.
                        //

                        if( outfile.is_open() )
                        {
                            outfile.close();
                        }

                        //
                        //  Open new output file.
                        //

                        filename.assign( data_node.getAttribute( "outdir" ) )
                            .append( "/" )
                            .append( currtime )
                            .append( ".proto" );

                        outfile.open( filename.c_str(), std::fstream::out | std::fstream::trunc );

                        if(! outfile.good() )
                        {
                            log->fatal( "Cannot open output file: %s", filename.c_str() );
                            exit( 1 );
                        }

                        log->warn( " output file: %s", filename.c_str() );
                    }

                    outfile << persist.ByteSize();
                    if(! persist.SerializeToOstream( &outfile ) )
                    {
                        log->fatal( "Cannot write to output file: %s", filename.c_str() );
                        exit( 1 );
                    }
                }
            }
        }

        infile.close();
    }
    return 0;
}
