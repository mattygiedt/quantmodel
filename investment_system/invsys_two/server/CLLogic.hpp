#ifndef CL_LOGIC_H
#define CL_LOGIC_H

enum CL_SYSTEM_STATUS
{
    CL_SYSTEM_FLAT = 0,
    CL_SYSTEM_LONG_FIRST,
    CL_SYSTEM_LONG_SECOND,
    CL_SYSTEM_SHORT_FIRST,
    CL_SYSTEM_SHORT_SECOND
};

enum CL_TRADE_STATUS
{
    CL_LOGIC_ENTRY_WAIT = 0,
    CL_LOGIC_LONG_ACTIVATED,
    CL_LOGIC_SHORT_ACTIVATED,
    CL_LOGIC_CONFIRM_ENTRY,
    CL_LOGIC_LONG,
    CL_LOGIC_SHORT,
    CL_LOGIC_CONFIRM_EXIT
};

struct TradeTarget
{
    TradeTarget()
    {

    }
};

class CLLogic : public TradeLogic
{
    public:

        CLLogic( )
        {
            timebar = NULL;
            is_open = false;

            unique_id = 0;
            best_bid = best_ask = 0LL;
        }

        void init( const XMLNode& config )
        {
            const XMLNode params = config.getChildNode( "params" );

            const time_t now = time( NULL );
            gmtime_r( &now, &timeinfo );
            strftime( timebuf, 16, "%H:%M:%S", &timeinfo );

            timebar = new TimeBar( 0 );

            timebar_pos = 0;
            last_entry_tm = 0;
            last_market_tm = 0;
            open_buy_qty = open_sell_qty = closed_qty = 0;
            open_buy_val = open_sell_val = open_buy_avg = open_sell_avg = closed_pl = 0.0;

            min_entry_one = 17;
            max_entry_one = 35;
            min_entry_two = 34;

            entry_one_ratio = 0.6;
            entry_two_ratio = 0.4651;

            trade_status[ 0 ] = CL_LOGIC_ENTRY_WAIT;
            trade_status[ 1 ] = CL_LOGIC_ENTRY_WAIT;

            security_id = atoi( params.getAttribute( "security_id" ) );
            multiplier = atof( params.getAttribute( "multiplier" ) );

            investment_system_id = params.getAttribute( "investment_system_id" );
            strategy_id = params.getAttribute( "strategy_id" );
            opening_tm = params.getAttribute( "opening_tm" );
            closing_tm = params.getAttribute( "closing_tm" );
            shutdown_tm = params.getAttribute( "shutdown_tm" );

            std::cout << timebuf << " -> INVSYS_TWO_CL INIT" << std::endl
                      << " investment_system_id: " << investment_system_id << std::endl
                      << " strategy_id: " << strategy_id << std::endl
                      << " security_id: " << security_id << std::endl
                      << " opening_tm: " << opening_tm << std::endl
                      << " closing_tm: " << closing_tm << std::endl
                      << " shutdown_tm: " << shutdown_tm << std::endl
                      << " multiplier: " << multiplier << std::endl;
        }

        void evaluate( const QMAPI::MarketDataBroadcast& broadcast,
                       std::vector< QMAPI::CreateTradeRequest >& trade_vec )
        {
            //
            //  Get the time right now
            //

            const time_t now = atol( broadcast.timestamp().c_str() );

            //
            //  Note that it's possible to gap seconds even during
            //  primary market hours.
            //

            while( now >= last_market_tm )
            {
                if( unlikely( last_market_tm == 0 ) )
                {
                    last_market_tm = now;
                }

                timebar_pos += 1;

                //
                //  Generate time variables
                //

                gmtime_r( &last_market_tm, &timeinfo );
                strftime( timebuf, 16, "%H:%M:%S", &timeinfo );

                //
                //  Update timebar?
                //

                if( timeinfo.tm_sec == 0 && timeinfo.tm_min % 5 == 0 )
                {
                    std::cout << " TIMEBAR: " << timebuf << std::endl;

                    timebar_pos = 0;

                    timebar_vec.push_back( timebar );
                    timebar = new TimeBar( last_market_tm );

                    if( trade_status[ 0 ] == CL_LOGIC_LONG_ACTIVATED ||
                        trade_status[ 0 ] == CL_LOGIC_SHORT_ACTIVATED )
                    {
                        trade_status[ 0 ] = CL_LOGIC_ENTRY_WAIT;
                    }

                    if( trade_status[ 1 ] == CL_LOGIC_LONG_ACTIVATED ||
                        trade_status[ 1 ] == CL_LOGIC_SHORT_ACTIVATED )
                    {
                        trade_status[ 1 ] = CL_LOGIC_ENTRY_WAIT;
                    }
                }

                //
                //  Is this an important last_market_tm period?
                //

                if( is_open == false && strcmp( opening_tm.c_str(), "NOW" ) == 0 )
                {
                    std::cout << timebuf << " -> INVSYS_TWO_CL OPEN" << std::endl;
                    is_open = true;
                }

                if( is_open == false && strcmp( opening_tm.c_str(), timebuf ) == 0 )
                {
                    std::cout << timebuf << " -> INVSYS_TWO_CL OPEN" << std::endl;
                    is_open = true;
                }

                if( is_open == true && strcmp( closing_tm.c_str(), timebuf ) == 0 )
                {
                    std::cout << timebuf << " -> INVSYS_TWO_CL CLOSED" << std::endl;
                    is_open = false;
                }

                if( strcmp( shutdown_tm.c_str(), timebuf ) == 0 )
                {
                    std::cout << timebuf << " -> INVSYS_TWO_CL SHUTDOWN" << std::endl;
                    exit( 0 );
                }

                //
                //  Increment the current market time
                //

                last_market_tm += 1;

                //
                //  Print out summary
                //

                std::cout
                    << timebuf << " [" << closed_pl << "] ["
                    << best_ask << " " << best_bid << "] ["
                    << timebar->getOpen() << "," << timebar->getHigh() << "," << timebar->getLow() << "," << timebar->getClose() << "] ["
                    << timebar->getUpHeight() << " " << timebar->getDownHeight() << "] ["
                    << timebar->getVolume() << " " << timebar->getAskAggressor() << " " << timebar->getBidAggressor() << "] "
                    << timebar->getAggressorRatio() << std::endl;
            }

            //
            //  We are (now) current, process the market data message
            //

            if( broadcast.broadcast_type() == QMAPI::MarketDataBroadcast::BID_QUOTE &&
                order_book.process_bid( broadcast ) == true )
            {
                order_book.getInsideQuotes( best_bid, best_ask );
            }
            else if( broadcast.broadcast_type() == QMAPI::MarketDataBroadcast::ASK_QUOTE &&
                     order_book.process_ask( broadcast ) == true )
            {
                order_book.getInsideQuotes( best_bid, best_ask );
            }
            else if( broadcast.broadcast_type() == QMAPI::MarketDataBroadcast::TRADE ||
                     broadcast.broadcast_type() == QMAPI::MarketDataBroadcast::ASK_TRADE ||
                     broadcast.broadcast_type() == QMAPI::MarketDataBroadcast::BID_TRADE )
            {
                timebar->addTrade( broadcast );
                testEntry( trade_vec );
            }

            testExit( trade_vec );
        }

        void testExit( std::vector< QMAPI::CreateTradeRequest >& trade_vec )
        {
            unsigned int signal_size = 0;
            std::string external_id;

            if( signal_size == 1 )
            {


            }
        }

        void testEntry( std::vector< QMAPI::CreateTradeRequest >& trade_vec )
        {
            if( canEnter() == false )
            {
                return;
            }

            std::string external_id;
            const unsigned int up_height = timebar->getUpHeight();
            const unsigned int down_height = timebar->getDownHeight();
            const double aggressor_ratio = timebar->getAggressorRatio();

            //
            //  Test for SHORT ENTRY ONE
            //

            if( trade_status[ 0 ] == CL_LOGIC_ENTRY_WAIT &&
                trade_status[ 1 ] == CL_LOGIC_ENTRY_WAIT &&
                up_height > 17 && up_height < 35 &&
                aggressor_ratio < ( 1 / entry_one_ratio ) )
            {
                aggressor_vec[ 0 ].clear();
                timebar->getAggressorData( aggressor_vec[ 0 ] );
                trade_status[ 0 ] = CL_LOGIC_SHORT_ACTIVATED;
                std::cout << "trade_status[ 0 ] = CL_LOGIC_SHORT_ACTIVATED" << std::endl;
                return;
            }
            else if( trade_status[ 0 ] == CL_LOGIC_SHORT_ACTIVATED )
            {
                timebar->getAggressorData( aggressor_vec[ 0 ] );

                //
                //  Look for...
                //
                
                std::cout << "SHORT ENTRY ONE:" << std::endl;

                data_riter = aggressor_vec[ 0 ].rbegin();
                while( data_riter !=  aggressor_vec[ 0 ].rend() )
                {
                    std::cout << "  " << (*data_riter).first << " " << (*data_riter).second << " "
                              << 1 / ( double( (*data_riter).first ) / double( (*data_riter).second ) ) << std::endl;
                    
                    ++data_riter;
                }
            }

            //
            //  Test for SHORT ENTRY TWO
            //

            if( trade_status[ 1 ] == CL_LOGIC_ENTRY_WAIT &&
                up_height > 35 && 
                aggressor_ratio < ( 1 / entry_two_ratio ) )
            {
                //aggressor_vec[ 1 ].clear();
                //timebar->getAggressorData( aggressor_vec[ 1 ] );
                //trade_status[ 1 ] = CL_LOGIC_SHORT_ACTIVATED;
                //std::cout << "trade_status[ 1 ] = CL_LOGIC_SHORT_ACTIVATED" << std::endl;
                return;
            }
            else if( trade_status[ 1 ] == CL_LOGIC_SHORT_ACTIVATED )
            {
                timebar->getAggressorData( aggressor_vec[ 1 ] );

                //
                //  Look for...
                //

                data_riter = aggressor_vec[ 1 ].rbegin();
                while( data_riter !=  aggressor_vec[ 1 ].rend() )
                {

                }
            }

            //
            //  Test for LONG ENTRY ONE
            //

            if( trade_status[ 0 ] == CL_LOGIC_ENTRY_WAIT &&
                trade_status[ 1 ] == CL_LOGIC_ENTRY_WAIT &&
                down_height > 17 && down_height < 35 &&
                aggressor_ratio < entry_one_ratio )
            {
                aggressor_vec[ 0 ].clear();
                timebar->getAggressorData( aggressor_vec[ 0 ] );
                trade_status[ 0 ] = CL_LOGIC_LONG_ACTIVATED;
                std::cout << "trade_status[ 0 ] = CL_LOGIC_LONG_ACTIVATED" << std::endl;
                return;
            }
            else if( trade_status[ 0 ] == CL_LOGIC_LONG_ACTIVATED )
            {
                timebar->getAggressorData( aggressor_vec[ 0 ] );

                //
                //  Look for...
                //

                data_riter = aggressor_vec[ 0 ].rbegin();
                while( data_riter !=  aggressor_vec[ 0 ].rend() )
                {
                    std::cout << "  " << (*data_riter).first << " " << (*data_riter).second << " " 
                              << ( double( (*data_riter).first ) / double( (*data_riter).second ) ) << std::endl;
                    
                    ++data_riter;
                }
            }

            //
            //  Test for LONG ENTRY TWO
            //

            if( trade_status[ 1 ] == CL_LOGIC_ENTRY_WAIT &&
                down_height > 35 &&
                aggressor_ratio < entry_two_ratio )
            {
                //aggressor_vec[ 1 ].clear();
                //timebar->getAggressorData( aggressor_vec[ 1 ] );
                //trade_status[ 1 ] = CL_LOGIC_LONG_ACTIVATED;
                //std::cout << "trade_status[ 1 ] = CL_LOGIC_LONG_ACTIVATED" << std::endl;
                return;
            }
            else if( trade_status[ 1 ] == CL_LOGIC_LONG_ACTIVATED )
            {
                timebar->getAggressorData( aggressor_vec[ 1 ] );

                //
                //  Look for...
                //

                data_riter = aggressor_vec[ 1 ].rbegin();
                while( data_riter !=  aggressor_vec[ 1 ].rend() )
                {

                }
            }
        }


        void confirmTrade( const std::string& ext_order_id )
        {
        }
/*
            if( internal_state == CL_LOGIC_CONFIRM_ENTRY &&
                ext_order_id == trade_entry.ext_order_id() )
            {
                if( trade_entry.side_code() == QMAPI::CreateTradeRequest::BUY )
                {
                    internal_state = CL_LOGIC_LONG;
                    addExecution( trade_entry, best_ask );

                    std::cout << " CONFIRM LONG: BUY "
                              << trade_entry.signal_size() << " @ " << best_ask << " ["
                              << target_data.retrace_price_1 << ","
                              << target_data.retrace_price_2 << ","
                              << target_data.retrace_price_3 << ","
                              << target_data.stop_loss << "]" << std::endl;
                }
                else
                {
                    internal_state = CL_LOGIC_SHORT;
                    addExecution( trade_entry, best_bid );

                    std::cout << " CONFIRM SHORT: SELL "
                              << trade_entry.signal_size() << " @ " << best_bid << " ["
                              << target_data.retrace_price_1 << ","
                              << target_data.retrace_price_2 << ","
                              << target_data.retrace_price_3 << ","
                              << target_data.stop_loss << "]" << std::endl;
                }
            }
            else if( internal_state == CL_LOGIC_CONFIRM_EXIT &&
                     ext_order_id == trade_exit.ext_order_id() )
            {
                if( trade_exit.side_code() == QMAPI::CreateTradeRequest::BUY )
                {
                    addExecution( trade_exit, best_ask );
                }
                else
                {
                    addExecution( trade_exit, best_bid );
                }

                if( open_sell_qty > 0 ) internal_state = CL_LOGIC_SHORT;
                else if( open_buy_qty > 0 ) internal_state = CL_LOGIC_LONG;
                else internal_state = CL_LOGIC_ENTRY_WAIT;
            }
        }
*/

    private:

        inline bool canEnter() const
        {
            if( timebar->canEnter() == false || timebar->getTimestamp() == 0 )
                return false;
            else
                return true;
        }

        inline void generateExternalOrderId( const std::string side, std::string& external_id )
        {
            char buf[ 16 ];
            sprintf( buf, "%03u:", ++unique_id );
            external_id.append( buf ).append( side ).append( ":TWO:CL:" ).append( timebuf );
        }

        inline std::string getInternalState( const CL_SYSTEM_STATUS status ) const
        {
            switch( status )
            {
                case CL_SYSTEM_FLAT: return "ENTRY_WAIT";
                case CL_SYSTEM_LONG_FIRST: return "LONG_FIRST";
                case CL_SYSTEM_LONG_SECOND: return "LONG_SECOND";
                case CL_SYSTEM_SHORT_FIRST: return "SHORT_FIRST";
                case CL_SYSTEM_SHORT_SECOND: return "SHORT_SECOND";
                default: return "";
            }
        }

        bool is_open;
        char timebuf[ 16 ];
        struct tm timeinfo;
        time_t last_entry_tm;

        long long int best_bid;
        long long int best_ask;
        unsigned int unique_id;
        unsigned int timebar_pos;
        unsigned int security_id;

        unsigned int min_entry_one;
        unsigned int max_entry_one;
        unsigned int min_entry_two;

        double entry_one_ratio;
        double entry_two_ratio;

        CL_SYSTEM_STATUS system_status;
        CL_TRADE_STATUS trade_status[ 2 ];

        TradeTarget target_data[ 2 ];
        QMAPI::CreateTradeRequest trade_entry[ 2 ];
        QMAPI::CreateTradeRequest trade_exit[ 2 ];

        TimeBar *timebar;
        std::vector< TimeBar* > timebar_vec;
        std::vector< TimeBar* >::const_iterator timebar_iter;

        std::vector< std::pair< int , int > > aggressor_vec[ 2 ];
        std::vector< std::pair< int , int > >::const_iterator data_iter;
        std::vector< std::pair< int , int > >::const_reverse_iterator data_riter;
};

#endif // CL_LOGIC_H
