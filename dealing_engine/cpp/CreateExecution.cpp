#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::createExecution(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    std::string ts, id, key, val, object_id;

    std::vector< quantmodel::Allocation* > alloc_vec;
    std::vector< quantmodel::Allocation* >::const_iterator alloc_iter;

    std::vector< quantmodel::Execution* > execution_vec;
    std::vector< quantmodel::Execution* >::const_iterator execution_iter;

    quantmodel::TimeStamp::getUTCTime( ts );
    const quantmodel::ExecutionReportData& er_data = request_msg.execution_report_data();

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( "createExecution: %s %s",
            er_data.execid().c_str(),
            er_data.multilegreportingtype().c_str() );
    }

    //
    //  Get the next execution id
    //

    parameters->getNextObjectId( id, object_id, dealing::DealingEngineParameters::EXECUTION );

    //
    //  Get the release and associated order->allocation from the active set
    //

    quantmodel::Release * const release = active_set->getReleaseByReleaseId( er_data.release_id() );

    if( NULL == release )
    {
        log->error( "Cannot find release_id: %s", er_data.release_id().c_str() );

        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
        response_msg.set_error_msg( "UNKNOWN RELEASE" );

        return;
    }

    quantmodel::Order * const order = active_set->getOrderByOrderId( release->getOrderId() );

    if( NULL == order )
    {
        log->error( "Cannot find order_id: %s from release_id: %s",
            release->getOrderId().c_str(),
            release->getReleaseId().c_str() );

        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
        response_msg.set_error_msg( "UNKNOWN ORDER" );

        return;
    }

    quantmodel::Allocation * const allocation = order->getAllocation( release->getAllocationId() );

    if( NULL == allocation )
    {
        log->error( "Cannot find allocation_id: %s in order_id: %s from release_id: %s",
            release->getAllocationId().c_str(),
            release->getOrderId().c_str(),
            release->getReleaseId().c_str() );

        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
        response_msg.set_error_msg( "UNKNOWN ALLOCATION" );

        return;
    }

    //
    //  Get the instrument from the instrument_map
    //

    std::string find_id;
    quantmodel::Instrument *instrument = NULL;
    std::map< const std::string, quantmodel::Instrument* >::const_iterator instrument_iter;

    if( er_data.multilegreportingtype() == "2" )
    {
        //
        //  Find the instrument based on securityID, as it will not match
        //  the spread instrument on the release.
        //

        find_id = er_data.securityid();
    }
    else
    {
        find_id = release->getInstrumentId();
    }

    if( ( instrument_iter = instrument_map.find( find_id ) ) != instrument_map.end() )
    {
        instrument = instrument_iter->second;
    }
    else
    {
        log->error( "instrument_map: unknown instrument_key: %s", key.c_str() );

        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
        response_msg.set_error_msg( "instrument_map: unknown instrument" );

        return;
    }

    //
    //  Set the exchange price factor
    //

    const mpf_class exchange_price_factor ( instrument->getAttribute( quantmodel::EXCHANGE_PRICE_FACTOR ) );

    if( cmp( exchange_price_factor, 0 ) == 0 )
    {
        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
        response_msg.set_error_msg( "INVALID EXCHANGE PRICE FACTOR" );

        return;
    }

    //
    //  Get the ER data and normalize if required
    //

    const mpf_class er_order_qty ( er_data.orderqty() );
    const mpf_class er_cum_qty ( er_data.cumqty() );
    const mpf_class er_last_qty ( er_data.lastshares() );
    const mpf_class er_leaves_qty ( er_data.leavesqty() );

    mpf_class er_avg_prc ( er_data.avgpx() );
    mpf_class er_limit_prc ( er_data.price() );
    mpf_class er_stop_prc ( er_data.stoppx() );
    mpf_class er_last_prc ( er_data.lastpx() );

    if( cmp( exchange_price_factor, 1 ) != 0 )
    {
        er_avg_prc *= exchange_price_factor;
        er_limit_prc *= exchange_price_factor;
        er_stop_prc *= exchange_price_factor;
        er_last_prc *= exchange_price_factor;
    }

    //
    //  Create the new execution
    //

    quantmodel::Execution *execution = new quantmodel::Execution( object_id );
    execution->setAttribute( quantmodel::SESSION_ID, er_data.session_id() );
    execution->setAttribute( quantmodel::ACCOUNT_ID, release->getAttribute( quantmodel::ACCOUNT_ID ) );
    execution->setAttribute( quantmodel::ORDER_ID, order->getOrderId() );
    execution->setAttribute( quantmodel::RELEASE_ID, release->getReleaseId() );
    execution->setAttribute( quantmodel::INVESTMENT_SYSTEM_ID, order->getAttribute( quantmodel::INVESTMENT_SYSTEM_ID ) );
    execution->setAttribute( quantmodel::INSTRUMENT_ID, instrument->getInstrumentId() );
    execution->setAttribute( quantmodel::CLIENT_ORDER_ID, er_data.clordid() );
    execution->setAttribute( quantmodel::COUNTERPARTY_EXECUTION_ID, er_data.execid() );
    execution->setAttribute( quantmodel::SIDE_CODE, release->getAttribute( quantmodel::SIDE_CODE ) );
    execution->setAttribute( quantmodel::EXECUTION_CCY, "USD" );
    execution->setAttribute( quantmodel::EXECUTION_STATUS, quantmodel::Execution::UNALLOCATED );
    execution->setAttribute( quantmodel::EXECUTION_PRC, er_last_prc );
    execution->setAttribute( quantmodel::EXECUTION_QTY, er_last_qty );
    execution->setAttribute( quantmodel::EXECUTION_VAL, er_last_prc * er_last_qty );
    execution->setAttribute( quantmodel::LEAVES_QTY, er_leaves_qty );
    execution->setAttribute( quantmodel::AVERAGE_PRC, er_avg_prc );
    execution->setAttribute( quantmodel::LEG_TYPE, er_data.multilegreportingtype() );
    execution->setAttribute( quantmodel::ADDED_DATETIME, ts );
    execution->setAttribute( quantmodel::UPDATED_DATETIME, ts );
    execution->setAttribute( quantmodel::ADDED_BY, "DEALING_ENGINE" );
    execution->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );

    execution->insert( database_msg, id );

    //
    //  Add the execution to the active set
    //

    active_set->addExecution( execution );

    //
    //  If the execution report fill is either a single security (1) or
    //  the spread (3) allocate the fill accross the release and order.
    //

    if( er_data.multilegreportingtype() != "2" )
    {
        execution->setAttribute( quantmodel::EXECUTION_STATUS, quantmodel::Execution::ALLOCATED );

        //
        //  Rebuild the EXECUTED_QTY, AVERAGE_PRC, EXECUTED_VAL for this release
        //

        mpf_class execution_qty ( 0 );
        mpf_class execution_prc ( 0 );
        mpf_class release_average_prc ( 0 );
        mpf_class release_executed_qty ( 0 );
        mpf_class release_executed_val ( 0 );

        active_set->getExecutionsByReleaseId( release->getReleaseId(), execution_vec );

        for( execution_iter = execution_vec.begin();
             execution_iter != execution_vec.end();
             ++execution_iter )
        {
            execution_qty = (*execution_iter)->getAttribute( quantmodel::EXECUTION_QTY );
            execution_prc = (*execution_iter)->getAttribute( quantmodel::EXECUTION_PRC );

            release_executed_qty += execution_qty;
            release_executed_val += execution_qty * execution_prc;
        }

        if( cmp( release_executed_qty, 0 ) > 0 )
        {
            release_average_prc = release_executed_val / release_executed_qty;
        }

        //
        //  Update the release with the fill data
        //

        release->setAttribute( quantmodel::CLIENT_ORDER_ID, er_data.clordid() );
        release->setAttribute( quantmodel::COUNTERPARTY_ORDER_ID, er_data.orderid() );
        release->setAttribute( quantmodel::EXECUTED_QTY, release_executed_qty );
        release->setAttribute( quantmodel::AVERAGE_PRC, release_average_prc );
        release->setAttribute( quantmodel::EXECUTED_VAL, release_executed_val );
        release->setAttribute( quantmodel::UPDATED_DATETIME, ts );
        release->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );

        if( cmp( er_order_qty, er_cum_qty ) == 0 )
        {
            release->setAttribute( quantmodel::RELEASE_STATUS, FILLED_RELEASE );
        }
        else
        {
            release->setAttribute( quantmodel::RELEASE_STATUS, PARTIALLY_FILLED_RELEASE );
        }

        release->update( database_msg );
        execution->update( database_msg );

        //
        //  Rebuild the order + allocation(s)
        //

        rebuildOrder( order, allocation, database_msg, ts );
    }

    //
    //  Update the broadcast with the new dealing object details.
    //

    quantmodel::OrderMessage* const response_order_message = response_msg.add_order();
    quantmodel::OrderMessage* const broadcast_order_message = broadcast_msg.add_order();

    order->asOrderMessage( response_order_message );
    order->asOrderMessage( broadcast_order_message );

    release->asOrderMessage( response_order_message );
    release->asOrderMessage( broadcast_order_message );

    execution->asOrderMessage( response_order_message );
    execution->asOrderMessage( broadcast_order_message );

    //
    //  Generate the acknowledgement and broadcast messages
    //

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

    quantmodel::TimeStamp::getMessageId( broadcast_msg.mutable_broadcast_id( ) );
    quantmodel::TimeStamp::getUTCTime( broadcast_msg.mutable_timestamp( ) );
    broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::CREATE_EXECUTION );
}
