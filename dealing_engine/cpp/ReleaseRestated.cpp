#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::releaseRestated(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    std::string ts, key, val;
    quantmodel::TimeStamp::getUTCTime( ts );

    quantmodel::Release *release = NULL;

    const quantmodel::ExecutionReportData& er_data = request_msg.execution_report_data();

    //
    //  Get the release from the active set
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

    quantmodel::Order *order = active_set->getOrderByOrderId( release->getOrderId() );

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

    quantmodel::Allocation *allocation = order->getAllocation( release->getAllocationId() );

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

    const mpf_class er_cum_qty ( er_data.cumqty() );
    const mpf_class er_order_qty ( er_data.orderqty() );
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
    //  Set the counterparty OrderID
    //

    release->setAttribute( quantmodel::COUNTERPARTY_ORDER_ID, er_data.orderid() );

    //
    //  Set the OrdType. These mirror the FIX::OrdType, except for MARKET_LIMIT
    //

    if( er_data.ordtype().compare( "K" ) == 0 )
    {
        release->setAttribute( quantmodel::RELEASE_TYPE, quantmodel::Order::MARKET_LIMIT );
    }
    else
    {
        release->setAttribute( quantmodel::RELEASE_TYPE, er_data.ordtype() );
    }

    //
    //  Set the release status
    //

    if( cmp( er_cum_qty, 0 ) == 0 )
    {
        release->setAttribute( quantmodel::RELEASE_STATUS, NEW_RELEASE );
    }
    else if( cmp( er_cum_qty, er_order_qty ) == 0 )
    {
        release->setAttribute( quantmodel::RELEASE_STATUS, FILLED_RELEASE );
    }
    else
    {
        release->setAttribute( quantmodel::RELEASE_STATUS, PARTIALLY_FILLED_RELEASE );
    }

    //
    //  Do we have a limit price?
    //

    if( er_data.has_price() && cmp( exchange_price_factor, 1 ) != 0 )
    {
        mpf_class limit_prc ( er_data.price() );
        limit_prc *= exchange_price_factor;
        release->setAttribute( quantmodel::LIMIT_PRC, limit_prc );
    }

    //
    //  Rebuild the order + allocation(s)
    //

    rebuildOrder( order, allocation, database_msg, ts );

    //
    //  Update the restated release in the database
    //

    release->update( database_msg );

    //
    //  Update clients of the restated release
    //

    quantmodel::OrderMessage * const response_order_message = response_msg.add_order();
    quantmodel::OrderMessage * const broadcast_order_message = broadcast_msg.add_order();

    order->asOrderMessage( response_order_message );
    order->asOrderMessage( broadcast_order_message );

    release->asOrderMessage( response_order_message );
    release->asOrderMessage( broadcast_order_message );

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

    quantmodel::TimeStamp::getMessageId( broadcast_msg.mutable_broadcast_id( ) );
    quantmodel::TimeStamp::getUTCTime( broadcast_msg.mutable_timestamp( ) );
    broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::RELEASE_RESTATED );
}
