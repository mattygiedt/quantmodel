#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include <quantmodel.hpp>

int main( int argc, char *argv[] )
{
    zmq::context_t ctx( 1 );
    zmq::socket_t pub( ctx, ZMQ_PUB );
    zmq::socket_t sub( ctx, ZMQ_SUB );
    
    const XMLNode& config = XMLNode::openFileHelper( argv[ 1 ] );
    const XMLNode& network = config.getChildNode( "network" );
    
    pub.bind( network.getAttribute( "external_broadcast" ) );
    sub.bind( network.getAttribute( "internal_broadcast" ) );
    sub.setsockopt( ZMQ_SUBSCRIBE, "", 0 );
    
    std::cout << "pub.bind: " << network.getAttribute( "external_broadcast" ) << std::endl;
    std::cout << "sub.bind: " << network.getAttribute( "internal_broadcast" ) << std::endl;
    
    while( 1 )
    {
        zmq::message_t msg;
        sub.recv( &msg );
        pub.send( msg );
        
        /*
        quantmodel::MarketDataQuoteData book_data;
        if(! quantmodel::ZMQMessage::recv( &book_data, &sub ) )
        {
            std::cerr << "UNABLE TO RECV ON fast_engine_broadcast SOCKET!!!";
            exit( 1 );
        }        
        quantmodel::ZMQMessage::send( &book_data, &pub );
        std::cout << book_data.ShortDebugString() << std::endl;
        */
    }
}
