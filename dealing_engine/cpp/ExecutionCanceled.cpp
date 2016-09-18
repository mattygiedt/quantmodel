#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::executionCanceled(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    std::string id, ts, key, val, object_id;

    std::vector< quantmodel::Allocation* > alloc_vec;
    std::vector< quantmodel::Allocation* >::const_iterator alloc_iter;

    std::vector< quantmodel::Execution* > execution_vec;
    std::vector< quantmodel::Execution* >::const_iterator execution_iter;

    quantmodel::TimeStamp::getUTCTime( ts );
    const quantmodel::ExecutionReportData& er_data = request_msg.execution_report_data();

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
            order->getOrderId().c_str(),
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
            order->getOrderId().c_str(),
            release->getReleaseId().c_str() );

        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
        response_msg.set_error_msg( "UNKNOWN ALLOCATION" );

        return;
    }

/*
    //
    //  Get the instrument from memcached
    //

    quantmodel::Instrument *instrument = NULL;
    key.append( "instrument::instrument_id::" )
       .append( release->getInstrumentId() );

    if( memcache.get( key, val ) )
    {
        quantmodel::DatabaseEngineResponse::ResultSet::Row row;
        row.ParseFromString( val );
        instrument = new quantmodel::Instrument( row );
    }
    else
    {
        log->error( "memcached: unknown instrument_key: %s", key.c_str() );

        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
        response_msg.set_error_msg( "memcached: unknown instrument" );

        return;
    }
*/

    //
    //  Get the instrument from the instrument_map
    //

    quantmodel::Instrument* instrument = NULL;
    std::map< const std::string, quantmodel::Instrument* >::const_iterator instrument_iter;

    if( ( instrument_iter = instrument_map.find( release->getInstrumentId() ) ) != instrument_map.end() )
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
    //  Get the executions for the release
    //

    active_set->getExecutionsByReleaseId( er_data.release_id(), execution_vec );

    //
    //  Cancel all executions
    //

    for( execution_iter = execution_vec.begin(); execution_iter != execution_vec.end(); ++execution_iter )
    {
        (*execution_iter)->setAttribute( quantmodel::EXECUTION_STATUS, quantmodel::Execution::CANCELED );
        (*execution_iter)->setAttribute( quantmodel::EXECUTION_PRC, "0" );
        (*execution_iter)->setAttribute( quantmodel::EXECUTION_QTY, "0" );
        (*execution_iter)->setAttribute( quantmodel::EXECUTION_VAL, "0" );
        (*execution_iter)->setAttribute( quantmodel::AVERAGE_PRC, "0" );
        (*execution_iter)->setAttribute( quantmodel::LEAVES_QTY, "0" );
        (*execution_iter)->setAttribute( quantmodel::UPDATED_DATETIME, ts );
        (*execution_iter)->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );
        (*execution_iter)->update( database_msg );
    }

    //
    //  Get the canceled execution quantity and price
    //

    const mpf_class er_lastshares_qty ( er_data.lastshares() );
    mpf_class er_last_prc ( er_data.lastpx() );

    if( cmp( exchange_price_factor, 1 ) != 0 )
    {
        er_last_prc *= exchange_price_factor;
    }

    const mpf_class canceled_val = er_last_prc * er_lastshares_qty;
    const mpf_class executed_qty ( release->getAttribute( quantmodel::EXECUTED_QTY ) );
    const mpf_class executed_val ( release->getAttribute( quantmodel::EXECUTED_VAL ) );

    const mpf_class new_executed_qty = executed_qty - er_lastshares_qty;
    const mpf_class new_executed_val = executed_val - canceled_val;

    //
    //  Update the release with the canceled execution data
    //

    quantmodel::Execution *execution = NULL;
    release->setAttribute( quantmodel::EXECUTED_QTY, new_executed_qty );
    release->setAttribute( quantmodel::EXECUTED_VAL, new_executed_val );

    if( cmp( 0, new_executed_qty ) == 0 )
    {
        release->setAttribute( quantmodel::AVERAGE_PRC, "0" );
        release->setAttribute( quantmodel::RELEASE_STATUS, NEW_RELEASE );
    }
    else
    {
        const mpf_class release_qty ( release->getAttribute( quantmodel::RELEASE_QTY ) );
        const mpf_class average_prc = ( new_executed_val / new_executed_qty );
        release->setAttribute( quantmodel::AVERAGE_PRC, average_prc );
        release->setAttribute( quantmodel::RELEASE_STATUS, PARTIALLY_FILLED_RELEASE );

        //
        //  Create a new single execution which matches the release values
        //

        parameters->getNextObjectId( id, object_id, dealing::DealingEngineParameters::EXECUTION );
        execution = new quantmodel::Execution( object_id );
        execution->setAttribute( quantmodel::SESSION_ID, er_data.session_id() );
        execution->setAttribute( quantmodel::ACCOUNT_ID, release->getAttribute( quantmodel::ACCOUNT_ID ) );
        execution->setAttribute( quantmodel::ORDER_ID, order->getOrderId() );
        execution->setAttribute( quantmodel::RELEASE_ID, release->getReleaseId() );
        execution->setAttribute( quantmodel::INVESTMENT_SYSTEM_ID, order->getAttribute( quantmodel::INVESTMENT_SYSTEM_ID ) );
        execution->setAttribute( quantmodel::INSTRUMENT_ID, order->getInstrumentId() );
        execution->setAttribute( quantmodel::CLIENT_ORDER_ID, er_data.clordid() );
        execution->setAttribute( quantmodel::COUNTERPARTY_EXECUTION_ID, er_data.execid() );
        execution->setAttribute( quantmodel::SIDE_CODE, release->getAttribute( quantmodel::SIDE_CODE ) );
        execution->setAttribute( quantmodel::EXECUTION_CCY, "USD" );
        execution->setAttribute( quantmodel::EXECUTION_STATUS, quantmodel::Execution::ALLOCATED );
        execution->setAttribute( quantmodel::EXECUTION_PRC, average_prc );
        execution->setAttribute( quantmodel::EXECUTION_QTY, new_executed_qty );
        execution->setAttribute( quantmodel::EXECUTION_VAL, new_executed_val  );
        execution->setAttribute( quantmodel::AVERAGE_PRC, average_prc );
        execution->setAttribute( quantmodel::LEAVES_QTY, release_qty - new_executed_qty );
        execution->setAttribute( quantmodel::ADDED_DATETIME, ts );
        execution->setAttribute( quantmodel::UPDATED_DATETIME, ts );
        execution->setAttribute( quantmodel::ADDED_BY, "DEALING_ENGINE" );
        execution->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );

        execution->insert( database_msg, id );

        //
        //  Add the execution to the active set
        //

        active_set->addExecution( execution );
    }

    release->setAttribute( quantmodel::UPDATED_DATETIME, ts );
    release->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );
    release->update( database_msg );

    //
    //  Rebuild the order + allocation(s)
    //

    rebuildOrder( order, allocation, database_msg, ts );

    //
    //  Update the broadcast with the new dealing object details.
    //

    quantmodel::OrderMessage * const response_order_message = response_msg.add_order();
    quantmodel::OrderMessage * const broadcast_order_message = broadcast_msg.add_order();

    order->asOrderMessage( response_order_message );
    order->asOrderMessage( broadcast_order_message );

    release->asOrderMessage( response_order_message );
    release->asOrderMessage( broadcast_order_message );

    if( NULL != execution )
    {
        execution->asOrderMessage( response_order_message );
        execution->asOrderMessage( broadcast_order_message );
    }

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

    quantmodel::TimeStamp::getMessageId( broadcast_msg.mutable_broadcast_id( ) );
    quantmodel::TimeStamp::getUTCTime( broadcast_msg.mutable_timestamp( ) );
    broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::EXECUTION_CANCELED );
}
