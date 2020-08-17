#ifndef MARKET_SNAPSHOT_H
#define MARKET_SNAPSHOT_H

struct TradeData;
struct MarketSnapshot;

struct TradeData
{
    time_t timestamp;
    double trade_prc;

    unsigned int buy_qty;
    unsigned int sell_qty;
    unsigned int unkn_qty;

    TradeData( const time_t& ts, const double& prc )
    {
        timestamp = ts;
        trade_prc = prc;
        buy_qty = sell_qty = unkn_qty = 0;
    }

    void aggregate( const quantmodel::CMEMarketData& trade_data )
    {
        if( trade_data.aggressor() == quantmodel::CMEMarketData::BUY )
        {
            buy_qty += trade_data.entry_size();
        }
        else if( trade_data.aggressor() == quantmodel::CMEMarketData::SELL )
        {
            sell_qty += trade_data.entry_size();
        }
        else
        {
            unkn_qty += trade_data.entry_size();
        }
    }

    unsigned int volume()
    {
        return buy_qty + sell_qty + unkn_qty;
    }
};

struct MarketSnapshot
{
    time_t timestamp;

    //std::string best_bid;
    //std::string best_ask;

    double open;
    double high;
    double low;
    double close;

    //std::map< const std::string, TradeData* > trade_map;
    //std::map< const std::string, TradeData* >::const_iterator trade_iter;

    std::map< const double, TradeData* > trade_map;
    std::map< const double, TradeData* >::const_iterator trade_iter;

    MarketSnapshot( const time_t& now, MarketSnapshot *lastSnapshot = NULL )
    {
        reset( now );
    }

    void processTrade( const quantmodel::CMEMarketData& trade_data, const double& price_factor )
    {
        //const std::string& entry_px = trade_data.entry_px();
        //const double trade_prc = atof( entry_px.c_str() ) * price_factor;

        const double trade_prc = trade_data.mantissa() * price_factor;

        if( trade_map.find( trade_prc ) == trade_map.end() )
        {
            trade_map.insert( std::pair< const double, TradeData* >(
                trade_prc, new TradeData( timestamp, trade_prc ) ) );
        }

        trade_map[ trade_prc ]->aggregate( trade_data );

        if( likely( open > 0 ) )
        {
            if( trade_prc > high ) high = trade_prc;
            if( trade_prc < low ) low = trade_prc;
            close = trade_prc;
        }
        else
        {
            open = high = low = close = trade_prc;
        }
    }

    void reset( const time_t& now )
    {
        timestamp = now;
        //best_bid = best_bid = "0.0";
        open = high = low = close = 0.0;

        for( trade_iter = trade_map.begin();
             trade_iter != trade_map.end();
             ++trade_iter )
        {
            delete trade_iter->second;
        }

        trade_map.clear();
    }
};

#endif // MARKET_SNAPSHOT_H