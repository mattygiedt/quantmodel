#ifndef QUANTMODEL_GLOBEXORDER_H
#define QUANTMODEL_GLOBEXORDER_H

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
    class GlobexOrder : public FIXApplication
    {
        public:

            GlobexOrder( const XMLNode& , const XMLNode&, zmq::socket_t * );
            ~GlobexOrder(){};

            //
            //  FIXApplication overloads
            //

            void start();
            void shutdown();

        private:

            bool resend_more;
            int resend_begin_num;
            FIX::MsgSeqNum lastMsgSeqNum;
            quantmodel::DealingEngineRequest dealing_request;

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

            void onMessage( const FIX42::ExecutionReport& , const FIX::SessionID& );
            void onMessage( const FIX42::Reject& , const FIX::SessionID& );
            void onMessage( const FIX42::BusinessMessageReject& , const FIX::SessionID& );
            void onMessage( const FIX42::OrderCancelReject& , const FIX::SessionID& );
            void onMessage( const FIX42::SecurityDefinition& , const FIX::SessionID& );

            void sendNewOrderSingle( const quantmodel::FixEngineRequest&, quantmodel::FixEngineResponse&, quantmodel::DatabaseEngineRequest& );
            void sendOrderCancelRequest( const quantmodel::FixEngineRequest&, quantmodel::FixEngineResponse&, quantmodel::DatabaseEngineRequest& );
            void sendOrderCancelReplaceRequest( const quantmodel::FixEngineRequest&, quantmodel::FixEngineResponse&, quantmodel::DatabaseEngineRequest& );
            void sendSecurityDefinitionRequest( const quantmodel::FixEngineRequest&, quantmodel::FixEngineResponse& ) const;
            void sendSequenceResetRequest() const;
            void sendResendRequest( const long& , const long& ) const;
            void sendTestRequest() const;

            void handleMessage( const FIX::Message& message, const FIX::SessionID& sessionID )
            {
                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( ">>> handleMessage: %s", printMessage( message ).c_str() );
                }

                message.getHeader().getField( lastMsgSeqNum );
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
                else if( msgTypeValue == "j" )
                {
                    onMessage( (const FIX42::BusinessMessageReject&)message, sessionID );
                }
                else if( msgTypeValue == "d" )
                {
                    onMessage( (const FIX42::SecurityDefinition&)message, sessionID );
                }
                else if( msgTypeValue == "0" || // HEARTBEAT
                         msgTypeValue == "1" || // TEST REQUEST
                         msgTypeValue == "2" || // RESEND REQUEST
                         msgTypeValue == "4" || // SEQUENCE RESET
                         msgTypeValue == "5" || // LOGOUT
                         msgTypeValue == "A" )  // LOGON
                {
                    //
                    //  NOP
                    //
                }
                else
                {
                    log->warn( "unknown message type[ %s ]: %s",
                        msgTypeValue.c_str(), printMessage( message ).c_str() );
                }

                //
                //  Do we need to ask the CME to send us more messages because
                //  the message gap is greater than 2500?
                //

                if( resend_more )
                {
                    if( lastMsgSeqNum.getValue() - resend_begin_num > 2000 )
                    {
                        log->info( " RESEND MORE: beg %ld, end %ld",
                            resend_begin_num, resend_begin_num + 2000 );

                        resend_more = true;
                        //sendResendRequest( resend_begin_num, resend_begin_num + 2500 );
                        resend_begin_num += 2000;
                    }
                    else
                    {
                        log->info( " RESEND COMPLETE: beg %ld, end %ld",
                            resend_begin_num, lastMsgSeqNum.getValue() );

                        resend_more = false;
                        //sendResendRequest( resend_begin_num, lastMsgSeqNum.getValue() );
                    }
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

                    case quantmodel::FixEngineRequest::SEQUENCE_RESET:

                        sendSequenceResetRequest();
                        response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
                        break;

                    case quantmodel::FixEngineRequest::RESEND_REQUEST:

                        sendResendRequest( lastMsgSeqNum.getValue(), 0 );
                        response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
                        break;

                    case quantmodel::FixEngineRequest::TEST_REQUEST:

                        sendTestRequest( );
                        response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
                        break;

                    default:

                        log->warn( "Unhandled FIX Engine request: " + request_msg.ShortDebugString() );
                }
            }

            void setCMEHeader( FIX::FieldMap& header ) const
            {
                //
                //  Set the sender sub id
                //

                FIX::SenderSubID senderSubID ( fix.getAttribute( "sender_sub_id" ) );
                header.setField( senderSubID );

                //
                //  Set the target sub id
                //

                FIX::TargetSubID targetSubID ( fix.getAttribute( "target_sub_id" ) );
                header.setField( targetSubID );

                //
                //  Set the sender location id
                //

                FIX::SenderLocationID senderLocationID ( fix.getAttribute( "sender_location_id" ) );
                header.setField( senderLocationID );

                //
                //  Set the LastSeqNumProcessed
                //

                header.setField( 369, lastMsgSeqNum.getString() );
            }

            void setCMEBody( FIX::Message& message ) const
            {
                //
                //  For all application messages sent from the client system
                //  to CME Globex, the tag ManualOrderIndicator is now mandatory.
                //

                message.setField( 1028, "N" );
            }
    };
}

#endif // QUANTMODEL_GLOBEXORDER_H
