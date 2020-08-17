#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <quantmodel.hpp>

#include "invsys_one.pb.h"

#include "MarketSnapshot.hpp"
#include "Timebar.hpp"
#include "Trend.hpp"
#include "Pivot.hpp"
#include "OrderExecution.hpp"
#include "System.hpp"
#include "TradeInstruction.hpp"
#include "TradeLogic.hpp"
#include "NOPLogic.hpp"
#include "ESVWAPLogic.hpp"
#include "CLVWAPLogic.hpp"
#include "YMVWAPLogic.hpp"
#include "NGVWAPLogic.hpp"

#include "DealingInterface.hpp"
#include "LimitOrderBook.hpp"

void initLogging( const XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

void initNetwork( const XMLNode& config, System& system )
{
    zmq::socket_t *s = NULL;
    zmq::context_t *ctx = new zmq::context_t( 1 );

    //
    //  Find the <network /> node
    //

    const XMLNode network_node = config.getChildNode( "network" );

    if( network_node.isAttributeSet( "invsys_callforward" ) )
    {
        system.log->info( " connecting to invsys_callforward address: %s", network_node.getAttribute( "invsys_callforward" ) );
        s = new zmq::socket_t( *ctx, ZMQ_REP );
        s->bind( network_node.getAttribute( "invsys_callforward" ) );
        system.socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "invsys_callforward", s ) );
    }
    else
    {
        system.log->fatal( "invsys_callforward address is not set" );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "invsys_broadcast" ) )
    {
        system.log->info( " connecting to invsys_broadcast address: %s", network_node.getAttribute( "invsys_broadcast" ) );
        s = new zmq::socket_t( *ctx, ZMQ_PUSH );
        s->bind( network_node.getAttribute( "invsys_broadcast" ) );
        system.socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "invsys_broadcast", s ) );
    }
    else
    {
        system.log->fatal( "invsys_broadcast address is not set" );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "dealing_callforward" ) )
    {
        system.log->info( " connecting to dealing engine callforward: %s", network_node.getAttribute( "dealing_callforward" ) );
        s = new zmq::socket_t( *ctx, ZMQ_REQ );
        s->connect( network_node.getAttribute( "dealing_callforward" ) );
        system.socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "dealing_callforward", s ) );
    }
    else
    {
        system.log->fatal( "dealing engine callforward is not set" );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "dealing_broadcast" ) )
    {
        system.log->info( " connecting to dealing engine broadcast: %s", network_node.getAttribute( "dealing_broadcast" ) );
        s = new zmq::socket_t( *ctx, ZMQ_SUB );
        s->setsockopt( ZMQ_SUBSCRIBE, "", 0 );
        s->connect( network_node.getAttribute( "dealing_broadcast" ) );
        system.socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "dealing_broadcast", s ) );
    }
    else
    {
        system.log->fatal( "dealing engine broadcast is not set" );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "fast_engine_broadcast" ) )
    {
        system.log->info( " connecting to fast_engine_broadcast address: %s", network_node.getAttribute( "fast_engine_broadcast" ) );

        s = new zmq::socket_t( *ctx, ZMQ_SUB );
        s->setsockopt( ZMQ_SUBSCRIBE, "", 0 );
        s->connect( network_node.getAttribute( "fast_engine_broadcast" ) );
        system.socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "fast_engine_broadcast", s ) );
    }
    else
    {
        system.log->fatal( "fast engine broadcast is not set" );
        exit( 1 );
    }
}

bool isTodayHoliday( const XMLNode& config, const System& system )
{
    time_t now;
    char buffer[ 32 ];
    const XMLNode calendar_node = config.getChildNode( "calendar" );

    //
    //  Get the current date
    //

    time( &now );
    memset( buffer, 0, 32 );
    if( strftime( buffer, 32, "%Y%m%d", gmtime( &now ) ) == 0 )
    {
        system.log->fatal( "strftime returns zero for now: %ld", now );
        exit( 1 );
    }

    for( int i=0; i<calendar_node.nChildNode( "holiday" ); i++ )
    {
        const char *day = calendar_node.getChildNode( "holiday", i ).getAttribute( "day" );

        if( strcmp( buffer, day ) == 0 )
        {
            system.log->fatal( "INVESTMENT SYSTEM SHUTDOWN: HOLIDAY %s", day );
            return true;
        }
    }

    system.log->info( "Trade date: %s is not a holiday", buffer );
    return false;
}

void processDummyTradeInstructionVector( System& system, std::vector< TradeInstruction* >& trade_instruction_vec )
{
    if( likely( trade_instruction_vec.size() > 0 ) )
    {
        std::vector< TradeInstruction* >::const_iterator trade_instruction_iter;

        for( trade_instruction_iter = trade_instruction_vec.begin();
             trade_instruction_iter != trade_instruction_vec.end();
             ++trade_instruction_iter )
        {
            TradeInstruction *instruction = (*trade_instruction_iter);
            delete instruction;
        }

        trade_instruction_vec.clear();
    }
}

bool updateSnapshot( System& system, TradeLogic *strategy )
{
    bool process_flag = false;

    //
    //  Process the broadcast to determine if we
    //  should close the last evaluation period.
    //

    if( likely( system.now >= system.next ) )
    {
        char buffer[ 32 ];
        quantmodel::InvestmentSystemBroadcast broadcast_msg;

        //
        //  Update the one and five minute timebars
        //

        if( likely( system.one_minute_timebar != NULL ) )
        {
            system.one_minute_timebar->update( system.snapshot );
        }

        if( likely( system.five_minute_timebar != NULL ) )
        {
            system.five_minute_timebar->update( system.snapshot );
        }

        //
        //  Get the current time
        //

        memset( buffer, 0, 32 );
        if( strftime( buffer, 32, "%H:%M.%S", gmtime( &system.now ) ) == 0 )
        {
            system.log->fatal( "strftime returns zero for now: %ld", system.now );
            exit( 1 );
        }

        system.current_tm = buffer;

        //
        //  Set the best bid / ask
        //

        const double best_ask = system.best_ask * system.price_factor;
        const double best_bid = system.best_bid * system.price_factor;

        //
        //  Update the current PL values
        //

        system.closed_qty = strategy->getClosedQty();
        system.closed_pl = strategy->getClosedPL();
        system.open_pl = strategy->getOpenPL( best_ask, best_bid );

        //
        //  Did we just close a five minute timebar?
        //

        if( strstr( buffer, "5.00" ) != NULL || strstr( buffer, "0.00" ) != NULL )
        {
            if( likely( system.one_minute_timebar != NULL ) )
            {
                system.one_minute_timebars.push_back( system.one_minute_timebar );
            }
            else
            {
                system.log->warn( "system.one_minute_timebar == NULL" );
            }

            system.log->info( "" );
            system.log->info( "CLOSED 5-MINUTE TIMEBAR @ %s", buffer );
            system.log->info( "ask: %.2f, bid: %.2f", best_ask, best_bid );

            if( likely( system.five_minute_timebar != NULL ) )
            {
                system.five_minute_timebar->print( system.log );
                system.log->info( "   OPEN_PL: %.2f", system.open_pl );
                system.log->info( " CLOSED_PL: %.2f", system.closed_pl );
                system.log->info( "" );

                //
                //  Process this event regardless if the fast_engine_brodcast
                //  causes a change to the order book.
                //

                process_flag = true;

                //
                //  Save the five-minute timebar
                //

                system.five_minute_timebars.push_back( system.five_minute_timebar );

                //
                //  Update the trend & pivot vectors
                //

                system.updateTrendVector( );
                system.updatePivotVector( );

                //
                //  Broadcast the newly closed 5-minute timebar
                //

                broadcast_msg.Clear();
                system.fillTimebarData( system.five_minute_timebar, broadcast_msg );

                quantmodel::TimeStamp::getMessageId( *broadcast_msg.mutable_broadcast_id( ) );
                quantmodel::TimeStamp::getUTCTime( *broadcast_msg.mutable_timestamp( ) );
                quantmodel::ZMQMessage::send( &broadcast_msg, system.socket_map[ "invsys_broadcast" ] );

                //
                //  Broadcast the last trend.
                //

                broadcast_msg.Clear();
                system.fillTrendData( system.trend_vec.back(), broadcast_msg );

                quantmodel::TimeStamp::getMessageId( *broadcast_msg.mutable_broadcast_id( ) );
                quantmodel::TimeStamp::getUTCTime( *broadcast_msg.mutable_timestamp( ) );
                quantmodel::ZMQMessage::send( &broadcast_msg, system.socket_map[ "invsys_broadcast" ] );

                //
                //  Broadcast the last pivot.
                //

                broadcast_msg.Clear();
                system.fillPivotData( system.pivot_vec.back(), broadcast_msg );

                quantmodel::TimeStamp::getMessageId( *broadcast_msg.mutable_broadcast_id( ) );
                quantmodel::TimeStamp::getUTCTime( *broadcast_msg.mutable_timestamp( ) );
                quantmodel::ZMQMessage::send( &broadcast_msg, system.socket_map[ "invsys_broadcast" ] );
            }
            else
            {
                system.log->warn( "system.five_minute_timebar == NULL" );
            }

            //
            //  Create a new one & five minute timebar
            //

            system.one_minute_timebar = new Timebar( system.now );
            system.five_minute_timebar = new Timebar( system.now );
        }
        else if( strstr( buffer, ".00" ) != NULL )
        {
            //
            //  CLOSED 1-MINUTE TIMEBAR
            //

            if( likely( system.one_minute_timebar != NULL ) )
            {
                system.one_minute_timebars.push_back( system.one_minute_timebar );
            }
            else
            {
                system.log->warn( "system.one_minute_timebar == NULL" );
            }

            //
            //  Create a new one minute timebar
            //

            system.one_minute_timebar = new Timebar( system.now );
        }

        //
        //  Reset the snapshot
        //

        system.snapshot->reset( system.now );
        system.next = system.now + 1;

        //
        //  Broadcast the update
        //

        broadcast_msg.Clear();
        system.fillSystemData( broadcast_msg, best_ask, best_bid );

        quantmodel::TimeStamp::getMessageId( *broadcast_msg.mutable_broadcast_id( ) );
        quantmodel::TimeStamp::getUTCTime( *broadcast_msg.mutable_timestamp( ) );
        quantmodel::ZMQMessage::send( &broadcast_msg, system.socket_map[ "invsys_broadcast" ] );
    }

    return process_flag;
}

int main( int argc, char *argv[] )
{
    //
    //  Main Variables
    //

    int rc;
    bool process_flag;
    TradeLogic *strategy;
    System system;

    std::vector< Trend* >::const_reverse_iterator trend_riter;
    std::vector< Pivot* >::const_reverse_iterator pivot_riter;

    std::vector< TradeInstruction* > trade_instruction_vec;

    quantmodel::CMEMarketData::EntryType entry_type;
    quantmodel::FastEngineBroadcast fast_engine_broadcast;
    quantmodel::DealingEngineBroadcast dealing_broadcast_msg;
    quantmodel::InvestmentSystemRequest request_msg;
    quantmodel::InvestmentSystemResponse response_msg;
    quantmodel::InvestmentSystemBroadcast broadcast_msg;
    investment_system::invsys_one::InvestmentSystemOneRequest invsys_one_request;

    //
    //  Load XML config
    //

    std::cout << "Loading configuration file: " << argv[ 1 ] << std::endl;
    const XMLNode& config = XMLNode::openFileHelper( argv[ 1 ] );

    //
    //  Initialize the system parameters
    //

    system.init( config.getChildNode( "parameters" ) );
    system.snapshot = new MarketSnapshot( 0 );
    system.now = system.next = time( NULL );

    std::cout << system.investment_system_id << " --> "
              << system.strategy_id << " system initialized."
              << std::endl;

    //
    //  Setup logging
    //

    initLogging( config );
    system.log = &log4cpp::Category::getInstance( system.investment_system_id.c_str() );

    //
    //  Is today a holiday?
    //

    if( isTodayHoliday( config, system ) )
    {
        exit( 0 );
    }

    //
    //  Initialize the trading system strategy
    //

    strategy = NULL;
    if( system.strategy_id == "ES_VWAP" )
    {
        strategy = new ESVWAPLogic( system );
    }
    else if( system.strategy_id == "CL_VWAP" )
    {
        strategy = new CLVWAPLogic( system );
    }
    else if( system.strategy_id == "YM_VWAP" )
    {
        strategy = new YMVWAPLogic( system );
    }
    else if( system.strategy_id == "NG_VWAP" )
    {
        strategy = new NGVWAPLogic( system );
    }
    else if( system.strategy_id == "NOP" )
    {
        strategy = new NOPLogic( system );
    }
    else
    {
        system.log->fatal( "UNKNOWN INVESTMENT SYSTEM: %s", system.strategy_id.c_str() );
        exit( 1 );
    }

    //
    //  Setup ZMQ
    //

    initNetwork( config, system );

    zmq::pollitem_t items [ ] =
    {
        { (*system.socket_map[ "dealing_broadcast" ]), 0, ZMQ_POLLIN, 0 },
        { (*system.socket_map[ "fast_engine_broadcast" ]), 0, ZMQ_POLLIN, 0 },
        { (*system.socket_map[ "invsys_callforward" ]), 0, ZMQ_POLLIN, 0 },
    };

    //
    //  Create the limit order book for the system instrument
    //

    LimitOrderBook book ( system.security_id );

    //
    //  Wait for dealing engine, fast engine, or client messages
    //

    while( ( rc = zmq::poll( items, 3, -1 ) ) >= 0 )
    {
        if( items[ 0 ].revents & ZMQ_POLLIN )
        {
            if(! quantmodel::ZMQMessage::recv( &dealing_broadcast_msg, system.socket_map[ "dealing_broadcast" ] ) )
            {
                system.log->fatal( "UNABLE TO RECV ON dealing_broadcast SOCKET!!!" );
                exit( 1 );
            }
        }
        else if( items[ 1 ].revents & ZMQ_POLLIN )
        {
            if(! quantmodel::ZMQMessage::recv( &fast_engine_broadcast, system.socket_map[ "fast_engine_broadcast" ] ) )
            {
                system.log->fatal( "UNABLE TO RECV ON fast_engine_broadcast SOCKET!!!" );
                exit( 1 );
            }

            const time_t now = atol( fast_engine_broadcast.timestamp().c_str() );

            //
            //  Have we gapped one or more seconds since last update?
            //

            while( now >= system.next )
            {
                system.now = system.next;

                //system.log->info( " system.now: %ld", system.now );

                book.getInsideQuotes( system.best_bid, system.best_ask );
                process_flag = updateSnapshot( system, strategy );

                if( unlikely( system.updateSystemStatus( ) ) )
                {
                    //
                    //  There was a change in status, figure out what to do
                    //

                    if( system.status == "OPEN" )
                    {
                        strategy->open( system );
                    }
                    else if( system.status == "CLOSED" )
                    {
                        strategy->close( system, trade_instruction_vec );
                        processDummyTradeInstructionVector( system, trade_instruction_vec );
                    }
                    else if( system.status == "SHUTDOWN" )
                    {
                        strategy->close( system, trade_instruction_vec );
                        processDummyTradeInstructionVector( system, trade_instruction_vec );
                        system.log->warn( "INVESTMENT SYSTEM SHUTDOW" );
                        exit( 0 );
                    }
                }

                if( process_flag == true )
                {
                    if( unlikely( strategy->evaluate( system, trade_instruction_vec ) == true ) )
                    {
                        processDummyTradeInstructionVector( system, trade_instruction_vec );
                    }
                }
            }

            //
            //  Process the cme incremental updates in the broadcast message
            //

            for( int i=0; i<fast_engine_broadcast.cme_market_data_size(); ++i )
            {
                const quantmodel::CMEMarketData& cme_data =
                    fast_engine_broadcast.cme_market_data( i );

                if( system.security_id == cme_data.security_id() )
                {
                    book.reset();
                    entry_type = cme_data.entry_type();

                    if( entry_type == quantmodel::CMEMarketData::BID )
                    {
                        if( unlikely( cme_data.price_level() == 0 ) )
                        {
                            system.log->warn( "zero price level: %s", cme_data.ShortDebugString().c_str() );
                            continue;
                        }

                        book.process_bid( cme_data );
                    }
                    else if( entry_type == quantmodel::CMEMarketData::ASK )
                    {
                        if( unlikely( cme_data.price_level() == 0 ) )
                        {
                            system.log->warn( "zero price level: %s", cme_data.ShortDebugString().c_str() );
                            continue;
                        }

                        book.process_ask( cme_data );
                    }
                    else if( entry_type == quantmodel::CMEMarketData::TRADE &&
                             cme_data.is_implied() == false )
                    {
                        system.snapshot->processTrade( cme_data, system.price_factor );
                    }

                    //
                    //  If the order book has changed, evaluate
                    //

                    if( book.hasChanged() == true )
                    {
                        book.getInsideQuotes( system.best_bid, system.best_ask );

                        //
                        //  This can happen if the entire book is cleared.
                        //

                        if( unlikely( system.status == "OPEN" &&
                            ( system.best_ask == 0 || system.best_bid == 0 ) ) )
                        {
                            system.log->warn( "zero price: bid %lld, ask %lld",
                                system.best_bid, system.best_ask );
                            continue;
                        }

                        if( unlikely( strategy->evaluate( system, trade_instruction_vec ) == true ) )
                        {
                            processDummyTradeInstructionVector( system, trade_instruction_vec );
                        }
                    }
                }
            }
        }
        else if( items[ 2 ].revents & ZMQ_POLLIN )
        {
            if(! quantmodel::ZMQMessage::recv( &request_msg, system.socket_map[ "invsys_callforward" ] ) )
            {
                system.log->fatal( "UNABLE TO RECV ON callforward SOCKET!!!" );
                exit( 1 );
            }

            response_msg.Clear();
            response_msg.set_request_id( request_msg.request_id() );
            quantmodel::TimeStamp::getMessageId( *response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( *response_msg.mutable_timestamp( ) );
            response_msg.set_type( quantmodel::InvestmentSystemResponse::ACKNOWLEDGEMENT );

            //
            //  Only process COMMAND requests. HEARTBEAT is implied via the ACK.
            //

            if( request_msg.type() == quantmodel::InvestmentSystemRequest::COMMAND )
            {
                const double best_ask = system.best_ask * system.price_factor;
                const double best_bid = system.best_bid * system.price_factor;

                invsys_one_request.Clear();
                invsys_one_request.ParseFromString( request_msg.delegate() );

                system.log->info( invsys_one_request.ShortDebugString() );

                if( invsys_one_request.type() == investment_system::invsys_one::InvestmentSystemOneRequest::LOGIN )
                {
                    system.fillLoginData( response_msg, best_ask, best_bid );
                    quantmodel::ZMQMessage::send( &response_msg, system.socket_map[ "invsys_callforward" ] );
                }
                else if( invsys_one_request.type() == investment_system::invsys_one::InvestmentSystemOneRequest::SHUTDOWN )
                {
                    system.status = "SHUTDOWN";

                    //
                    //  Acknowledge the shutdown request.
                    //

                    quantmodel::ZMQMessage::send( &response_msg, system.socket_map[ "invsys_callforward" ] );

                    //
                    //  Broadcast the shudown to all connected clients.
                    //

                    broadcast_msg.Clear();
                    system.fillSystemData( broadcast_msg, best_ask, best_bid );

                    quantmodel::TimeStamp::getMessageId( *broadcast_msg.mutable_broadcast_id( ) );
                    quantmodel::TimeStamp::getUTCTime( *broadcast_msg.mutable_timestamp( ) );
                    quantmodel::ZMQMessage::send( &broadcast_msg, system.socket_map[ "invsys_broadcast" ] );

                    //
                    //  Shutdown the investment system.
                    //

                    strategy->close( system, trade_instruction_vec );
                    processDummyTradeInstructionVector( system, trade_instruction_vec );
                    system.log->warn( "INVESTMENT SYSTEM SHUTDOW" );
                    exit( 0 );
                }
            }
            else
            {
                system.log->info( invsys_one_request.ShortDebugString() );
                quantmodel::ZMQMessage::send( &response_msg, system.socket_map[ "invsys_callforward" ] );
            }
        }
    }

    exit( 0 );
}
