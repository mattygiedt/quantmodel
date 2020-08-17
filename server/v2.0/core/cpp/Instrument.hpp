#ifndef QUANTMODEL_INSTRUMENT_H
#define QUANTMODEL_INSTRUMENT_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class Instrument : public DataObject
    {
        public:

            Instrument( const std::string& instrument_id )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                data.set_instrument_id( instrument_id );
                exchange_price_factor = 0;
            }

            Instrument( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                for( int k=0; k<row.column_size(); ++k )
                {
                    setAttribute( row.column( k ).name(), row.column( k ).value() );
                }

                exchange_price_factor = ( getAttribute( quantmodel::EXCHANGE_PRICE_FACTOR ) );
            }

            Instrument( const quantmodel::InstrumentData& instrument_data )
            {
                data.CopyFrom( instrument_data );
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                exchange_price_factor = ( getAttribute( quantmodel::EXCHANGE_PRICE_FACTOR ) );
            }

            ~Instrument(){};

            inline const std::string& getInstrumentId() { return data.instrument_id(); }
            inline const std::string& getName() { return data.name(); }

            void displayPrice( const long& prc, std::string& display_prc ) const
            {
                const mpf_class raw_prc( prc );
                toString( display_prc, raw_prc * exchange_price_factor );
            }

            void displayPrice( const long long& prc, std::string& display_prc ) const
            {
                char buf[ MAX_DIGITS ];
                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%lld", prc );
                const mpf_class raw_prc( buf );
                toString( display_prc, raw_prc * exchange_price_factor );
            }

            void displayPrice( const std::string& prc, std::string& display_prc ) const
            {
                const mpf_class raw_prc( prc );
                toString( display_prc, raw_prc * exchange_price_factor );
            }

            inline bool const isSymbolExchange( const std::pair< const std::string , const std::string >& key ) const
            {
                return key.first == data.symbol() && key.second == data.security_exchange();
            }

        private:

            // prevent copying and assignment; not implemented
            Instrument (const Instrument &);
            Instrument & operator= (const Instrument &);

            quantmodel::InstrumentData data;
            mpf_class exchange_price_factor;
    };
}

#endif // QUANTMODEL_INSTRUMENT_H


                       