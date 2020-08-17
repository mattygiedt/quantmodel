#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include <quantmodel.hpp>

using namespace std;

double exchange_price_factor = 0.01;

std::string toString( const mpf_class& f )
{
    std::string str;
    char buf[ MAX_DIGITS ];
    memset( buf, 0, MAX_DIGITS );
    gmp_snprintf( buf, MAX_DIGITS, GMP_FORMAT, &f );
    str = buf;
    return str;
}

std::string displayPrice( const long long& prc )
{
    char buf[ MAX_DIGITS ];
    memset( buf, 0, MAX_DIGITS );
    sprintf( buf, "%lld", prc );
    const mpf_class raw_prc( buf );
    return toString( raw_prc * exchange_price_factor );
}

int main( int argc, char *argv[] )
{
    int i, len;
    std::fstream infile, outfile;
    char buf[ 32 * 1024 ];

    quantmodel::FastEngineBroadcast fast_engine_broadcast;

    infile.open( argv[ 1 ], std::fstream::in | std::ifstream::binary );
    const unsigned int security_id = atoi( argv[ 2 ] );

    while( infile.good() )
    {
        fast_engine_broadcast.Clear();

        infile >> len;
        infile.read( buf, len );
        fast_engine_broadcast.ParsePartialFromArray( buf, len );

        for( i=0; i<fast_engine_broadcast.cme_market_data_size(); ++i )
        {
            const quantmodel::CMEMarketData& cme_market_data =
                fast_engine_broadcast.cme_market_data( i );

            if( security_id == cme_market_data.security_id() )
            {
                std::cout << "[ " << fast_engine_broadcast.timestamp() << " ] "
                          << cme_market_data.ShortDebugString()
                          << " --> " << displayPrice( cme_market_data.mantissa() )
                          << std::endl;
            }
        }
    }

    infile.close();
    exit( 0 );
}

int main2( int argc, char *argv[] )
{
    int i, len;
    std::fstream infile, outfile;
    char buf[ 32 * 1024 ];

    quantmodel::FastEngineBroadcast fast_engine_broadcast;

    infile.open( argv[ 1 ], std::fstream::in | std::ifstream::binary );
    const unsigned int security_id = atoi( argv[ 2 ] );

    while( infile.good() )
    {
        fast_engine_broadcast.Clear();

        infile >> len;
        infile.read( buf, len );
        fast_engine_broadcast.ParsePartialFromArray( buf, len );

        for( i=0; i<fast_engine_broadcast.cme_market_data_size(); ++i )
        {
            const quantmodel::CMEMarketData& cme_market_data =
                fast_engine_broadcast.cme_market_data( i );

            if( security_id == cme_market_data.security_id() )
            {
                std::cout << "[ " << fast_engine_broadcast.timestamp() << " ] "
                          << cme_market_data.ShortDebugString()
                          << std::endl;
            }
        }
    }

    infile.close();
    exit( 0 );
}
