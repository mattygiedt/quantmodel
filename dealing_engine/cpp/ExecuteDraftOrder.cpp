#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::executeDraftOrder(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg )
{
    int i, j;

    quantmodel::DealingEngineRequest create_order_request_msg;
    quantmodel::DealingEngineResponse create_order_response_msg;

    quantmodel::DealingEngineRequest release_order_request_msg;
    quantmodel::DealingEngineResponse release_order_response_msg;

    //
    //  Create the order from the draft order
    //

    create_order_request_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getUTCTime( create_order_request_msg.mutable_timestamp( ) );
    create_order_request_msg.set_type( quantmodel::DealingEngineRequest::CREATE_ORDER );
    create_order_request_msg.mutable_draft_order()->CopyFrom( request_msg.draft_order( ) );

    createOrder(
        create_order_request_msg,
        create_order_response_msg,
        broadcast_msg,
        database_msg );

    createAuditEvent( create_order_request_msg,
        create_order_response_msg, database_msg );

    //
    //  Immediately release the order(s)
    //

    release_order_request_msg.Clear();
    release_order_request_msg.set_request_id( request_msg.request_id() );
    quantmodel::TimeStamp::getUTCTime( release_order_request_msg.mutable_timestamp( ) );
    release_order_request_msg.set_type( quantmodel::DealingEngineRequest::RELEASE_ORDER );

    for( i=0; i<create_order_response_msg.order_size(); ++i )
    {
        release_order_response_msg.Clear();
        release_order_request_msg.mutable_order()->CopyFrom(
            create_order_response_msg.order( i ) );

        releaseOrder(
            release_order_request_msg,
            release_order_response_msg,
            broadcast_msg,
            database_msg );

        createAuditEvent( release_order_request_msg,
            release_order_response_msg, database_msg );

        //
        //  Add the order to the response
        //

        for( j=0; j<release_order_response_msg.order_size(); ++j )
        {
            response_msg.add_order()->CopyFrom(
                release_order_response_msg.order( j ) );
        }
    }

    //
    //  ACKNOWLEDGEMENT
    //

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );
}
