#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::updateOrder(
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

    const quantmodel::OrderData& order_data = request_msg.order().order_data( );

    //
    //  Get the order from the active set
    //

    quantmodel::Order *order = active_set->getOrderByOrderId( order_data.order_id() );

    //
    //  Only update updateable fields:
    //
    //      ORDER_TYPE
    //      LIMIT_PRC
    //      STOP_PRC
    //      MARK_PRC
    //

    order->setAttribute( quantmodel::ORDER_TYPE, order_data.order_type() );
    order->setAttribute( quantmodel::LIMIT_PRC, order_data.limit_prc() );
    order->setAttribute( quantmodel::STOP_PRC, order_data.stop_prc() );
    order->setAttribute( quantmodel::MARK_PRC, order_data.mark_prc() );
    order->setAttribute( quantmodel::UPDATED_DATETIME, ts );
    order->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );

    //
    //  Update the order in the database
    //

    order->update( database_msg );
    order->asOrderMessage( response_msg.add_order() );
    order->asOrderMessage( broadcast_msg.add_order() );

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

    quantmodel::TimeStamp::getMessageId( broadcast_msg.mutable_broadcast_id( ) );
    quantmodel::TimeStamp::getUTCTime( broadcast_msg.mutable_timestamp( ) );
    broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::UPDATE_ORDER );
}
