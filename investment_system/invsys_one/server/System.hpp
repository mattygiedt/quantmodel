#ifndef SYSTEM_H
#define SYSTEM_H

struct MarketSnapshot;
struct Timebar;
struct Trend;
struct Pivot;
struct OrderExecution;
struct System;

struct System
{
    unsigned int security_id;
    std::string instrument_id;
    std::string investment_system_id;
    std::string strategy_id;
    std::string signal_id;
    std::string opening_tm;
    std::string closing_tm;
    std::string current_tm;
    std::string shutdown_tm;
    std::string status;

    long long best_bid;
    long long best_ask;

    double open_pl;
    double closed_pl;
    double price_factor;
    unsigned int closed_qty;

    time_t now;
    time_t next;

    log4cpp::Category *log;
    std::map< const std::string, zmq::socket_t* > socket_map;

    MarketSnapshot *snapshot;
    Timebar *one_minute_timebar;
    Timebar *five_minute_timebar;

    std::vector< Trend* > trend_vec;
    std::vector< Pivot* > pivot_vec;
    std::vector< MarketSnapshot* > snapshots;
    std::vector< OrderExecution* > order_execution_vec;
    std::vector< Timebar* > one_minute_timebars;
    std::vector< Timebar* > five_minute_timebars;

    void init( const XMLNode& parameters )
    {
        investment_system_id = parameters.getAttribute( "investment_system_id" );
        strategy_id = parameters.getAttribute( "strategy_id" );
        signal_id = parameters.getAttribute( "signal_id" );
        instrument_id = parameters.getAttribute( "instrument_id" );
        opening_tm = parameters.getAttribute( "opening_tm" );
        closing_tm = parameters.getAttribute( "closing_tm" );
        shutdown_tm = parameters.getAttribute( "shutdown_tm" );
        price_factor = atof( parameters.getAttribute( "price_factor" ) );

        status = "PRE_OPEN";

        security_id = atoi( instrument_id.c_str() );
        closed_qty = 0;
        best_bid = best_ask = 0;
        open_pl = closed_pl = 0.0;

        log = NULL;
        current_tm = "";
        now = next = 0;
        one_minute_timebar = five_minute_timebar = NULL;

        std::vector< Trend* >::const_iterator trend_iter;
        std::vector< Pivot* >::const_iterator pivot_iter;
        std::vector< Timebar* >::const_iterator timebar_iter;
        std::vector< MarketSnapshot* >::const_iterator snapshot_iter;
        std::vector< OrderExecution* >::const_iterator order_execution_iter;

        for( trend_iter = trend_vec.begin(); trend_iter != trend_vec.end(); ++trend_iter )
        {
            delete (*trend_iter);
        }

        for( pivot_iter = pivot_vec.begin(); pivot_iter != pivot_vec.end(); ++pivot_iter )
        {
            delete (*pivot_iter);
        }

        for( snapshot_iter = snapshots.begin(); snapshot_iter != snapshots.end(); ++snapshot_iter )
        {
            delete (*snapshot_iter);
        }

        for( timebar_iter = one_minute_timebars.begin(); timebar_iter != one_minute_timebars.end(); ++timebar_iter )
        {
            delete (*timebar_iter);
        }

        for( timebar_iter = five_minute_timebars.begin(); timebar_iter != five_minute_timebars.end(); ++timebar_iter )
        {
            delete (*timebar_iter);
        }

        for( order_execution_iter = order_execution_vec.begin(); order_execution_iter != order_execution_vec.end(); ++order_execution_iter )
        {
            delete (*order_execution_iter);
        }

        trend_vec.clear();
        pivot_vec.clear();
        snapshots.clear();
        order_execution_vec.clear();
        one_minute_timebars.clear();
        five_minute_timebars.clear();
    }

    bool updateSystemStatus( )
    {
        if( unlikely( opening_tm == current_tm && status == "PRE_OPEN" ) )
        {
            log->warn( " opening_tm == current_tm" );
            status = "OPEN";
            return true;
        }
        else if( unlikely( closing_tm == current_tm && status == "OPEN" ) )
        {
            log->warn( " closing_tm == current_tm" );
            status = "CLOSED";
            return true;
        }
        else if( unlikely( shutdown_tm == current_tm ) )
        {
            log->warn( " shutdown_tm [ %s ] == current_tm [ %s ]",
                shutdown_tm.c_str(), current_tm.c_str() );
            status = "SHUTDOWN";
            return true;
        }

        return false;
    }

    void updateTrendVector( )
    {
        if( likely( trend_vec.size() > 0 ) )
        {
            if( trend_vec.back()->isUpTrend() )
            {
                if( five_minute_timebars.back()->high >=
                    trend_vec.back()->high )
                {
                    trend_vec.push_back( new Trend(
                        five_minute_timebars.back()->timestamp,
                        five_minute_timebars.back()->high,
                        five_minute_timebars.back()->low,
                        UP_TREND ) );
                }
                else if( five_minute_timebars.back()->low <=
                         trend_vec.back()->low )
                {
                    trend_vec.push_back( new Trend(
                        five_minute_timebars.back()->timestamp,
                        five_minute_timebars.back()->high,
                        five_minute_timebars.back()->low,
                        DOWN_TREND ) );
                }
                else
                {
                    trend_vec.push_back( new Trend(
                        five_minute_timebars.back()->timestamp,
                        five_minute_timebars.back()->high,
                        five_minute_timebars.back()->low,
                        UP_TREND ) );
                }
            }
            else
            {
                if( five_minute_timebars.back()->low <=
                    trend_vec.back()->low )
                {
                    trend_vec.push_back( new Trend(
                        five_minute_timebars.back()->timestamp,
                        five_minute_timebars.back()->high,
                        five_minute_timebars.back()->low,
                        DOWN_TREND ) );
                }
                else if( five_minute_timebars.back()->high >=
                         trend_vec.back()->high )
                {
                    trend_vec.push_back( new Trend(
                        five_minute_timebars.back()->timestamp,
                        five_minute_timebars.back()->high,
                        five_minute_timebars.back()->low,
                        UP_TREND ) );
                }
                else
                {
                    trend_vec.push_back( new Trend(
                        five_minute_timebars.back()->timestamp,
                        five_minute_timebars.back()->high,
                        five_minute_timebars.back()->low,
                        DOWN_TREND ) );
                }
            }
        }
        else
        {
            if( five_minute_timebars.back()->isUpBar() )
            {
                trend_vec.push_back( new Trend(
                    five_minute_timebars.back()->timestamp,
                    five_minute_timebars.back()->high,
                    five_minute_timebars.back()->low,
                    UP_TREND ) );
            }
            else
            {
                trend_vec.push_back( new Trend(
                    five_minute_timebars.back()->timestamp,
                    five_minute_timebars.back()->high,
                    five_minute_timebars.back()->low,
                    DOWN_TREND ) );
            }
        }
    }

    void updatePivotVector( )
    {
        //const size_t old_pivot_vec_size = pivot_vec.size();

        if( likely( trend_vec.size() > 0 && pivot_vec.size() > 0 ) )
        {
            if( trend_vec.back()->isUpTrend() )
            {
                if( pivot_vec.back()->isUpPivot() )
                {
                    //
                    //  We have flipped from a previous down_trend to an up_trend.
                    //  Create a new down pivot.
                    //

                    pivot_vec.push_back( new Pivot(
                        trend_vec.back()->timestamp,
                        trend_vec.back()->price(),
                        DOWN_PIVOT ) );
                }
                else
                {
                    if( pivot_vec.back()->price <= trend_vec.back()->price() )
                    {
                        //
                        //  We are in an up_trend, and the current down_pivot price is less than
                        //  or equal to the current trend price. Update the pivot accordingly.
                        //

                        pivot_vec.back()->price = trend_vec.back()->price();
                        pivot_vec.back()->timestamp = trend_vec.back()->timestamp;

                        //log->info( " UPDATED DOWN_PIVOT: [ %ld %f ]",
                        //    pivot_vec.back()->timestamp,
                        //    pivot_vec.back()->price );
                    }
                }
            }
            else
            {
                //
                //  We are in a down_trend
                //

                if( pivot_vec.back()->isDownPivot() )
                {
                    //
                    //  We have flipped from a previous up_trend to our current down_trend.
                    //  Create a new up_pivot.
                    //

                    pivot_vec.push_back( new Pivot(
                        trend_vec.back()->timestamp,
                        trend_vec.back()->price(),
                        UP_PIVOT ) );
                }
                else
                {
                    if( pivot_vec.back()->price >= trend_vec.back()->price() )
                    {
                        //
                        //  We are in a down_trend and the current up_pivot price is greater than
                        //  or equal to the current trend price. Update pivot accordingly.
                        //

                        pivot_vec.back()->price = trend_vec.back()->price();
                        pivot_vec.back()->timestamp = trend_vec.back()->timestamp;

                        //log->info( " UPDATED UP_PIVOT: [ %ld %f ]",
                        //    pivot_vec.back()->timestamp,
                        //    pivot_vec.back()->price );
                    }
                }
            }
        }
        else if( unlikely( trend_vec.size() > 0 && pivot_vec.size() == 0 ) )
        {
            if( trend_vec.back()->isUpTrend() )
            {
                pivot_vec.push_back( new Pivot(
                    trend_vec.back()->timestamp,
                    trend_vec.back()->price(),
                    DOWN_PIVOT ) );
            }
            else
            {
                pivot_vec.push_back( new Pivot(
                    trend_vec.back()->timestamp,
                    trend_vec.back()->price(),
                    UP_PIVOT ) );
            }
        }
    }

    void fillSystemData(
        quantmodel::InvestmentSystemBroadcast &broadcast_msg,
        const double& ask, const double& bid )
    {
        std::string delegate_msg;
        investment_system::invsys_one::InvestmentSystemOneData data_msg;

        data_msg.set_type( investment_system::invsys_one::InvestmentSystemOneData::SYSTEM );
        data_msg.set_opening_tm( opening_tm );
        data_msg.set_closing_tm( closing_tm );
        data_msg.set_update_tm( current_tm );
        data_msg.set_best_bid( bid );
        data_msg.set_best_ask( ask );
        data_msg.set_status( status );
        data_msg.set_open_pl( open_pl );
        data_msg.set_closed_pl( closed_pl );

        //
        //  Add the timebar
        //

        if( likely( five_minute_timebar != NULL ) )
        {
            investment_system::invsys_one::Timebar *timebar_data = data_msg.add_timebar();
            timebar_data->set_timestamp( five_minute_timebar->timestamp );
            timebar_data->set_open( five_minute_timebar->open );
            timebar_data->set_high( five_minute_timebar->high );
            timebar_data->set_low( five_minute_timebar->low );
            timebar_data->set_close( five_minute_timebar->close );
            timebar_data->set_buy_qty( five_minute_timebar->buy_qty );
            timebar_data->set_sell_qty( five_minute_timebar->sell_qty );
            timebar_data->set_unkn_qty( five_minute_timebar->unkn_qty );
            timebar_data->set_buy_val( five_minute_timebar->buy_val );
            timebar_data->set_sell_val( five_minute_timebar->sell_val );
            timebar_data->set_unkn_val( five_minute_timebar->unkn_val );
        }

        //
        //  Assign the delegate message
        //

        data_msg.SerializeToString( &delegate_msg );
        broadcast_msg.set_type( quantmodel::InvestmentSystemBroadcast::DATA );
        broadcast_msg.set_delegate( delegate_msg );
    }

    void fillTimebarData( Timebar * const timebar, quantmodel::InvestmentSystemBroadcast &broadcast_msg  )
    {
        std::string delegate_msg;
        investment_system::invsys_one::InvestmentSystemOneData data_msg;

        data_msg.set_type( investment_system::invsys_one::InvestmentSystemOneData::TIMEBAR );

        //
        //  Add the timebar
        //

        investment_system::invsys_one::Timebar *timebar_data = data_msg.add_timebar();
        timebar_data->set_timestamp( timebar->timestamp );
        timebar_data->set_open( timebar->open );
        timebar_data->set_high( timebar->high );
        timebar_data->set_low( timebar->low );
        timebar_data->set_close( timebar->close );

        timebar_data->set_buy_qty( timebar->buy_qty );
        timebar_data->set_sell_qty( timebar->sell_qty );
        timebar_data->set_unkn_qty( timebar->unkn_qty );

        timebar_data->set_buy_val( timebar->buy_val );
        timebar_data->set_sell_val( timebar->sell_val );
        timebar_data->set_unkn_val( timebar->unkn_val );

        //
        //  Assign the delegate message
        //

        data_msg.SerializeToString( &delegate_msg );
        broadcast_msg.set_type( quantmodel::InvestmentSystemBroadcast::DATA );
        broadcast_msg.set_delegate( delegate_msg );
    }

    void fillTrendData( Trend * const trend, quantmodel::InvestmentSystemBroadcast &broadcast_msg )
    {
        std::string delegate_msg;
        investment_system::invsys_one::InvestmentSystemOneData data_msg;

        data_msg.set_type( investment_system::invsys_one::InvestmentSystemOneData::TREND );

        //
        //  Add the trend
        //

        investment_system::invsys_one::Trend *trend_data = data_msg.add_trend();
        trend_data->set_timestamp( trend->timestamp );

        if( trend->isUpTrend() ) trend_data->set_type( investment_system::invsys_one::Trend::UP_TREND );
        else trend_data->set_type( investment_system::invsys_one::Trend::DOWN_TREND );

        trend_data->set_high( trend->high );
        trend_data->set_low( trend->low );

        //
        //  Assign the delegate message
        //

        data_msg.SerializeToString( &delegate_msg );
        broadcast_msg.set_type( quantmodel::InvestmentSystemBroadcast::DATA );
        broadcast_msg.set_delegate( delegate_msg );
    }

    void fillPivotData( Pivot * const pivot, quantmodel::InvestmentSystemBroadcast &broadcast_msg )
    {
        std::string delegate_msg;
        investment_system::invsys_one::InvestmentSystemOneData data_msg;

        data_msg.set_type( investment_system::invsys_one::InvestmentSystemOneData::PIVOT );

        //
        //  Add the pivot
        //

        investment_system::invsys_one::Pivot *pivot_data = data_msg.add_pivot();
        pivot_data->set_timestamp( pivot->timestamp );

        if( pivot->isUpPivot() ) pivot_data->set_type( investment_system::invsys_one::Pivot::UP_PIVOT );
        else pivot_data->set_type( investment_system::invsys_one::Pivot::UP_PIVOT );

        pivot_data->set_timestamp( pivot->timestamp );
        pivot_data->set_price( pivot->price );

        //
        //  Assign the delegate message
        //

        data_msg.SerializeToString( &delegate_msg );
        broadcast_msg.set_type( quantmodel::InvestmentSystemBroadcast::DATA );
        broadcast_msg.set_delegate( delegate_msg );
    }

    void fillOrderExecutionData( OrderExecution * const exec, quantmodel::InvestmentSystemBroadcast &broadcast_msg )
    {
        std::string delegate_msg;
        investment_system::invsys_one::InvestmentSystemOneData data_msg;

        data_msg.set_type( investment_system::invsys_one::InvestmentSystemOneData::EXECUTION );

        //
        //  Add the order execution
        //

        investment_system::invsys_one::OrderExecution *exec_data = data_msg.add_order_execution();
        exec_data->set_timestamp( exec->timestamp );
        exec_data->set_timebar( exec->timebar );

        if( exec->side_code == SIDE_CODE_BUY ) exec_data->set_type( investment_system::invsys_one::OrderExecution::BUY );
        else exec_data->set_type( investment_system::invsys_one::OrderExecution::SELL );

        exec_data->set_price( exec->price );
        exec_data->set_quantity( exec->quantity );

        //
        //  Assign the delegate message
        //

        data_msg.SerializeToString( &delegate_msg );
        broadcast_msg.set_type( quantmodel::InvestmentSystemBroadcast::DATA );
        broadcast_msg.set_delegate( delegate_msg );
    }

    void fillLoginData( quantmodel::InvestmentSystemResponse &response_msg,
        const double& ask, const double& bid )
    {
        std::string delegate_msg;

        std::vector< Trend* >::const_iterator trend_iter;
        std::vector< Pivot* >::const_iterator pivot_iter;
        std::vector< Timebar* >::const_iterator timebar_iter;
        std::vector< OrderExecution* >::const_iterator order_execution_iter;

        investment_system::invsys_one::InvestmentSystemOneData data_msg;

        data_msg.set_type( investment_system::invsys_one::InvestmentSystemOneData::LOGIN );
        data_msg.set_opening_tm( opening_tm );
        data_msg.set_closing_tm( closing_tm );
        data_msg.set_update_tm( current_tm );
        data_msg.set_best_bid( bid );
        data_msg.set_best_ask( ask );

        //
        //  Add the order executions
        //

        for( order_execution_iter = order_execution_vec.begin();
             order_execution_iter != order_execution_vec.end();
             ++order_execution_iter )
        {
            investment_system::invsys_one::OrderExecution *order_execution_data = data_msg.add_order_execution();

            order_execution_data->set_timestamp( (*order_execution_iter)->timestamp );
            order_execution_data->set_timebar( (*order_execution_iter)->timebar );

            if( (*order_execution_iter)->side_code == SIDE_CODE_BUY ) order_execution_data->set_type( investment_system::invsys_one::OrderExecution::BUY );
            else order_execution_data->set_type( investment_system::invsys_one::OrderExecution::SELL );

            order_execution_data->set_price( (*order_execution_iter)->price );
            order_execution_data->set_quantity( (*order_execution_iter)->quantity );
        }

        //
        //  Add the 5-minute timebars
        //

        for( timebar_iter = five_minute_timebars.begin();
             timebar_iter != five_minute_timebars.end();
             ++timebar_iter )
        {
            investment_system::invsys_one::Timebar *timebar_data = data_msg.add_timebar();
            timebar_data->set_timestamp( (*timebar_iter)->timestamp );
            timebar_data->set_open( (*timebar_iter)->open );
            timebar_data->set_high( (*timebar_iter)->high );
            timebar_data->set_low( (*timebar_iter)->low );
            timebar_data->set_close( (*timebar_iter)->close );
            timebar_data->set_buy_qty( (*timebar_iter)->buy_qty );
            timebar_data->set_sell_qty( (*timebar_iter)->sell_qty );
            timebar_data->set_unkn_qty( (*timebar_iter)->unkn_qty );
            timebar_data->set_buy_val( (*timebar_iter)->buy_val );
            timebar_data->set_sell_val( (*timebar_iter)->sell_val );
            timebar_data->set_unkn_val( (*timebar_iter)->unkn_val );
        }

        //
        //  Add the trends
        //

        for( trend_iter = trend_vec.begin();
             trend_iter != trend_vec.end();
             ++trend_iter )
        {
            investment_system::invsys_one::Trend *trend_data = data_msg.add_trend();
            trend_data->set_timestamp( (*trend_iter)->timestamp );

            if( (*trend_iter)->isUpTrend() ) trend_data->set_type( investment_system::invsys_one::Trend::UP_TREND );
            else trend_data->set_type( investment_system::invsys_one::Trend::DOWN_TREND );

            trend_data->set_high( (*trend_iter)->high );
            trend_data->set_low( (*trend_iter)->low );
        }

        //
        //  Add the pivots
        //

        for( pivot_iter = pivot_vec.begin();
             pivot_iter != pivot_vec.end();
             ++pivot_iter )
        {
            investment_system::invsys_one::Pivot *pivot_data = data_msg.add_pivot();
            pivot_data->set_timestamp( (*pivot_iter)->timestamp );

            if( (*pivot_iter)->isUpPivot() ) pivot_data->set_type( investment_system::invsys_one::Pivot::UP_PIVOT );
            else pivot_data->set_type( investment_system::invsys_one::Pivot::UP_PIVOT );

            pivot_data->set_timestamp( (*pivot_iter)->timestamp );
            pivot_data->set_price( (*pivot_iter)->price );
        }

        //
        //  Assign the delegate message
        //

        data_msg.SerializeToString( &delegate_msg );
        response_msg.set_delegate( delegate_msg );
    }

};

#endif // SYSTEM_H