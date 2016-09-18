#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::releaseOrder(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg )
{
    std::string ts, id, object_id;
    quantmodel::TimeStamp::getUTCTime( ts );

    quantmodel::FixEngineRequest release_request;
    quantmodel::FixEngineResponse release_response;

    quantmodel::DealingEngineRequest compliance_request;
    quantmodel::DealingEngineResponse compliance_response;

    std::map< const std::string , std::string >::const_iterator fix_session_iter;
    std::map< const std::string , std::map< const std::string, std::string > >::const_iterator fix_session_map_iter;
    std::map< const std::string , quantmodel::Account* >::const_iterator account_iter;
    std::map< const std::string , quantmodel::Instrument* >::const_iterator instrument_iter;

    //
    //  If we're not sending to FIX, reject
    //

    if(! parameters->sendToFix() )
    {
        //
        //  REJECT
        //

        response_msg.set_request_id( request_msg.request_id( ) );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
        response_msg.set_error_msg( "SEND TO FIX == FALSE" );

        return;
    }

    //
    //  Get the order data from the request
    //

    const quantmodel::OrderData& order_data = request_msg.order().order_data();

    //
    //  Get the order from the active set
    //

    quantmodel::Order * const order = active_set->getOrderByOrderId( order_data.order_id() );

    if( NULL == order )
    {
        //
        //  REJECT
        //

        response_msg.set_request_id( request_msg.request_id( ) );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
        response_msg.set_error_msg( "UNKNOWN ORDER ID" );

        return;
    }

    //
    //  Get the instrument on the order
    //

    if( ( instrument_iter = instrument_map.find( order->getInstrumentId() ) ) == instrument_map.end() )
    {
        //
        //  REJECT
        //

        response_msg.set_request_id( request_msg.request_id( ) );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
        response_msg.set_error_msg( "UNKNOWN INSTRUMENT" );

        return;
    }

    //
    //  Is there anything to release on the order?
    //

    const mpf_class order_qty ( order->getAttribute( quantmodel::ORDER_QTY ) );
    const mpf_class order_released_qty ( order->getAttribute( quantmodel::RELEASED_QTY ) );

    if( cmp( order_qty, order_released_qty ) > 0 )
    {
        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( "releasing order %s", order->getOrderId().c_str() );
        }

        //
        //  Add the order to the order_msg
        //

        quantmodel::OrderMessage * const response_order = response_msg.add_order();
        quantmodel::OrderMessage * const broadcast_order = broadcast_msg.add_order();

        order->asOrderMessage( response_order );
        order->asOrderMessage( broadcast_order );

        //
        //  Get the allocations on the order
        //

        std::vector< quantmodel::Allocation* > alloc_vec;
        std::vector< quantmodel::Allocation* >::const_iterator alloc_iter;

        order->getAllocations( alloc_vec );

        for( alloc_iter = alloc_vec.begin(); alloc_iter != alloc_vec.end(); ++alloc_iter )
        {
            quantmodel::Allocation* allocation = (*alloc_iter);

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " %s allocation_qty: %s, released_qty: %s",
                    allocation->getAllocationId().c_str(),
                    allocation->getAttribute( quantmodel::ALLOCATION_QTY ).c_str(),
                    allocation->getAttribute( quantmodel::RELEASED_QTY ).c_str() );
            }

            //
            //  Is there anything to release on this allocation?
            //

            const mpf_class released_qty ( allocation->getAttribute( quantmodel::RELEASED_QTY ) );
            const mpf_class allocation_qty ( allocation->getAttribute( quantmodel::ALLOCATION_QTY ) );

            if( cmp( allocation_qty, released_qty ) > 0 )
            {
                const mpf_class qty_to_release = allocation_qty - released_qty;

                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( " %s releasing %f",
                        allocation->getAllocationId().c_str(),
                        qty_to_release.get_d() );
                }

                //
                //  Get the account on the allocation
                //

                if( ( account_iter = account_map.find( allocation->getAccountId() ) ) == account_map.end() )
                {
                    //
                    //  REJECT
                    //

                    response_msg.set_request_id( request_msg.request_id( ) );
                    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                    response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
                    response_msg.set_error_msg( "UNKNOWN ACCOUNT" );

                    return;
                }

                //
                //  Get the next release id
                //

                parameters->getNextObjectId( id, object_id, dealing::DealingEngineParameters::RELEASE );

                //
                //  Create a new release
                //

                quantmodel::Release *release = new quantmodel::Release( object_id );
                release->setAttribute( quantmodel::ACCOUNT_ID, allocation->getAccountId() );
                release->setAttribute( quantmodel::ORDER_ID, order->getOrderId() );
                release->setAttribute( quantmodel::ALLOCATION_ID, allocation->getAllocationId() );
                release->setAttribute( quantmodel::INVESTMENT_SYSTEM_ID, order->getAttribute( quantmodel::INVESTMENT_SYSTEM_ID ) );
                release->setAttribute( quantmodel::INSTRUMENT_ID, order->getInstrumentId() );
                release->setAttribute( quantmodel::RELEASE_TYPE, order->getAttribute( quantmodel::ORDER_TYPE ) );
                release->setAttribute( quantmodel::TIF, order->getAttribute( quantmodel::TIF ) );
                release->setAttribute( quantmodel::SIDE_CODE, order->getAttribute( quantmodel::SIDE_CODE ) );
                release->setAttribute( quantmodel::LIMIT_PRC, order->getAttribute( quantmodel::LIMIT_PRC ) );
                release->setAttribute( quantmodel::STOP_PRC, order->getAttribute( quantmodel::STOP_PRC ) );
                release->setAttribute( quantmodel::RELEASE_QTY, qty_to_release );
                release->setAttribute( quantmodel::ADDED_DATETIME, ts );
                release->setAttribute( quantmodel::UPDATED_DATETIME, ts );
                release->setAttribute( quantmodel::ADDED_BY, "DEALING_ENGINE" );
                release->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );

                //
                //  Validate the release using the VALIDATE_RELEASE handler
                //

                compliance_request.Clear();
                compliance_response.Clear();

                compliance_request.set_request_id( request_msg.request_id() );
                quantmodel::TimeStamp::getUTCTime( compliance_request.mutable_timestamp( ) );
                compliance_request.set_type( quantmodel::DealingEngineRequest::VALIDATE_RELEASE );
                release->copyTo( compliance_request.mutable_order()->add_release_data() );

                validateRelease( compliance_request, compliance_response, broadcast_msg, database_msg );

                release->setAttribute( quantmodel::COMPLIANCE_AUDIT_ID, compliance_response.response_id() );
                release->setAttribute( quantmodel::SESSION_ID, compliance_response.account_limit().session_id() );

                //
                //  Audit the validation
                //

                createAuditEvent( compliance_request, compliance_response, database_msg );

                //
                //  If we have a valid release, send it to the FIX Engine
                //

                if( compliance_response.type() == quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT )
                {
                    //
                    //  Send the release to the fix engine
                    //

                    release_request.Clear();
                    release_request.set_request_id( request_msg.request_id() );
                    quantmodel::TimeStamp::getUTCTime( release_request.mutable_timestamp( ) );
                    release_request.set_type( quantmodel::FixEngineRequest::CREATE_RELEASE );
                    release_request.set_session_id( release->getSessionId() );
                    release->copyTo( release_request.mutable_release_data() );
                    instrument_iter->second->copyTo( release_request.mutable_instrument_data() );
                    account_iter->second->copyTo( release_request.mutable_account_data() );

                    if( unlikely( log->isDebugEnabled() ) )
                    {
                        log->info( " <-- " + release_request.ShortDebugString() );
                    }

                    if( quantmodel::ZMQMessage::sendRPC(
                            &release_request,
                            &release_response,
                            socket_map[ "fix_callforward" ] ) )
                    {
                        if( unlikely( log->isDebugEnabled() ) )
                        {
                            log->info( " --> " + release_response.ShortDebugString() );
                        }

                        if( release_response.type() != quantmodel::FixEngineResponse::ACKNOWLEDGEMENT )
                        {
                            log->error( " FIX Engine rejected release request %s",
                                release_request.ShortDebugString().c_str() );
                            log->error( " FIX Engine response %s",
                                release_response.ShortDebugString().c_str() );

                            //
                            //  Set the release status to REJECTED
                            //

                            release->setAttribute( quantmodel::RELEASE_STATUS, quantmodel::Release::REJECTED );
                            release->setAttribute( quantmodel::RELEASE_QTY, "0" );
                        }
                    }
                    else
                    {
                        log->error( " FIX Engine rejected release request %s",
                                release_request.ShortDebugString().c_str() );
                        log->error( " FIX Engine response %s",
                            release_response.ShortDebugString().c_str() );

                        //
                        //  Set the release status to REJECTED
                        //

                        release->setAttribute( quantmodel::RELEASE_STATUS, quantmodel::Release::REJECTED );
                        release->setAttribute( quantmodel::RELEASE_QTY, "0" );
                    }
                }
                else
                {
                    //
                    //  Set the release status to COMPLIANCE_REJECT
                    //

                    release->setAttribute( quantmodel::RELEASE_STATUS, quantmodel::Release::COMPLIANCE_REJECT );
                    release->setAttribute( quantmodel::RELEASE_QTY, "0" );
                }

                //
                //  Add the release to the active set
                //

                active_set->addRelease( release );

                //
                //  Add the new release to the database
                //

                release->insert( database_msg, id );

                //
                //  Add the release to the order message
                //

                release->asOrderMessage( response_order );
                release->asOrderMessage( broadcast_order );
            }
        }
    }
    else
    {
        log->warn( "ORDER %s is fully released: %s == %s",
             order->getOrderId().c_str(),
             order->getAttribute( quantmodel::ORDER_QTY ).c_str(),
             order->getAttribute( quantmodel::RELEASED_QTY ).c_str() );
    }

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

    quantmodel::TimeStamp::getMessageId( broadcast_msg.mutable_broadcast_id( ) );
    quantmodel::TimeStamp::getUTCTime( broadcast_msg.mutable_timestamp( ) );
    broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::RELEASE_ORDER );
}
