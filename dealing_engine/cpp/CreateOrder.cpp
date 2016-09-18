#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::createOrder(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    std::string id, object_id, val, key, ts;

    quantmodel::TimeStamp::getUTCTime( ts );

    const quantmodel::DraftOrderData& draft_order_data =
        request_msg.draft_order( ).draft_order_data( );

    //
    //  Get the instrument from the instrument_map
    //

    quantmodel::Instrument *instrument = NULL;
    std::map< const std::string, quantmodel::Instrument* >::const_iterator instrument_iter;

    if( ( instrument_iter = instrument_map.find( draft_order_data.instrument_id() ) ) != instrument_map.end() )
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
    //  Ensure that we don't have a duplicate external order id
    //

    if( draft_order_data.external_order_id() != "" &&
        active_set->hasOrderByExternalOrderId( draft_order_data.external_order_id() ) )
    {
        log->error( "active_set: duplicate external order id: %s",
            draft_order_data.external_order_id().c_str() );

        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
        response_msg.set_error_msg( "active_set: duplicate external order id: " +
            draft_order_data.external_order_id() );

        return;
    }


    //
    //  Get the next order id
    //

    parameters->getNextObjectId( id, object_id, dealing::DealingEngineParameters::ORDER );

    //
    //  Create a new order
    //

    quantmodel::Order *order = new quantmodel::Order( object_id );
    order->setAttribute( quantmodel::EXTERNAL_ORDER_ID, draft_order_data.external_order_id() );
    order->setAttribute( quantmodel::INVESTMENT_SYSTEM_ID, draft_order_data.investment_system_id() );
    order->setAttribute( quantmodel::LIMIT_PRC, draft_order_data.limit_prc() );
    order->setAttribute( quantmodel::STOP_PRC, draft_order_data.stop_prc() );
    order->setAttribute( quantmodel::INSTRUMENT_ID, instrument->getAttribute( quantmodel::INSTRUMENT_ID ) );
    order->setAttribute( quantmodel::SYMBOL, instrument->getAttribute( quantmodel::SYMBOL ) );
    order->setAttribute( quantmodel::ENTRY_PRC, instrument->getAttribute( quantmodel::LAST_PRC ) );
    order->setAttribute( quantmodel::BASE_CCY, instrument->getAttribute( quantmodel::BASE_CCY ) );
    order->setAttribute( quantmodel::ADDED_DATETIME, ts );
    order->setAttribute( quantmodel::UPDATED_DATETIME, ts );
    order->setAttribute( quantmodel::ADDED_BY, "DEALING_ENGINE" );
    order->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );

    //
    //  set the order type
    //

    switch( draft_order_data.order_type() )
    {
        case quantmodel::DraftOrderData::MARKET:
            order->setAttribute( quantmodel::ORDER_TYPE, quantmodel::Order::MARKET );
            break;

        case quantmodel::DraftOrderData::LIMIT:
            order->setAttribute( quantmodel::ORDER_TYPE, quantmodel::Order::LIMIT );
            break;

        case quantmodel::DraftOrderData::STOP:
            order->setAttribute( quantmodel::ORDER_TYPE, quantmodel::Order::STOP );
            break;

        case quantmodel::DraftOrderData::STOP_LIMIT:
            order->setAttribute( quantmodel::ORDER_TYPE, quantmodel::Order::STOP_LIMIT );
            break;

        case quantmodel::DraftOrderData::MARKET_ON_CLOSE:
            order->setAttribute( quantmodel::ORDER_TYPE, quantmodel::Order::MARKET_ON_CLOSE );
            break;

        case quantmodel::DraftOrderData::MARKET_LIMIT:
            order->setAttribute( quantmodel::ORDER_TYPE, quantmodel::Order::MARKET_LIMIT );
            break;
    }

    //
    //  set the time in force
    //

    switch( draft_order_data.tif() )
    {
        case quantmodel::DraftOrderData::DAY:
            order->setAttribute( quantmodel::TIF, quantmodel::Order::DAY );
            break;

        case quantmodel::DraftOrderData::GTC:
            order->setAttribute( quantmodel::TIF, quantmodel::Order::GTC );
            break;

        case quantmodel::DraftOrderData::OPG:
            order->setAttribute( quantmodel::TIF, quantmodel::Order::OPG );
            break;

        case quantmodel::DraftOrderData::IOC:
            order->setAttribute( quantmodel::TIF, quantmodel::Order::IOC );
            break;

        case quantmodel::DraftOrderData::FOK:
            order->setAttribute( quantmodel::TIF, quantmodel::Order::FOK );
            break;

        case quantmodel::DraftOrderData::GTD:
            order->setAttribute( quantmodel::TIF, quantmodel::Order::GTD );
            break;

        case quantmodel::DraftOrderData::GIS:
            order->setAttribute( quantmodel::TIF, quantmodel::Order::GIS );
            break;
    }

    //
    //  set the side_code,
    //

    switch( draft_order_data.side_code() )
    {
        case quantmodel::DraftOrderData::BUY:
            order->setAttribute( quantmodel::SIDE_CODE, quantmodel::Order::BUY );
            break;

        case quantmodel::DraftOrderData::SELL:
            order->setAttribute( quantmodel::SIDE_CODE, quantmodel::Order::SELL );
            break;
    }

    //
    //  set the order_qty, open_qty
    //

    const mpf_class order_qty ( draft_order_data.order_qty() );
    order->setAttribute( quantmodel::ORDER_QTY, order_qty );
    order->setAttribute( quantmodel::OPEN_QTY, order_qty );

    //
    //  Add the order to the database request
    //

    order->insert( database_msg, id );

    //
    //  Add the order to the active set
    //

    active_set->addOrder( order );

    //
    //  Create the orders allocations
    //

    mpf_class total_alloc_qty = 0;

    for( int i=0; i<request_msg.draft_order().draft_allocation_data_size(); ++i )
    {
        //
        //  Get the next allocation id
        //

        parameters->getNextObjectId( id, object_id, dealing::DealingEngineParameters::ALLOCATION );

        const quantmodel::DraftAllocationData& draft_alloc_data =
            request_msg.draft_order().draft_allocation_data( i );

        const mpf_class alloc_qty ( draft_alloc_data.allocation_qty() );
        total_alloc_qty += alloc_qty;

        quantmodel::Allocation *alloc = new quantmodel::Allocation( object_id );
        alloc->setAttribute( quantmodel::ORDER_ID, order->getOrderId() );
        alloc->setAttribute( quantmodel::ACCOUNT_ID, draft_alloc_data.account_id() );
        alloc->setAttribute( quantmodel::ALLOCATION_QTY, alloc_qty );
        alloc->setAttribute( quantmodel::INSTRUMENT_ID, instrument->getAttribute( quantmodel::INSTRUMENT_ID ) );
        alloc->setAttribute( quantmodel::SIDE_CODE, order->getAttribute( quantmodel::SIDE_CODE ) );
        alloc->setAttribute( quantmodel::ADDED_DATETIME, ts );
        alloc->setAttribute( quantmodel::UPDATED_DATETIME, ts );
        alloc->setAttribute( quantmodel::ADDED_BY, "DEALING_ENGINE" );
        alloc->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );

        //
        //  Add the allocation to the database request
        //

        alloc->insert( database_msg, id );

        //
        //  Add the allocation to the order
        //

        order->addAllocation( alloc );
    }

    //
    //  Ensure that the total allocation quantity equals the order quantity
    //

    if( order_qty != total_alloc_qty )
    {
        //
        //  Reject
        //

        log->error( "order_qty: %f != total_alloc_qty: %f",
            order_qty.get_d(), total_alloc_qty.get_d() );

        order->reject();
        order->update( database_msg );
    }

    //
    //  Add the order to the response and broadcast messages
    //

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );
    order->asOrderMessage( response_msg.add_order() );

    quantmodel::TimeStamp::getMessageId( broadcast_msg.mutable_broadcast_id( ) );
    quantmodel::TimeStamp::getUTCTime( broadcast_msg.mutable_timestamp( ) );
    broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::CREATE_ORDER );
    order->asOrderMessage( broadcast_msg.add_order() );
}
