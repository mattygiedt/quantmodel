#ifndef QUANTMODEL_TRADINGTECHNOLOGIESORDER_H
#define QUANTMODEL_TRADINGTECHNOLOGIESORDER_H

//#include <quickfix/Message.h>
//#include <quickfix/Application.h>
//#include <quickfix/MessageCracker.h>
//
//#include <quickfix/fix42/Logon.h>
//#include <quickfix/fix42/Reject.h>
//#include <quickfix/fix42/Logout.h>
//#include <quickfix/fix42/ExecutionReport.h>
//#include <quickfix/fix42/OrderCancelReject.h>
//#include <quickfix/fix42/MarketDataIncrementalRefresh.h>
//#include <quickfix/fix42/MarketDataSnapshotFullRefresh.h>
//#include <quickfix/fix42/MarketDataRequestReject.h>
//#include <quickfix/fix42/SecurityDefinition.h>

#include "FIXApplication.hpp"

using namespace std;

namespace fix_engine
{
    class TradingTechnologiesOrder : public FIXApplication
    {
        public:

            TradingTechnologiesOrder( const XMLNode& , const XMLNode&, zmq::socket_t * );
            ~TradingTechnologiesOrder(){};

            //
            //  FIXApplication overloads
            //

            void start();
            void shutdown();

            void login();
            void logout();

        private:

            //
            //  default quickfix/Application.h overloads
            //

            void toAdmin( FIX::Message& , const FIX::SessionID& );

            void toApp( FIX::Message& , const FIX::SessionID& )
                throw( FIX::DoNotSend );

            void fromAdmin( const FIX::Message& , const FIX::SessionID& )
                throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon );

            void fromApp( const FIX::Message& , const FIX::SessionID& )
                throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );

            void onCreate( const FIX::SessionID& );

            void onLogon( const FIX::SessionID& );

            void onLogout( const FIX::SessionID& );

            void onMessage( const FIX42::ExecutionReport& , const FIX::SessionID&  );
            void onMessage( const FIX42::Reject& , const FIX::SessionID&  );
            void onMessage( const FIX42::OrderCancelReject& , const FIX::SessionID&  );
            void onMessage( const FIX42::SecurityDefinition& , const FIX::SessionID&  );

            void sendNewOrderSingle( const quantmodel::FixEngineRequest&, quantmodel::FixEngineResponse&, quantmodel::DatabaseEngineRequest& );
            void sendOrderCancelRequest( const quantmodel::FixEngineRequest&, quantmodel::FixEngineResponse&, quantmodel::DatabaseEngineRequest& );
            void sendOrderCancelReplaceRequest( const quantmodel::FixEngineRequest&, quantmodel::FixEngineResponse&, quantmodel::DatabaseEngineRequest& );
            void sendSecurityDefinitionRequest( const quantmodel::FixEngineRequest&, quantmodel::FixEngineResponse&, quantmodel::DatabaseEngineRequest& );

            const void handleMessage( const FIX::Message& message, const FIX::SessionID& sessionID )
            {
                const std::string& msgTypeValue = message.getHeader().getField( FIX::FIELD::MsgType );

                if( msgTypeValue == "8" )
                {
                    onMessage( (const FIX42::ExecutionReport&)message, sessionID );
                }
                else if( msgTypeValue == "3" )
                {
                    onMessage( (const FIX42::Reject&)message, sessionID );
                }
                else if( msgTypeValue == "9" )
                {
                    onMessage( (const FIX42::OrderCancelReject&)message, sessionID );
                }
                else if( msgTypeValue == "d" )
                {
                    onMessage( (const FIX42::SecurityDefinition&)message, sessionID );
                }
                else if( msgTypeValue == "0" ||
                         msgTypeValue == "1" ||
                         msgTypeValue == "A" ||
                         msgTypeValue == "5" )
                {
                    //
                    //  NOP, HeartBeat, TestRequest, Login, Logout
                    //
                }
                else
                {
                    log->warn( "unknown message type: %s", msgTypeValue.c_str() );
                    log->warn( "  %s", printMessage( message ).c_str() );
                }
            }

            void handleMessage(
                const quantmodel::FixEngineRequest& request_msg,
                quantmodel::FixEngineResponse& response_msg,
                quantmodel::DatabaseEngineRequest& database_msg )
            {
                //
                //  Service the request based on the request type
                //

                switch( request_msg.type() )
                {
                    case quantmodel::FixEngineRequest::CREATE_RELEASE:

                        sendNewOrderSingle( request_msg, response_msg, database_msg );
                        break;

                    case quantmodel::FixEngineRequest::CANCEL_RELEASE:

                        sendOrderCancelRequest( request_msg, response_msg, database_msg );
                        break;

                    case quantmodel::FixEngineRequest::REPLACE_RELEASE:

                        sendOrderCancelReplaceRequest( request_msg, response_msg, database_msg );
                        break;

                    case quantmodel::FixEngineRequest::DEFINE_SECURITY:

                        sendSecurityDefinitionRequest( request_msg, response_msg, database_msg );
                        break;

                    case quantmodel::FixEngineRequest::FIXENGINE_LOGOUT:

                        logout();
                        response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
                        break;

                    case quantmodel::FixEngineRequest::FIXENGINE_LOGIN:

                        login();
                        response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
                        break;

                    case quantmodel::FixEngineRequest::SHUTDOWN:

                        shutdown();
                        response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
                        break;

                    default:

                        log->warn( "Unhandled FIX Engine request: " + request_msg.ShortDebugString() );
                }
            }

    };
}

#endif // QUANTMODEL_TRADINGTECHNOLOGIESORDER_H
