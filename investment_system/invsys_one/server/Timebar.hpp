#ifndef TIMEBAR_H
#define TIMEBAR_H

struct TradeData;
struct MarketSnapshot;
struct Timebar;

struct Timebar
{
    time_t timestamp;

    unsigned int buy_qty;
    unsigned int sell_qty;
    unsigned int unkn_qty;

    double buy_val;
    double sell_val;
    double unkn_val;

    double open;
    double high;
    double low;
    double close;

    bool marker;

    std::vector< MarketSnapshot* > snapshots;
    std::map< const double, TradeData* >::const_iterator trade_iter;

    Timebar( const time_t& time )
    {
        timestamp = time;

        buy_qty = sell_qty = unkn_qty = 0;

        open = high = low = close =
        buy_val = sell_val = unkn_val = 0.0;

        marker = false;
    }

    void update( MarketSnapshot * const snapshot )
    {
        if( snapshot->open != 0.0 )
        {
            if( likely( open != 0.0 ) )
            {
                if( high < snapshot->high ) high = snapshot->high;
                if( low > snapshot->low ) low = snapshot->low;
                close = snapshot->close;
            }
            else
            {
                open = snapshot->open;
                high = snapshot->high;
                low = snapshot->low;
                close = snapshot->close;
            }

            for( trade_iter = snapshot->trade_map.begin();
                 trade_iter != snapshot->trade_map.end();
                 ++trade_iter )
            {
                buy_qty += trade_iter->second->buy_qty;
                sell_qty += trade_iter->second->sell_qty;
                unkn_qty += trade_iter->second->unkn_qty;

                buy_val += trade_iter->second->trade_prc * double( trade_iter->second->buy_qty );
                sell_val += trade_iter->second->trade_prc * double( trade_iter->second->sell_qty );
                unkn_val += trade_iter->second->trade_prc * double( trade_iter->second->unkn_qty );
            }
        }

        //snapshots.push_back( snapshot );
    }

    unsigned int volume()
    {
        return buy_qty + sell_qty + unkn_qty;
    }

    double value()
    {
        return buy_val + sell_val + unkn_val;
    }

    bool isUpBar()
    {
        return close > open;
    }

    bool isDownBar()
    {
        return open > close;
    }

    double vwap()
    {
        return ( buy_qty + sell_qty + unkn_qty ) > 0 ?
               ( ( buy_val + sell_val + unkn_val ) / double( buy_qty + sell_qty + unkn_qty ) ) :
               0.0;
    }

    double buyVWAP()
    {
        return buy_qty > 0 ? buy_val / double( buy_qty ) : 0.0;
    }

    double sellVWAP()
    {
        return sell_qty > 0 ? sell_val / double( sell_qty ) : 0.0;
    }

    double unknVWAP()
    {
        return unkn_qty > 0 ? unkn_val / double( unkn_qty ) : 0.0;
    }

    double getTimebarHeight()
    {
        return fabs( open - close );
    }

    size_t getTimebarHeight( const double tick_size )
    {
        return size_t( ( getTimebarHeight() / tick_size ) + 0.5 );
    }

    double getTimebarRange()
    {
        return high - low;
    }

    size_t getTimebarRange( const double tick_size )
    {
        return size_t( ( getTimebarRange() / tick_size ) + 0.5 );
    }

    double positiveDirectionalMovement( const double& prev_high )
    {
        if( high - prev_high > 0 ) return high - prev_high;
        else return 0;
    }

    double negativeDirectionalMovement( const double& prev_low )
    {
        if( prev_low - low > 0 ) return prev_low - low;
        else return 0;
    }

    void print( log4cpp::Category * const log )
    {
        log->info( " TIMESTAMP: %ld", timestamp );
        log->info( " OHLC [ %.4f %.4f %.4f %.4f ]", open, high, low, close );
        log->info( " VOLM [ %d %d %d ] %d", buy_qty, sell_qty, unkn_qty, volume() );
        log->info( " VWAP [ %.4f %.4f %.4f ]", buyVWAP(), sellVWAP(), unknVWAP() );
    }
};

#endif // TIMEBAR_H
