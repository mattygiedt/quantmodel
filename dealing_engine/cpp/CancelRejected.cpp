#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::cancelRejected(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    std::string ts;
    quantmodel::TimeStamp::getUTCTime( ts );

    for( int i=0; i<request_msg.order().release_data_size(); ++i )
    {
        //
        //  Get the release from the active set
        //

        quantmodel::Release * const release = active_set->getReleaseByReleaseId(
            request_msg.order().release_data( i ).release_id() );

        const mpf_class release_qty ( release->getAttribute( quantmodel::RELEASE_QTY ) );
        const mpf_class executed_qty ( release->getAttribute( quantmodel::EXECUTED_QTY ) );

        //
        //  Update the release status
        //

        if( cmp( executed_qty, 0 ) == 0 )
        {
            release->setAttribute( quantmodel::RELEASE_STATUS, NEW_RELEASE );
        }
        else if( cmp( release_qty, executed_qty ) == 0 )
        {
            release->setAttribute( quantmodel::RELEASE_STATUS, FILLED_RELEASE );
        }
        else
        {
            release->setAttribute( quantmodel::RELEASE_STATUS, PARTIALLY_FILLED_RELEASE );
        }

        release->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );
        release->setAttribute( quantmodel::UPDATED_DATETIME, ts );
        release->update( database_msg );

        release->asOrderMessage( response_msg.add_order() );
        release->asOrderMessage( broadcast_msg.add_order() );
    }

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

    quantmodel::TimeStamp::getMessageId( broadcast_msg.mutable_broadcast_id( ) );
    quantmodel::TimeStamp::getUTCTime( broadcast_msg.mutable_timestamp( ) );
    broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::CANCEL_REJECTED );
}
