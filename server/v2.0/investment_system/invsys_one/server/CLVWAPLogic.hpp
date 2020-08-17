#ifndef CL_VWAP_LOGIC_H
#define CL_VWAP_LOGIC_H

enum CL_VWAP_ENTRY_LEVEL
{
    CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO = 0,
    CL_VWAP_LOGIC_ENTRY_LEVEL_ONE,
    CL_VWAP_LOGIC_ENTRY_LEVEL_TWO,
    CL_VWAP_LOGIC_ENTRY_LEVEL_THREE,
    CL_VWAP_LOGIC_ENTRY_LEVEL_FOUR,
    CL_VWAP_LOGIC_ENTRY_LEVEL_FIVE
};

enum CL_VWAP_LOGIC_STATUS
{
    CL_VWAP_LOGIC_ENTRY_WAIT = 0,
    CL_VWAP_LOGIC_LONG,
    CL_VWAP_LOGIC_SHORT,
    CL_VWAP_LOGIC_CLOSED
};

class CLVWAPLogic : public TradeLogic
{
    public:

        CLVWAPLogic( const System& system )
        {
            last_timebar_timestamp = 0;
            VALUE = VOLUME = VWAP = 0.0;

            multiplier = 1000.0;
            status = CL_VWAP_LOGIC_ENTRY_WAIT;
            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO;

            price_offset[ 0 ] = 0.5;
            price_offset[ 1 ] = 1.05; //.96
            price_offset[ 2 ] = 1.75; //1.6

            position_multiplier[ 0 ] = 1;
            position_multiplier[ 1 ] = 2;
            position_multiplier[ 2 ] = 4;

            can_enter = false;
            use_trailing_stop = false;
            stop_entry_tm = 0;
            start_vwap_tm = 0;
            DAILY_LOSS_LIMIT = -5000.0;

            init();
        }

        void addExecutionInternal( )
        {
            if( open_buy_qty == 0 && open_sell_qty == 0 )
            {
                status = CL_VWAP_LOGIC_ENTRY_WAIT;
            }
            else if( open_buy_qty > 0 )
            {
                status = CL_VWAP_LOGIC_LONG;
            }
            else
            {
                status = CL_VWAP_LOGIC_SHORT;
            }
        }

        void open( System& system )
        {
            is_open = true;
            can_enter = true;
            status = CL_VWAP_LOGIC_ENTRY_WAIT;
            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO;
            start_vwap_tm = system.now - ( 3 * 60 * 60 ) - ( 30 * 60 ); // 0500AM
            stop_entry_tm = system.now + ( 3 * 60 * 60 ) + ( 15 * 60 ); // 1145AM
        }

        void close( System& system, std::vector< TradeInstruction* >& trade_instruction_vec )
        {
            is_open = false;
            can_enter = false;
            status = CL_VWAP_LOGIC_CLOSED;

            if( open_buy_qty > 0 )
            {
                trade_instruction_vec.push_back(
                    new TradeInstruction(
                        TRADE_INSTRUCTION_SIDE_SHORT,
                        TRADE_INSTRUCTION_TYPE_MARKET,
                        open_buy_qty ) );

                addExecution( open_buy_qty, best_bid, SIDE_CODE_SELL );
            }
            else if( open_sell_qty > 0 )
            {
                trade_instruction_vec.push_back(
                    new TradeInstruction(
                        TRADE_INSTRUCTION_SIDE_LONG,
                        TRADE_INSTRUCTION_TYPE_MARKET,
                        open_sell_qty ) );

                addExecution( open_sell_qty, best_ask, SIDE_CODE_BUY );
            }
        }

        bool evaluate( System& system, std::vector< TradeInstruction* >& trade_instruction_vec )
        {
            if( is_open == false ||
                system.five_minute_timebars.size() < 1 )
            {
                return false;
            }

            exit_qty = entry_qty = 0;
            best_ask = system.best_ask * system.price_factor;
            best_bid = system.best_bid * system.price_factor;

            //
            //  Calculate VWAP from the timebars
            //

            if( unlikely( last_timebar_timestamp != system.five_minute_timebar->timestamp ) )
            {
                VALUE = VOLUME = VWAP = 0.0;
                last_timebar_timestamp = system.five_minute_timebar->timestamp;

                for( timebar_iter = system.five_minute_timebars.begin();
                     timebar_iter != system.five_minute_timebars.end();
                     ++timebar_iter )
                {
                    if( (*timebar_iter)->timestamp >= start_vwap_tm )
                    {
                        VALUE += (*timebar_iter)->value();
                        VOLUME += (*timebar_iter)->volume();
                    }
                }

                if( VOLUME > 0 )
                {
                    VWAP = VALUE / VOLUME;
                }
                else
                {
                    system.log->warn( "VWAP calculation error: VOLUME == 0" );
                    return false;
                }

                //if( best_ask > VWAP )
                //{
                //    VWAP = roundUp( VWAP );
                //}
                //else
                //{
                //    VWAP = roundDown( VWAP );
                //}

                system.log->info( " VWAP: %f", VWAP );
                system.log->info( " VOLUME: %f", VOLUME );
                system.log->info( " BEST_ASK: %f", best_ask );
                system.log->info( " BEST_BID: %f", best_bid );

                if( status == CL_VWAP_LOGIC_ENTRY_WAIT )
                {
                    system.log->info( " STATUS: CL_VWAP_LOGIC_ENTRY_WAIT" );
                }
                else if( status == CL_VWAP_LOGIC_LONG )
                {
                    system.log->info( " STATUS: CL_VWAP_LOGIC_LONG" );
                }
                else if( status == CL_VWAP_LOGIC_SHORT )
                {
                    system.log->info( " STATUS: CL_VWAP_LOGIC_SHORT" );
                }

                if( curr_entry_level == CL_VWAP_LOGIC_ENTRY_LEVEL_FIVE )
                {
                    system.log->info( " CURR_ENTRY_LEVEL: CL_VWAP_LOGIC_ENTRY_LEVEL_FIVE" );
                }
                else if( curr_entry_level == CL_VWAP_LOGIC_ENTRY_LEVEL_FOUR )
                {
                    system.log->info( " CURR_ENTRY_LEVEL: CL_VWAP_LOGIC_ENTRY_LEVEL_FOUR" );
                }
                else if( curr_entry_level == CL_VWAP_LOGIC_ENTRY_LEVEL_THREE )
                {
                    system.log->info( " CURR_ENTRY_LEVEL: CL_VWAP_LOGIC_ENTRY_LEVEL_THREE" );
                }
                else if( curr_entry_level == CL_VWAP_LOGIC_ENTRY_LEVEL_TWO )
                {
                    system.log->info( " CURR_ENTRY_LEVEL: CL_VWAP_LOGIC_ENTRY_LEVEL_TWO" );
                }
                else if( curr_entry_level == CL_VWAP_LOGIC_ENTRY_LEVEL_ONE )
                {
                    system.log->info( " CURR_ENTRY_LEVEL: CL_VWAP_LOGIC_ENTRY_LEVEL_ONE" );
                }
                else if( curr_entry_level == CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO )
                {
                    system.log->info( " CURR_ENTRY_LEVEL: CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO" );
                }

                //
                //  DONE_FOR_DAY?
                //

                if( system.five_minute_timebar->timestamp > stop_entry_tm &&
                    status == CL_VWAP_LOGIC_ENTRY_WAIT )
                {
                    system.log->info( " DONE_FOR_DAY: system.now > stop_entry_tm" );
                    system.status = "DONE";

                    is_open = false;
                    return false;
                }

                system.log->info( " SYSTEM_PL: %f", getSystemPL( best_ask, best_bid ) );

                //
                //  ENTRY LOGIC
                //

                if( can_enter == true &&
                    system.five_minute_timebar->timestamp < stop_entry_tm )
                {
                    //
                    //  Calculate the entry levels
                    //

                    if( best_ask > VWAP )
                    {
                        entry_level[ 0 ] = VWAP + price_offset[ 0 ];
                        entry_level[ 1 ] = VWAP + price_offset[ 1 ];
                        entry_level[ 2 ] = VWAP + price_offset[ 2 ];

                        entry_side = TRADE_INSTRUCTION_SIDE_SHORT;

                        //
                        //  Are we able to enter into a SHORT position?
                        //

                        if( curr_entry_level == CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO )
                        {
                            if( best_ask > entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO ] )
                            {
                                entry_prc_one[ 0 ] = best_ask;
                                entry_qty = position_multiplier[ CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO ];
                                exit_level_one[ 0 ] = false;
                                curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ONE;

                                system.log->info( "  ENTRY_LEVEL_ZERO: %f > %f, SELL %ld",
                                    best_ask, entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO ], entry_qty );
                            }
                            else
                            {
                                system.log->info( "  FAIL ENTRY_LEVEL_ZERO: %f <= %f",
                                    best_ask, entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO ] );
                            }
                        }
                        else if( curr_entry_level == CL_VWAP_LOGIC_ENTRY_LEVEL_ONE )
                        {
                            if( best_ask > entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_ONE ] )
                            {
                                entry_prc_two[ 0 ] = entry_prc_two[ 1 ] = best_ask;
                                entry_qty = position_multiplier[ CL_VWAP_LOGIC_ENTRY_LEVEL_ONE ];
                                exit_level_two[ 0 ] = exit_level_two[ 1 ] = false;
                                curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_TWO;

                                system.log->info( "  ENTRY_LEVEL_ONE: %f > %f, SELL %ld",
                                    best_ask, entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_ONE ], entry_qty );
                            }
                            else
                            {
                                system.log->info( "  FAIL ENTRY_LEVEL_ONE: %f <= %f",
                                    best_ask, entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_ONE ] );
                            }
                        }
                        else if( curr_entry_level == CL_VWAP_LOGIC_ENTRY_LEVEL_TWO )
                        {
                            if( best_ask > entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_TWO ] )
                            {
                                entry_prc_three[ 0 ] = entry_prc_three[ 1 ] = entry_prc_three[ 2 ] = best_ask;
                                entry_qty = position_multiplier[ CL_VWAP_LOGIC_ENTRY_LEVEL_TWO ];
                                exit_level_three[ 0 ] = exit_level_three[ 1 ] = exit_level_three[ 2 ] = false;
                                curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_THREE;

                                system.log->info( "  ENTRY_LEVEL_TWO: %f > %f, SELL %ld",
                                    best_ask, entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_TWO ], entry_qty );
                            }
                            else
                            {
                                system.log->info( "  FAIL ENTRY_LEVEL_TWO: %f <= %f",
                                    best_ask, entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_TWO ] );
                            }
                        }
                    }
                    else if( best_bid < VWAP )
                    {
                        entry_level[ 0 ] = VWAP - price_offset[ 0 ];
                        entry_level[ 1 ] = VWAP - price_offset[ 1 ];
                        entry_level[ 2 ] = VWAP - price_offset[ 2 ];

                        entry_side = TRADE_INSTRUCTION_SIDE_LONG;

                        //
                        //  Are we able to enter into a LONG position?
                        //

                        if( curr_entry_level == CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO )
                        {
                            if( best_bid < entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO ] )
                            {
                                entry_prc_one[ 0 ] = best_bid;
                                entry_qty = position_multiplier[ CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO ];
                                exit_level_one[ 0 ] = stop_exit_one[ 0 ] = false;
                                curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ONE;

                                system.log->info( "  ENTRY_LEVEL_ZERO: %f < %f, BUY %ld",
                                    best_bid, entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO ], entry_qty );
                            }
                            else
                            {
                                system.log->info( "  FAIL ENTRY_LEVEL_ZERO: %f >= %f",
                                    best_bid, entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO ] );
                            }
                        }
                        else if( curr_entry_level == CL_VWAP_LOGIC_ENTRY_LEVEL_ONE )
                        {
                            if( best_bid < entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_ONE ] )
                            {
                                entry_prc_two[ 0 ] = entry_prc_two[ 1 ] = best_bid;
                                entry_qty = position_multiplier[ CL_VWAP_LOGIC_ENTRY_LEVEL_ONE ];
                                exit_level_two[ 0 ] = exit_level_two[ 1 ] =
                                stop_exit_two[ 0 ] = stop_exit_two[ 1 ] = false;
                                curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_TWO;

                                system.log->info( "  ENTRY_LEVEL_ONE: %f < %f, BUY %ld",
                                    best_bid, entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_ONE ], entry_qty );
                            }
                            else
                            {
                                system.log->info( "  FAIL ENTRY_LEVEL_ONE: %f >= %f",
                                    best_bid, entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_ONE ] );
                            }
                        }
                        else if( curr_entry_level == CL_VWAP_LOGIC_ENTRY_LEVEL_TWO )
                        {
                            if( best_bid < entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_TWO ] )
                            {
                                entry_prc_three[ 0 ] = entry_prc_three[ 1 ] = entry_prc_three[ 2 ] = best_bid;
                                entry_qty = position_multiplier[ CL_VWAP_LOGIC_ENTRY_LEVEL_TWO ];
                                exit_level_three[ 0 ] = exit_level_three[ 1 ] = exit_level_three[ 2 ] =
                                stop_exit_three[ 0 ] = stop_exit_three[ 1 ] = stop_exit_three[ 2 ] = false;
                                curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_THREE;

                                system.log->info( "  ENTRY_LEVEL_TWO: %f < %f, BUY %ld",
                                    best_bid, entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_TWO ], entry_qty );
                            }
                            else
                            {
                                system.log->info( "  FAIL ENTRY_LEVEL_TWO: %f >= %f",
                                    best_bid, entry_level[ CL_VWAP_LOGIC_ENTRY_LEVEL_TWO ] );
                            }
                        }
                    }

                    //
                    //  Do we have any entry trades?
                    //

                    if( unlikely( entry_qty > 0 ) )
                    {
                        trade_instruction_vec.push_back(
                            new TradeInstruction(
                                entry_side,
                                TRADE_INSTRUCTION_TYPE_MARKET,
                                entry_qty ) );

                        if( entry_side == TRADE_INSTRUCTION_SIDE_SHORT )
                        {
                            addExecution( entry_qty, best_bid, SIDE_CODE_SELL );
                        }
                        else
                        {
                            addExecution( entry_qty, best_ask, SIDE_CODE_BUY );
                        }

                        return true;
                    }
                }
            }

            //
            //  EXIT LOGIC
            //

            if( status == CL_VWAP_LOGIC_LONG )
            {
                exit_prc = best_bid;
                exit_side = TRADE_INSTRUCTION_SIDE_SHORT;
                const double system_pl = getSystemPL( best_ask, best_bid );

                if( curr_entry_level >= CL_VWAP_LOGIC_ENTRY_LEVEL_THREE )
                {
                    exit_level[ 0 ] = VWAP - 1.4;
                    exit_level[ 1 ] = VWAP - 1.2;
                    exit_level[ 2 ] = VWAP - 0.9;

                    if( use_trailing_stop == false )
                    {
                        if( exit_level_three[ 0 ] == false && exit_prc >= exit_level[ 0 ] )
                        {
                            exit_level_three[ 0 ] = true;
                            exit_qty += 2;

                            system.log->info( " EXIT_LEVEL_THREE_ZERO: %f >= %f, SELL %ld",
                                exit_prc, exit_level[ 0 ], exit_qty );
                        }

                        if( exit_level_three[ 1 ] == false && exit_prc >= exit_level[ 1 ] )
                        {
                            exit_level_three[ 1 ] = true;
                            exit_qty += 1;

                            system.log->info( " EXIT_LEVEL_THREE_ONE: %f >= %f, SELL %ld",
                                exit_prc, exit_level[ 1 ], exit_qty );
                        }

                        if( exit_level_three[ 2 ] == false && exit_prc >= exit_level[ 2 ] )
                        {
                            exit_level_three[ 2 ] = true;
                            exit_qty += 1;

                            can_enter = false;
                            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_TWO;

                            system.log->info( " EXIT_LEVEL_THREE_TWO: %f >= %f, SELL %ld",
                                exit_prc, exit_level[ 2 ], exit_qty );
                        }
                    }
                    else
                    {
                        if( exit_level_three[ 0 ] == false && exit_prc >= exit_level[ 0 ] )
                        {
                            exit_level_three[ 0 ] = true;

                            if( exit_prc > entry_prc_three[ 0 ] )
                            {
                                stop_level_three[ 0 ] = exit_prc;
                            }
                            else
                            {
                                exit_qty += 2;
                                stop_exit_three[ 0 ] = true;
                            }
                        }
                        else if( stop_exit_three[ 0 ] == false && exit_level_three[ 0 ] == true && exit_prc > stop_level_three[ 0 ] )
                        {
                            entry_prc_three[ 0 ] += exit_prc - stop_level_three[ 0 ];
                            stop_level_three[ 0 ] = exit_prc;
                        }
                        else if( stop_exit_three[ 0 ] == false && exit_level_three[ 0 ] == true && ( exit_prc > VWAP || exit_prc <= entry_prc_three[ 0 ] ) )
                        {
                            exit_qty += 2;
                            stop_exit_three[ 0 ] = true;
                        }

                        if( exit_level_three[ 1 ] == false && exit_prc >= exit_level[ 1 ] )
                        {
                            exit_level_three[ 1 ] = true;

                            if( exit_prc > entry_prc_three[ 1 ] )
                            {
                                stop_level_three[ 1 ] = exit_prc;
                            }
                            else
                            {
                                exit_qty += 1;
                                stop_exit_three[ 1 ] = true;
                            }
                        }
                        else if( stop_exit_three[ 1 ] == false && exit_level_three[ 1 ] == true && exit_prc > stop_level_three[ 1 ] )
                        {
                            entry_prc_three[ 1 ] += exit_prc - stop_level_three[ 1 ];
                            stop_level_three[ 1 ] = exit_prc;
                        }
                        else if( stop_exit_three[ 1 ] == false && exit_level_three[ 1 ] == true && ( exit_prc > VWAP || exit_prc <= entry_prc_three[ 1 ] ) )
                        {
                            exit_qty += 1;
                            stop_exit_three[ 1 ] = true;
                        }

                        if( exit_level_three[ 2 ] == false && exit_prc >= exit_level[ 2 ] )
                        {
                            can_enter = false;

                            if( exit_prc > entry_prc_three[ 2 ] )
                            {
                                stop_level_three[ 2 ] = exit_prc;
                            }
                            else
                            {
                                exit_qty += 1;
                                stop_exit_three[ 2 ] = true;
                                curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_TWO;
                            }
                        }
                        else if( stop_exit_three[ 2 ] == false && exit_level_three[ 2 ] == true && exit_prc > stop_level_three[ 2 ] )
                        {
                            entry_prc_three[ 2 ] += exit_prc - stop_level_three[ 2 ];
                            stop_level_three[ 2 ] = exit_prc;
                        }
                        else if( stop_exit_three[ 2 ] == false && exit_level_three[ 2 ] == true && ( exit_prc > VWAP || exit_prc <= entry_prc_three[ 2 ] ) )
                        {
                            exit_qty += 1;
                            stop_exit_three[ 2 ] = true;
                            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_TWO;
                        }
                    }
                }

                if( curr_entry_level >= CL_VWAP_LOGIC_ENTRY_LEVEL_TWO )
                {
                    exit_level[ 0 ] = VWAP - 0.71;
                    exit_level[ 1 ] = VWAP - 0.45;

                    if( use_trailing_stop == false )
                    {
                        if( exit_level_two[ 0 ] == false && exit_prc >= exit_level[ 0 ] )
                        {
                            exit_level_two[ 0 ] = true;
                            exit_qty += 1;

                            system.log->info( " EXIT_LEVEL_TWO_ZERO: VWAP %f >= %f SELL %ld",
                                exit_prc, exit_level[ 0 ], exit_qty );
                        }

                        if( exit_level_two[ 1 ] == false && exit_prc >= exit_level[ 1 ] )
                        {
                            exit_level_two[ 1 ] = true;
                            exit_qty += 1;

                            system.log->info( " EXIT_LEVEL_TWO_ONE: VWAP %f >= %f, SELL %ld",
                                exit_prc, exit_level[ 1 ], exit_qty );

                            //
                            //  This is the last level 2, set the current entry level to 1.
                            //

                            can_enter = false;
                            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ONE;
                        }
                    }
                    else
                    {
                        if( exit_level_two[ 0 ] == false && exit_prc >= exit_level[ 0 ] )
                        {
                            exit_level_two[ 0 ] = true;

                            if( exit_prc > entry_prc_two[ 0 ] )
                            {
                                stop_level_two[ 0 ] = exit_prc;
                            }
                            else
                            {
                                exit_qty += 1;
                                stop_exit_two[ 0 ] = true;
                            }
                        }
                        else if( exit_level_two[ 0 ] == true && stop_exit_two[ 0 ] == false && exit_prc > stop_level_two[ 0 ] )
                        {
                            entry_prc_two[ 0 ] += exit_prc - stop_level_two[ 0 ];
                            stop_level_two[ 0 ] = exit_prc;
                        }
                        else if( exit_level_two[ 0 ] == true && stop_exit_two[ 0 ] == false && ( exit_prc > VWAP || exit_prc <= entry_prc_two[ 0 ] ) )
                        {
                            exit_qty += 1;
                            stop_exit_two[ 0 ] = true;
                        }

                        if( exit_level_two[ 1 ] == false && exit_prc >= exit_level[ 1 ] )
                        {
                            can_enter = false;
                            exit_level_two[ 1 ] = true;

                            if( exit_prc > entry_prc_two[ 1 ] )
                            {
                                stop_level_two[ 1 ] = exit_prc;
                            }
                            else
                            {
                                exit_qty += 1;
                                stop_exit_two[ 1 ] = true;
                                curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ONE;
                            }
                        }
                        else if( exit_level_two[ 1 ] == true && stop_exit_two[ 1 ] == false && exit_prc > stop_level_two[ 1 ] )
                        {
                            entry_prc_two[ 1 ] += exit_prc - stop_level_two[ 1 ];
                            stop_level_two[ 1 ] = exit_prc;
                        }
                        else if( exit_level_two[ 1 ] == true && stop_exit_two[ 1 ] == false && ( exit_prc > VWAP || exit_prc <= entry_prc_two[ 1 ] ) )
                        {
                            exit_qty += 1;
                            stop_exit_two[ 1 ] = true;
                            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ONE;
                        }
                    }
                }

                if( curr_entry_level >= CL_VWAP_LOGIC_ENTRY_LEVEL_ONE )
                {
                    exit_level[ 0 ] = VWAP - 0.3;

                    if( use_trailing_stop == false )
                    {
                        if( exit_level_one[ 0 ] == false && exit_prc >= exit_level[ 0 ] )
                        {
                            exit_level_one[ 0 ] = true;
                            exit_qty += 1;

                            system.log->info( " EXIT_LEVEL_ONE_ZERO: VWAP %f >= %f, SELL %ld",
                                exit_prc, exit_level[ 0 ], exit_qty );

                            system.status = "DONE";
                            is_open = false;

                            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO;
                        }
                    }
                    else
                    {
                        if( exit_level_one[ 0 ] == false && exit_prc >= exit_level[ 0 ] )
                        {
                            can_enter = false;
                            exit_level_one[ 0 ] = true;

                            if( exit_prc > entry_prc_one[ 0 ] )
                            {
                                stop_level_one[ 0 ] = exit_prc;
                            }
                            else
                            {
                                exit_qty += 1;
                                is_open = false;
                                system.status = "DONE";
                                curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO;
                                system.log->info( " EXIT_LEVEL_ONE_ZERO: VWAP %f >= %f, SELL %ld",
                                    exit_prc, exit_level[ 0 ], exit_qty );
                            }
                        }
                        else if( exit_level_one[ 0 ] == true && exit_prc > stop_level_one[ 0 ] )
                        {
                            entry_prc_one[ 0 ] += exit_prc - stop_level_one[ 0 ];
                            stop_level_one[ 0 ] = exit_prc;
                        }
                        else if( exit_level_one[ 0 ] == true && ( exit_prc > VWAP || exit_prc <= entry_prc_one[ 0 ] ) )
                        {
                            exit_qty += 1;
                            is_open = false;
                            system.status = "DONE";
                            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO;
                        }
                    }
                }

                if( system_pl < DAILY_LOSS_LIMIT )
                {
                    system.log->warn( "CL_VWAP_LOGIC_LONG_EXIT: DAILY_LOSS_LIMIT @ %ld", system.now );

                    system.status = "DONE";
                    is_open = false;
                    exit_qty = open_buy_qty;
                }
            }
            else if( status == CL_VWAP_LOGIC_SHORT )
            {
                exit_prc = best_ask;
                exit_side = TRADE_INSTRUCTION_SIDE_LONG;
                const double system_pl = getSystemPL( best_ask, best_bid );

                if( curr_entry_level >= CL_VWAP_LOGIC_ENTRY_LEVEL_THREE )
                {
                    exit_level[ 0 ] = VWAP + 1.4;
                    exit_level[ 1 ] = VWAP + 1.2;
                    exit_level[ 2 ] = VWAP + 0.9;

                    if( use_trailing_stop == false )
                    {
                        if( exit_level_three[ 0 ] == false && exit_prc <= exit_level[ 0 ] )
                        {
                            exit_level_three[ 0 ] = true;
                            exit_qty += 2;

                            system.log->info( " EXIT_LEVEL_THREE_ZERO: %f <= %f, BUY %ld",
                                exit_prc, exit_level[ 0 ], exit_qty );
                        }

                        if( exit_level_three[ 1 ] == false && exit_prc <= exit_level[ 1 ] )
                        {
                            exit_level_three[ 1 ] = true;
                            exit_qty += 1;

                            system.log->info( " EXIT_LEVEL_THREE_ONE: %f <= %f, BUY %ld",
                                exit_prc, exit_level[ 1 ], exit_qty );
                        }

                        if( exit_level_three[ 2 ] == false && exit_prc <= exit_level[ 2 ] )
                        {
                            exit_level_three[ 2 ] = true;
                            exit_qty += 1;

                            can_enter = false;
                            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_TWO;

                            system.log->info( " EXIT_LEVEL_THREE_TWO: %f <= %f, BUY %ld",
                                exit_prc, exit_level[ 2 ], exit_qty );
                        }
                    }
                    else
                    {
                        if( exit_level_three[ 0 ] == false && exit_prc <= exit_level[ 0 ] )
                        {
                            exit_level_three[ 0 ] = true;

                            if( exit_prc < entry_prc_three[ 0 ] )
                            {
                                stop_level_three[ 0 ] = exit_prc;
                            }
                            else
                            {
                                exit_qty += 2;
                                stop_exit_three[ 0 ] = true;
                                system.log->info( " EXIT_LEVEL_THREE_ZERO: %f <= %f, BUY %ld",
                                    exit_prc, exit_level[ 0 ], exit_qty );
                            }
                        }
                        else if( exit_level_three[ 0 ] == true && stop_exit_three[ 0 ] == false && exit_prc < stop_level_three[ 0 ] )
                        {
                            entry_prc_three[ 0 ] -= stop_level_three[ 0 ] - exit_prc;
                            stop_level_three[ 0 ] = exit_prc;
                        }
                        else if( exit_level_three[ 0 ] == true && stop_exit_three[ 0 ] == false && ( exit_prc < VWAP || exit_prc >= entry_prc_three[ 0 ] ) )
                        {
                            exit_qty += 2;
                            stop_exit_three[ 0 ] = true;
                        }

                        if( exit_level_three[ 1 ] == false && exit_prc <= exit_level[ 1 ] )
                        {
                            exit_level_three[ 1 ] = true;

                            if( exit_prc < entry_prc_three[ 1 ] )
                            {
                                stop_level_three[ 1 ] = exit_prc;
                            }
                            else
                            {
                                exit_qty += 1;
                                stop_exit_three[ 1 ] = true;
                                system.log->info( " EXIT_LEVEL_THREE_ONE: %f <= %f, BUY %ld",
                                    exit_prc, exit_level[ 1 ], exit_qty );
                            }
                        }
                        else if( exit_level_three[ 1 ] == true && stop_exit_three[ 1 ] == false && exit_prc < stop_level_three[ 1 ] )
                        {
                            entry_prc_three[ 1 ] -= stop_level_three[ 1 ] - exit_prc;
                            stop_level_three[ 1 ] = exit_prc;
                        }
                        else if( exit_level_three[ 1 ] == true && stop_exit_three[ 1 ] == false && ( exit_prc < VWAP || exit_prc >= entry_prc_three[ 1 ] ) )
                        {
                            exit_qty += 1;
                            stop_exit_three[ 1 ] = true;
                        }

                        if( exit_level_three[ 2 ] == false && exit_prc <= exit_level[ 2 ] )
                        {
                            can_enter = false;
                            exit_level_three[ 2 ] = true;

                            if( exit_prc < entry_prc_three[ 2 ] )
                            {
                                stop_level_three[ 2 ] = exit_prc;
                            }
                            else
                            {
                                exit_qty += 1;
                                stop_exit_three[ 2 ] = true;
                                curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_TWO;

                                system.log->info( " EXIT_LEVEL_THREE_TWO: %f <= %f, BUY %ld",
                                    exit_prc, exit_level[ 2 ], exit_qty );
                            }
                        }
                        else if( exit_level_three[ 2 ] == true && stop_exit_three[ 2 ] == false && exit_prc < stop_level_three[ 2 ] )
                        {
                            entry_prc_three[ 2 ] -= stop_level_three[ 2 ] - exit_prc;
                            stop_level_three[ 2 ] = exit_prc;
                        }
                        else if( exit_level_three[ 2 ] == true && stop_exit_three[ 2 ] == false && ( exit_prc < VWAP || exit_prc >= entry_prc_three[ 2 ] ) )
                        {
                            exit_qty += 1;
                            stop_exit_three[ 2 ] = true;
                            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_TWO;
                        }
                    }
                }

                if( curr_entry_level >= CL_VWAP_LOGIC_ENTRY_LEVEL_TWO )
                {
                    exit_level[ 0 ] = VWAP + 0.71;
                    exit_level[ 1 ] = VWAP + 0.45;

                    if( use_trailing_stop == false )
                    {
                        if( exit_level_two[ 0 ] == false && exit_prc <= exit_level[ 0 ] )
                        {
                            exit_level_two[ 0 ] = true;
                            exit_qty += 1;

                            system.log->info( " EXIT_LEVEL_TWO_ZERO: VWAP %f <= %f, BUY %ld",
                                exit_prc, exit_level[ 0 ], exit_qty );
                        }

                        if( exit_level_two[ 1 ] == false && exit_prc <= exit_level[ 1 ] )
                        {
                            exit_level_two[ 1 ] = true;
                            exit_qty += 1;

                            system.log->info( " EXIT_LEVEL_TWO_ONE: VWAP %f <= %f, BUY %ld",
                                exit_prc, exit_level[ 1 ], exit_qty );

                            //
                            //  This is the last level 2, set the current entry level to 1.
                            //

                            can_enter = false;
                            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ONE;
                        }
                    }
                    else
                    {
                        if( exit_level_two[ 0 ] == false && exit_prc <= exit_level[ 0 ] )
                        {
                            exit_level_two[ 0 ] = true;

                            if( exit_prc < entry_prc_two[ 0 ] )
                            {
                                stop_level_two[ 0 ] = exit_prc;
                            }
                            else
                            {
                                exit_qty += 1;
                                stop_exit_two[ 0 ] = true;

                                system.log->info( " EXIT_LEVEL_TWO_ZERO: VWAP %f <= %f, BUY %ld",
                                    exit_prc, exit_level[ 0 ], exit_qty );
                            }
                        }
                        else if( exit_level_two[ 0 ] == true && stop_exit_two[ 0 ] == false && exit_prc < stop_level_two[ 0 ] )
                        {
                            entry_prc_two[ 0 ] -= stop_level_two[ 0 ] - exit_prc;
                            stop_level_two[ 0 ] = exit_prc;
                        }
                        else if( exit_level_two[ 0 ] == true && stop_exit_two[ 0 ] == false && ( exit_prc < VWAP || exit_prc >= entry_prc_two[ 0 ] ) )
                        {
                            exit_qty += 1;
                            stop_exit_two[ 0 ] = true;
                        }

                        if( exit_level_two[ 1 ] == false && exit_prc <= exit_level[ 1 ] )
                        {
                            can_enter = false;
                            exit_level_two[ 1 ] = true;

                            if( exit_prc < entry_prc_two[ 1 ] )
                            {
                                stop_level_two[ 1 ] = exit_prc;
                            }
                            else
                            {
                                exit_qty += 1;
                                stop_exit_two[ 1 ] = true;
                                curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ONE;
                                system.log->info( " EXIT_LEVEL_TWO_ONE: VWAP %f <= %f, BUY %ld",
                                    exit_prc, exit_level[ 1 ], exit_qty );
                            }
                        }
                        else if( exit_level_two[ 1 ] == true && stop_exit_two[ 1 ] == false && exit_prc < stop_level_two[ 1 ] )
                        {
                            entry_prc_two[ 1 ] -= stop_level_two[ 1 ] - exit_prc;
                            stop_level_two[ 1 ] = exit_prc;
                        }
                        else if( exit_level_two[ 1 ] == true && stop_exit_two[ 1 ] == false && ( exit_prc < VWAP || exit_prc >= entry_prc_two[ 1 ] ) )
                        {
                            exit_qty += 1;
                            stop_exit_two[ 1 ] = true;
                            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ONE;
                        }
                    }
                }

                if( curr_entry_level >= CL_VWAP_LOGIC_ENTRY_LEVEL_ONE )
                {
                    exit_level[ 0 ] = VWAP + 0.3;

                    if( use_trailing_stop == false )
                    {
                        if( exit_level_one[ 0 ] == false && exit_prc <= exit_level[ 0 ] )
                        {
                            exit_level_one[ 0 ] = true;
                            exit_qty += 1;

                            system.log->info( " EXIT_LEVEL_ONE_ZERO: VWAP %f <= %f, BUY %ld",
                                exit_prc, exit_level[ 0 ], exit_qty );

                            is_open = false;
                            system.status = "DONE";

                            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO;
                        }
                    }
                    else
                    {
                        if( exit_level_one[ 0 ] == false && exit_prc <= exit_level[ 0 ] )
                        {
                            can_enter = false;
                            exit_level_one[ 0 ] = true;

                            if( exit_prc < entry_prc_one[ 0 ] )
                            {
                                stop_level_one[ 0 ] = exit_prc;
                                system.log->info( " STOP_EXIT_PRC: %f", entry_prc_one[ 0 ] );
                            }
                            else
                            {
                                exit_qty += 1;
                                is_open = false;
                                system.status = "DONE";
                                curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO;
                                system.log->info( " EXIT_LEVEL_ONE_ZERO: VWAP %f <= %f, BUY %ld",
                                    exit_prc, exit_level[ 0 ], exit_qty );
                            }
                        }
                        else if( exit_level_one[ 0 ] == true && exit_prc < stop_level_one[ 0 ] )
                        {
                            entry_prc_one[ 0 ] -= stop_level_one[ 0 ] - exit_prc;
                            stop_level_one[ 0 ] = exit_prc;
                            system.log->info( " NEW STOP_EXIT_PRC: %f", entry_prc_one[ 0 ] );
                        }
                        else if( exit_level_one[ 0 ] == true && ( exit_prc < VWAP || exit_prc >= entry_prc_one[ 0 ] ) )
                        {
                            exit_qty += 1;
                            is_open = false;
                            system.status = "DONE";
                            curr_entry_level = CL_VWAP_LOGIC_ENTRY_LEVEL_ZERO;
                            system.log->info( " STOP_EXIT: %f >= %f", exit_prc, entry_prc_one[ 0 ] );
                        }
                    }
                }

                if( system_pl < DAILY_LOSS_LIMIT )
                {
                    system.log->warn( "CL_VWAP_LOGIC_SHORT_EXIT: DAILY_LOSS_LIMIT @ %ld", system.now );

                    is_open = false;
                    system.status = "DONE";
                    exit_qty = open_sell_qty;
                }
            }

            //
            //  Do we have any exit trades?
            //

            if( unlikely( exit_qty > 0 ) )
            {
                trade_instruction_vec.push_back(
                    new TradeInstruction(
                        exit_side,
                        TRADE_INSTRUCTION_TYPE_MARKET,
                        exit_qty ) );

                if( exit_side == TRADE_INSTRUCTION_SIDE_SHORT )
                {
                    addExecution( exit_qty, best_bid, SIDE_CODE_SELL );
                }
                else
                {
                    addExecution( exit_qty, best_ask, SIDE_CODE_BUY );
                }

                return true;
            }

            //
            //  No exit trades
            //

            return false;
        }

    private:

        unsigned int entry_qty, exit_qty;
        TRADE_INSTRUCTION_SIDE entry_side, exit_side;

        double VALUE, VOLUME, VWAP, DAILY_LOSS_LIMIT;
        time_t start_vwap_tm, stop_entry_tm, last_timebar_timestamp;

        double exit_prc;
        double exit_level[ 4 ];
        double entry_level[ 4 ];
        double price_offset[ 4 ];

        double entry_prc_one[ 1 ];
        double entry_prc_two[ 2 ];
        double entry_prc_three[ 3 ];

        double stop_level_one[ 1 ];
        double stop_level_two[ 2 ];
        double stop_level_three[ 3 ];

        unsigned int position_multiplier[ 4 ];

        bool can_enter;
        bool use_trailing_stop;

        bool exit_level_one[ 1 ];
        bool exit_level_two[ 2 ];
        bool exit_level_three[ 3 ];

        bool stop_exit_one[ 1 ];
        bool stop_exit_two[ 2 ];
        bool stop_exit_three[ 3 ];

        std::vector< Timebar* >::const_iterator timebar_iter;

        CL_VWAP_LOGIC_STATUS status;
        CL_VWAP_ENTRY_LEVEL curr_entry_level;

};

#endif // CL_VWAP_LOGIC_H
