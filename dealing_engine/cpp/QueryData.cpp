#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::queryOrder(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    quantmodel::Order *order = NULL;

    std::vector< quantmodel::Release* > release_vec;
    std::vector< quantmodel::Release* >::const_iterator release_iter;
    std::vector< quantmodel::Execution* > execution_vec;
    std::vector< quantmodel::Execution* >::const_iterator execution_iter;

    //
    //  ORDER_ID takes precedence over EXTERNAL_ORDER_ID
    //

    if( request_msg.order().order_data().order_id() != "" )
    {
        order = active_set->getOrderByOrderId(
            request_msg.order().order_data().order_id() );
    }
    else if( request_msg.order().order_data().external_order_id() != "" )
    {
        order = active_set->getOrderByExternalOrderId(
            request_msg.order().order_data().external_order_id() );
    }

    if( NULL == order )
    {
        //
        //  REJECT
        //

        response_msg.set_request_id( request_msg.request_id( ) );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
        response_msg.set_error_msg( "INVALID ORDER_ID" );

        return;
    }

    quantmodel::OrderMessage * const response_order_message = response_msg.add_order();

    //
    //  Add the order to the response
    //

    order->asOrderMessage( response_order_message );

    //
    //  Get the releases and executions for the order
    //

    active_set->getReleasesByOrderId( order->getOrderId(), release_vec );
    active_set->getExecutionsByOrderId( order->getOrderId(), execution_vec );

    for( release_iter = release_vec.begin(); release_iter != release_vec.end(); ++release_iter )
    {
        (*release_iter)->asOrderMessage( response_order_message );
    }

    for( execution_iter = execution_vec.begin(); execution_iter != execution_vec.end(); ++execution_iter )
    {
        (*execution_iter)->asOrderMessage( response_order_message );
    }

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );
}

const void dealing::RequestHandler::queryRelease(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    quantmodel::OrderMessage *response_order_message = response_msg.add_order();

    for( int i=0; i<request_msg.order().release_data_size(); i++ )
    {
        quantmodel::Release * const release = active_set->getReleaseByReleaseId(
            request_msg.order().release_data( i ).release_id() );

        if( NULL == release )
        {
            //
            //  REJECT
            //

            response_msg.set_request_id( request_msg.request_id( ) );
            quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
            response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
            response_msg.set_error_msg( "INVALID RELEASE_ID" );

            return;
        }

        //
        //  Add the release to the response
        //

        release->asOrderMessage( response_order_message );
    }

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );
}

const void dealing::RequestHandler::queryExecution(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    quantmodel::OrderMessage * const response_order_message = response_msg.add_order();

    for( int i=0; i<request_msg.order().execution_data_size(); i++ )
    {
        quantmodel::Execution * const execution = active_set->getExecutionByExecutionId(
            request_msg.order().execution_data( i ).execution_id() );

        if( NULL == execution )
        {
            //
            //  REJECT
            //

            response_msg.set_request_id( request_msg.request_id( ) );
            quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
            response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
            response_msg.set_error_msg( "INVALID EXECUTION_ID" );

            return;
        }

        //
        //  Add the execution to the response
        //

        execution->asOrderMessage( response_order_message );
    }

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );
}
