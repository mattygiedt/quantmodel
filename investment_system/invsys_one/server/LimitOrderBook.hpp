#ifndef LIMIT_ORDER_BOOK_H
#define LIMIT_ORDER_BOOK_H

#include <zmq.hpp>

#include <quantmodel.hpp>

struct LimitOrderEntry
{
    int quantity;
    unsigned int level;
    long long price;

    void assign( const long long& entry_px, const int entry_size, const unsigned int price_level )
    {
        quantity = entry_size;
        level = price_level;
        price = entry_px;
    }

    void copyFrom( const quantmodel::CMEMarketData& data )
    {
        quantity = data.entry_size();
        level = data.price_level();
        price = data.mantissa();
    }

    void copyFrom( const LimitOrderEntry& entry, const unsigned int new_level )
    {
        quantity = entry.quantity;
        level = new_level;
        price = entry.price;
    }

    void copyTo( LimitOrderEntry * const entry ) const
    {
        entry->quantity = quantity;
        entry->level = level;
        entry->price = price;
    }
};

class LimitOrderBook
{
    public:

        LimitOrderBook( const unsigned int& id )
        {
            security_id = id;
            last_update_tm = 0LL;
            has_changed = false;

            zero.quantity = 0;
            zero.level = 0;
            zero.price = 0LL;

            reset( true );
        }

        ~LimitOrderBook(){}

        const inline unsigned int getDepth() const { return 10; }
        const inline bool hasChanged() const { return has_changed; }
        const inline unsigned int getSecurtyID() const { return security_id; }
        const inline uint64_t getLastUpdateTime() const { return last_update_tm; }

        inline void getInsideQuotes( long long& bid_prc, long long& ask_prc ) const
        {
            bid_prc = bid[ 0 ].price;
            ask_prc = ask[ 0 ].price;
        }

        void reset( const bool flag = false )
        {
            has_changed = false;

            if( unlikely( flag == true ) )
            {
                for( unsigned int i=1; i<=10; ++i )
                {
                    bid[ i-1 ].copyFrom( zero, i );
                    ask[ i-1 ].copyFrom( zero, i );
                }
            }
        }

        void process_bid( const quantmodel::CMEMarketData& data )
        {
            const unsigned int price_level = data.price_level();

            //
            //  Sanity check
            //

            if( unlikely( price_level == 0 ) )
            {
                std::cerr << "INVALID MKT DATA: " << data.ShortDebugString() << std::endl;
                return;
            }

            last_update_tm = data.sending_tm();
            const quantmodel::CMEMarketData::UpdateAction update_action = data.update_action();

            //
            //  Only set has_changed for ADD / DELETE to price_level 1
            //

            if( update_action == quantmodel::CMEMarketData::CHANGE )
            {
                bid[ price_level - 1 ].assign( data.mantissa(), data.entry_size(), price_level );
            }
            else if( update_action == quantmodel::CMEMarketData::ADD )
            {
                has_changed = price_level == 1;
                shift_right( bid, price_level );
                bid[ price_level - 1 ].assign( data.mantissa(), data.entry_size(), price_level );
            }
            else if( update_action == quantmodel::CMEMarketData::DELETE )
            {
                has_changed = price_level == 1;
                shift_left( bid, price_level );
                bid[ 9 ].copyFrom( zero, 10 );
            }
        }

        void process_ask( const quantmodel::CMEMarketData& data )
        {
            const unsigned int price_level = data.price_level();

            //
            //  Sanity check
            //

            if( unlikely( price_level == 0 ) )
            {
                std::cerr << "INVALID MKT DATA: " << data.ShortDebugString() << std::endl;
                return;
            }

            last_update_tm = data.sending_tm();
            const quantmodel::CMEMarketData::UpdateAction update_action = data.update_action();

            //
            //  Only set has_changed for ADD / DELETE to price_level 1
            //

            if( update_action == quantmodel::CMEMarketData::CHANGE )
            {
                ask[ price_level - 1 ].assign( data.mantissa(), data.entry_size(), price_level );
            }
            else if( update_action == quantmodel::CMEMarketData::ADD )
            {
                has_changed = price_level == 1;
                shift_right( ask, price_level );
                ask[ price_level - 1 ].assign( data.mantissa(), data.entry_size(), price_level );
            }
            else if( update_action == quantmodel::CMEMarketData::DELETE )
            {
                has_changed = price_level == 1;
                shift_left( ask, price_level );
                ask[ 9 ].copyFrom( zero, 10 );
            }
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

        bool has_changed;
        unsigned int security_id;
        uint64_t last_update_tm;

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
