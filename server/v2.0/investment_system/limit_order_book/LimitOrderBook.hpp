#ifndef LIMIT_ORDER_BOOK_H
#define LIMIT_ORDER_BOOK_H

#include <quantmodel.hpp>

struct LimitOrderEntry
{
        bool changed;
        int quantity;
        int orders;
        google::protobuf::int64 price;
        char price_str[ 64 ];

    LimitOrderEntry()
    {
        changed = false;
        quantity = 0;
        orders = 0;
        price = 0LL;
    }

    inline void assign( const int order_size, 
                        const int entry_size, 
                        bool has_changed = true )
    {
        changed = has_changed;
        orders = order_size;
        quantity = entry_size;
    }

    inline void copyTo( quantmodel::MarketDataQuoteData& data, const unsigned int price_level )
    {
        sprintf( price_str, "%lld", (long long int)price );
        data.set_quote_prc( price_str );
        data.set_quote_qty( quantity );
        data.set_level( price_level );
        data.set_orders( orders );
    }
};

class LimitOrderBook
{
    public:

        LimitOrderBook( const unsigned int id, 
                        const unsigned int depth,
                        const google::protobuf::int64 start_prc,
                        const unsigned int tick_sz )
            : _security_id( id )
            , _book_depth( depth )
            , _start_prc( start_prc )
            , _tick_sz( tick_sz )
            , _best_bid_idx( 0 )
            , _best_ask_idx( 0 )
        { 
            for( unsigned int i=0; i<65536; ++i )
            {
                bid[ i ].price = start_prc + ( i * tick_sz );
                ask[ i ].price = start_prc + ( i * tick_sz );
            }
        }

        ~LimitOrderBook(){}

        inline unsigned int getPriceOffset( const google::protobuf::int64& price ) const
        {
            return ( price - _start_prc ) / _tick_sz;
        }

        bool hasChanged( quantmodel::MarketDataQuoteData& data, const unsigned int price_level, const int bidFlag )
        {
            if( bidFlag == 1 && bid[ _best_bid_idx + price_level - 1 ].changed )
            {
                bid[ _best_bid_idx + price_level - 1 ].changed = false;
                bid[ _best_bid_idx + price_level - 1 ].copyTo( data, price_level );
                return true;
            }
            else if( bidFlag == 0 && ask[ _best_ask_idx + price_level - 1 ].changed )
            {
                ask[ _best_ask_idx + price_level - 1 ].changed = false;
                ask[ _best_ask_idx + price_level - 1 ].copyTo( data, price_level );
                return true;
            }
            else
            {
                return false;
            }
        }

        void reset( )
        {
            for( unsigned int i=0; i<_book_depth; ++i )
            {
                bid[ i + _best_bid_idx ].changed = 
                ask[ i + _best_ask_idx ].changed = false;
            }
        }

        void reset( const unsigned int price_level, 
                    const unsigned int bid_flag )
        {
            bid_flag == 1 ? bid[ _best_bid_idx + price_level - 1 ].changed = false : ask[ _best_ask_idx + price_level - 1 ].changed = false;
        }

        void process_bid( const quantmodel::CMEMarketData& data )
        {
            const unsigned int& idx = getPriceOffset( data.mantissa() );
            const quantmodel::CMEMarketData::UpdateAction& update_action = data.update_action();
            
            if( 1 == data.price_level() )
            {
                _best_bid_idx = idx;
            }

            if( update_action == quantmodel::CMEMarketData::CHANGE )
            {
                bid[ idx ].assign( data.orders(), data.entry_size() );
            }
            else if( update_action == quantmodel::CMEMarketData::ADD )
            {
                bid[ idx ].assign( data.orders(), data.entry_size() );
            }
            else if( update_action == quantmodel::CMEMarketData::DELETE )
            {
                --_best_bid_idx;
                bid[ _best_bid_idx + 9 ].assign( 0, 0 );
            }
        }

        void process_ask( const quantmodel::CMEMarketData& data )
        {
            const unsigned int& idx = getPriceOffset( data.mantissa() );
            const quantmodel::CMEMarketData::UpdateAction& update_action = data.update_action();
            
            if( 1 == data.price_level() )
            {
                _best_ask_idx = idx;
            }
            
            if( update_action == quantmodel::CMEMarketData::CHANGE || 
                update_action == quantmodel::CMEMarketData::ADD )
            {
                ask[ idx ].assign( data.orders(), data.entry_size() );
            }
            else if( update_action == quantmodel::CMEMarketData::DELETE )
            {
                ++_best_ask_idx;
                ask[ _best_ask_idx + 9 ].assign( 0, 0 );
            }
        }

        inline void copyLimitOrderEntry( quantmodel::MarketDataQuoteData& data, 
                                         const unsigned int price_level, 
                                         const unsigned int bid_flag )
        {
            bid_flag == 1 ? bid[ _best_bid_idx + price_level - 1 ].copyTo( data, price_level ) : ask[ _best_ask_idx + price_level - 1 ].copyTo( data, price_level );
        }

    private:

        LimitOrderBook (const LimitOrderBook &);
        LimitOrderBook & operator= (const LimitOrderBook &);

        unsigned int _security_id;
        unsigned int _book_depth;
        unsigned int _start_prc;
        unsigned int _tick_sz;
        unsigned int _best_bid_idx;
        unsigned int _best_ask_idx;

        LimitOrderEntry bid[ 65536 ];
        LimitOrderEntry ask[ 65536 ];
};

#endif // LIMIT_ORDER_BOOK_H
