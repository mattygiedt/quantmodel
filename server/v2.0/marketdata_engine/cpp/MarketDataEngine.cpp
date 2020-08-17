#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <quantmodel.hpp>

#define LOG_DELAY 29;

using namespace std;

void loadInstrumentMap( const XMLNode& config,
                        std::map< const std::string , quantmodel::Instrument* >& instrument_map,
                        zmq::socket_t* database_callforward,
                        log4cpp::Category* log )
{
    quantmodel::DatabaseEngineRequest request;
    quantmodel::DatabaseEngineResponse response;

    const XMLNode& instruments = config.getChildNode( "instruments" );

    //
    //  Load each configured instrument by symbol group
    //

    for( int i=0; i<instruments.nChildNode( ); ++i )
    {
        const XMLNode& symbol = instruments.getChildNode( i );

        request.Clear();
        quantmodel::TimeStamp::getMessageId( request.mutable_request_id( ) );
        quantmodel::TimeStamp::getUTCTime( request.mutable_timestamp( ) );
        quantmodel::QuantModelProcedures::GET_ACTIVE_INSTRUMENTS_BY_SYMBOL(
            request, symbol.getAttribute( "name" ) );

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " <-- " + request.ShortDebugString() );
        }

        if( quantmodel::ZMQMessage::sendRPC( &request, &response, database_callforward ) )
        {
            for( int i=0; i<response.result_set_size(); ++i )
            {
                const quantmodel::DatabaseEngineResponse::ResultSet& rs = response.result_set( i );

                for( int j=0; j<rs.row_size(); ++j )
                {
                    quantmodel::Instrument *instrument = new quantmodel::Instrument( rs.row( j ) );
                    instrument_map.insert( std::pair< const std::string , quantmodel::Instrument* >(
                        instrument->getInstrumentId(), instrument ) );

                    log->info( " loaded instrument: %s %s %s",
                        instrument->getInstrumentId().c_str(),
                        instrument->getAttribute( quantmodel::SYMBOL ).c_str(),
                        instrument->getAttribute( quantmodel::NAME ).c_str() );
                }
            }
        }
    }
}

void initLogging( const XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

void initNetwork( const XMLNode& config, std::map< const std::string , zmq::socket_t* >& socket_map, log4cpp::Category* log )
{
    zmq::socket_t *s = NULL;
    zmq::context_t *send_ctx = new zmq::context_t( 1 );
    zmq::context_t *recv_ctx = new zmq::context_t( 1 );

    //
    //  Find the <network /> node
    //

    const XMLNode network_node = config.getChildNode( "network" );

    if( network_node.isAttributeSet( "database_callforward" ) )
    {
        log->info( " connecting to database engine callforward: %s", network_node.getAttribute( "database_callforward" ) );
        s = new zmq::socket_t( *recv_ctx, ZMQ_REQ );
        s->connect( network_node.getAttribute( "database_callforward" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "database_callforward", s ) );
    }
    else
    {
        log->fatal( "database engine callforward is not set" );
        exit( 1 );
    }

    //
    //  Bind the broadcast addresses
    //

    if( network_node.isAttributeSet( "broadcast_price" ) )
    {
        log->info( " binding broadcast_price address: %s", network_node.getAttribute( "broadcast_price" ) );
        s = new zmq::socket_t( *send_ctx, ZMQ_PUB );
        s->bind( network_node.getAttribute( "broadcast_price" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "broadcast_price", s ) );
    }
    else
    {
        log->fatal( "broadcast_price address is not set" );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "broadcast_trade" ) )
    {
        log->info( " binding broadcast_trade address: %s", network_node.getAttribute( "broadcast_trade" ) );
        s = new zmq::socket_t( *send_ctx, ZMQ_PUB );
        s->bind( network_node.getAttribute( "broadcast_trade" ) );
        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "broadcast_trade", s ) );
    }
    else
    {
        log->fatal( "broadcast_trade address is not set" );
        exit( 1 );
    }

    //
    //  Connect to the listed market data providers
    //

    if( network_node.nChildNode( "provider" ) > 0 )
    {
        s = new zmq::socket_t( *recv_ctx, ZMQ_SUB );
        s->setsockopt( ZMQ_SUBSCRIBE, "", 0 );

        for( int i=0; i<network_node.nChildNode( "provider" ); i++ )
        {
            log->info( "  adding provider address: %s",
                network_node.getChildNode( "provider", i ).getAttribute( "address" ) );
            s->connect( network_node.getChildNode( "provider", i ).getAttribute( "address" ) );
        }

        socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "marketdata_source", s ) );
    }
    else
    {
        log->fatal( "no configured market data providers" );
        exit( 1 );
    }
}

void handleMessage(
    const quantmodel::FixMarketData& fix_market_data,
    quantmodel::MarketDataBroadcast& price_broadcast_msg,
    quantmodel::MarketDataBroadcast& trade_broadcast_msg,
    std::map< const std::string, std::string >& last_price_map,
    quantmodel::Instrument * const instrument,
    log4cpp::Category * const log )
{
    std::string price;

    quantmodel::MarketDataPriceData *price_data;
    quantmodel::MarketDataTradeData *trade_data;

    //
    //  Get the display price from the instrument
    //

    const std::string& instrument_id = instrument->getInstrumentId();
    instrument->displayPrice( fix_market_data.entry_px(), price );

    //
    //  Has the last price changed?
    //

    if( last_price_map[ instrument_id ] != price )
    {
        last_price_map[ instrument_id ] = price;

        price_data = price_broadcast_msg.add_price_data();
        price_data->set_instrument_id( instrument_id );
        price_data->set_last_prc( price );
    }

    //
    //  Add the trade
    //

    trade_data = trade_broadcast_msg.add_trade_data();
    trade_data->set_instrument_id( instrument_id );
    trade_data->set_trade_prc( price );
    trade_data->set_trade_qty( atoi( fix_market_data.entry_size( ).c_str() ) );
    trade_data->set_entry_tm( 0 );
    trade_data->set_aggressor( quantmodel::MarketDataTradeData::UNKNOWN );
}

void handleMessage(
    const quantmodel::CMEMarketData& cme_market_data,
    quantmodel::MarketDataBroadcast& price_broadcast_msg,
    quantmodel::MarketDataBroadcast& trade_broadcast_msg,
    std::map< const std::string, std::string >& last_price_map,
    quantmodel::Instrument * const instrument,
    log4cpp::Category * const log )
{
    std::string price;

    quantmodel::MarketDataPriceData *price_data;
    quantmodel::MarketDataTradeData *trade_data;

    //
    //  Get the display price from the instrument
    //

    const std::string& instrument_id = instrument->getInstrumentId();
    //instrument->displayPrice( cme_market_data.entry_px(), price );
    instrument->displayPrice( cme_market_data.mantissa(), price );

    //
    //  Has the last price changed?
    //

    if( last_price_map[ instrument_id ] != price )
    {
        last_price_map[ instrument_id ] = price;

        price_data = price_broadcast_msg.add_price_data();
        price_data->set_instrument_id( instrument_id );
        price_data->set_last_prc( price );
    }

    //
    //  Add the trade
    //

    trade_data = trade_broadcast_msg.add_trade_data();
    trade_data->set_instrument_id( instrument_id );
    trade_data->set_trade_prc( price );
    trade_data->set_trade_qty( cme_market_data.entry_size( ) );
    trade_data->set_entry_tm( cme_market_data.entry_tm( ) );

    if( cme_market_data.aggressor() == quantmodel::CMEMarketData::UNKNOWN )
    {
        trade_data->set_aggressor( quantmodel::MarketDataTradeData::UNKNOWN );
    }
    else if( cme_market_data.aggressor() == quantmodel::CMEMarketData::BUY )
    {
        trade_data->set_aggressor( quantmodel::MarketDataTradeData::BUY );
    }
    else
    {
        trade_data->set_aggressor( quantmodel::MarketDataTradeData::SELL );
    }
}

void TRADING_TECHNOLOGIES( const char *file )
{
    int i;
    log4cpp::Category* log;
    std::string broadcast_id, broadcast_ts;
    std::map< const std::string, zmq::socket_t* > socket_map;
    zmq::socket_t *marketdata_source,
                  *price_broadcast,
                  *trade_broadcast;

    std::map< const unsigned int, std::string > security_map;
    std::map< const std::string, std::string > last_price_map;
    std::map< const std::string, quantmodel::Instrument* > instrument_map;

    std::map< const unsigned int, std::string >::const_iterator security_iter;
    std::map< const std::string, quantmodel::Instrument* >::const_iterator instrument_iter;

    quantmodel::FixEngineBroadcast fix_engine_broadcast;
    quantmodel::MarketDataBroadcast price_broadcast_msg, trade_broadcast_msg;

    //
    //  Load the XML configuration
    //

    cout << "Loading configuration file: " << file << endl;
    const XMLNode config = XMLNode::openFileHelper( file );

    //
    //  Setup log4cpp
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "MarketDataEngine" );
    log->warn( "Starting QuantModel MarketDataEngine v2.1" );

    //
    //  Setup ZMQ
    //

    initNetwork( config, socket_map, log );
    marketdata_source = socket_map[ "marketdata_source" ];
    price_broadcast = socket_map[ "broadcast_price" ];
    trade_broadcast = socket_map[ "broadcast_trade" ];

    //
    //  Load the instruments, security map, and last price map
    //

    loadInstrumentMap( config, instrument_map, socket_map[ "database_callforward" ], log );

    for( instrument_iter = instrument_map.begin(); instrument_iter != instrument_map.end(); ++instrument_iter )
    {
        last_price_map.insert( std::pair< std::string , std::string >( instrument_iter->first , "0.0" ) );

        security_map.insert( std::pair< const unsigned int , std::string >(
            atoi( instrument_iter->first.c_str() ), instrument_iter->second->getInstrumentId() ) );
    }

    log->info( "Market Data Engine initialized, waiting for market data." );

    while( true )
    {
        if(! quantmodel::ZMQMessage::recv( &fix_engine_broadcast, marketdata_source ) )
        {
            log->fatal( "UNABLE TO RECV ON marketdata_source SOCKET!!!" );
            exit( 1 );
        }

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " fix_engine_broadcast --> " + fix_engine_broadcast.ShortDebugString() );
        }

        //
        //  Clear the broadcast messages, generate a new ID
        //

        broadcast_id.clear();
        price_broadcast_msg.Clear();
        trade_broadcast_msg.Clear();

        quantmodel::TimeStamp::getMessageId( broadcast_id );

        //
        //  Copy the timestamp from the fast_engine to our broadcast
        //

        broadcast_ts.assign( fix_engine_broadcast.timestamp() );

        //
        //  Loop through the fast broadcast for CME market data and ensure
        //  we can handle the instrument prior to calling handleMessage
        //

        for( i=0; i<fix_engine_broadcast.fix_market_data_size(); ++i )
        {
            const quantmodel::FixMarketData& fix_market_data = fix_engine_broadcast.fix_market_data( i );

            if( ( fix_market_data.entry_type() == quantmodel::FixMarketData::TRADE ) &&
                ( instrument_iter = instrument_map.find( fix_market_data.instrument_id() ) ) != instrument_map.end() )
            {
                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( " fix_market_data --> " + fix_market_data.ShortDebugString() );
                }

                handleMessage(
                    fix_market_data,
                    price_broadcast_msg,
                    trade_broadcast_msg,
                    last_price_map,
                    instrument_iter->second,
                    log );
            }
        }

        //
        //  Publish the messages
        //

        if( price_broadcast_msg.price_data_size() > 0 )
        {
            price_broadcast_msg.set_broadcast_id( broadcast_id );
            price_broadcast_msg.set_timestamp( broadcast_ts );
            price_broadcast_msg.set_type( quantmodel::MarketDataBroadcast::PRICE );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " <-- %d: %s", price_broadcast_msg.ByteSize(),
                    price_broadcast_msg.ShortDebugString().c_str() );
            }

            quantmodel::ZMQMessage::send( &price_broadcast_msg, price_broadcast );
        }

        if( trade_broadcast_msg.trade_data_size() > 0 )
        {
            trade_broadcast_msg.set_broadcast_id( broadcast_id );
            trade_broadcast_msg.set_timestamp( broadcast_ts );
            trade_broadcast_msg.set_type( quantmodel::MarketDataBroadcast::TRADE );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " <-- %d: %s", trade_broadcast_msg.ByteSize(),
                    trade_broadcast_msg.ShortDebugString().c_str() );
            }

            quantmodel::ZMQMessage::send( &trade_broadcast_msg, trade_broadcast );
        }
    }
}

void CME_FIXFAST( const char *file )
{
    int i;
    long msg_count;
    double duration;
    log4cpp::Category* log;
    struct timespec timer_1, timer_2;
    time_t next_log_time, curr_log_time;
    std::string broadcast_id, broadcast_ts;
    std::map< const std::string, zmq::socket_t* > socket_map;
    zmq::socket_t *marketdata_source,
                  *price_broadcast,
                  *trade_broadcast;

    std::map< const unsigned int, std::string > security_map;
    std::map< const std::string, std::string > last_price_map;
    std::map< const std::string, quantmodel::Instrument* > instrument_map;

    std::map< const unsigned int, std::string >::const_iterator security_iter;
    std::map< const std::string, quantmodel::Instrument* >::const_iterator instrument_iter;

    quantmodel::FastEngineBroadcast fast_engine_broadcast;
    quantmodel::MarketDataBroadcast price_broadcast_msg, trade_broadcast_msg;

    //
    //  Load the XML configuration
    //

    cout << "Loading configuration file: " << file << endl;
    const XMLNode config = XMLNode::openFileHelper( file );

    //
    //  Setup log4cpp
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "MarketDataEngine" );
    log->warn( "Starting QuantModel MarketDataEngine v2.1" );

    //
    //  Setup ZMQ
    //

    initNetwork( config, socket_map, log );
    marketdata_source = socket_map[ "marketdata_source" ];
    price_broadcast = socket_map[ "broadcast_price" ];
    trade_broadcast = socket_map[ "broadcast_trade" ];

    //
    //  Load the instruments, security map, and last price map
    //

    loadInstrumentMap( config, instrument_map, socket_map[ "database_callforward" ], log );

    for( instrument_iter = instrument_map.begin(); instrument_iter != instrument_map.end(); ++instrument_iter )
    {
        last_price_map.insert( std::pair< std::string , std::string >( instrument_iter->first , "0.0" ) );

        security_map.insert( std::pair< const unsigned int , std::string >(
            atoi( instrument_iter->first.c_str() ), instrument_iter->second->getInstrumentId() ) );
    }

    msg_count = 0L;
    duration = 0.0;
    next_log_time = time( &curr_log_time ) + LOG_DELAY;

    log->info( "Market Data Engine initialized, waiting for market data." );

    while( true )
    {
        if(! quantmodel::ZMQMessage::recv( &fast_engine_broadcast, marketdata_source ) )
        {
            log->fatal( "UNABLE TO RECV ON marketdata_source SOCKET!!!" );
            exit( 1 );
        }

        clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &timer_1 );

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " fast_engine_broadcast --> " + fast_engine_broadcast.ShortDebugString() );
        }

        //
        //  Clear the broadcast messages, generate a new ID
        //

        broadcast_id.clear();
        price_broadcast_msg.Clear();
        trade_broadcast_msg.Clear();

        quantmodel::TimeStamp::getMessageId( broadcast_id );

        //
        //  Copy the timestamp from the fast_engine to our broadcast
        //

        broadcast_ts.assign( fast_engine_broadcast.timestamp() );

        //
        //  Loop through the fast broadcast for CME market data and ensure
        //  we can handle the instrument prior to calling handleMessage
        //

        for( i=0; i<fast_engine_broadcast.cme_market_data_size(); ++i )
        {
            const quantmodel::CMEMarketData& cme_market_data = fast_engine_broadcast.cme_market_data( i );

            if( ( cme_market_data.is_implied() == false ) &&
                ( cme_market_data.entry_type() == quantmodel::CMEMarketData::TRADE ) &&
                ( security_iter = security_map.find( cme_market_data.security_id() ) ) != security_map.end() &&
                ( instrument_iter = instrument_map.find( security_iter->second ) ) != instrument_map.end() )
            {
                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( " cme_market_data --> " + cme_market_data.ShortDebugString() );
                }

                handleMessage(
                    cme_market_data,
                    price_broadcast_msg,
                    trade_broadcast_msg,
                    last_price_map,
                    instrument_iter->second,
                    log );
            }
        }

        //
        //  Publish the messages
        //

        if( price_broadcast_msg.price_data_size() > 0 )
        {
            price_broadcast_msg.set_broadcast_id( broadcast_id );
            price_broadcast_msg.set_timestamp( broadcast_ts );
            price_broadcast_msg.set_type( quantmodel::MarketDataBroadcast::PRICE );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " <-- %d: %s", price_broadcast_msg.ByteSize(),
                    price_broadcast_msg.ShortDebugString().c_str() );
            }

            quantmodel::ZMQMessage::send( &price_broadcast_msg, price_broadcast );
        }

        if( trade_broadcast_msg.trade_data_size() > 0 )
        {
            trade_broadcast_msg.set_broadcast_id( broadcast_id );
            trade_broadcast_msg.set_timestamp( broadcast_ts );
            trade_broadcast_msg.set_type( quantmodel::MarketDataBroadcast::TRADE );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " <-- %d: %s", trade_broadcast_msg.ByteSize(),
                    trade_broadcast_msg.ShortDebugString().c_str() );
            }

            quantmodel::ZMQMessage::send( &trade_broadcast_msg, trade_broadcast );
        }

        ++msg_count;
        clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &timer_2 );
        duration += quantmodel::TimeStamp::nano_diff( timer_2, timer_1 );

        if( next_log_time < time( &curr_log_time ) )
        {
            log->info( "%ld %.2f", msg_count, ( duration / (double)( msg_count * 1000 ) ) );
            next_log_time = curr_log_time + LOG_DELAY;
        }
    }

    exit( 1 );
}

int main( int argc, char *argv[] )
{
    const XMLNode config = XMLNode::openFileHelper( argv[ 1 ] );

    if( config.isAttributeSet( "mode" ) )
    {
        if( strcmp( "CME", config.getAttribute( "mode" ) ) == 0 )
        {
            std::cout << "STARTING CME FIXFAST ENGINE" << std::endl;
            CME_FIXFAST( argv[ 1 ] );
        }
        else if( strcmp( "TT", config.getAttribute( "mode" ) ) == 0 )
        {
            std::cout << "STARTING TRADING TECHNOLOGIES ENGINE" << std::endl;
            TRADING_TECHNOLOGIES( argv[ 1 ] );
        }
        else
        {
            std::cerr << "Invalid configuration: unknown engine '"
                      << config.getAttribute( "mode" ) << "'" << std::endl;
            exit( 1 );
        }
    }
    else
    {
        std::cerr << "Invalid configuration: Market Data Engine mode must be defined." << std::endl;
        exit( 1 );
    }
}
    