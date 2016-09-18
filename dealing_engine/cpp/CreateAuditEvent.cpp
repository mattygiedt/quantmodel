#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

void auditOrder(
    const quantmodel::DealingEngineRequest& request_msg,
    const quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DatabaseEngineRequest& audit_msg,
    const quantmodel::OrderData& order_data,
    const std::string& request_type,
    const std::string& response_type )
{
    std::string ts, order_id, instrument_id, side_code, tif, order_status, order_qty,
                order_released_qty, order_executed_qty, order_average_prc,
                limit_prc, stop_prc, investment_system_id;

    quantmodel::TimeStamp::getUTCTime( ts );

    investment_system_id = order_data.investment_system_id();
    order_id = order_data.order_id();
    instrument_id = order_data.instrument_id();
    side_code = order_data.side_code();
    tif = order_data.tif();
    order_status = order_data.order_status();
    order_qty = order_data.order_qty();
    order_released_qty = order_data.released_qty();
    order_executed_qty = order_data.executed_qty();
    order_average_prc = order_data.average_prc();
    limit_prc = order_data.limit_prc();
    stop_prc = order_data.stop_prc();

    quantmodel::QuantModelProcedures::INSERT_DEALING_ORDER_AUDIT_EVENT(
        audit_msg,
        request_msg.request_id(),
        request_type,
        request_msg.timestamp(),
        response_msg.response_id(),
        response_type,
        response_msg.timestamp(),
        response_msg.error_msg(),
        investment_system_id,
        order_id,
        instrument_id,
        side_code,
        tif,
        limit_prc,
        stop_prc,
        order_status,
        order_qty,
        order_released_qty,
        order_executed_qty,
        order_average_prc,
        ts,
        "DEALING_ENGINE" );
}

void auditRelease(
    const quantmodel::DealingEngineRequest& request_msg,
    const quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DatabaseEngineRequest& audit_msg,
    const quantmodel::ReleaseData& release_data,
    const std::string& request_type,
    const std::string& response_type )
{
    std::string ts, order_id, release_id, session_id, account_id, instrument_id,
                side_code, tif, release_status, release_qty,
                release_executed_qty, release_average_prc,
                limit_prc, stop_prc, investment_system_id;

    quantmodel::TimeStamp::getUTCTime( ts );

    investment_system_id = release_data.investment_system_id();
    order_id = release_data.order_id();
    release_id = release_data.release_id();
    session_id = release_data.session_id();
    account_id = release_data.account_id();
    instrument_id = release_data.instrument_id();
    side_code = release_data.side_code();
    tif = release_data.tif();
    release_status = release_data.release_status();
    release_qty = release_data.release_qty();
    release_executed_qty = release_data.executed_qty();
    release_average_prc = release_data.average_prc();
    limit_prc = release_data.limit_prc();
    stop_prc = release_data.stop_prc();

    quantmodel::QuantModelProcedures::INSERT_DEALING_RELEASE_AUDIT_EVENT(
        audit_msg,
        request_msg.request_id(),
        request_type,
        request_msg.timestamp(),
        response_msg.response_id(),
        response_type,
        response_msg.timestamp(),
        response_msg.error_msg(),
        investment_system_id,
        order_id,
        release_id,
        session_id,
        account_id,
        instrument_id,
        side_code,
        tif,
        limit_prc,
        stop_prc,
        release_status,
        release_qty,
        release_executed_qty,
        release_average_prc,
        ts,
        "DEALING_ENGINE" );
}

void auditExecution(
    const quantmodel::DealingEngineRequest& request_msg,
    const quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DatabaseEngineRequest& audit_msg,
    const quantmodel::ExecutionData& execution_data,
    const std::string& request_type,
    const std::string& response_type )
{
    std::string ts, order_id, release_id, execution_id, session_id, account_id,
                instrument_id, side_code, execution_status, execution_qty,
                execution_prc, investment_system_id;

    quantmodel::TimeStamp::getUTCTime( ts );

    investment_system_id = execution_data.investment_system_id();
    order_id = execution_data.order_id();
    release_id = execution_data.release_id();
    execution_id = execution_data.execution_id();
    session_id = execution_data.session_id();
    account_id = execution_data.account_id();
    instrument_id = execution_data.instrument_id();
    side_code = execution_data.side_code();
    execution_status = execution_data.execution_status();
    execution_qty = execution_data.execution_qty();
    execution_prc = execution_data.execution_prc();

    quantmodel::QuantModelProcedures::INSERT_DEALING_EXECUTION_AUDIT_EVENT(
        audit_msg,
        request_msg.request_id(),
        request_type,
        request_msg.timestamp(),
        response_msg.response_id(),
        response_type,
        response_msg.timestamp(),
        response_msg.error_msg(),
        investment_system_id,
        order_id,
        release_id,
        execution_id,
        session_id,
        account_id,
        instrument_id,
        side_code,
        execution_status,
        execution_qty,
        execution_prc,
        ts,
        "DEALING_ENGINE" );
}

void auditFix(
    const quantmodel::DealingEngineRequest& request_msg,
    const quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DatabaseEngineRequest& audit_msg,
    const std::string& request_type,
    const std::string& response_type )
{
    std::string ts;
    const quantmodel::FixEngineData& fix_engine_data = request_msg.fix_engine_data();
    quantmodel::TimeStamp::getUTCTime( ts );

    quantmodel::QuantModelProcedures::INSERT_DEALING_FIX_AUDIT_EVENT(
        audit_msg,
        request_msg.request_id(),
        request_type,
        request_msg.timestamp(),
        response_msg.response_id(),
        response_type,
        response_msg.timestamp(),
        response_msg.error_msg(),
        fix_engine_data.fix_engine_id(),
        fix_engine_data.session_id(),
        ts,
        "DEALING_ENGINE" );
}

const void dealing::RequestHandler::createAuditEvent(
    const quantmodel::DealingEngineRequest& request_msg,
    const quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DatabaseEngineRequest& audit_msg ) const
{
    std::string ts, response_type;
    static const quantmodel::OrderData empty_order_data;
    static const quantmodel::ReleaseData empty_release_data;
    static const quantmodel::ExecutionData empty_execution_data;

    if( response_msg.type() == quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT )
    {
        response_type = "ACKNOWLEDGEMENT";
    }
    else if( response_msg.type() == quantmodel::DealingEngineResponse::REJECT )
    {
        response_type = "REJECT";
    }
    else if( response_msg.type() == quantmodel::DealingEngineResponse::ERROR )
    {
        response_type = "ERROR";
    }
    else
    {
        response_type = "UNKNOWN";
    }

    if( request_msg.type() == quantmodel::DealingEngineRequest::EXECUTE_SIGNAL )
    {
        quantmodel::TimeStamp::getUTCTime( ts );
        quantmodel::QuantModelProcedures::INSERT_DEALING_AUDIT_EVENT(
            audit_msg,
            request_msg.request_id(),
            "EXECUTE_SIGNAL",
            request_msg.timestamp(),
            response_msg.response_id(),
            response_type,
            response_msg.timestamp(),
            response_msg.error_msg(),
            ts,
            "DEALING_ENGINE" );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::EXECUTE_DRAFT_ORDER )
    {
        quantmodel::TimeStamp::getUTCTime( ts );
        quantmodel::QuantModelProcedures::INSERT_DEALING_AUDIT_EVENT(
            audit_msg,
            request_msg.request_id(),
            "EXECUTE_DRAFT_ORDER",
            request_msg.timestamp(),
            response_msg.response_id(),
            response_type,
            response_msg.timestamp(),
            response_msg.error_msg(),
            ts,
            "DEALING_ENGINE" );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::CREATE_DRAFT_ORDER )
    {
        quantmodel::TimeStamp::getUTCTime( ts );
        quantmodel::QuantModelProcedures::INSERT_DEALING_AUDIT_EVENT(
            audit_msg,
            request_msg.request_id(),
            "CREATE_DRAFT_ORDER",
            request_msg.timestamp(),
            response_msg.response_id(),
            response_type,
            response_msg.timestamp(),
            response_msg.error_msg(),
            ts,
            "DEALING_ENGINE" );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::CREATE_ORDER )
    {
        if( response_msg.order_size( ) == 1 )
        {
            auditOrder( request_msg, response_msg, audit_msg,
                response_msg.order( 0 ).order_data(),
                    "CREATE_ORDER", response_type );
        }
        else
        {
            auditOrder( request_msg, response_msg, audit_msg,
                empty_order_data, "CREATE_ORDER", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::UPDATE_ORDER )
    {
        if( response_msg.order_size( ) == 1 )
        {
            auditOrder( request_msg, response_msg, audit_msg,
                response_msg.order( 0 ).order_data(),
                    "UPDATE_ORDER", response_type );
        }
        else
        {
            auditOrder( request_msg, response_msg, audit_msg,
                empty_order_data, "UPDATE_ORDER", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::CANCEL_ORDER )
    {
        if( response_msg.order_size( ) == 1 )
        {
            auditOrder( request_msg, response_msg, audit_msg,
                response_msg.order( 0 ).order_data(),
                    "CANCEL_ORDER", response_type );
        }
        else
        {
            auditOrder( request_msg, response_msg, audit_msg,
                empty_order_data, "CANCEL_ORDER", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::RELEASE_ORDER )
    {
        if( response_msg.order_size( ) == 1 )
        {
            auditOrder( request_msg, response_msg, audit_msg,
                response_msg.order( 0 ).order_data(),
                    "RELEASE_ORDER", response_type );
        }
        else
        {
            auditOrder( request_msg, response_msg, audit_msg,
                empty_order_data, "RELEASE_ORDER", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::CREATE_RELEASE )
    {
        if( response_msg.order_size( ) == 1 )
        {
            for( int i=0; i<response_msg.order( 0 ).release_data_size(); ++i )
            {
                auditRelease( request_msg, response_msg, audit_msg,
                    response_msg.order( 0 ).release_data( i ),
                        "CREATE_RELEASE", response_type );
            }
        }
        else
        {
            auditRelease( request_msg, response_msg, audit_msg,
                empty_release_data, "CREATE_RELEASE", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::VALIDATE_RELEASE )
    {
        if( response_msg.order_size( ) == 1 )
        {
            for( int i=0; i<response_msg.order( 0 ).release_data_size(); ++i )
            {
                auditRelease( request_msg, response_msg, audit_msg,
                    response_msg.order( 0 ).release_data( i ),
                        "VALIDATE_RELEASE", response_type );
            }
        }
        else
        {
            auditRelease( request_msg, response_msg, audit_msg,
                empty_release_data, "VALIDATE_RELEASE", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::CANCEL_RELEASE )
    {
        if( response_msg.order_size( ) == 1 )
        {
            for( int i=0; i<response_msg.order( 0 ).release_data_size(); ++i )
            {
                auditRelease( request_msg, response_msg, audit_msg,
                    response_msg.order( 0 ).release_data( i ),
                        "CANCEL_RELEASE", response_type );
            }
        }
        else
        {
            auditRelease( request_msg, response_msg, audit_msg,
                empty_release_data, "CANCEL_RELEASE", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::REPLACE_RELEASE )
    {
        if( response_msg.order_size( ) == 1 )
        {
            for( int i=0; i<response_msg.order( 0 ).release_data_size(); ++i )
            {
                auditRelease( request_msg, response_msg, audit_msg,
                    response_msg.order( 0 ).release_data( i ),
                        "REPLACE_RELEASE", response_type );
            }
        }
        else
        {
            auditRelease( request_msg, response_msg, audit_msg,
                empty_release_data, "REPLACE_RELEASE", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::RELEASE_REJECTED )
    {
        if( response_msg.order_size( ) == 1 )
        {
            for( int i=0; i<response_msg.order( 0 ).release_data_size(); ++i )
            {
                auditRelease( request_msg, response_msg, audit_msg,
                    response_msg.order( 0 ).release_data( i ),
                        "RELEASE_REJECTED", response_type );
            }
        }
        else
        {
            auditRelease( request_msg, response_msg, audit_msg,
                empty_release_data, "RELEASE_REJECTED", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::CANCEL_REJECTED )
    {
        if( response_msg.order_size( ) == 1 )
        {
            for( int i=0; i<response_msg.order( 0 ).release_data_size(); ++i )
            {
                auditRelease( request_msg, response_msg, audit_msg,
                    response_msg.order( 0 ).release_data( i ),
                        "CANCEL_REJECTED", response_type );
            }
        }
        else
        {
            auditRelease( request_msg, response_msg, audit_msg,
                empty_release_data, "CANCEL_REJECTED", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::RELEASE_RESTATED )
    {
        if( response_msg.order_size( ) == 1 )
        {
            for( int i=0; i<response_msg.order( 0 ).release_data_size(); ++i )
            {
                auditRelease( request_msg, response_msg, audit_msg,
                    response_msg.order( 0 ).release_data( i ),
                        "RELEASE_RESTATED", response_type );
            }
        }
        else
        {
            auditRelease( request_msg, response_msg, audit_msg,
                empty_release_data, "RELEASE_RESTATED", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::RELEASE_CANCELED )
    {
        if( response_msg.order_size( ) == 1 )
        {
            for( int i=0; i<response_msg.order( 0 ).release_data_size(); ++i )
            {
                auditRelease( request_msg, response_msg, audit_msg,
                    response_msg.order( 0 ).release_data( i ),
                        "RELEASE_CANCELED", response_type );
            }
        }
        else
        {
            auditRelease( request_msg, response_msg, audit_msg,
                empty_release_data, "RELEASE_CANCELED", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::RELEASE_REPLACED )
    {
        if( response_msg.order_size( ) == 1 )
        {
            for( int i=0; i<response_msg.order( 0 ).release_data_size(); ++i )
            {
                auditRelease( request_msg, response_msg, audit_msg,
                    response_msg.order( 0 ).release_data( i ),
                        "RELEASE_REPLACED", response_type );
            }
        }
        else
        {
            auditRelease( request_msg, response_msg, audit_msg,
                empty_release_data, "RELEASE_REPLACED", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::CREATE_EXECUTION )
    {
        if( response_msg.order_size( ) == 1 )
        {
            for( int i=0; i<response_msg.order( 0 ).execution_data_size(); ++i )
            {
                auditExecution( request_msg, response_msg, audit_msg,
                    response_msg.order( 0 ).execution_data( i ),
                        "CREATE_EXECUTION", response_type );
            }
        }
        else
        {
            auditExecution( request_msg, response_msg, audit_msg,
                empty_execution_data, "CREATE_EXECUTION", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::EXECUTION_CANCELED )
    {
        if( response_msg.order_size( ) == 1 )
        {
            for( int i=0; i<response_msg.order( 0 ).execution_data_size(); ++i )
            {
                auditExecution( request_msg, response_msg, audit_msg,
                    response_msg.order( 0 ).execution_data( i ),
                        "EXECUTION_CANCELED", response_type );
            }
        }
        else
        {
            auditExecution( request_msg, response_msg, audit_msg,
                empty_execution_data, "EXECUTION_CANCELED", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::QUERY_ORDER )
    {
        if( response_msg.order_size( ) == 1 )
        {
            auditOrder( request_msg, response_msg, audit_msg,
                response_msg.order( 0 ).order_data(),
                    "QUERY_ORDER", response_type );
        }
        else
        {
            auditOrder( request_msg, response_msg, audit_msg,
                empty_order_data, "QUERY_ORDER", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::QUERY_RELEASE )
    {
        if( response_msg.order_size( ) == 1 )
        {
            for( int i=0; i<response_msg.order( 0 ).release_data_size(); ++i )
            {
                auditRelease( request_msg, response_msg, audit_msg,
                    response_msg.order( 0 ).release_data( i ),
                        "QUERY_RELEASE", response_type );
            }
        }
        else
        {
            auditRelease( request_msg, response_msg, audit_msg,
                empty_release_data, "QUERY_RELEASE", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::QUERY_EXECUTION )
    {
        if( response_msg.order_size( ) == 1 )
        {
            for( int i=0; i<response_msg.order( 0 ).execution_data_size(); ++i )
            {
                auditExecution( request_msg, response_msg, audit_msg,
                    response_msg.order( 0 ).execution_data( i ),
                        "QUERY_EXECUTION", response_type );
            }
        }
        else
        {
            auditExecution( request_msg, response_msg, audit_msg,
                empty_execution_data, "QUERY_EXECUTION", response_type );
        }
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::FIXENGINE_LOGIN )
    {
        auditFix( request_msg, response_msg, audit_msg, "FIXENGINE_LOGIN", response_type );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::FIXENGINE_LOGOUT )
    {
        auditFix( request_msg, response_msg, audit_msg, "FIXENGINE_LOGOUT", response_type );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::LOGIN_FIXENGINE )
    {
        auditFix( request_msg, response_msg, audit_msg, "LOGIN_FIXENGINE", response_type );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::LOGOUT_FIXENGINE )
    {
        auditFix( request_msg, response_msg, audit_msg, "LOGOUT_FIXENGINE", response_type );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::ACTIVE_SET )
    {
        quantmodel::TimeStamp::getUTCTime( ts );
        quantmodel::QuantModelProcedures::INSERT_DEALING_AUDIT_EVENT(
            audit_msg,
            request_msg.request_id(),
            "ACTIVE_SET",
            request_msg.timestamp(),
            response_msg.response_id(),
            response_type,
            response_msg.timestamp(),
            response_msg.error_msg(),
            ts,
            "DEALING_ENGINE" );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::BROADCAST_DATA )
    {
        quantmodel::TimeStamp::getUTCTime( ts );
        quantmodel::QuantModelProcedures::INSERT_DEALING_AUDIT_EVENT(
            audit_msg,
            request_msg.request_id(),
            "BROADCAST_DATA",
            request_msg.timestamp(),
            response_msg.response_id(),
            response_type,
            response_msg.timestamp(),
            response_msg.error_msg(),
            ts,
            "DEALING_ENGINE" );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::ROLLOVER )
    {
        quantmodel::TimeStamp::getUTCTime( ts );
        quantmodel::QuantModelProcedures::INSERT_DEALING_AUDIT_EVENT(
            audit_msg,
            request_msg.request_id(),
            "ROLLOVER",
            request_msg.timestamp(),
            response_msg.response_id(),
            response_type,
            response_msg.timestamp(),
            response_msg.error_msg(),
            ts,
            "DEALING_ENGINE" );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::CURRENT_STATUS )
    {
        quantmodel::TimeStamp::getUTCTime( ts );
        quantmodel::QuantModelProcedures::INSERT_DEALING_AUDIT_EVENT(
            audit_msg,
            request_msg.request_id(),
            "CURRENT_STATUS",
            request_msg.timestamp(),
            response_msg.response_id(),
            response_type,
            response_msg.timestamp(),
            response_msg.error_msg(),
            ts,
            "DEALING_ENGINE" );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::UPDATE_STATUS )
    {
        quantmodel::TimeStamp::getUTCTime( ts );
        quantmodel::QuantModelProcedures::INSERT_DEALING_AUDIT_EVENT(
            audit_msg,
            request_msg.request_id(),
            "UPDATE_STATUS",
            request_msg.timestamp(),
            response_msg.response_id(),
            response_type,
            response_msg.timestamp(),
            response_msg.error_msg(),
            ts,
            "DEALING_ENGINE" );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::MARKETDATA_SUBSCRIBE )
    {
        quantmodel::TimeStamp::getUTCTime( ts );
        quantmodel::QuantModelProcedures::INSERT_DEALING_AUDIT_EVENT(
            audit_msg,
            request_msg.request_id(),
            "MARKETDATA_SUBSCRIBE",
            request_msg.timestamp(),
            response_msg.response_id(),
            response_type,
            response_msg.timestamp(),
            response_msg.error_msg(),
            ts,
            "DEALING_ENGINE" );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::MARKETDATA_UNSUBSCRIBE )
    {
        quantmodel::TimeStamp::getUTCTime( ts );
        quantmodel::QuantModelProcedures::INSERT_DEALING_AUDIT_EVENT(
            audit_msg,
            request_msg.request_id(),
            "MARKETDATA_UNSUBSCRIBE",
            request_msg.timestamp(),
            response_msg.response_id(),
            response_type,
            response_msg.timestamp(),
            response_msg.error_msg(),
            ts,
            "DEALING_ENGINE" );
    }
    else if( request_msg.type() == quantmodel::DealingEngineRequest::RELOAD_STATIC_DATA )
    {
        quantmodel::TimeStamp::getUTCTime( ts );
        quantmodel::QuantModelProcedures::INSERT_DEALING_AUDIT_EVENT(
            audit_msg,
            request_msg.request_id(),
            "RELOAD_STATIC_DATA",
            request_msg.timestamp(),
            response_msg.response_id(),
            response_type,
            response_msg.timestamp(),
            response_msg.error_msg(),
            ts,
            "DEALING_ENGINE" );
    }
    else
    {
        log->warn( "UNKNOWN AUDIT REQUEST TYPE." );
    }
}
