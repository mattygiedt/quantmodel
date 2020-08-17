#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include <quantmodel.hpp>

using namespace std;

int main( int argc, char *argv[] )
{
    int i, len;
    std::fstream infile, outfile;
    char buf[ 32 * 1024 ];

    quantmodel::FastEngineBroadcast fast_engine_broadcast;
    quantmodel::FastEngineBroadcast market_data_broadcast;

    infile.open( argv[ 1 ], std::fstream::in | std::ifstream::binary );
    outfile.open( argv[ 2 ], std::fstream::out | std::fstream::trunc );

    const unsigned int security_id = atoi( argv[ 3 ] );

    while( infile.good() )
    {
        fast_engine_broadcast.Clear();
        market_data_broadcast.Clear();

        infile >> len;
        infile.read( buf, len );
        fast_engine_broadcast.ParsePartialFromArray( buf, len );

        market_data_broadcast.set_timestamp( fast_engine_broadcast.timestamp() );
        market_data_broadcast.set_broadcast_type( fast_engine_broadcast.broadcast_type() );

        for( i=0; i<fast_engine_broadcast.cme_market_data_size(); ++i )
        {
            const quantmodel::CMEMarketData& cme_market_data = fast_engine_broadcast.cme_market_data( i );

            //
            //  Cleanup the 0 price level and missing required is_implied field
            //

            if( security_id == cme_market_data.security_id() )
            {
                if( ( cme_market_data.entry_type() == quantmodel::CMEMarketData::BID ||
                      cme_market_data.entry_type() == quantmodel::CMEMarketData::ASK ) &&
                      cme_market_data.price_level() == 0 )
                {
                    continue;
                }

                quantmodel::CMEMarketData *cme_data =
                    market_data_broadcast.add_cme_market_data();

                cme_data->CopyFrom( cme_market_data );

                if(! cme_data->IsInitialized() )
                {
                    cme_data->set_is_implied( false );
                }
            }
        }

        if( market_data_broadcast.cme_market_data_size() > 0 )
        {
            outfile << market_data_broadcast.ByteSize();
            if(! market_data_broadcast.SerializeToOstream( &outfile ) )
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

