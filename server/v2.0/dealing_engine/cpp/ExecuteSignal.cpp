#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::executeSignal(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg )
{
    int i, j, k;
    quantmodel::DealingEngineRequest draft_order_request_msg;
    quantmodel::DealingEngineResponse draft_order_response_msg;

    quantmodel::DealingEngineRequest create_order_request_msg;
    quantmodel::DealingEngineResponse create_order_response_msg;

    quantmodel::DealingEngineRequest release_order_request_msg;
    quantmodel::DealingEngineResponse release_order_response_msg;

    //
    //  Create the draft order from the signal
    //

    draft_order_request_msg.set_request_id( request_msg.request_id() );
    quantmodel::TimeStamp::getUTCTime( draft_order_request_msg.mutable_timestamp( ) );
    draft_order_request_msg.set_type( quantmodel::DealingEngineRequest::CREATE_DRAFT_ORDER );

    for( i=0; i<request_msg.signal_data_size(); ++i )
    {
        draft_order_request_msg.add_signal_data()->CopyFrom( request_msg.signal_data( i ) );
    }

    createDraftOrder(
        draft_order_request_msg,
        draft_order_response_msg,
        broadcast_msg,
        database_msg );

    createAuditEvent( draft_order_request_msg, draft_order_response_msg, database_msg );

    if( draft_order_response_msg.type() != quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT )
    {
        //
        //  REJECT
        //

        response_msg.set_request_id( request_msg.request_id( ) );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
        response_msg.set_error_msg( draft_order_response_msg.error_msg() );

        return;
    }

    //
    //  Create the orders from the draft orders
    //

    create_order_request_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getUTCTime( create_order_request_msg.mutable_timestamp( ) );
    create_order_request_msg.set_type( quantmodel::DealingEngineRequest::CREATE_ORDER );

    for( i=0; i<draft_order_response_msg.draft_order_size(); ++i )
    {
        create_order_response_msg.Clear();
        create_order_request_msg.mutable_draft_order()->CopyFrom(
            draft_order_response_msg.draft_order( i ) );

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

        for( j=0; j<create_order_response_msg.order_size(); ++j )
        {
            release_order_response_msg.Clear();
            release_order_request_msg.mutable_order()->CopyFrom(
                create_order_response_msg.order( j ) );

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

            for( k=0; k<release_order_response_msg.order_size(); ++k )
            {
                response_msg.add_order()->CopyFrom(
                    release_order_response_msg.order( k ) );
            }
        }
    }

    //
    //  ACKNOWLEDGEMENT
    //

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

    broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::EXECUTE_SIGNAL );
}
