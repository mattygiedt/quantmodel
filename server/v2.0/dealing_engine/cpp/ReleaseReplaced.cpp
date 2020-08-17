#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::releaseReplaced(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    std::string ts, key, val;

    std::vector< quantmodel::Allocation* > alloc_vec;
    std::vector< quantmodel::Allocation* >::const_iterator alloc_iter;

    quantmodel::Order *order = NULL;
    quantmodel::Release *release = NULL;

    quantmodel::TimeStamp::getUTCTime( ts );
    const quantmodel::ExecutionReportData& er_data = request_msg.execution_report_data();

    //
    //  Get the release and its' order from the active set
    //

    release = active_set->getReleaseByReleaseId( er_data.release_id() );

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

    order = active_set->getOrderByOrderId( release->getOrderId() );

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

    quantmodel::Instrument *instrument = NULL;
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
    //  Get the ER data and normalize if required
    //

    const mpf_class er_order_qty ( er_data.orderqty() );
    const mpf_class er_cum_qty ( er_data.cumqty() );
    mpf_class er_avg_prc ( er_data.avgpx() );
    mpf_class er_limit_prc ( er_data.price() );
    mpf_class er_stop_prc ( er_data.stoppx() );

    if( cmp( exchange_price_factor, 1 ) != 0 )
    {
        er_avg_prc *= exchange_price_factor;
        er_limit_prc *= exchange_price_factor;
        er_stop_prc *= exchange_price_factor;
    }

    //
    //  Update the release with the replaced data
    //

    release->setAttribute( quantmodel::RELEASE_STATUS, REPLACED_RELEASE );
    release->setAttribute( quantmodel::CLIENT_ORDER_ID, er_data.clordid() );
    release->setAttribute( quantmodel::COUNTERPARTY_ORDER_ID, er_data.orderid() );
    release->setAttribute( quantmodel::RELEASE_QTY, er_order_qty );
    release->setAttribute( quantmodel::EXECUTED_QTY, er_cum_qty );
    release->setAttribute( quantmodel::AVERAGE_PRC, er_avg_prc );
    release->setAttribute( quantmodel::LIMIT_PRC, er_limit_prc );
    release->setAttribute( quantmodel::STOP_PRC, er_stop_prc );
    release->setAttribute( quantmodel::EXECUTED_VAL, er_avg_prc * er_cum_qty );
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
    //  Rebuild the order + allocation
    //

    rebuildOrder( order, allocation, database_msg, ts );

    //
    //  Update the order_msg
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
    broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::RELEASE_REPLACED );
}
