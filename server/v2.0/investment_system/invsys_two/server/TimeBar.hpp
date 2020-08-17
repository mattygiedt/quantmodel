#ifndef TIMEBAR_H
#define TIMEBAR_H

class TimeBar
{
    public:

        TimeBar( const time_t& timebar_tm = 0 )
        {
            can_enter = true;
            timestamp = timebar_tm;
            open = high = low = close = volume = value =
            bid_aggressor = ask_aggressor = 0;
        }

        inline time_t getTimestamp() const { return timestamp; }

        inline bool canEnter() const { return can_enter; }
        inline bool isUpBar() const { return open < close; }
        inline bool isDownBar() const { return open > close; }

        inline unsigned int getBarHeight() const { return high - low; }
        inline unsigned int getUpHeight() const { return isUpBar() == true ? close - open : 0; }
        inline unsigned int getDownHeight() const { return isDownBar() == true ? open - close : 0; }

        inline unsigned int getLow() const { return low; }
        inline unsigned int getOpen() const { return open; }
        inline unsigned int getHigh() const { return high; }
        inline unsigned int getClose() const { return close; }
        inline unsigned int getVolume() const { return volume; }
        inline unsigned int getBidAggressor() const { return bid_aggressor; }
        inline unsigned int getAskAggressor() const { return ask_aggressor; }
        inline double getVWAP() const { return open == 0 ? 0 : double( value ) / double( volume ); }

        inline void closeEntry( ) { can_enter = false; }
        
        inline double getAggressorRatio( ) const { return bid_aggressor > 0 ? double( ask_aggressor ) / double( bid_aggressor ) : 1; }

        void addTrade( const QMAPI::MarketDataBroadcast& broadcast )
        {
            //std::cout << broadcast.ShortDebugString() << std::endl;

            const int64_t entry_price = broadcast.entry_price();
            volume += broadcast.entry_size();

            if( open == 0 )
            {
                open = high = low = close = entry_price;
                return;
            }

            if( high < entry_price )
            {
                high = entry_price;
            }

            if( low > entry_price )
            {
                low = entry_price;
            }

            close = entry_price;

            if( broadcast.broadcast_type() == QMAPI::MarketDataBroadcast::BID_TRADE )
            {
                bid_aggressor += broadcast.entry_size();
            }
            else if( broadcast.broadcast_type() == QMAPI::MarketDataBroadcast::ASK_TRADE )
            {
                ask_aggressor += broadcast.entry_size();
            }
        }

        inline void getAggressorData( std::vector< std::pair< int , int > >& vec ) const
        {
            vec.push_back( std::pair< int , int > ( bid_aggressor, ask_aggressor ) );
        }

    private:

        bool can_enter;
        time_t timestamp;
        unsigned int volume;
        unsigned int value;
        unsigned int open;
        unsigned int high;
        unsigned int low;
        unsigned int close;
        unsigned int bid_aggressor;
        unsigned int ask_aggressor;
};

#endif // TIMEBAR_H
