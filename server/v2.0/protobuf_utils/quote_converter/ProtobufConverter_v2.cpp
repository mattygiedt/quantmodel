#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include <quantmodel.hpp>

using namespace std;

int main( int argc, char *argv[] )
{
    int len;
    bool write_data = false;
    std::fstream infile, outfile;
    char buf[ 32 * 1024 ];

    quantmodel::FastEngineBroadcast fast_engine_broadcast;
    quantmodel::FastEngineBroadcast orig_fast_engine_broadcast;

    infile.open( argv[ 1 ], std::fstream::in | std::ifstream::binary );
    outfile.open( argv[ 2 ], std::fstream::out | std::fstream::trunc );

    const std::string instrument_id = argv[ 3 ];
    const unsigned int security_id = atoi( instrument_id.c_str() );

    while( infile.good() )
    {
        write_data = false;
        fast_engine_broadcast.Clear();
        orig_fast_engine_broadcast.Clear();

        infile >> len;
        infile.read( buf, len );
        orig_fast_engine_broadcast.ParsePartialFromArray( buf, len );

        //
        //  Copy broadcast level data
        //

        fast_engine_broadcast.set_timestamp( orig_fast_engine_broadcast.timestamp() );
        fast_engine_broadcast.set_broadcast_type( orig_fast_engine_broadcast.broadcast_type() );

        //
        //  Iterate through each fast message
        //

        for( int i=0; i<orig_fast_engine_broadcast.cme_market_data_size(); ++i )
        {
            const quantmodel::CMEMarketData& orig_cme_market_data =
                orig_fast_engine_broadcast.cme_market_data( i );

            //
            //  Filter out bad market data from early V2 Fast Engine
            //

            if( ( orig_cme_market_data.entry_type() == quantmodel::CMEMarketData::BID && orig_cme_market_data.price_level() == 0 ) ||
                ( orig_cme_market_data.entry_type() == quantmodel::CMEMarketData::ASK && orig_cme_market_data.price_level() == 0 ) )
            {
                continue;
            }

            if( security_id == orig_cme_market_data.security_id() )
            {
                write_data = true;

                quantmodel::CMEMarketData *cme_market_data =
                    fast_engine_broadcast.add_cme_market_data();

                //
                //  Copy orig values into new market data
                //

                cme_market_data->set_sending_tm( orig_cme_market_data.sending_tm() );
                cme_market_data->set_is_implied( orig_cme_market_data.is_implied() );
                cme_market_data->set_exponent( orig_cme_market_data.exponent() );
                cme_market_data->set_price_level( orig_cme_market_data.price_level() );
                cme_market_data->set_security_id( orig_cme_market_data.security_id() );
                cme_market_data->set_entry_size( orig_cme_market_data.entry_size() );
                cme_market_data->set_aggressor( orig_cme_market_data.aggressor() );
                cme_market_data->set_entry_type( orig_cme_market_data.entry_type() );
                cme_market_data->set_update_action( orig_cme_market_data.update_action() );

                if( orig_cme_market_data.mantissa() != 0 )
                {
                    cme_market_data->set_mantissa( orig_cme_market_data.mantissa() );
                }
                else
                {
                    cme_market_data->set_mantissa( atol( orig_cme_market_data.entry_px().c_str() ) );
                }
            }
        }

        if( write_data == true )
        {
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

