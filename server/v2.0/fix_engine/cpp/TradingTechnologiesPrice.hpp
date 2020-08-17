#ifndef QUANTMODEL_TRADINGTECHNOLOGIESPRICE_H
#define QUANTMODEL_TRADINGTECHNOLOGIESPRICE_H

#include "FIXApplication.hpp"

using namespace std;

namespace fix_engine
{
    class TradingTechnologiesPrice : public FIXApplication
    {
        public:

            TradingTechnologiesPrice( const XMLNode& , const XMLNode& );
            ~TradingTechnologiesPrice(){};

            //
            //  FIXApplication overloads
            //

            void start();
            void shutdown();

            void login();
            void logout();

        private:

            quantmodel::FixEngineBroadcast fix_engine_broadcast;
            std::map< const std::string , const std::string > market_data_map;

            bool send_market_data_request_required;

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

            void onMessage( const FIX42::MarketDataSnapshotFullRefresh& , const FIX::SessionID& );
            void onMessage( const FIX42::MarketDataIncrementalRefresh& , const FIX::SessionID& );
            void onMessage( const FIX42::MarketDataRequestReject& , const FIX::SessionID& );
            void onMessage( const FIX42::SecurityDefinition& , const FIX::SessionID& );

            void sendMarketDataRequest( const quantmodel::FixEngineRequest&, quantmodel::FixEngineResponse& );
            void sendSecurityDefinitionRequest( const quantmodel::FixEngineRequest&, quantmodel::FixEngineResponse& );

            const void handleMessage( const FIX::Message& message, const FIX::SessionID& sessionID )
            {
                const std::string& msgTypeValue = message.getHeader().getField( FIX::FIELD::MsgType );

                if( msgTypeValue == "X" )
                {
                    onMessage( (const FIX42::MarketDataIncrementalRefresh&)message, sessionID );
                }
                else if( msgTypeValue == "W" )
                {
                    onMessage( (const FIX42::MarketDataSnapshotFullRefresh&)message, sessionID );
                }
                else if( msgTypeValue == "Y" )
                {
                    onMessage( (const FIX42::MarketDataRequestReject&)message, sessionID );
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
                    case quantmodel::FixEngineRequest::MARKETDATA_SUBSCRIBE:

                        sendMarketDataRequest( request_msg, response_msg );
                        break;

                    case quantmodel::FixEngineRequest::DEFINE_SECURITY:

                        sendSecurityDefinitionRequest( request_msg, response_msg );
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

#endif // QUANTMODEL_TRADINGTECHNOLOGIESPRICE_H



            