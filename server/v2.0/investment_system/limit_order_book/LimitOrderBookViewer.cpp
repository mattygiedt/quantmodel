#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <execinfo.h>
#include <signal.h>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include "LimitOrderBook.hpp"

void handler(int sig) 
{
    void *array[10];
    size_t size;
    
    // get void*'s for all entries on the stack
    size = backtrace( array, 10 );
    
    // print out all the frames to stderr
    fprintf( stderr, "Error: signal %d:\n", sig );
    backtrace_symbols_fd( array, size, STDERR_FILENO );
    exit( 1 );
}

int main( int argc, char *argv[] )
{
    //signal( SIGSEGV, handler );   // install our handler
    time_t now, next;
    char buf[ 8 ];
    zmq::context_t ctx( 1 );
    unsigned int security_id;
    unsigned int book_depth;
    long long int start_prc;
    unsigned int tick_sz;
    
    quantmodel::MarketDataQuoteData book_data;
    quantmodel::MarketDataTradeData trade_data;
    quantmodel::FastEngineBroadcast fast_engine_broadcast;
    
    log4cpp::PropertyConfigurator::configure( argv[ 2 ] );
    log4cpp::Category *log = &log4cpp::Category::getInstance( "" );

    const XMLNode& config = XMLNode::openFileHelper( argv[ 1 ] );
    const XMLNode& network = config.getChildNode( "network" );
    const XMLNode& security = config.getChildNode( "security" );
        
    if(! network.isAttributeSet( "fast_engine_broadcast" ) )
    {
        log->fatal( "network.fast engine broadcast is not set" );
        exit( 1 );
    }
    
    if(! network.isAttributeSet( "client_broadcast" ) )
    {
        log->fatal( "network.client_broadcast address is not set" );
        exit( 1 );
    }
    
    if(! security.isAttributeSet( "security_id" ) )
    {
        log->fatal( "security.security_id is not set" );
        exit( 1 );
    }
    
    if(! security.isAttributeSet( "book_depth" ) )
    {
        log->fatal( "security.book_depth is not set" );
        exit( 1 );
    }
    
    if(! security.isAttributeSet( "start_prc" ) )
    {
        log->fatal( "security.start_prc is not set" );
        exit( 1 );
    }
    
    if(! security.isAttributeSet( "tick_sz" ) )
    {
        log->fatal( "security.tick_sz is not set" );
        exit( 1 );
    }

    if(! security.isAttributeSet( "volume_reset_tm" ) )
    {
        log->fatal( "security.security_id is not set" );
        exit( 1 );
    }

    now = next = 0;
    security_id = atoi( security.getAttribute( "security_id" ) );
    book_depth = atoi( security.getAttribute( "book_depth" ) );
    tick_sz = atoi( security.getAttribute( "tick_sz" ) );
    start_prc = atoll( security.getAttribute( "start_prc" ) );
    LimitOrderBook book( security_id, book_depth, start_prc, tick_sz );
    book_data.set_instrument_id( security.getAttribute( "security_id" ) );
    trade_data.set_instrument_id( security.getAttribute( "security_id" ) );
    trade_data.set_entry_tm( 0 );
    trade_data.set_trade_qty( 0 );

    //log->info( " connecting to fast_engine_broadcast address: %s", network.getAttribute( "fast_engine_broadcast" ) );

    zmq::socket_t sub( ctx, ZMQ_SUB );
    sub.setsockopt( ZMQ_SUBSCRIBE, "", 0 );
    sub.connect( network.getAttribute( "fast_engine_broadcast" ) );

    //log->info( " connecting to client_broadcast address: %s", network.getAttribute( "client_broadcast" ) );

    zmq::socket_t pub( ctx, ZMQ_PUB );
    pub.connect( network.getAttribute( "client_broadcast" ) );

    //int message_count = 0;
    try
    {    
        while( 1 )
        {
            if(! quantmodel::ZMQMessage::recv( &fast_engine_broadcast, &sub ) )
            {
                log->fatal( "UNABLE TO RECV ON fast_engine_broadcast SOCKET!!!" );
                exit( 1 );
            }

            for( int i=0; i<fast_engine_broadcast.cme_market_data_size(); ++i )
            {
                const quantmodel::CMEMarketData& cme_data =
                    fast_engine_broadcast.cme_market_data( i );

                if( security_id == cme_data.security_id() )
                {
                    const quantmodel::CMEMarketData::EntryType& entry_type = cme_data.entry_type();

                    if( entry_type == quantmodel::CMEMarketData::BID )
                    {
                        book.process_bid( cme_data );
                    }
                    else if( entry_type == quantmodel::CMEMarketData::ASK )
                    {
                        book.process_ask( cme_data );
                    }
                    else if( entry_type == quantmodel::CMEMarketData::TRADE && cme_data.is_implied() == false )
                    {
                        trade_data.set_trade_qty( trade_data.trade_qty() + cme_data.entry_size() );
                    }
                }
            }

            if( ( next = time( NULL ) ) != now )
            {
                //char message_count_buf[ 16 ];
                //sprintf( message_count_buf, "%d", message_count );
                //log->debug( message_count_buf );
                
                //std::cout << message_count << std::endl;
                
                now = next;
                memset( buf, 0, 8 );
                strftime( buf, 8, "%H%M", localtime( &now ) );

                if( strcmp( security.getAttribute( "volume_reset_tm" ), buf ) == 0 )
                {
                    trade_data.set_trade_qty( 0 );
                }

                book_data.set_type( quantmodel::MarketDataQuoteData::BID );
                for( unsigned int i=0; i<book_depth; ++i )
                {
                    if( book.hasChanged( book_data, i+1, 1 ) )
                    {
                        quantmodel::ZMQMessage::send( &book_data, &pub );
                    }
                }

                book_data.set_type( quantmodel::MarketDataQuoteData::ASK );
                for( unsigned int i=0; i<book_depth; ++i )
                {
                    if( book.hasChanged( book_data, i+1, 0 ) )
                    {
                        quantmodel::ZMQMessage::send( &book_data, &pub );
                        
                        //std::cout << book_data.ShortDebugString() << std::endl;
                        
                        //if( //log->isDebugEnabled() )
                        //{
                        //    //log->debug( book_data.ShortDebugString() );
                        //}
                    }
                }

                quantmodel::ZMQMessage::send( &trade_data, &pub );
            }
        }
    }
    catch( const std::exception& ex )
    {
        log->fatal( "EXCEPTION: %s", ex.what() );
    }
    catch( const std::string& ex )
    {
        log->fatal( "EXCEPTION: " + ex );
    }
    catch( ... )
    {
        log->fatal( "UNKNOWN EXCEPTION ... " );
    }

    sub.close();
    pub.close();

    google::protobuf::ShutdownProtobufLibrary();

    //log->removeAllAppenders();

    return 0;
}