#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

void cancelOrderInternal( quantmodel::Order* order,
                          quantmodel::DatabaseEngineRequest& database_msg,
                          const std::string& ts )
{
    std::vector< quantmodel::Allocation* > allocations;
    std::vector< quantmodel::Allocation* >::const_iterator alloc_iter;

    //
    //  Get the orders allocations
    //

    order->getAllocations( allocations );

    //
    //  For each allocation, set its allocation_qty equal to its executed_qty
    //

    for( alloc_iter = allocations.begin(); alloc_iter != allocations.end(); ++alloc_iter )
    {
        quantmodel::Allocation* allocation = (*alloc_iter);

        allocation->setAttribute( quantmodel::ALLOCATION_QTY, allocation->getAttribute( quantmodel::EXECUTED_QTY ) );
        allocation->setAttribute( quantmodel::UPDATED_DATETIME, ts );
        allocation->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );

        allocation->update( database_msg );
    }

    //
    //  Set the order status to CANCELED
    //

    order->setAttribute( quantmodel::OPEN_QTY, "0" );
    order->setAttribute( quantmodel::ORDER_STATUS, quantmodel::Order::CANCELED );
    order->setAttribute( quantmodel::UPDATED_DATETIME, ts );
    order->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );

    //
    //  Update the order
    //

    order->update( database_msg );
}

const void dealing::RequestHandler::cancelOrder(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    std::string ts;
    quantmodel::TimeStamp::getUTCTime( ts );

    //
    //  Get the order data from the request
    //

    const quantmodel::OrderData& order_data = request_msg.order().order_data();

    //
    //  Get the order from the active set
    //

    quantmodel::Order * const order = active_set->getOrderByOrderId( order_data.order_id() );

    //
    //  If the order_status is NEW or SUPPRESSED, cancel
    //

    if( order->getAttribute( quantmodel::ORDER_STATUS ).compare( NEW_ORDER ) == 0 ||
        order->getAttribute( quantmodel::ORDER_STATUS ).compare( SUPPRESSED_ORDER ) == 0 )
    {
        log->info( " canceling order: %s", order->getOrderId().c_str() );
        cancelOrderInternal( order, database_msg, ts );
        order->asOrderMessage( response_msg.add_order() );
        order->asOrderMessage( broadcast_msg.add_order() );
    }
    else if( order->getAttribute( quantmodel::ORDER_STATUS ).compare( PARTIALLY_FILLED_ORDER ) == 0 )
    {
        //
        //  Need to ensure that this order has no open (not canceled)
        //  releases.
        //

        std::vector< quantmodel::Release* > release_vec;
        std::vector< quantmodel::Release* >::const_iterator release_iter;

        active_set->getReleasesByOrderId( order->getOrderId(), release_vec );

        for( release_iter = release_vec.begin(); release_iter != release_vec.end(); ++release_iter )
        {
            const std::string release_status = (*release_iter)->getAttribute( quantmodel::RELEASE_STATUS );

            if( release_status.compare( NEW_RELEASE ) == 0 ||
                release_status.compare( PENDING_CANCEL_RELEASE ) == 0 ||
                release_status.compare( PENDING_REPLACE_RELEASE ) == 0 ||
                release_status.compare( REPLACED_RELEASE ) == 0 ||
                release_status.compare( PARTIALLY_FILLED_RELEASE ) == 0 )
            {
                //
                //  REJECT
                //

                response_msg.set_request_id( request_msg.request_id( ) );
                quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
                response_msg.set_error_msg( "INVALID ORDER STATUS: ORDER HAS ACTIVE RELEASE(S)" );

                return;
            }
        }

        log->info( " canceling order: %s", order->getOrderId().c_str() );
        cancelOrderInternal( order, database_msg, ts );
        order->asOrderMessage( response_msg.add_order() );
        order->asOrderMessage( broadcast_msg.add_order() );
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
        response_msg.set_error_msg( "INVALID ORDER STATUS" );

        return;
    }

    //
    //  ACKNOWLEDGEMENT
    //

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

    //
    //  Broadcast the changes
    //

    quantmodel::TimeStamp::getMessageId( broadcast_msg.mutable_broadcast_id( ) );
    quantmodel::TimeStamp::getUTCTime( broadcast_msg.mutable_timestamp( ) );
    broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::CANCEL_ORDER );
}
