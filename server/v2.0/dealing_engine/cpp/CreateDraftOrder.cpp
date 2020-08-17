#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

bool isValidPrice( const std::string& prc )
{
    if( prc.size() == 0 ) return false;

    for( size_t i=0; i<prc.size(); ++i )
    {
        const char ch = prc[ i ];

        if( ch == '0' || ch == '1' || ch == '2' ||
            ch == '3' || ch == '4' || ch == '5' ||
            ch == '6' || ch == '7' || ch == '8' ||
            ch == '9' || ch == '.' || ch == '-' )
        {
            continue;
        }

        return false;
    }

    return true;
}

const void dealing::RequestHandler::createDraftOrder(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    char buf[ 32 ];
    std::string ts;
    quantmodel::TimeStamp::getUTCTime( ts );

    //quantmodel::DraftOrderData draft_order;

    quantmodel::Strategy *strategy;

    std::vector< quantmodel::Strategy* > strategy_vec;

    std::vector< quantmodel::Strategy* >::const_iterator strategy_iter;
    std::vector< quantmodel::AccountStrategy * >::const_iterator account_strategy_iter;

    std::map< const std::string , quantmodel::Account* >::const_iterator account_iter;
    std::map< const std::string , quantmodel::Instrument* >::const_iterator instrument_iter;
    std::map< const std::string , quantmodel::InvestmentSystem* >::const_iterator investment_system_iter;

    //
    //  For each signal
    //

    for( int i=0; i < request_msg.signal_data_size(); ++i )
    {
        //
        //  Get the signal data
        //

        const quantmodel::SignalData& signal_data = request_msg.signal_data( i );

        instrument_iter = instrument_map.find( signal_data.instrument_id() );
        investment_system_iter = investment_system_map.find( signal_data.investment_system_id() );

        if( signal_data.external_order_id() != "" )
        {
            if( signal_data.external_order_id().size() > 16 )
            {
                response_msg.set_request_id( request_msg.request_id() );
                quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
                response_msg.set_error_msg( "EXTERNAL ORDER ID SIZE > 16: " + signal_data.external_order_id() );

                return;
            }

            if( active_set->hasOrderByExternalOrderId( signal_data.external_order_id() ) )
            {
                response_msg.set_request_id( request_msg.request_id() );
                quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
                response_msg.set_error_msg( "DUPLICATE ACTIVE EXTERNAL ORDER ID: " + signal_data.external_order_id() );

                return;
            }
        }

        if( investment_system_iter == investment_system_map.end() )
        {
            response_msg.set_request_id( request_msg.request_id() );
            quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
            response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
            response_msg.set_error_msg( "UNKNOWN INVESTMENT SYSTEM: " + signal_data.investment_system_id() );

            return;
        }

        if( instrument_iter == instrument_map.end() )
        {
            response_msg.set_request_id( request_msg.request_id() );
            quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
            response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
            response_msg.set_error_msg( "UNKNOWN INSTRUMENT" );

            return;
        }

        //
        //  Get the strategies for this investment system
        //

        investment_system_iter->second->getStrategies( strategy_vec );

        if( strategy_vec.size() == 0 )
        {
            response_msg.set_request_id( request_msg.request_id() );
            quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
            response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
            response_msg.set_error_msg( "INVESTMENT SYSTEM HAS NO ACTIVE STRATEGIES" );

            return;
        }

        //
        //  Find the strategy based on the signal_id
        //

        strategy = NULL;
        for( strategy_iter = strategy_vec.begin();
             strategy_iter != strategy_vec.end();
             ++strategy_iter )
        {
            if( (*strategy_iter)->getSignalId() == signal_data.signal_id() )
            {
                strategy = (*strategy_iter);
                break;
            }
        }

        if( NULL == strategy )
        {
            response_msg.set_request_id( request_msg.request_id() );
            quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
            response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
            response_msg.set_error_msg( "UNKNOWN INVESTMENT SYSTEM STRATEGY" );

            return;
        }

        //
        //  Create a new draft order on the response
        //

        quantmodel::DraftOrderMessage * const draft_order_msg = response_msg.add_draft_order();
        quantmodel::DraftOrderData * const draft_order_data = draft_order_msg->mutable_draft_order_data();

        draft_order_data->set_external_order_id( signal_data.external_order_id() );
        draft_order_data->set_instrument_id( signal_data.instrument_id() );
        draft_order_data->set_investment_system_id( signal_data.investment_system_id() );

        if( signal_data.side_code() == "BUY" )
        {
            draft_order_data->set_side_code( quantmodel::DraftOrderData::BUY );
        }
        else if( signal_data.side_code() == "SELL" )
        {
            draft_order_data->set_side_code( quantmodel::DraftOrderData::SELL );
        }
        else
        {
            response_msg.set_request_id( request_msg.request_id() );
            quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
            response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
            response_msg.set_error_msg( "UNKNOWN SIDE CODE" );

            return;
        }

        if( signal_data.order_type() == "MARKET" )
        {
            draft_order_data->set_order_type( quantmodel::DraftOrderData::MARKET );
        }
        else if( signal_data.order_type() == "LIMIT" )
        {
            if( isValidPrice( signal_data.limit_prc() ) )
            {
                draft_order_data->set_order_type( quantmodel::DraftOrderData::LIMIT );
                draft_order_data->set_limit_prc( signal_data.limit_prc() );
            }
            else
            {
                response_msg.set_request_id( request_msg.request_id() );
                quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
                response_msg.set_error_msg( "INVALID LIMIT PRICE" );

                return;
            }
        }
        else if( signal_data.order_type() == "STOP" )
        {
            if( isValidPrice( signal_data.stop_prc() ) )
            {
                draft_order_data->set_order_type( quantmodel::DraftOrderData::STOP );
                draft_order_data->set_limit_prc( signal_data.stop_prc() );
            }
            else
            {
                response_msg.set_request_id( request_msg.request_id() );
                quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
                response_msg.set_error_msg( "INVALID STOP PRICE" );

                return;
            }
        }
        else if( signal_data.order_type() == "STOP_LIMIT" )
        {
            if( isValidPrice( signal_data.stop_prc() ) &&
                isValidPrice( signal_data.limit_prc() ) )
            {
                draft_order_data->set_order_type( quantmodel::DraftOrderData::STOP_LIMIT );
                draft_order_data->set_stop_prc( signal_data.stop_prc() );
                draft_order_data->set_limit_prc( signal_data.limit_prc() );
            }
            else
            {
                response_msg.set_request_id( request_msg.request_id() );
                quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
                response_msg.set_error_msg( "INVALID STOP_LIMIT PRICE" );

                return;
            }
        }
        else if( signal_data.order_type() == "MARKET_LIMIT" )
        {
            draft_order_data->set_order_type( quantmodel::DraftOrderData::MARKET_LIMIT );
        }
        else
        {
            response_msg.set_request_id( request_msg.request_id() );
            quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
            response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
            response_msg.set_error_msg( "UNKNOWN ORDER TYPE" );

            return;
        }

        //
        //  Get the accounts exposed to this signal, add the allocation
        //  to the draft order
        //

        const int signal_qty = atoi( signal_data.signal_qty().c_str() );
        int order_qty;
        order_qty = 0;

        for( account_strategy_iter = account_strategy_vec.begin();
             account_strategy_iter != account_strategy_vec.end();
             ++account_strategy_iter )
        {
            if( (*account_strategy_iter)->isEnabled() == true &&
                (*account_strategy_iter)->getStrategyId() == strategy->getStrategyId() )
            {
                //
                //  Get the account, ensure it is active
                //

                account_iter = account_map.find( (*account_strategy_iter)->getAccountId() );

                if( account_iter != account_map.end() &&
                    account_iter->second->isActive() )
                {
                    quantmodel::DraftAllocationData *draft_allocation_data;
                    draft_allocation_data = draft_order_msg->add_draft_allocation_data();

                    const int account_weight = atoi( (*account_strategy_iter)->getAttribute( quantmodel::WEIGHT ).c_str() );
                    const int draft_alloc_qty = signal_qty * account_weight;
                    order_qty += draft_alloc_qty;

                    sprintf( buf, "%d", draft_alloc_qty );
                    draft_allocation_data->set_account_id( (*account_strategy_iter)->getAccountId() );
                    draft_allocation_data->set_allocation_qty( buf );
                }
            }
        }

        sprintf( buf, "%d", order_qty );
        draft_order_data->set_order_qty( buf );
    }

    //
    //  ACKNOWLEDGEMENT
    //

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );
}
