#ifndef LIMIT_ORDER_BOOK_H
#define LIMIT_ORDER_BOOK_H

struct LimitOrderEntry
{
    int quantity;
    unsigned int level;
    long long int price;

    void assign( const long long int& entry_px, const int entry_size, const unsigned int price_level )
    {
        price = entry_px;
        quantity = entry_size;
        level = price_level;
    }

    void copyFrom( const QMAPI::MarketDataBroadcast& data )
    {
        price = data.entry_price();
        quantity = data.entry_size();
        level = data.price_level();
    }

    void copyFrom( const LimitOrderEntry& entry, const unsigned int new_level )
    {
        price = entry.price;
        quantity = entry.quantity;
        level = new_level;
    }

    void copyTo( LimitOrderEntry * const entry ) const
    {
        entry->price = price;
        entry->quantity = quantity;
        entry->level = level;
    }
};

class LimitOrderBook
{
    public:

        LimitOrderBook( const unsigned int id = 0 )
        {
            security_id = id;

            zero.price = 0LL;
            zero.quantity = 0;
            zero.level = 0;

            reset( );
        }

        ~LimitOrderBook(){}

        const inline unsigned int getDepth() const { return 10; }
        const inline unsigned int getSecurtyID() const { return security_id; }

        inline void getInsideQuotes( long long int& bid_prc, long long int& ask_prc ) const
        {
            bid_prc = bid[ 0 ].price;
            ask_prc = ask[ 0 ].price;
        }

        inline void getBestAsk( LimitOrderEntry * const best ) const { best->copyFrom( ask[ 0 ], 1 ); }
        inline void getBestBid( LimitOrderEntry * const best ) const { best->copyFrom( bid[ 0 ], 1 ); }

        void reset( )
        {
            for( unsigned int i=1; i<=10; ++i )
            {
                bid[ i-1 ].copyFrom( zero, i );
                ask[ i-1 ].copyFrom( zero, i );
            }
        }

        bool process_bid( const QMAPI::MarketDataBroadcast& data )
        {
            bool has_changed = false;
            const unsigned int price_level = data.price_level();
            const QMAPI::MarketDataBroadcast::UpdateAction update_action = data.update_action();

            //
            //  Only set has_changed for ADD / DELETE to price_level 1
            //

            if( update_action == QMAPI::MarketDataBroadcast::CHANGE )
            {
                bid[ price_level - 1 ].assign( data.entry_price(), data.entry_size(), price_level );
            }
            else if( update_action == QMAPI::MarketDataBroadcast::ADD )
            {
                has_changed = price_level == 1;
                shift_right( bid, price_level );
                bid[ price_level - 1 ].assign( data.entry_price(), data.entry_size(), price_level );
            }
            else if( update_action == QMAPI::MarketDataBroadcast::DELETE )
            {
                has_changed = price_level == 1;
                shift_left( bid, price_level );
                bid[ 9 ].copyFrom( zero, 10 );
            }

            return has_changed;
        }

        bool process_ask( const QMAPI::MarketDataBroadcast& data )
        {
            bool has_changed = false;
            const unsigned int price_level = data.price_level();
            const QMAPI::MarketDataBroadcast::UpdateAction update_action = data.update_action();

            //
            //  Only set has_changed for ADD / DELETE to price_level 1
            //

            if( update_action == QMAPI::MarketDataBroadcast::CHANGE )
            {
                ask[ price_level - 1 ].assign( data.entry_price(), data.entry_size(), price_level );
            }
            else if( update_action == QMAPI::MarketDataBroadcast::ADD )
            {
                has_changed = price_level == 1;
                shift_right( ask, price_level );
                ask[ price_level - 1 ].assign( data.entry_price(), data.entry_size(), price_level );
            }
            else if( update_action == QMAPI::MarketDataBroadcast::DELETE )
            {
                has_changed = price_level == 1;
                shift_left( ask, price_level );
                ask[ 9 ].copyFrom( zero, 10 );
            }

            return has_changed;
        }

        void getOrderBook( LimitOrderEntry * const bid_arr , LimitOrderEntry * const ask_arr ) const
        {
            bid_arr[ 0 ].copyFrom( bid[ 0 ], 1 );
            bid_arr[ 1 ].copyFrom( bid[ 1 ], 2 );
            bid_arr[ 2 ].copyFrom( bid[ 2 ], 3 );
            bid_arr[ 3 ].copyFrom( bid[ 3 ], 4 );
            bid_arr[ 4 ].copyFrom( bid[ 4 ], 5 );
            bid_arr[ 5 ].copyFrom( bid[ 5 ], 6 );
            bid_arr[ 6 ].copyFrom( bid[ 6 ], 7 );
            bid_arr[ 7 ].copyFrom( bid[ 7 ], 8 );
            bid_arr[ 8 ].copyFrom( bid[ 8 ], 9 );
            bid_arr[ 9 ].copyFrom( bid[ 9 ], 10 );

            ask_arr[ 0 ].copyFrom( ask[ 0 ], 1 );
            ask_arr[ 1 ].copyFrom( ask[ 1 ], 2 );
            ask_arr[ 2 ].copyFrom( ask[ 2 ], 3 );
            ask_arr[ 3 ].copyFrom( ask[ 3 ], 4 );
            ask_arr[ 4 ].copyFrom( ask[ 4 ], 5 );
            ask_arr[ 5 ].copyFrom( ask[ 5 ], 6 );
            ask_arr[ 6 ].copyFrom( ask[ 6 ], 7 );
            ask_arr[ 7 ].copyFrom( ask[ 7 ], 8 );
            ask_arr[ 8 ].copyFrom( ask[ 8 ], 9 );
            ask_arr[ 9 ].copyFrom( ask[ 9 ], 10 );
        }

    private:

        LimitOrderBook (const LimitOrderBook &);
        LimitOrderBook & operator= (const LimitOrderBook &);

        unsigned int security_id;

        LimitOrderEntry zero;
        LimitOrderEntry bid[ 10 ];
        LimitOrderEntry ask[ 10 ];

        void shift_right( LimitOrderEntry * const entry_arr , const unsigned int& pos ) const
        {
            switch( pos )
            {
                case 1:
                    entry_arr[ 9 ].copyFrom( entry_arr[ 8 ], 10 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 7 ], 9 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 6 ], 8 );
                    entry_arr[ 6 ].copyFrom( entry_arr[ 5 ], 7 );
                    entry_arr[ 5 ].copyFrom( entry_arr[ 4 ], 6 );
                    entry_arr[ 4 ].copyFrom( entry_arr[ 3 ], 5 );
                    entry_arr[ 3 ].copyFrom( entry_arr[ 2 ], 4 );
                    entry_arr[ 2 ].copyFrom( entry_arr[ 1 ], 3 );
                    entry_arr[ 1 ].copyFrom( entry_arr[ 0 ], 2 );
                    return;

                case 2:
                    entry_arr[ 9 ].copyFrom( entry_arr[ 8 ], 10 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 7 ], 9 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 6 ], 8 );
                    entry_arr[ 6 ].copyFrom( entry_arr[ 5 ], 7 );
                    entry_arr[ 5 ].copyFrom( entry_arr[ 4 ], 6 );
                    entry_arr[ 4 ].copyFrom( entry_arr[ 3 ], 5 );
                    entry_arr[ 3 ].copyFrom( entry_arr[ 2 ], 4 );
                    entry_arr[ 2 ].copyFrom( entry_arr[ 1 ], 3 );
                    return;

                case 3:
                    entry_arr[ 9 ].copyFrom( entry_arr[ 8 ], 10 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 7 ], 9 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 6 ], 8 );
                    entry_arr[ 6 ].copyFrom( entry_arr[ 5 ], 7 );
                    entry_arr[ 5 ].copyFrom( entry_arr[ 4 ], 6 );
                    entry_arr[ 4 ].copyFrom( entry_arr[ 3 ], 5 );
                    entry_arr[ 3 ].copyFrom( entry_arr[ 2 ], 4 );
                    return;

                case 4:
                    entry_arr[ 9 ].copyFrom( entry_arr[ 8 ], 10 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 7 ], 9 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 6 ], 8 );
                    entry_arr[ 6 ].copyFrom( entry_arr[ 5 ], 7 );
                    entry_arr[ 5 ].copyFrom( entry_arr[ 4 ], 6 );
                    entry_arr[ 4 ].copyFrom( entry_arr[ 3 ], 5 );
                    return;

                case 5:
                    entry_arr[ 9 ].copyFrom( entry_arr[ 8 ], 10 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 7 ], 9 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 6 ], 8 );
                    entry_arr[ 6 ].copyFrom( entry_arr[ 5 ], 7 );
                    entry_arr[ 5 ].copyFrom( entry_arr[ 4 ], 6 );
                    return;

                case 6:
                    entry_arr[ 9 ].copyFrom( entry_arr[ 8 ], 10 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 7 ], 9 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 6 ], 8 );
                    entry_arr[ 6 ].copyFrom( entry_arr[ 5 ], 7 );
                    return;

                case 7:
                    entry_arr[ 9 ].copyFrom( entry_arr[ 8 ], 10 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 7 ], 9 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 6 ], 8 );
                    return;

                case 8:
                    entry_arr[ 9 ].copyFrom( entry_arr[ 8 ], 10 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 7 ], 9 );
                    return;

                case 9:
                    entry_arr[ 9 ].copyFrom( entry_arr[ 8 ], 10 );
                    return;

                default:
                    return;
            }
        }

        void shift_left( LimitOrderEntry * const entry_arr , const unsigned int& pos ) const
        {
            switch( pos )
            {
                case 1:
                    entry_arr[ 0 ].copyFrom( entry_arr[ 1 ], 1 );
                    entry_arr[ 1 ].copyFrom( entry_arr[ 2 ], 2 );
                    entry_arr[ 2 ].copyFrom( entry_arr[ 3 ], 3 );
                    entry_arr[ 3 ].copyFrom( entry_arr[ 4 ], 4 );
                    entry_arr[ 4 ].copyFrom( entry_arr[ 5 ], 5 );
                    entry_arr[ 5 ].copyFrom( entry_arr[ 6 ], 6 );
                    entry_arr[ 6 ].copyFrom( entry_arr[ 7 ], 7 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 8 ], 8 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 9 ], 9 );
                    return;

                case 2:
                    entry_arr[ 1 ].copyFrom( entry_arr[ 2 ], 2 );
                    entry_arr[ 2 ].copyFrom( entry_arr[ 3 ], 3 );
                    entry_arr[ 3 ].copyFrom( entry_arr[ 4 ], 4 );
                    entry_arr[ 4 ].copyFrom( entry_arr[ 5 ], 5 );
                    entry_arr[ 5 ].copyFrom( entry_arr[ 6 ], 6 );
                    entry_arr[ 6 ].copyFrom( entry_arr[ 7 ], 7 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 8 ], 8 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 9 ], 9 );
                    return;

                case 3:
                    entry_arr[ 2 ].copyFrom( entry_arr[ 3 ], 3 );
                    entry_arr[ 3 ].copyFrom( entry_arr[ 4 ], 4 );
                    entry_arr[ 4 ].copyFrom( entry_arr[ 5 ], 5 );
                    entry_arr[ 5 ].copyFrom( entry_arr[ 6 ], 6 );
                    entry_arr[ 6 ].copyFrom( entry_arr[ 7 ], 7 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 8 ], 8 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 9 ], 9 );
                    return;

                case 4:
                    entry_arr[ 3 ].copyFrom( entry_arr[ 4 ], 4 );
                    entry_arr[ 4 ].copyFrom( entry_arr[ 5 ], 5 );
                    entry_arr[ 5 ].copyFrom( entry_arr[ 6 ], 6 );
                    entry_arr[ 6 ].copyFrom( entry_arr[ 7 ], 7 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 8 ], 8 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 9 ], 9 );
                    return;

                case 5:
                    entry_arr[ 4 ].copyFrom( entry_arr[ 5 ], 5 );
                    entry_arr[ 5 ].copyFrom( entry_arr[ 6 ], 6 );
                    entry_arr[ 6 ].copyFrom( entry_arr[ 7 ], 7 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 8 ], 8 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 9 ], 9 );
                    return;

                case 6:
                    entry_arr[ 5 ].copyFrom( entry_arr[ 6 ], 6 );
                    entry_arr[ 6 ].copyFrom( entry_arr[ 7 ], 7 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 8 ], 8 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 9 ], 9 );
                    return;

                case 7:
                    entry_arr[ 6 ].copyFrom( entry_arr[ 7 ], 7 );
                    entry_arr[ 7 ].copyFrom( entry_arr[ 8 ], 8 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 9 ], 9 );
                    return;

                case 8:
                    entry_arr[ 7 ].copyFrom( entry_arr[ 8 ], 8 );
                    entry_arr[ 8 ].copyFrom( entry_arr[ 9 ], 9 );
                    return;

                case 9:
                    entry_arr[ 8 ].copyFrom( entry_arr[ 9 ], 9 );
                    return;

                default:
                    return;
            }
        }
};

#endif // LIMIT_ORDER_BOOK_H
