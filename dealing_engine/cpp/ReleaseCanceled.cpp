#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::releaseCanceled(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    std::string ts, key, val;

    std::vector< quantmodel::Release* > release_vec;
    std::vector< quantmodel::Release* >::const_iterator release_iter;
    std::vector< quantmodel::Allocation* > alloc_vec;
    std::vector< quantmodel::Allocation* >::const_iterator alloc_iter;

    quantmodel::TimeStamp::getUTCTime( ts );
    const quantmodel::ExecutionReportData& er_data = request_msg.execution_report_data();

    //
    //  Get the release and its' order from the active set
    //

    quantmodel::Release * const release = active_set->getReleaseByReleaseId( er_data.release_id() );
    quantmodel::Order * const order = active_set->getOrderByOrderId( release->getOrderId() );

    //
    //  Update the canceled release
    //

    release->setAttribute( quantmodel::RELEASE_STATUS, CANCELED_RELEASE );
    release->setAttribute( quantmodel::RELEASE_QTY, release->getAttribute( quantmodel::EXECUTED_QTY ) );
    release->setAttribute( quantmodel::UPDATED_DATETIME, ts );
    release->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );

    release->update( database_msg );

    //
    //  Get the allocation
    //

    quantmodel::Allocation *allocation = order->getAllocation( release->getAllocationId() );

    if( NULL == allocation )
    {
        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
        response_msg.set_error_msg( "UNKNOWN ALLOCATION" );

        return;
    }

    //
    //  Rebuild the order
    //

    rebuildOrder( order, allocation, database_msg, ts );

    //
    //  Set the response and broadcast OrderMessage fields
    //

    quantmodel::OrderMessage * const response_order_message = response_msg.add_order();
    quantmodel::OrderMessage * const broadcast_order_message = broadcast_msg.add_order();

    order->asOrderMessage( response_order_message );
    order->asOrderMessage( broadcast_order_message );

    release->asOrderMessage( response_order_message );
    release->asOrderMessage( broadcast_order_message );

    //
    //  Generate the acknowledgement and broadcast messages
    //

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

    quantmodel::TimeStamp::getMessageId( broadcast_msg.mutable_broadcast_id( ) );
    quantmodel::TimeStamp::getUTCTime( broadcast_msg.mutable_timestamp( ) );
    broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::RELEASE_CANCELED );
}
