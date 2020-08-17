#ifndef LIMIT_ORDER_BOOK_H
#define LIMIT_ORDER_BOOK_H

#include <quantmodel.hpp>

struct LimitOrderEntry
{
        bool changed;
        int quantity;
        int orders;
        unsigned int level;
        google::protobuf::int64 price;
        char price_str[ 64 ];
        
        int qty_delta_pos;
        int ord_delta_pos;
        int qty_delta_arr[ 2048 ];
        int ord_delta_arr[ 2048 ];

    LimitOrderEntry()
    {
        changed = false;
        quantity = 0;
        orders = 0;
        level = 0;
        price = 0LL;
    }

    inline void assign( const google::protobuf::int64 entry_px, 
                        const int order_size, 
                        const int entry_size, 
                        const unsigned int price_level, 
                        bool has_changed = true )
    {
        changed = has_changed;
        orders = order_size;
        quantity = entry_size;
        level = price_level;
        price = entry_px;
    }

    inline void copyFrom( const LimitOrderEntry& entry, 
                          const unsigned int new_level )
    {
        changed = true;
        quantity = entry.quantity;
        orders = entry.orders;
        level = new_level;
        price = entry.price;
    }

    inline void copyTo( quantmodel::MarketDataQuoteData& data )
    {
        sprintf( price_str, "%lld", (long long int)price );
        data.set_quote_prc( price_str );
        data.set_quote_qty( quantity );
        data.set_level( level );
        data.set_orders( orders );
    }
};

class LimitOrderBook
{
    public:

        LimitOrderBook( const unsigned int id )
        {
            security_id = id;
        }

        ~LimitOrderBook(){}
        
        bool hasChanged( quantmodel::MarketDataQuoteData& data, const int level, const int bidFlag )
        {
            if( bidFlag == 1 && bid[ level ].changed )
            {
                bid[ level ].changed = false;
                bid[ level ].copyTo( data );
                return true;
            }
            else if( bidFlag == 0 && ask[ level ].changed )
            {
                ask[ level ].changed = false;
                ask[ level ].copyTo( data );
                return true;
            }
            else
            {
                return false;
            }
        }
        
        void reset( )
        {
            for( unsigned int i=0; i<10; ++i )
            {
                bid[ i ].changed = ask[ i ].changed = false;
            }
        }
        
        void reset( const unsigned int price_level, 
                    const unsigned int bid_flag )
        {
            bid_flag == 1 ? bid[ price_level ].changed = false : ask[ price_level ].changed = false;
        }

        void process_bid( const quantmodel::CMEMarketData& data )
        {
            const unsigned int price_level = data.price_level();
            const quantmodel::CMEMarketData::UpdateAction& update_action = data.update_action();
            
            if( ( price_level < 1 ) || ( price_level > 10 ) )
            {
                std::cout << "BID INVALID PRICE LEVEL: " << price_level << std::endl;
                return;
            }
            
            if( update_action == quantmodel::CMEMarketData::CHANGE )
            {
                bid[ price_level - 1 ].assign( data.mantissa(), data.orders(), data.entry_size(), price_level );
            }
            else if( update_action == quantmodel::CMEMarketData::ADD )
            {
                shift_right( bid, price_level );
                bid[ price_level - 1 ].assign( data.mantissa(), data.orders(), data.entry_size(), price_level );
            }
            else if( update_action == quantmodel::CMEMarketData::DELETE )
            {
                shift_left( bid, price_level );
                bid[ 9 ].assign( 0LL, 0, 0, 10 );
            }
        }

        void process_ask( const quantmodel::CMEMarketData& data )
        {
            const unsigned int price_level = data.price_level();
            const quantmodel::CMEMarketData::UpdateAction& update_action = data.update_action();
            
            if( ( price_level < 1 ) || ( price_level > 10 ) )
            {
                std::cout << "ASK INVALID PRICE LEVEL: " << price_level << std::endl;
                return;
            }

            if( update_action == quantmodel::CMEMarketData::CHANGE )
            {
                ask[ price_level - 1 ].assign( data.mantissa(), data.orders(), data.entry_size(), price_level );
            }
            else if( update_action == quantmodel::CMEMarketData::ADD )
            {
                shift_right( ask, price_level );
                ask[ price_level - 1 ].assign( data.mantissa(), data.orders(), data.entry_size(), price_level );
            }
            else if( update_action == quantmodel::CMEMarketData::DELETE )
            {
                shift_left( ask, price_level );
                ask[ 9 ].assign( 0LL, 0, 0, 10 );
            }
        }
        
        inline void copyLimitOrderEntry( quantmodel::MarketDataQuoteData& data, 
                                         const unsigned int price_level, 
                                         const unsigned int bid_flag )
        {
            bid_flag == 1 ? bid[ price_level ].copyTo( data ) : ask[ price_level ].copyTo( data );
        }

    private:

        LimitOrderBook (const LimitOrderBook &);
        LimitOrderBook & operator= (const LimitOrderBook &);

        unsigned int security_id;

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
