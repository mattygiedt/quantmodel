#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::cancelRelease(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg )
{
    std::string ts;
    quantmodel::TimeStamp::getUTCTime( ts );

    quantmodel::FixEngineRequest release_request;
    quantmodel::FixEngineResponse release_response;

    std::map< const std::string , quantmodel::Account* >::const_iterator account_iter;
    std::map< const std::string , quantmodel::Instrument* >::const_iterator instrument_iter;

    //
    //  Ensure valid message
    //

    if( request_msg.order().release_data_size() != 1 )
    {
        //
        //  ERROR
        //

        response_msg.set_request_id( request_msg.request_id( ) );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
        response_msg.set_error_msg( "INVALID MESSAGE: RELEASE DATA SIZE != 1" );

        return;
    }

    //
    //  Get the release data from the request
    //

    const quantmodel::ReleaseData& release_data = request_msg.order().release_data( 0 );

    //
    //  Get the release from the active set
    //

    quantmodel::Release * const release = active_set->getReleaseByReleaseId( release_data.release_id() );

    //
    //  Ensure valid release...
    //

    if( NULL != release )
    {
        //
        //  Double check the release status
        //

        if( release->getAttribute( quantmodel::RELEASE_STATUS ).compare( NEW_RELEASE ) == 0 ||
            release->getAttribute( quantmodel::RELEASE_STATUS ).compare( PARTIALLY_FILLED_RELEASE ) == 0 ||
            release->getAttribute( quantmodel::RELEASE_STATUS ).compare( REPLACED_RELEASE ) == 0 ||
            release->getAttribute( quantmodel::RELEASE_STATUS ).compare( PENDING_REPLACE_RELEASE ) == 0 )
        {
            //
            //  Get the instrument on the release
            //

            if( ( instrument_iter = instrument_map.find( release->getInstrumentId() ) ) == instrument_map.end() )
            {
                //
                //  REJECT
                //

                response_msg.set_request_id( request_msg.request_id( ) );
                quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
                response_msg.set_error_msg( "UNKNOWN INSTRUMENT" );

                return;
            }

            //
            //  Get the account on the release
            //

            if( ( account_iter = account_map.find( release->getAccountId() ) ) == account_map.end() )
            {
                //
                //  REJECT
                //

                response_msg.set_request_id( request_msg.request_id( ) );
                quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
                response_msg.set_error_msg( "UNKNOWN ACCOUNT" );

                return;
            }

            //
            //  Send the release to the fix engine as a cancel request
            //

            release_request.Clear();
            release_request.set_request_id( request_msg.request_id() );
            quantmodel::TimeStamp::getUTCTime( release_request.mutable_timestamp( ) );
            release_request.set_type( quantmodel::FixEngineRequest::CANCEL_RELEASE );
            release_request.set_session_id( release->getSessionId() );
            release->copyTo( release_request.mutable_release_data() );
            instrument_iter->second->copyTo( release_request.mutable_instrument_data() );
            account_iter->second->copyTo( release_request.mutable_account_data() );

            if( quantmodel::ZMQMessage::sendRPC(
                    &release_request,
                    &release_response,
                    socket_map[ "fix_callforward" ] ) )
            {
                if( release_response.type() == quantmodel::FixEngineResponse::ACKNOWLEDGEMENT )
                {
                    //
                    //  Update release status to PENDING_CANCEL
                    //

                    release->setAttribute( quantmodel::RELEASE_STATUS, PENDING_CANCEL_RELEASE );
                    release->setAttribute( quantmodel::UPDATED_DATETIME, ts );
                    release->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );
                    release->update( database_msg );

                    //
                    //  Add the updated release to the ack and bcast
                    //

                    release->asOrderMessage( response_msg.add_order() );
                    release->asOrderMessage( broadcast_msg.add_order() );
                }
                else
                {
                    //
                    //  REJECT
                    //

                    response_msg.set_request_id( request_msg.request_id( ) );
                    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                    response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
                    response_msg.set_error_msg( "FIX ENGINE REJECT" );
                    return;
                }
            }
            else
            {
                //
                //  REJECT
                //

                response_msg.set_request_id( request_msg.request_id( ) );
                quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
                response_msg.set_error_msg( "FIX ENGINE REJECT" );
                return;
            }
        }
        else
        {
            //
            //  REJECT
            //

            response_msg.set_request_id( request_msg.request_id( ) );
            quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
            response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
            response_msg.set_error_msg( "CANCEL RELEASE: INVALID RELEASE STATUS" );
            return;
        }
    }
    else
    {
        //
        //  ERROR
        //

        response_msg.set_request_id( request_msg.request_id( ) );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
        response_msg.set_error_msg( "INVALID MESSAGE: UNKNOWN RELEASE" );
        return;
    }

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

    quantmodel::TimeStamp::getMessageId( broadcast_msg.mutable_broadcast_id( ) );
    quantmodel::TimeStamp::getUTCTime( broadcast_msg.mutable_timestamp( ) );
    broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::CANCEL_RELEASE );
}
