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

    quantmodel::MarketDataBroadcast broadcast_msg;

    infile.open( argv[ 1 ], std::fstream::in | std::ifstream::binary );
    const std::string instrument_id = argv[ 2 ];
    //const unsigned int security_id = atoi( argv[ 2 ] );

    while( infile.good() )
    {
        broadcast_msg.Clear();

        infile >> len;
        infile.read( buf, len );
        broadcast_msg.ParsePartialFromArray( buf, len );

        //if( broadcast_msg.type() == quantmodel::MarketDataBroadcast::TRADE )
        //{
        //    for( int i=0; i<broadcast_msg.trade_data_size(); ++i )
        //    {
        //        const quantmodel::MarketDataTradeData& trade_data = broadcast_msg.trade_data( i );
        //
        //        if( instrument_id == "0" || trade_data.instrument_id() == instrument_id )
        //        {
        //            std::cout << trade_data.ShortDebugString() << std::endl;
        //        }
        //    }
        //}
        //else if( broadcast_msg.type() == quantmodel::MarketDataBroadcast::QUOTE )
        //{
        //    for( int i=0; i<broadcast_msg.quote_data_size(); ++i )
        //    {
        //        const quantmodel::MarketDataQuoteData& quote_data = broadcast_msg.quote_data( i );
        //
        //        if( instrument_id == "0" || ( quote_data.instrument_id() == instrument_id && quote_data.level() == 0 ) )
        //        {
        //            std::cout << quote_data.ShortDebugString() << std::endl;
        //        }
        //    }
        //}


        std::cout << broadcast_msg.ShortDebugString() << std::endl;
    }

    infile.close();
    exit( 0 );
}
