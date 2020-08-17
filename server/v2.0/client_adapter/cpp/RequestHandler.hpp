#ifndef CLIENT_ADAPTER_REQUESTHANDLER_H
#define CLIENT_ADAPTER_REQUESTHANDLER_H

#include <zmq.hpp>

#include <quantmodel.hpp>

namespace client_adapter
{
    class RequestHandler
    {
        public:

            RequestHandler( std::map< const std::string, zmq::socket_t* >& map )
            {
                std::map< const std::string, zmq::socket_t* >::iterator iter;
                iter = map.begin();
                while( iter != map.end() )
                {
                    socket_map.insert(
                        pair< const std::string , zmq::socket_t* >(
                            iter->first, iter->second ) );
                    ++iter;
                }
            }

            ~RequestHandler(){};

            void initLogging()
            {
                log = &log4cpp::Category::getInstance( "RequestHandler" );
            }

            const void heartbeat(
                const quantmodel::ClientAdapterRequest& request_msg,
                      quantmodel::ClientAdapterResponse& response_msg )
            {
                response_msg.set_type( quantmodel::ClientAdapterResponse::ACKNOWLEDGEMENT );

                quantmodel::HeartbeatData heartbeat_data;
                quantmodel::TimeStamp::getMessageId( *heartbeat_data.mutable_heartbeat_id( ) );
                quantmodel::TimeStamp::getUTCTime( *heartbeat_data.mutable_timestamp( ) );
                quantmodel::ZMQMessage::send( &heartbeat_data, socket_map[ "internal_broadcast" ] );
            }

            const void dealingRequest(
                const quantmodel::ClientAdapterRequest& request_msg,
                      quantmodel::ClientAdapterResponse& response_msg )
            {
                if(! request_msg.has_dealing_request() )
                {
                    response_msg.set_type( quantmodel::ClientAdapterResponse::ERROR );
                    response_msg.set_error_msg( "NO DEALING REQUEST" );
                    return;
                }

                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( " <-- " + request_msg.dealing_request().ShortDebugString() );
                }

                if( quantmodel::ZMQMessage::sendRPC( &request_msg.dealing_request(),
                    response_msg.mutable_dealing_response(), socket_map[ "dealing_callforward" ] ) )
                {
                    response_msg.set_type( quantmodel::ClientAdapterResponse::ACKNOWLEDGEMENT );
                }
                else
                {
                    response_msg.set_type( quantmodel::ClientAdapterResponse::ERROR );
                    response_msg.set_error_msg( "dealing callforward error" );
                }
            }

            const void databaseRequest(
                const quantmodel::ClientAdapterRequest& request_msg,
                      quantmodel::ClientAdapterResponse& response_msg )
            {
                if(! request_msg.has_database_request() )
                {
                    response_msg.set_type( quantmodel::ClientAdapterResponse::ERROR );
                    response_msg.set_error_msg( "NO DATABASE REQUEST" );
                    return;
                }

                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( " <-- " + request_msg.database_request().ShortDebugString() );
                }

                if( quantmodel::ZMQMessage::sendRPC( &request_msg.database_request(),
                    response_msg.mutable_database_response(), socket_map[ "database_callforward" ] ) )
                {
                    response_msg.set_type( quantmodel::ClientAdapterResponse::ACKNOWLEDGEMENT );
                }
                else
                {
                    response_msg.set_type( quantmodel::ClientAdapterResponse::ERROR );
                    response_msg.set_error_msg( "database callforward error" );
                }
            }

        private:

            RequestHandler (const RequestHandler &);
            RequestHandler & operator= (const RequestHandler &);

            log4cpp::Category *log;
            std::map< const std::string, zmq::socket_t* > socket_map;
    };
}
#endif // CLIENT_ADAPTER_REQUESTHANDLER_H
