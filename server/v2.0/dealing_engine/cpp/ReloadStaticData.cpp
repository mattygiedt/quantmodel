#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::reloadStaticData(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg )
{
    quantmodel::DatabaseEngineRequest request;
    quantmodel::DatabaseEngineResponse response;

    std::map< const std::pair< const std::string, const std::string >, std::set< std::string > >::iterator symbol_exchange_iter;
    std::map< std::pair< const std::string, const std::string > , quantmodel::AccountLimit* >::iterator account_limit_iter;
    std::map< const std::string, std::map< const std::pair< const std::string, const std::string > , quantmodel::AccountLimit* > >::iterator account_limit_map_iter;

    std::vector< quantmodel::AccountStrategy* >::iterator account_strategy_iter;
    std::map< const std::string, quantmodel::Account* >::iterator account_iter;
    std::map< const std::string, quantmodel::Strategy* >::iterator strategy_iter;
    std::map< const std::string, quantmodel::Instrument* >::iterator instrument_iter;
    std::map< const std::string, quantmodel::InvestmentSystem* >::iterator investment_system_iter;

    //
    //  Clear the instruments
    //

    for( instrument_iter = instrument_map.begin();
         instrument_iter != instrument_map.end();
         ++instrument_iter )
    {
        delete instrument_iter->second;
    }

    instrument_map.clear();

    //
    //  Load the instruments
    //

    request.Clear();
    response.Clear();
    quantmodel::TimeStamp::getMessageId( request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request.mutable_timestamp( ) );
    quantmodel::QuantModelProcedures::GET_ACTIVE_INSTRUMENTS( request );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + request.ShortDebugString() );
    }

    if( quantmodel::ZMQMessage::sendRPC( &request, &response, socket_map[ "database_callforward" ] ) )
    {
        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " --> " + response.ShortDebugString() );
        }

        for( int i=0; i<response.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response.result_set( i );

            for( int j=0; j<rs.row_size(); ++j )
            {
                quantmodel::Instrument *instrument =
                    new quantmodel::Instrument( rs.row( j ) );

                instrument_map.insert(
                    std::pair< const std::string , quantmodel::Instrument* >(
                        instrument->getInstrumentId(), instrument ) );
            }
        }
    }

    //
    //  Clear the symbol exhange map
    //

    for( symbol_exchange_iter = symbol_exchange_map.begin();
         symbol_exchange_iter != symbol_exchange_map.end();
         ++symbol_exchange_iter )
    {
        symbol_exchange_iter->second.clear();
    }

    symbol_exchange_map.clear();

    //
    //  Load the symbol exchange map
    //

    for( instrument_iter = instrument_map.begin();
         instrument_iter != instrument_map.end();
         ++instrument_iter )
    {
        const std::pair< const std::string, const std::string > key(
            instrument_iter->second->getAttribute( quantmodel::SYMBOL ),
            instrument_iter->second->getAttribute( quantmodel::SECURITY_EXCHANGE ) );

        const std::string instrument_id = instrument_iter->second->getInstrumentId();

        if( ( symbol_exchange_iter = symbol_exchange_map.find( key ) ) == symbol_exchange_map.end() )
        {
            std::set< std::string > instrument_set;
            instrument_set.insert( instrument_id );

            symbol_exchange_map.insert(
                std::pair< const std::pair< const std::string, const std::string > , std::set< std::string > >(
                    key, instrument_set ) );
        }
        else
        {
            symbol_exchange_iter->second.insert( instrument_id );
        }
    }

    //
    //  Clear the account limits
    //

    for( account_limit_map_iter = account_limit_map.begin();
         account_limit_map_iter != account_limit_map.end();
         ++account_limit_map_iter )
    {
        for( account_limit_iter = account_limit_map_iter->second.begin();
         account_limit_iter != account_limit_map_iter->second.end();
         ++account_limit_iter )
        {
            delete account_limit_iter->second;
        }
        account_limit_map_iter->second.clear();
    }

    account_limit_map.clear();

    //
    //  Load the account limits
    //

    request.Clear();
    response.Clear();
    quantmodel::TimeStamp::getMessageId( request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request.mutable_timestamp( ) );
    quantmodel::QuantModelProcedures::GET_ACCOUNT_LIMITS( request );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + request.ShortDebugString() );
    }

    if( quantmodel::ZMQMessage::sendRPC( &request, &response, socket_map[ "database_callforward" ] ) )
    {
        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " --> " + response.ShortDebugString() );
        }

        for( int i=0; i<response.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response.result_set( i );

            for( int j=0; j<rs.row_size(); ++j )
            {
                quantmodel::AccountLimit *account_limit =
                    new quantmodel::AccountLimit( rs.row( j ) );

                if( account_limit_map.count( account_limit->getAccountId() ) == 0 )
                {
                    account_limit_map.insert(
                        std::pair< const std::string, std::map< const std::pair< const std::string, const std::string > , quantmodel::AccountLimit* > >(
                            account_limit->getAccountId(),
                            std::map< const std::pair< const std::string, const std::string > , quantmodel::AccountLimit* >() ) );
                }

                account_limit_map[ account_limit->getAccountId() ].insert(
                    std::pair< const std::pair< const std::string, const std::string >, quantmodel::AccountLimit* >(
                               std::pair< const std::string, const std::string >(
                                          account_limit->getSymbol(),
                                          account_limit->getSecurityExchange() ),
                               account_limit ) );

            }
        }
    }

    //
    //  Clear the investment systems
    //

    for( investment_system_iter = investment_system_map.begin();
         investment_system_iter != investment_system_map.end();
         ++investment_system_iter )
    {
        delete investment_system_iter->second;
    }

    investment_system_map.clear();

    //
    //  Load the investment systems
    //

    request.Clear();
    response.Clear();
    quantmodel::TimeStamp::getMessageId( request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request.mutable_timestamp( ) );
    quantmodel::QuantModelProcedures::GET_INVESTMENT_SYSTEMS( request );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + request.ShortDebugString() );
    }

    if( quantmodel::ZMQMessage::sendRPC( &request, &response, socket_map[ "database_callforward" ] ) )
    {
        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " --> " + response.ShortDebugString() );
        }

        for( int i=0; i<response.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response.result_set( i );

            for( int j=0; j<rs.row_size(); ++j )
            {
                quantmodel::InvestmentSystem *investment_system =
                    new quantmodel::InvestmentSystem( rs.row( j ) );

                investment_system_map.insert(
                    std::pair< const std::string , quantmodel::InvestmentSystem* >(
                        investment_system->getInvestmentSystemId(), investment_system ) );
            }
        }
    }

    //
    //  Clear the strategies
    //

    for( strategy_iter = strategy_map.begin();
         strategy_iter != strategy_map.end();
         ++strategy_iter )
    {
        delete strategy_iter->second;
    }

    strategy_map.clear();

    //
    //  Load the strategies
    //

    request.Clear();
    response.Clear();
    quantmodel::TimeStamp::getMessageId( request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request.mutable_timestamp( ) );
    quantmodel::QuantModelProcedures::GET_STRATEGIES( request );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + request.ShortDebugString() );
    }

    if( quantmodel::ZMQMessage::sendRPC( &request, &response, socket_map[ "database_callforward" ] ) )
    {
        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " --> " + response.ShortDebugString() );
        }

        for( int i=0; i<response.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response.result_set( i );

            for( int j=0; j<rs.row_size(); ++j )
            {
                quantmodel::Strategy *strategy =
                    new quantmodel::Strategy( rs.row( j ) );

                strategy_map.insert(
                    std::pair< const std::string , quantmodel::Strategy* >(
                        strategy->getStrategyId(), strategy ) );

                //
                //  Add the strategy to the appropriate investment system
                //

                if( investment_system_map.count( strategy->getInvestmentSystemId() ) == 0 )
                {
                    log->warn( "Unknown investment_system_id: %s for strategy_id: %s",
                        strategy->getInvestmentSystemId().c_str(),
                        strategy->getStrategyId().c_str() );
                }
                else
                {
                    investment_system_map[ strategy->getInvestmentSystemId() ]->
                        addStrategy( strategy );
                }
            }
        }
    }

    //
    //  Clear the account strategies
    //

    for( account_strategy_iter = account_strategy_vec.begin();
         account_strategy_iter != account_strategy_vec.end();
         ++account_strategy_iter )
    {
        delete (*account_strategy_iter);
    }

    account_strategy_vec.clear();

    //
    //  Load the account strategies
    //

    request.Clear();
    response.Clear();
    quantmodel::TimeStamp::getMessageId( request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request.mutable_timestamp( ) );
    quantmodel::QuantModelProcedures::GET_ACCOUNT_STRATEGIES( request );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + request.ShortDebugString() );
    }

    if( quantmodel::ZMQMessage::sendRPC( &request, &response, socket_map[ "database_callforward" ] ) )
    {
        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " --> " + response.ShortDebugString() );
        }

        for( int i=0; i<response.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response.result_set( i );

            for( int j=0; j<rs.row_size(); ++j )
            {
                account_strategy_vec.push_back(
                    new quantmodel::AccountStrategy( rs.row( j ) ) );
            }
        }
    }

    //
    //  Clear the accounts
    //

    for( account_iter = account_map.begin();
         account_iter != account_map.end();
         ++account_iter )
    {
        delete account_iter->second;
    }

    account_map.clear();

    //
    //  Load the accounts
    //

    request.Clear();
    response.Clear();
    quantmodel::TimeStamp::getMessageId( request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request.mutable_timestamp( ) );
    quantmodel::QuantModelProcedures::GET_ACCOUNTS( request );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + request.ShortDebugString() );
    }

    if( quantmodel::ZMQMessage::sendRPC( &request, &response, socket_map[ "database_callforward" ] ) )
    {
        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " --> " + response.ShortDebugString() );
        }

        for( int i=0; i<response.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response.result_set( i );

            for( int j=0; j<rs.row_size(); ++j )
            {
                quantmodel::Account *account =
                    new quantmodel::Account( rs.row( j ) );

                account_map.insert(
                    std::pair< const std::string , quantmodel::Account* >(
                        account->getAccountId(), account ) );
            }
        }
    }

    //
    //  ACK
    //

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

    return;
}
