#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::releaseRejected(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    std::string ts;
    quantmodel::TimeStamp::getUTCTime( ts );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " rejecting release: %s",
            request_msg.execution_report_data().release_id().c_str() );
    }

    //
    //  Get the release from the active set
    //

    quantmodel::Release *release = active_set->getReleaseByReleaseId(
        request_msg.execution_report_data().release_id() );

    if( NULL != release )
    {
        //
        //  Update the release status to REJECTED
        //

        release->setAttribute( quantmodel::RELEASE_STATUS, REJECTED_RELEASE );
        release->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );
        release->setAttribute( quantmodel::UPDATED_DATETIME, ts );
        release->update( database_msg );

        release->asOrderMessage( response_msg.add_order() );
        release->asOrderMessage( broadcast_msg.add_order() );

        response_msg.set_request_id( request_msg.request_id( ) );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

        quantmodel::TimeStamp::getMessageId( broadcast_msg.mutable_broadcast_id( ) );
        quantmodel::TimeStamp::getUTCTime( broadcast_msg.mutable_timestamp( ) );
        broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::RELEASE_REJECTED );
    }
    else
    {
        response_msg.set_request_id( request_msg.request_id( ) );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
        response_msg.set_error_msg( "unknown release" );
    }
}
