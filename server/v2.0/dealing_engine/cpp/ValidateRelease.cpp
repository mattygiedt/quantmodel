#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::validateRelease(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg ) const
{
    quantmodel::AccountLimit *account_limit;
    quantmodel::AccountPosition *account_position;
    quantmodel::InstrumentPosition *instrument_position;

    std::vector< quantmodel::Release* > release_vec;
    std::vector< quantmodel::Release* >::const_iterator release_iter;

    std::map< const std::string, quantmodel::Instrument* >::const_iterator instrument_iter;

    std::vector< quantmodel::InstrumentPosition* > instrument_position_vec;
    std::vector< quantmodel::InstrumentPosition* >::const_iterator instrument_position_iter;

    std::map< const std::pair< const std::string, const std::string >, std::set< std::string > >::const_iterator symbol_exchange_iter;
    std::map< const std::pair< const std::string, const std::string > , quantmodel::AccountLimit* >::const_iterator account_limit_iter;
    std::map< const std::string, std::map< const std::pair< const std::string, const std::string > , quantmodel::AccountLimit* > >::const_iterator account_limit_map_iter;

    if( request_msg.order().release_data_size() != 1 )
    {
        //
        //  ERROR
        //

        response_msg.set_request_id( request_msg.request_id( ) );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
        response_msg.set_error_msg( "INVALID MESSAGE: INVALID RELEASE SIZE" );
        return;
    }

    const quantmodel::ReleaseData& release_data = request_msg.order().release_data( 0 );

    //
    //  Get the account_limits for this account + symbol + security_exchange
    //

    account_limit_map_iter = account_limit_map.find( release_data.account_id() );

    if( account_limit_map_iter == account_limit_map.end() )
    {
        //
        //  REJECT
        //

        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
        response_msg.set_error_msg( "VALIDATE RELEASE REJECT: NO ACCOUNT LIMITS FOR ACCOUNT" );
        response_msg.add_order()->add_release_data()->CopyFrom( release_data );
        return;
    }

    //
    //  Get the instrument symbol + exchange key
    //

    instrument_iter = instrument_map.find( release_data.instrument_id() );

    if( instrument_iter == instrument_map.end() )
    {
        //
        //  REJECT
        //

        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
        response_msg.set_error_msg( "VALIDATE RELEASE REJECT: UNKNOWN INSTRUMENT" );
        response_msg.add_order()->add_release_data()->CopyFrom( release_data );
        return;
    }

    const std::pair< const std::string , const std::string > key(
        instrument_iter->second->getAttribute( quantmodel::SYMBOL ),
        instrument_iter->second->getAttribute( quantmodel::SECURITY_EXCHANGE ) );

    //
    //  Get the set of instruments that have this symbol + exchange
    //

    symbol_exchange_iter = symbol_exchange_map.find( key );

    if( symbol_exchange_iter->second.size() == 0 )
    {
        //
        //  REJECT
        //

        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
        response_msg.set_error_msg( "VALIDATE RELEASE REJECT: NO INSTRUMENTS FOR SYMBOL + EXCHANGE" );
        response_msg.add_order()->add_release_data()->CopyFrom( release_data );
        return;
    }

    //
    //  Get the account limits with the same key ( symbol + exchange )
    //

    account_limit_iter = account_limit_map_iter->second.find( key );

    if( account_limit_iter == account_limit_map_iter->second.end() )
    {
        //
        //  REJECT
        //

        response_msg.set_request_id( request_msg.request_id() );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
        response_msg.set_error_msg( "VALIDATE RELEASE REJECT: NO ACCOUNT LIMITS FOR SYMBOL + EXCHANGE" );
        response_msg.add_order()->add_release_data()->CopyFrom( release_data );
        return;
    }

    account_limit = account_limit_iter->second;

    //
    //  Copy the account limit to the response
    //

    account_limit->copyTo( response_msg.mutable_account_limit() );

    //
    //  Is the release_qty less than or equal to the maximum release quantity?
    //

    const mpf_class new_release_qty ( release_data.release_qty() );
    const mpf_class max_release_qty ( account_limit->getMaxReleaseQty() );

    if( new_release_qty > max_release_qty )
    {
        log->error( "account[ %s ]->instrument[ %s %s ]: invalid release: release_qty (%s) > max_release_qty (%s)",
            account_limit->getAccountId().c_str(),
            account_limit->getSymbol().c_str(),
            account_limit->getSecurityExchange().c_str(),
            release_data.release_qty().c_str(),
            account_limit->getMaxReleaseQty().c_str() );

        response_msg.set_request_id( request_msg.request_id( ) );
        quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
        quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
        response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
        response_msg.set_error_msg( "invalid release: release_qty > max_release_qty" );
        response_msg.add_order()->add_release_data()->CopyFrom( release_data );
        return;
    }

    //
    //  Get the existing releases for this account
    //

    active_set->getReleasesByAccountId( release_data.account_id(), release_vec );

    //
    //  Build the current open position for all instruments with the same
    //  symbol + security exchange
    //

    mpf_class long_pos, short_pos, curr_long_pos, curr_short_pos;

    account_position = active_set->getAccountPosition( release_data.account_id() );
    account_position->getInstrumentPositions( instrument_position_vec );

    long_pos = 0;
    short_pos = 0;

    for( instrument_position_iter = instrument_position_vec.begin();
         instrument_position_iter != instrument_position_vec.end();
         ++instrument_position_iter )
    {
        instrument_position = (*instrument_position_iter);
        instrument_iter = instrument_map.find( instrument_position->getInstrumentId() );

        if( instrument_iter != instrument_map.end() )
        {
            if( instrument_iter->second->isSymbolExchange( key ) )
            {
                if( instrument_position->getExposure() == quantmodel::InstrumentPosition::LONG )
                {
                    long_pos += instrument_position->getOpenPosition();
                }
                else if( instrument_position->getExposure() == quantmodel::InstrumentPosition::SHORT )
                {
                    short_pos += instrument_position->getOpenPosition();
                }
            }
        }
    }

    curr_long_pos = long_pos;
    curr_short_pos = short_pos;

    //
    //  Iterate over the existing releases, treating open as executed
    //

    for( release_iter = release_vec.begin(); release_iter != release_vec.end(); ++release_iter )
    {
        instrument_iter = instrument_map.find( (*release_iter)->getInstrumentId() );

        if( instrument_iter != instrument_map.end() )
        {
            if( instrument_iter->second->isSymbolExchange( key ) )
            {
                quantmodel::Release *release = (*release_iter);

                //
                //  Test for un-executed quantity for NEW and PARTIALLY_FILLED
                //

                if( release->getAttribute( quantmodel::RELEASE_STATUS ) == "1" ||
                    release->getAttribute( quantmodel::RELEASE_STATUS ) == "2" )
                {
                    const mpf_class release_qty ( release->getAttribute( quantmodel::RELEASE_QTY ) );

                    if( release_qty > 0 )
                    {
                        const mpf_class executed_qty ( release->getAttribute( quantmodel::EXECUTED_QTY ) );

                        if( cmp( release_qty, executed_qty ) > 0 )
                        {
                            //
                            //  Add the un-executed quantity to the open position
                            //

                            if( release->getAttribute( quantmodel::SIDE_CODE ) == "1" )
                            {
                                long_pos += release_qty - executed_qty;
                            }
                            else
                            {
                                short_pos += release_qty - executed_qty;
                            }
                        }
                    }
                }
            }
        }
    }

    //
    //  Treat the new_release as executed, determine if it would exceed the
    //  worst case position.
    //

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );
    response_msg.add_order()->add_release_data()->CopyFrom( release_data );

    if( release_data.side_code() == "1" )
    {
        long_pos += new_release_qty;
        const mpf_class max_long_pos ( account_limit->getMaxLongQty() );

        if( cmp( long_pos, max_long_pos ) > 0 )
        {
            log->error( "account[ %s ]->instrument[ %s %s ]: invalid release: worst case long position (%f) > max_open_long_qty (%f)",
                account_limit->getAccountId().c_str(),
                account_limit->getSymbol().c_str(),
                account_limit->getSecurityExchange().c_str(),
                long_pos.get_d(),
                max_long_pos.get_d() );

            response_msg.set_error_msg( "invalid release: worst case long position exceeded" );
            response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
        }
    }
    else
    {
        short_pos += new_release_qty;
        const mpf_class max_short_pos ( account_limit->getMaxShortQty() );

        if( cmp( short_pos, max_short_pos ) > 0 )
        {
            log->error( "account[ %s ]->instrument[ %s %s ]: invalid release: worst case short position (%f) > max_open_short_qty (%f)",
                account_limit->getAccountId().c_str(),
                account_limit->getSymbol().c_str(),
                account_limit->getSecurityExchange().c_str(),
                short_pos.get_d(),
                max_short_pos.get_d() );

            response_msg.set_error_msg( "invalid release: worst case short position exceeded" );
            response_msg.set_type( quantmodel::DealingEngineResponse::REJECT );
        }
    }
}
