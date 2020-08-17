#ifndef QUANTMODEL_API_HPP
#define QUANTMODEL_API_HPP

#include <set>
#include <vector>
#include <string>

#include <zmq.hpp>
#include <pthread.h>

namespace QMAPI
{
    class QuantModelAPI
    {
        public:

            QuantModelAPI();
            ~QuantModelAPI();

            std::string const version();
            bool init( const std::string& );
            bool start();
            bool shutdown();

            zmq::socket_t * getTradeDataSocket( );
            zmq::socket_t * getMarketDataSocket( );

            inline bool isInitialized() const { return is_initialized; }
            inline std::string getTradeRpcAddr() const { return trade_rpc_addr; }
            bool addMktdataBcastAddr( const unsigned int , const std::string& );
            void getMktdataBcastAddrs( std::set< std::string >& );

            inline bool hasSecurityId( const unsigned int id ) const { return security_id_set.count( id ) == 1; }
            inline bool hasExternalOrderId( const std::string& id ) const { return ext_order_id_set.count( id ) == 1; }

        private:

            // prevent copying and assignment; not implemented
            QuantModelAPI (const QuantModelAPI &);
            QuantModelAPI & operator= (const QuantModelAPI &);

            static void* consumeMessages( void* );

            zmq::socket_t * getIpcREQSocket( );
            zmq::socket_t * getIpcSUBSocket( );
            zmq::socket_t * getInprocPUBSocket( );

            inline std::string getTradeBcastAddr() const { return trade_bcast_addr; }
            inline std::string getTradeInprocAddr() const { return trade_inproc_addr; }
            inline std::string getMktdataInprocAddr() const { return mktdata_inproc_addr; }

            bool is_debug;
            bool is_initialized;
            volatile static bool publish_events;

            std::string trade_rpc_addr;
            std::string trade_bcast_addr;
            std::string trade_inproc_addr;
            std::string mktdata_inproc_addr;

            pthread_t message_consumer;

            zmq::context_t *ipc_ctx;
            zmq::context_t *inproc_ctx;
            zmq::socket_t *trade_rpc_socket;

            std::set< std::string > ext_order_id_set;
            std::set< unsigned int > security_id_set;
            std::set< std::string > mktdata_addr_set;

    };
}

#endif // QUANTMODEL_API_HPP