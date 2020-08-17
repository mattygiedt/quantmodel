#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <map>

#define   likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)
#define FORCE_CACHE_LINE_ALIGNMENT __attribute__ ((aligned (64)))

#include "qmapi.pb.h"
#include "xmlParser.hpp"
#include "QuantModelAPI.hpp"

#include "TimeBar.hpp"
#include "LimitOrderBook.hpp"
#include "TradeLogic.hpp"
#include "InvestmentSystem.hpp"

int main( int argc, char *argv[] )
{
    int rc;

    zmq::socket_t *tradedata;
    zmq::socket_t *marketdata;

    QMAPI::QuantModelAPI qmapi;
    QMAPI::MarketDataBroadcast marketDataBroadcast;

    std::vector< QMAPI::CreateTradeRequest > createTradeVector;
    std::vector< QMAPI::CreateTradeRequest >::const_iterator createTradeVectorIterator;

    InvestmentSystem investmentSystem;
    investmentSystem.init( argv[ 1 ] );

    std::cout << qmapi.version() << std::endl;
    qmapi.init( argv[ 1 ] );
    qmapi.start( );

    tradedata = qmapi.getTradeDataSocket();
    marketdata = qmapi.getMarketDataSocket();

    zmq::pollitem_t items [ ] =
    {
        { (*tradedata), 0, ZMQ_POLLIN, 0 },
        { (*marketdata), 0, ZMQ_POLLIN, 0 }
    };

    while( ( rc = zmq::poll( items, 2, -1 ) ) > 0 )
    {
        if( items[ 0 ].revents & ZMQ_POLLIN )
        {

        }
        else if( items[ 1 ].revents & ZMQ_POLLIN )
        {
            zmq::message_t msg;
            marketdata->recv( &msg );
            createTradeVector.clear();
            marketDataBroadcast.Clear();
            marketDataBroadcast.ParsePartialFromArray( msg.data(), msg.size() );

            //std::cout << " >> " << marketDataBroadcast.ShortDebugString() << std::endl;

            investmentSystem.handleMarketDataBroadcast( marketDataBroadcast, createTradeVector );

            if( createTradeVector.size() > 0 )
            {
                for( createTradeVectorIterator = createTradeVector.begin();
                     createTradeVectorIterator != createTradeVector.end();
                     ++createTradeVectorIterator )
                {
                    investmentSystem.confirmTrade( (*createTradeVectorIterator).ext_order_id() );
                }
            }
        }
        else
        {

        }
    }

    exit( 0 );
}
