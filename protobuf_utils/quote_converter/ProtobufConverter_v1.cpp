#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include <quantmodel.hpp>

using namespace std;

int main( int argc, char *argv[] )
{
    int len;
    std::fstream infile, outfile;
    char buf[ 32 * 1024 ];
    char price_buf[ MAX_DIGITS ];
    quantmodel::MarketDataBroadcast broadcast_msg;
    quantmodel::FastEngineBroadcast fast_engine_broadcast;

    infile.open( argv[ 1 ], std::fstream::in | std::ifstream::binary );
    outfile.open( argv[ 2 ], std::fstream::out | std::fstream::trunc );

    const std::string instrument_id = argv[ 3 ];
    const double price_multiplier = atof( argv[ 4 ] );
    const unsigned int security_id = atoi( instrument_id.c_str() );

    while( infile.good() )
    {
        broadcast_msg.Clear();
        fast_engine_broadcast.Clear();

        infile >> len;
        infile.read( buf, len );
        broadcast_msg.ParseFromArray( buf, len );

        if( broadcast_msg.type() == quantmodel::MarketDataBroadcast::TRADE )
        {
            for( int i=0; i<broadcast_msg.trade_data_size(); ++i )
            {
                const quantmodel::MarketDataTradeData& trade_data = broadcast_msg.trade_data( i );

                if( trade_data.instrument_id() == instrument_id )
                {
                    //
                    //  Convert price to fast_engine price
                    //

                    const mpf_class raw_prc( trade_data.trade_prc() );
                    const mpf_class mkt_prc = raw_prc * price_multiplier;

                    memset( price_buf, 0, MAX_DIGITS );
                    gmp_snprintf( price_buf, MAX_DIGITS, GMP_FORMAT, &mkt_prc );
                    const long long int mantissa = atol( price_buf );

                    //
                    //  Create a new fast_engine_broadcast.cme_market_data
                    //

                    if( fast_engine_broadcast.has_timestamp() == false )
                    {
                        fast_engine_broadcast.set_timestamp( broadcast_msg.timestamp() );
                    }

                    quantmodel::CMEMarketData *cme_market_data =
                        fast_engine_broadcast.add_cme_market_data();

                    cme_market_data->set_sending_tm( 0 );
                    cme_market_data->set_is_implied( false );
                    cme_market_data->set_mantissa( mantissa );
                    cme_market_data->set_exponent( 0 );
                    cme_market_data->set_security_id( security_id );
                    cme_market_data->set_entry_size( trade_data.trade_qty() );
                    cme_market_data->set_aggressor( quantmodel::CMEMarketData::UNKNOWN );
                    cme_market_data->set_entry_type( quantmodel::CMEMarketData::TRADE );
                    cme_market_data->set_update_action( quantmodel::CMEMarketData::ADD );
                }
            }
        }
        else if( broadcast_msg.type() == quantmodel::MarketDataBroadcast::QUOTE )
        {
            for( int i=0; i<broadcast_msg.quote_data_size(); ++i )
            {
                const quantmodel::MarketDataQuoteData& quote_data = broadcast_msg.quote_data( i );

                if( quote_data.instrument_id() == instrument_id )
                {
                    if( quote_data.level() == 0 )
                    {
                        //
                        //  Convert price to fast_engine price
                        //

                        const mpf_class raw_prc( quote_data.quote_prc() );
                        const mpf_class mkt_prc = raw_prc * price_multiplier;

                        memset( price_buf, 0, MAX_DIGITS );
                        gmp_snprintf( price_buf, MAX_DIGITS, GMP_FORMAT, &mkt_prc );
                        const long long int mantissa = atol( price_buf );

                        //
                        //  Create a new fast_engine_broadcast.cme_market_data
                        //

                        if( fast_engine_broadcast.has_timestamp() == false )
                        {
                            fast_engine_broadcast.set_timestamp( broadcast_msg.timestamp() );
                        }

                        quantmodel::CMEMarketData *cme_market_data =
                            fast_engine_broadcast.add_cme_market_data();

                        cme_market_data->set_sending_tm( 0 );
                        cme_market_data->set_price_level( 1 );
                        cme_market_data->set_is_implied( false );
                        cme_market_data->set_mantissa( mantissa );
                        cme_market_data->set_exponent( 0 );
                        cme_market_data->set_security_id( security_id );
                        cme_market_data->set_entry_size( quote_data.quote_qty() );
                        cme_market_data->set_update_action( quantmodel::CMEMarketData::ADD );

                        if( quote_data.type() == quantmodel::MarketDataQuoteData::BID )
                        {
                            cme_market_data->set_entry_type( quantmodel::CMEMarketData::BID );
                        }
                        else
                        {
                            cme_market_data->set_entry_type( quantmodel::CMEMarketData::ASK );
                        }
                    }
                }
            }
        }

        if( fast_engine_broadcast.cme_market_data_size() > 0 )
        {
            fast_engine_broadcast.set_broadcast_type( quantmodel::FastEngineBroadcast::CME );
            outfile << fast_engine_broadcast.ByteSize();
            if(! fast_engine_broadcast.SerializeToOstream( &outfile ) )
            {
                cerr << "unable to persist message.";
                exit( 1 );
            }
        }
    }

    infile.close();
    outfile.close();

    exit( 0 );
}

