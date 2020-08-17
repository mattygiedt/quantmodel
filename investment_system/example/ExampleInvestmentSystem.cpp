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

#include "qmapi.pb.h"
#include "QuantModelAPI.hpp"
#include "LimitOrderBook.hpp"

int main( int argc, char *argv[] )
{
    int rc;
    LimitOrderBook lob;
    zmq::socket_t *tradedata;
    zmq::socket_t *marketdata;

    QMAPI::QuantModelAPI qmapi;
    QMAPI::MarketDataBroadcast marketDataBroadcast;

    std::cout << qmapi.version() << std::endl;
    qmapi.init( "qmapi.xml" );
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
            zmq::message_t recvmsg;
            marketDataBroadcast.Clear();
            marketdata->recv( &recvmsg );
            marketDataBroadcast.ParseFromArray( recvmsg.data(), recvmsg.size() );

            if( marketDataBroadcast.broadcast_type() == QMAPI::MarketDataBroadcast::BID_QUOTE &&
                lob.process_bid( marketDataBroadcast ) == true )
            {

            }
            else if( marketDataBroadcast.broadcast_type() == QMAPI::MarketDataBroadcast::ASK_QUOTE &&
                     lob.process_ask( marketDataBroadcast ) == true )
            {

            }
            else // marketDataBroadcast.broadcast_type() == QMAPI::MarketDataBroadcast::TRADE
            {

            }
        }
        else
        {

        }
    }

    exit( 0 );
}
