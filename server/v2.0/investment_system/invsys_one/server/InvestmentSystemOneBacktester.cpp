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
#include "ESVWAPLogic.hpp"
#include "CLVWAPLogic.hpp"
#include "NQVWAPLogic.hpp"
#include "YMVWAPLogic.hpp"
#include "NGVWAPLogic.hpp"

#include "DealingInterface.hpp"
#include "LimitOrderBook.hpp"

struct SystemResult
{
    std::string day;
    double max_open_pl;
    double min_open_pl;
    double max_closed_pl;
    double min_closed_pl;
    double closed_pl;
    unsigned int closed_qty;
    unsigned int volume;

    SystemResult( const std::string d )
    {
        day = d;
        max_open_pl = min_open_pl = max_closed_pl = min_closed_pl = closed_pl = 0.0;
        closed_qty = volume = 0;
    }

    void update( System& system )
    {
        if( system.open_pl > max_open_pl ) max_open_pl = system.open_pl;
        if( system.open_pl < min_open_pl ) min_open_pl = system.open_pl;
        if( system.closed_pl > max_closed_pl ) max_closed_pl = system.closed_pl;
        if( system.closed_pl < min_closed_pl ) min_closed_pl = system.closed_pl;

        closed_pl = system.closed_pl;
        closed_qty = system.closed_qty;
    }
};

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

    if( network_node.isAttributeSet( "client_callforward" ) )
    {
        system.log->info( " connecting to client_callforward address: %s", network_node.getAttribute( "client_callforward" ) );
        s = new zmq::socket_t( *ctx, ZMQ_REP );
        s->connect( network_node.getAttribute( "client_callforward" ) );
        system.socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "client_callforward", s ) );
    }
    else
    {
        system.log->fatal( "client_callforward address is not set" );
        exit( 1 );
    }

    if( network_node.isAttributeSet( "client_broadcast" ) )
    {
        system.log->info( " connecting to client_broadcast address: %s", network_node.getAttribute( "client_broadcast" ) );
        s = new zmq::socket_t( *ctx, ZMQ_PUSH );
        s->connect( network_node.getAttribute( "client_broadcast" ) );
        system.socket_map.insert( std::pair< const std::string , zmq::socket_t* >( "client_broadcast", s ) );
    }
    else
    {
        system.log->fatal( "client_broadcast address is not set" );
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

void processTradeInstructionVector( System& system, std::vector< TradeInstruction* >& trade_instruction_vec )
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
        //  Update the current best BID / ASK & PL values
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
    }

    return process_flag;
}

void executeSystem( const std::string& file, System& system, TradeLogic *strategy, SystemResult *result )
{
    int len;
    bool process_flag;
    std::fstream infile;
    char buf[ 32 * 1024 ];

    LimitOrderBook book ( system.security_id );
    quantmodel::CMEMarketData::EntryType entry_type;
    quantmodel::FastEngineBroadcast fast_engine_broadcast;
    std::vector< TradeInstruction* > trade_instruction_vec;

    system.log->info( "Loading file: %s", file.c_str() );
    infile.open( file.c_str(), std::fstream::in | std::ifstream::binary );

    while( infile.good() )
    {
        fast_engine_broadcast.Clear();
        infile >> len;
        infile.read( buf, len );
        fast_engine_broadcast.ParsePartialFromArray( buf, len );

        const time_t now = atol( fast_engine_broadcast.timestamp().c_str() );

        if( system.next == 0 )
        {
            system.next = now;
        }

        //
        //  Have we gapped one or more seconds since last update?
        //

        while( now >= system.next )
        {
            system.now = system.next;

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
                    system.log->warn( "INVESTMENT SYSTEM OPEN" );
                }
                else if( system.status == "CLOSED" )
                {
                    strategy->close( system, trade_instruction_vec );
                    processTradeInstructionVector( system, trade_instruction_vec );
                    system.log->warn( "INVESTMENT SYSTEM CLOSED" );
                }
                else if( system.status == "SHUTDOWN" )
                {
                    strategy->close( system, trade_instruction_vec );
                    processTradeInstructionVector( system, trade_instruction_vec );
                    system.log->warn( "INVESTMENT SYSTEM SHUTDOWN" );
                }
            }

            if( process_flag == true )
            {
                if( unlikely( strategy->evaluate( system, trade_instruction_vec ) == true ) )
                {
                    processTradeInstructionVector( system, trade_instruction_vec );
                }
            }
        }

        //
        //  We are current -- system.now is less than system.next. Process the
        //  cme incremental updates in the broadcast message.
        //

        for( int i=0; i<fast_engine_broadcast.cme_market_data_size(); ++i )
        {
            const quantmodel::CMEMarketData& cme_data =
                fast_engine_broadcast.cme_market_data( i );

            if( system.security_id == cme_data.security_id() )
            {
                book.reset( false );
                entry_type = cme_data.entry_type();

                if( entry_type == quantmodel::CMEMarketData::BID )
                {
                    if( unlikely( cme_data.price_level() == 0 ) )
                    {
                        //system.log->warn( "zero price level: %s", cme_data.ShortDebugString().c_str() );
                        continue;
                    }

                    book.process_bid( cme_data );
                }
                else if( entry_type == quantmodel::CMEMarketData::ASK )
                {
                    if( unlikely( cme_data.price_level() == 0 ) )
                    {
                        //system.log->warn( "zero price level: %s", cme_data.ShortDebugString().c_str() );
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

                    if( unlikely( system.status == "OPEN" && ( system.best_ask == 0 || system.best_bid == 0 ) ) )
                    {
                        system.log->warn( "zero price: bid %lld, ask %lld",
                            system.best_bid, system.best_ask );
                        continue;
                    }

                    if( unlikely( strategy->evaluate( system, trade_instruction_vec ) == true ) )
                    {
                        processTradeInstructionVector( system, trade_instruction_vec );
                    }
                }
            }
        }

        result->update( system );
    }

    infile.close();

    if( system.status != "SHUTDOWN" )
    {
        system.log->fatal( "%s did not shutdown.", result->day.c_str() );
        exit( 1 );
    }

    result->update( system );
}

int main( int argc, char *argv[] )
{
    //
    //  Main Variables
    //

    System system;
    TradeLogic *strategy = NULL;
    const double TRADE_COST = 2.0;
    std::vector< SystemResult* > system_result_vec;
    std::vector< SystemResult* >::const_iterator system_result_iter;

    system.snapshot = new MarketSnapshot( 0 );

    //
    //  Load XML config
    //

    const XMLNode& config = XMLNode::openFileHelper( "backtester.xml" );

    //
    //  Setup logging
    //

    initLogging( config );

    //
    //  Iterate over each day in the back test file
    //

    for( int i=0; i<config.nChildNode( "parameters" ); ++i )
    {
        const XMLNode data_node = config.getChildNode( "parameters", i );

        system.init( data_node );

        if( system.log == NULL )
        {
            system.log = &log4cpp::Category::getInstance( system.investment_system_id.c_str() );
        }

        system.log->info( "Loading investment_system: %s, strategy: %s",
            system.investment_system_id.c_str(),
            system.strategy_id.c_str() );

        //
        //  Initialize the trading system strategy
        //

        if( strategy != NULL )
        {
            delete strategy;
            strategy = NULL;
        }

        if( system.strategy_id == "ES_VWAP" )
        {
            strategy = new ESVWAPLogic( system );
        }
        else if( system.strategy_id == "CL_VWAP" )
        {
            strategy = new CLVWAPLogic( system );
        }
        else if( system.strategy_id == "NQ_VWAP" )
        {
            strategy = new NQVWAPLogic( system );
        }
        else if( system.strategy_id == "YM_VWAP" )
        {
            strategy = new YMVWAPLogic( system );
        }
        else if( system.strategy_id == "NG_VWAP" )
        {
            strategy = new NGVWAPLogic( system );
        }
        else
        {
            system.log->fatal( "UNKNOWN INVESTMENT SYSTEM: %s", system.strategy_id.c_str() );
            exit( 1 );
        }

        //
        //  Create a new system result for this date
        //

        SystemResult *system_result = new SystemResult( data_node.getAttribute( "date" ) );

        //
        //  Execute the system
        //

        executeSystem( data_node.getAttribute( "filename" ), system, strategy, system_result );

        //
        //  Add the system result to the result vector
        //

        system.log->fatal( "%s CLOSED_PL: %ld %f",
            system_result->day.c_str(),
            system_result->closed_qty,
            system_result->closed_pl );

        system_result_vec.push_back( system_result );
    }

    if( system_result_vec.size() > 0 )
    {
        //
        //  Create a new output file to persist the system results.
        //

        double total_closed_pl = 0.0;
        double losing_pl = 0.0;
        double winning_pl = 0.0;
        double winning_pct = 0.0;
        double profit_factor = 0.0;
        unsigned int losing_days = 0;
        unsigned int total_trades = 0;

        std::ofstream file;
        std::string filename = system.investment_system_id;
        filename.append( ".csv" );
        file.open( filename.c_str(), ios::out | ios::trunc );

        //
        //  Print out the header row
        //

        file << "DAY,MIN_OPEN_PL,MAX_OPEN_PL,MIN_CLOSED_PL,MAX_CLOSED_PL,TRADES,CLOSED_PL" << std::endl;

        //
        //  Iterate over each system result and print it
        //

        for( system_result_iter = system_result_vec.begin(); system_result_iter != system_result_vec.end(); ++system_result_iter )
        {
            file << (*system_result_iter)->day << ','
                 << (*system_result_iter)->min_open_pl << ','
                 << (*system_result_iter)->max_open_pl << ','
                 << (*system_result_iter)->min_closed_pl << ','
                 << (*system_result_iter)->max_closed_pl << ','
                 << (*system_result_iter)->closed_qty << ','
                 << (*system_result_iter)->closed_pl << std::endl;

            total_trades += (*system_result_iter)->closed_qty;
            total_closed_pl += (*system_result_iter)->closed_pl;

            if( (*system_result_iter)->closed_pl < 0.0 )
            {
                losing_pl += fabs( (*system_result_iter)->closed_pl );
                losing_days += 1;
            }
            else
            {
                winning_pl += (*system_result_iter)->closed_pl;
            }

            delete (*system_result_iter);
        }

        winning_pct = ( ( 1 - ( double( losing_days ) / double( system_result_vec.size() ) ) ) * 100 );
        profit_factor = losing_pl == 0.0 ? 100 : double( winning_pl / losing_pl );

        //
        //  Print out the footer
        //

        file << std::endl << std::endl
             << "TOTAL TRADES," << total_trades << std::endl
             << "GROSS_PL," << total_closed_pl << std::endl
             << "NET_PL," << total_closed_pl - ( TRADE_COST * total_trades ) << std::endl
             << "WINNING PCT," << winning_pct << std::endl
             << "PROFIT FACTOR," << profit_factor << std::endl;

        file.close();
    }


    exit( 0 );
}
