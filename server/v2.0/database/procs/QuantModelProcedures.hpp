/*
 QuantModelProcedures.hpp auto generated on:
 Mon May 21 09:57:27 CDT 2012
 DO NOT EDIT
*/

#ifndef QUANTMODEL_PROCEDURES_H
#define QUANTMODEL_PROCEDURES_H

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

namespace quantmodel
{
  class QuantModelProcedures
  {
    public:

    const inline static void GET_DEALING_EVENTS_BY_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_order_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_DEALING_EVENTS_BY_ORDER_ID( " << "'" << p_order_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_DEALING_EVENTS_BY_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_FIX_EVENTS_BY_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_order_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_FIX_EVENTS_BY_ORDER_ID( " << "'" << p_order_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_FIX_EVENTS_BY_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ORDERS_FROM_ADDED_DATETIME( quantmodel::DatabaseEngineRequest& request, const string& p_added_datetime ) 
    {
        std::ostringstream sql;
        sql << "call GET_ORDERS_FROM_ADDED_DATETIME( " << "'" << p_added_datetime << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ORDERS_FROM_ADDED_DATETIME" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void INSERT_AUDIT_EVENT( quantmodel::DatabaseEngineRequest& request, const string& p_order_id, const string& p_element_id, const string& p_element_type, const string& p_audit_desc, const string& p_added_datetime, const string& p_added_by, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_AUDIT_EVENT( " << "'" << p_order_id << "'," << "'" << p_element_id << "'," << "'" << p_element_type << "'," << "'" << p_audit_desc << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_AUDIT_EVENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_COMPLIANCE_AUDIT_EVENT( quantmodel::DatabaseEngineRequest& request, const string& p_compliance_audit_id, const string& p_session_id, const string& p_account_id, const string& p_instrument_id, const string& p_order_id, const string& p_allocation_id, const string& p_release_id, const string& p_side_code, const string& p_release_qty, const string& p_worst_case_long_qty, const string& p_worst_case_short_qty, const string& p_curr_open_long_qty, const string& p_curr_open_short_qty, const string& p_max_open_long_qty, const string& p_max_open_short_qty, const string& p_max_release_qty, const string& p_result_code, const string& p_added_datetime, const string& p_added_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_COMPLIANCE_AUDIT_EVENT( " << p_compliance_audit_id << "," << "'" << p_session_id << "'," << p_account_id << "," << p_instrument_id << "," << "'" << p_order_id << "'," << "'" << p_allocation_id << "'," << "'" << p_release_id << "'," << p_side_code << "," << "'" << p_release_qty << "'," << p_worst_case_long_qty << "," << p_worst_case_short_qty << "," << p_curr_open_long_qty << "," << p_curr_open_short_qty << "," << p_max_open_long_qty << "," << p_max_open_short_qty << "," << p_max_release_qty << "," << "'" << p_result_code << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_COMPLIANCE_AUDIT_EVENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_DEALING_AUDIT_EVENT( quantmodel::DatabaseEngineRequest& request, const string& p_request_id, const string& p_request_type, const string& p_request_ts, const string& p_response_id, const string& p_response_type, const string& p_response_ts, const string& p_error_msg, const string& p_added_datetime, const string& p_added_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_DEALING_AUDIT_EVENT( " << "'" << p_request_id << "'," << "'" << p_request_type << "'," << "'" << p_request_ts << "'," << "'" << p_response_id << "'," << "'" << p_response_type << "'," << "'" << p_response_ts << "'," << "'" << p_error_msg << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_DEALING_AUDIT_EVENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_DEALING_EXECUTION_AUDIT_EVENT( quantmodel::DatabaseEngineRequest& request, const string& p_request_id, const string& p_request_type, const string& p_request_ts, const string& p_response_id, const string& p_response_type, const string& p_response_ts, const string& p_error_msg, const string& p_investment_system_id, const string& p_order_id, const string& p_release_id, const string& p_execution_id, const string& p_session_id, const string& p_account_id, const string& p_instrument_id, const string& p_side_code, const string& p_execution_status, const string& p_execution_qty, const string& p_execution_prc, const string& p_added_datetime, const string& p_added_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_DEALING_EXECUTION_AUDIT_EVENT( " << "'" << p_request_id << "'," << "'" << p_request_type << "'," << "'" << p_request_ts << "'," << "'" << p_response_id << "'," << "'" << p_response_type << "'," << "'" << p_response_ts << "'," << "'" << p_error_msg << "'," << "'" << p_investment_system_id << "'," << "'" << p_order_id << "'," << "'" << p_release_id << "'," << "'" << p_execution_id << "'," << "'" << p_session_id << "'," << "'" << p_account_id << "'," << "'" << p_instrument_id << "'," << "'" << p_side_code << "'," << "'" << p_execution_status << "'," << "'" << p_execution_qty << "'," << "'" << p_execution_prc << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_DEALING_EXECUTION_AUDIT_EVENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_DEALING_FIX_AUDIT_EVENT( quantmodel::DatabaseEngineRequest& request, const string& p_request_id, const string& p_request_type, const string& p_request_ts, const string& p_response_id, const string& p_response_type, const string& p_response_ts, const string& p_error_msg, const string& p_fix_engine_id, const string& p_session_id, const string& p_added_datetime, const string& p_added_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_DEALING_FIX_AUDIT_EVENT( " << "'" << p_request_id << "'," << "'" << p_request_type << "'," << "'" << p_request_ts << "'," << "'" << p_response_id << "'," << "'" << p_response_type << "'," << "'" << p_response_ts << "'," << "'" << p_error_msg << "'," << "'" << p_fix_engine_id << "'," << "'" << p_session_id << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_DEALING_FIX_AUDIT_EVENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_DEALING_ORDER_AUDIT_EVENT( quantmodel::DatabaseEngineRequest& request, const string& p_request_id, const string& p_request_type, const string& p_request_ts, const string& p_response_id, const string& p_response_type, const string& p_response_ts, const string& p_error_msg, const string& p_investment_system_id, const string& p_order_id, const string& p_instrument_id, const string& p_side_code, const string& p_tif, const string& p_limit_prc, const string& p_stop_prc, const string& p_order_status, const string& p_order_qty, const string& p_order_released_qty, const string& p_order_executed_qty, const string& p_order_average_prc, const string& p_added_datetime, const string& p_added_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_DEALING_ORDER_AUDIT_EVENT( " << "'" << p_request_id << "'," << "'" << p_request_type << "'," << "'" << p_request_ts << "'," << "'" << p_response_id << "'," << "'" << p_response_type << "'," << "'" << p_response_ts << "'," << "'" << p_error_msg << "'," << "'" << p_investment_system_id << "'," << "'" << p_order_id << "'," << "'" << p_instrument_id << "'," << "'" << p_side_code << "'," << "'" << p_tif << "'," << "'" << p_limit_prc << "'," << "'" << p_stop_prc << "'," << "'" << p_order_status << "'," << "'" << p_order_qty << "'," << "'" << p_order_released_qty << "'," << "'" << p_order_executed_qty << "'," << "'" << p_order_average_prc << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_DEALING_ORDER_AUDIT_EVENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_DEALING_RELEASE_AUDIT_EVENT( quantmodel::DatabaseEngineRequest& request, const string& p_request_id, const string& p_request_type, const string& p_request_ts, const string& p_response_id, const string& p_response_type, const string& p_response_ts, const string& p_error_msg, const string& p_investment_system_id, const string& p_order_id, const string& p_release_id, const string& p_session_id, const string& p_account_id, const string& p_instrument_id, const string& p_side_code, const string& p_tif, const string& p_limit_prc, const string& p_stop_prc, const string& p_release_status, const string& p_release_qty, const string& p_release_executed_qty, const string& p_release_average_prc, const string& p_added_datetime, const string& p_added_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_DEALING_RELEASE_AUDIT_EVENT( " << "'" << p_request_id << "'," << "'" << p_request_type << "'," << "'" << p_request_ts << "'," << "'" << p_response_id << "'," << "'" << p_response_type << "'," << "'" << p_response_ts << "'," << "'" << p_error_msg << "'," << "'" << p_investment_system_id << "'," << "'" << p_order_id << "'," << "'" << p_release_id << "'," << "'" << p_session_id << "'," << "'" << p_account_id << "'," << "'" << p_instrument_id << "'," << "'" << p_side_code << "'," << "'" << p_tif << "'," << "'" << p_limit_prc << "'," << "'" << p_stop_prc << "'," << "'" << p_release_status << "'," << "'" << p_release_qty << "'," << "'" << p_release_executed_qty << "'," << "'" << p_release_average_prc << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_DEALING_RELEASE_AUDIT_EVENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_FIX_AUDIT_EVENT( quantmodel::DatabaseEngineRequest& request, const string& p_session_id, const string& p_message_direction, const string& p_account, const string& p_avgpx, const string& p_clordid, const string& p_cumqty, const string& p_execid, const string& p_execrefid, const string& p_exectranstype, const string& p_lastpx, const string& p_lastshares, const string& p_msgtype, const string& p_orderid, const string& p_orderqty, const string& p_ordstatus, const string& p_ordtype, const string& p_origclordid, const string& p_price, const string& p_securityid, const string& p_sendercompid, const string& p_sendersubid, const string& p_sendingtime, const string& p_shares, const string& p_side, const string& p_symbol, const string& p_targetcompid, const string& p_targetsubid, const string& p_text, const string& p_timeinforce, const string& p_transacttime, const string& p_stoppx, const string& p_senderlocationid, const string& p_exectype, const string& p_leavesqty, const string& p_securitytype, const string& p_maturitymonthyear, const string& p_customerorfirm, const string& p_securityexchange, const string& p_maxshow, const string& p_manualorderindicator, const string& p_cticode, const string& p_correlationclordid, const string& p_added_datetime, const string& p_added_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_FIX_AUDIT_EVENT( " << "'" << p_session_id << "'," << "'" << p_message_direction << "'," << "'" << p_account << "'," << "'" << p_avgpx << "'," << "'" << p_clordid << "'," << "'" << p_cumqty << "'," << "'" << p_execid << "'," << "'" << p_execrefid << "'," << "'" << p_exectranstype << "'," << "'" << p_lastpx << "'," << "'" << p_lastshares << "'," << "'" << p_msgtype << "'," << "'" << p_orderid << "'," << "'" << p_orderqty << "'," << "'" << p_ordstatus << "'," << "'" << p_ordtype << "'," << "'" << p_origclordid << "'," << "'" << p_price << "'," << "'" << p_securityid << "'," << "'" << p_sendercompid << "'," << "'" << p_sendersubid << "'," << "'" << p_sendingtime << "'," << "'" << p_shares << "'," << "'" << p_side << "'," << "'" << p_symbol << "'," << "'" << p_targetcompid << "'," << "'" << p_targetsubid << "'," << "'" << p_text << "'," << "'" << p_timeinforce << "'," << "'" << p_transacttime << "'," << "'" << p_stoppx << "'," << "'" << p_senderlocationid << "'," << "'" << p_exectype << "'," << "'" << p_leavesqty << "'," << "'" << p_securitytype << "'," << "'" << p_maturitymonthyear << "'," << "'" << p_customerorfirm << "'," << "'" << p_securityexchange << "'," << "'" << p_maxshow << "'," << "'" << p_manualorderindicator << "'," << "'" << p_cticode << "'," << "'" << p_correlationclordid << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_FIX_AUDIT_EVENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void GET_ACCOUNT_LIMITS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACCOUNT_LIMITS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACCOUNT_LIMITS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ACCOUNT_LIMITS_BY_ACCOUNT_ID( quantmodel::DatabaseEngineRequest& request, const string& p_account_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACCOUNT_LIMITS_BY_ACCOUNT_ID( " << p_account_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACCOUNT_LIMITS_BY_ACCOUNT_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ACCOUNT_LIMIT_BY_ACCOUNT_INSTRUMENT_ID( quantmodel::DatabaseEngineRequest& request, const string& p_account_id, const string& p_instrument_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACCOUNT_LIMIT_BY_ACCOUNT_INSTRUMENT_ID( " << p_account_id << "," << p_instrument_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACCOUNT_LIMIT_BY_ACCOUNT_INSTRUMENT_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void INSERT_ACCOUNT_LIMIT( quantmodel::DatabaseEngineRequest& request, const string& p_account_id, const string& p_symbol, const string& p_security_exchange, const string& p_session_id, const string& p_max_open_long_qty, const string& p_max_open_short_qty, const string& p_max_release_qty, const string& p_is_active, const string& p_added_datetime, const string& p_added_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_ACCOUNT_LIMIT( " << p_account_id << "," << "'" << p_symbol << "'," << "'" << p_security_exchange << "'," << "'" << p_session_id << "'," << p_max_open_long_qty << "," << p_max_open_short_qty << "," << p_max_release_qty << "," << p_is_active << "," << "'" << p_added_datetime << "'," << "'" << p_added_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_ACCOUNT_LIMIT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_ACCOUNT_LIMIT( quantmodel::DatabaseEngineRequest& request, const string& p_account_id, const string& p_symbol, const string& p_security_exchange, const string& p_session_id, const string& p_max_open_long_qty, const string& p_max_open_short_qty, const string& p_max_release_qty, const string& p_is_active, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_ACCOUNT_LIMIT( " << p_account_id << "," << "'" << p_symbol << "'," << "'" << p_security_exchange << "'," << "'" << p_session_id << "'," << p_max_open_long_qty << "," << p_max_open_short_qty << "," << p_max_release_qty << "," << p_is_active << "," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_ACCOUNT_LIMIT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_ACCOUNT_LIMIT_SESSION_IDS( quantmodel::DatabaseEngineRequest& request, const string& p_account_id, const string& p_session_id, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_ACCOUNT_LIMIT_SESSION_IDS( " << p_account_id << "," << "'" << p_session_id << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_ACCOUNT_LIMIT_SESSION_IDS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void CLEARDOWN( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call CLEARDOWN";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "CLEARDOWN" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void DELETE_POSITION_OVERNIGHT( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call DELETE_POSITION_OVERNIGHT";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "DELETE_POSITION_OVERNIGHT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void GET_ACTIVE_INSTRUMENTS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACTIVE_INSTRUMENTS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACTIVE_INSTRUMENTS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ACTIVE_INSTRUMENTS_BY_SYMBOL( quantmodel::DatabaseEngineRequest& request, const string& p_symbol ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACTIVE_INSTRUMENTS_BY_SYMBOL( " << "'" << p_symbol << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACTIVE_INSTRUMENTS_BY_SYMBOL" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ACTIVE_INSTRUMENT_IDS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACTIVE_INSTRUMENT_IDS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACTIVE_INSTRUMENT_IDS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ALLOCATIONS_BY_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_order_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_ALLOCATIONS_BY_ORDER_ID( " << "'" << p_order_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ALLOCATIONS_BY_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ALLOCATION_BY_ALLOCATION_ID( quantmodel::DatabaseEngineRequest& request, const string& p_allocation_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_ALLOCATION_BY_ALLOCATION_ID( " << "'" << p_allocation_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ALLOCATION_BY_ALLOCATION_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_EXECUTIONS_BY_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_order_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_EXECUTIONS_BY_ORDER_ID( " << "'" << p_order_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_EXECUTIONS_BY_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_EXECUTION_BY_EXECUTION_ID( quantmodel::DatabaseEngineRequest& request, const string& p_execution_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_EXECUTION_BY_EXECUTION_ID( " << "'" << p_execution_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_EXECUTION_BY_EXECUTION_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_FIX_ENGINE_BY_ACCOUNT_INSTRUMENT_ID( quantmodel::DatabaseEngineRequest& request, const string& p_account_id, const string& p_instrument_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_FIX_ENGINE_BY_ACCOUNT_INSTRUMENT_ID( " << p_account_id << "," << p_instrument_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_FIX_ENGINE_BY_ACCOUNT_INSTRUMENT_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_FIX_ENGINE_BY_SESSION_ID( quantmodel::DatabaseEngineRequest& request, const string& p_session_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_FIX_ENGINE_BY_SESSION_ID( " << "'" << p_session_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_FIX_ENGINE_BY_SESSION_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INSTRUMENTS_BY_INSTRUMENT_TYPE( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_type ) 
    {
        std::ostringstream sql;
        sql << "call GET_INSTRUMENTS_BY_INSTRUMENT_TYPE( " << p_instrument_type << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INSTRUMENTS_BY_INSTRUMENT_TYPE" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INSTRUMENTS_BY_INVESTMENT_SYSTEM( quantmodel::DatabaseEngineRequest& request, const string& p_investment_system_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_INSTRUMENTS_BY_INVESTMENT_SYSTEM( " << "'" << p_investment_system_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INSTRUMENTS_BY_INVESTMENT_SYSTEM" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INSTRUMENTS_BY_SYMBOL( quantmodel::DatabaseEngineRequest& request, const string& p_symbol ) 
    {
        std::ostringstream sql;
        sql << "call GET_INSTRUMENTS_BY_SYMBOL( " << "'" << p_symbol << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INSTRUMENTS_BY_SYMBOL" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INSTRUMENTS_BY_SYMBOL_MATURITY_MONTH_YEAR( quantmodel::DatabaseEngineRequest& request, const string& p_symbol, const string& p_maturity_month_year ) 
    {
        std::ostringstream sql;
        sql << "call GET_INSTRUMENTS_BY_SYMBOL_MATURITY_MONTH_YEAR( " << "'" << p_symbol << "'," << "'" << p_maturity_month_year << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INSTRUMENTS_BY_SYMBOL_MATURITY_MONTH_YEAR" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INSTRUMENT_BY_EXCHANGE_SECURITY_ID( quantmodel::DatabaseEngineRequest& request, const string& p_security_exchange, const string& p_security_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_INSTRUMENT_BY_EXCHANGE_SECURITY_ID( " << "'" << p_security_exchange << "'," << "'" << p_security_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INSTRUMENT_BY_EXCHANGE_SECURITY_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INSTRUMENT_BY_FIX_ENGINE_ID( quantmodel::DatabaseEngineRequest& request, const string& p_fix_engine_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_INSTRUMENT_BY_FIX_ENGINE_ID( " << "'" << p_fix_engine_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INSTRUMENT_BY_FIX_ENGINE_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INSTRUMENT_BY_INSTRUMENT_ID( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_INSTRUMENT_BY_INSTRUMENT_ID( " << p_instrument_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INSTRUMENT_BY_INSTRUMENT_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INSTRUMENT_SYMBOLS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_INSTRUMENT_SYMBOLS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INSTRUMENT_SYMBOLS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_OPEN_EXECUTIONS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_OPEN_EXECUTIONS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_OPEN_EXECUTIONS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_OPEN_ORDERS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_OPEN_ORDERS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_OPEN_ORDERS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_OPEN_RELEASES( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_OPEN_RELEASES";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_OPEN_RELEASES" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ORDER_BY_EXTERNAL_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_external_order_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_ORDER_BY_EXTERNAL_ORDER_ID( " << "'" << p_external_order_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ORDER_BY_EXTERNAL_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ORDER_BY_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_order_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_ORDER_BY_ORDER_ID( " << "'" << p_order_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ORDER_BY_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_OVERNIGHT_POSITIONS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_OVERNIGHT_POSITIONS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_OVERNIGHT_POSITIONS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_RELEASES_BY_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_order_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_RELEASES_BY_ORDER_ID( " << "'" << p_order_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_RELEASES_BY_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_RELEASE_BY_RELEASE_ID( quantmodel::DatabaseEngineRequest& request, const string& p_release_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_RELEASE_BY_RELEASE_ID( " << "'" << p_release_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_RELEASE_BY_RELEASE_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void INSERT_ALLOCATION( quantmodel::DatabaseEngineRequest& request, const string& p_id, const string& p_allocation_id, const string& p_order_id, const string& p_account_id, const string& p_instrument_id, const string& p_side_code, const string& p_allocation_qty, const string& p_released_qty, const string& p_executed_qty, const string& p_executed_val, const string& p_average_prc, const string& p_added_datetime, const string& p_added_by, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_ALLOCATION( " << p_id << "," << "'" << p_allocation_id << "'," << "'" << p_order_id << "'," << p_account_id << "," << p_instrument_id << "," << p_side_code << "," << "'" << p_allocation_qty << "'," << "'" << p_released_qty << "'," << "'" << p_executed_qty << "'," << "'" << p_executed_val << "'," << "'" << p_average_prc << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_ALLOCATION" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_EXECUTION( quantmodel::DatabaseEngineRequest& request, const string& p_id, const string& p_execution_id, const string& p_session_id, const string& p_account_id, const string& p_order_id, const string& p_release_id, const string& p_investment_system_id, const string& p_instrument_id, const string& p_client_order_id, const string& p_counterparty_execution_id, const string& p_side_code, const string& p_execution_ccy, const string& p_execution_status, const string& p_execution_prc, const string& p_execution_qty, const string& p_execution_val, const string& p_leaves_qty, const string& p_average_prc, const string& p_leg_type, const string& p_added_datetime, const string& p_added_by, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_EXECUTION( " << p_id << "," << "'" << p_execution_id << "'," << "'" << p_session_id << "'," << p_account_id << "," << "'" << p_order_id << "'," << "'" << p_release_id << "'," << "'" << p_investment_system_id << "'," << p_instrument_id << "," << "'" << p_client_order_id << "'," << "'" << p_counterparty_execution_id << "'," << p_side_code << "," << "'" << p_execution_ccy << "'," << "'" << p_execution_status << "'," << "'" << p_execution_prc << "'," << "'" << p_execution_qty << "'," << "'" << p_execution_val << "'," << "'" << p_leaves_qty << "'," << "'" << p_average_prc << "'," << p_leg_type << "," << "'" << p_added_datetime << "'," << "'" << p_added_by << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_EXECUTION" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_ORDER( quantmodel::DatabaseEngineRequest& request, const string& p_id, const string& p_order_id, const string& p_external_order_id, const string& p_investment_system_id, const string& p_order_type, const string& p_order_status, const string& p_instrument_id, const string& p_symbol, const string& p_tif, const string& p_side_code, const string& p_base_ccy, const string& p_order_qty, const string& p_open_qty, const string& p_released_qty, const string& p_executed_qty, const string& p_limit_prc, const string& p_stop_prc, const string& p_mark_prc, const string& p_average_prc, const string& p_entry_prc, const string& p_released_val, const string& p_executed_val, const string& p_added_datetime, const string& p_added_by, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_ORDER( " << p_id << "," << "'" << p_order_id << "'," << "'" << p_external_order_id << "'," << "'" << p_investment_system_id << "'," << p_order_type << "," << p_order_status << "," << p_instrument_id << "," << "'" << p_symbol << "'," << p_tif << "," << p_side_code << "," << "'" << p_base_ccy << "'," << "'" << p_order_qty << "'," << "'" << p_open_qty << "'," << "'" << p_released_qty << "'," << "'" << p_executed_qty << "'," << "'" << p_limit_prc << "'," << "'" << p_stop_prc << "'," << "'" << p_mark_prc << "'," << "'" << p_average_prc << "'," << "'" << p_entry_prc << "'," << "'" << p_released_val << "'," << "'" << p_executed_val << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_ORDER" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_POSITION_OVERNIGHT( quantmodel::DatabaseEngineRequest& request, const string& p_account_id, const string& p_instrument_id, const string& p_investment_system_id, const string& p_position_type, const string& p_open_pos, const string& p_open_prc, const string& p_added_datetime, const string& p_added_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_POSITION_OVERNIGHT( " << p_account_id << "," << p_instrument_id << "," << "'" << p_investment_system_id << "'," << p_position_type << "," << "'" << p_open_pos << "'," << "'" << p_open_prc << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_POSITION_OVERNIGHT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_RELEASE( quantmodel::DatabaseEngineRequest& request, const string& p_id, const string& p_release_id, const string& p_session_id, const string& p_account_id, const string& p_order_id, const string& p_allocation_id, const string& p_investment_system_id, const string& p_instrument_id, const string& p_compliance_audit_id, const string& p_client_order_id, const string& p_counterparty_order_id, const string& p_release_status, const string& p_release_type, const string& p_tif, const string& p_side_code, const string& p_release_qty, const string& p_limit_prc, const string& p_stop_prc, const string& p_average_prc, const string& p_executed_val, const string& p_executed_qty, const string& p_added_datetime, const string& p_added_by, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_RELEASE( " << p_id << "," << "'" << p_release_id << "'," << "'" << p_session_id << "'," << p_account_id << "," << "'" << p_order_id << "'," << "'" << p_allocation_id << "'," << "'" << p_investment_system_id << "'," << p_instrument_id << "," << "'" << p_compliance_audit_id << "'," << "'" << p_client_order_id << "'," << "'" << p_counterparty_order_id << "'," << p_release_status << "," << p_release_type << "," << p_tif << "," << p_side_code << "," << "'" << p_release_qty << "'," << "'" << p_limit_prc << "'," << "'" << p_stop_prc << "'," << "'" << p_average_prc << "'," << "'" << p_executed_val << "'," << "'" << p_executed_qty << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_RELEASE" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void MARK_INSTRUMENT( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id, const string& p_mark_prc, const string& p_mark_datetime, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call MARK_INSTRUMENT( " << p_instrument_id << "," << "'" << p_mark_prc << "'," << "'" << p_mark_datetime << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "MARK_INSTRUMENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_ALLOCATION( quantmodel::DatabaseEngineRequest& request, const string& p_allocation_id, const string& p_order_id, const string& p_account_id, const string& p_instrument_id, const string& p_side_code, const string& p_allocation_qty, const string& p_released_qty, const string& p_executed_qty, const string& p_executed_val, const string& p_average_prc, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_ALLOCATION( " << "'" << p_allocation_id << "'," << "'" << p_order_id << "'," << p_account_id << "," << p_instrument_id << "," << p_side_code << "," << "'" << p_allocation_qty << "'," << "'" << p_released_qty << "'," << "'" << p_executed_qty << "'," << "'" << p_executed_val << "'," << "'" << p_average_prc << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_ALLOCATION" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_EXECUTION( quantmodel::DatabaseEngineRequest& request, const string& p_execution_id, const string& p_session_id, const string& p_account_id, const string& p_order_id, const string& p_release_id, const string& p_investment_system_id, const string& p_instrument_id, const string& p_client_order_id, const string& p_counterparty_execution_id, const string& p_side_code, const string& p_execution_ccy, const string& p_execution_status, const string& p_execution_prc, const string& p_execution_qty, const string& p_execution_val, const string& p_leaves_qty, const string& p_average_prc, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_EXECUTION( " << "'" << p_execution_id << "'," << "'" << p_session_id << "'," << p_account_id << "," << "'" << p_order_id << "'," << "'" << p_release_id << "'," << "'" << p_investment_system_id << "'," << p_instrument_id << "," << "'" << p_client_order_id << "'," << "'" << p_counterparty_execution_id << "'," << p_side_code << "," << "'" << p_execution_ccy << "'," << "'" << p_execution_status << "'," << "'" << p_execution_prc << "'," << "'" << p_execution_qty << "'," << "'" << p_execution_val << "'," << "'" << p_leaves_qty << "'," << "'" << p_average_prc << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_EXECUTION" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_INSTRUMENT( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id, const string& p_instrument_type, const string& p_name, const string& p_security_id, const string& p_symbol, const string& p_cusip, const string& p_sedol, const string& p_isin, const string& p_ric, const string& p_security_type, const string& p_security_desc, const string& p_security_exchange, const string& p_maturity_month_year, const string& p_base_ccy, const string& p_last_prc, const string& p_multiplier, const string& p_exchange_price_factor, const string& p_is_active, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_INSTRUMENT( " << p_instrument_id << "," << p_instrument_type << "," << "'" << p_name << "'," << "'" << p_security_id << "'," << "'" << p_symbol << "'," << "'" << p_cusip << "'," << "'" << p_sedol << "'," << "'" << p_isin << "'," << "'" << p_ric << "'," << "'" << p_security_type << "'," << "'" << p_security_desc << "'," << "'" << p_security_exchange << "'," << "'" << p_maturity_month_year << "'," << "'" << p_base_ccy << "'," << "'" << p_last_prc << "'," << "'" << p_multiplier << "'," << "'" << p_exchange_price_factor << "'," << p_is_active << "," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_INSTRUMENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_INSTRUMENT_LAST_PRICE( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id, const string& p_last_prc, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_INSTRUMENT_LAST_PRICE( " << p_instrument_id << "," << "'" << p_last_prc << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_INSTRUMENT_LAST_PRICE" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_ORDER( quantmodel::DatabaseEngineRequest& request, const string& p_order_id, const string& p_order_type, const string& p_order_status, const string& p_instrument_id, const string& p_symbol, const string& p_tif, const string& p_side_code, const string& p_base_ccy, const string& p_order_qty, const string& p_open_qty, const string& p_released_qty, const string& p_executed_qty, const string& p_limit_prc, const string& p_stop_prc, const string& p_mark_prc, const string& p_average_prc, const string& p_released_val, const string& p_executed_val, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_ORDER( " << "'" << p_order_id << "'," << p_order_type << "," << p_order_status << "," << p_instrument_id << "," << "'" << p_symbol << "'," << p_tif << "," << p_side_code << "," << "'" << p_base_ccy << "'," << "'" << p_order_qty << "'," << "'" << p_open_qty << "'," << "'" << p_released_qty << "'," << "'" << p_executed_qty << "'," << "'" << p_limit_prc << "'," << "'" << p_stop_prc << "'," << "'" << p_mark_prc << "'," << "'" << p_average_prc << "'," << "'" << p_released_val << "'," << "'" << p_executed_val << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_ORDER" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_RELEASE( quantmodel::DatabaseEngineRequest& request, const string& p_release_id, const string& p_session_id, const string& p_account_id, const string& p_order_id, const string& p_investment_system_id, const string& p_instrument_id, const string& p_client_order_id, const string& p_counterparty_order_id, const string& p_release_status, const string& p_release_type, const string& p_tif, const string& p_side_code, const string& p_release_qty, const string& p_limit_prc, const string& p_average_prc, const string& p_executed_val, const string& p_executed_qty, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_RELEASE( " << "'" << p_release_id << "'," << "'" << p_session_id << "'," << p_account_id << "," << "'" << p_order_id << "'," << "'" << p_investment_system_id << "'," << p_instrument_id << "," << "'" << p_client_order_id << "'," << "'" << p_counterparty_order_id << "'," << p_release_status << "," << p_release_type << "," << p_tif << "," << p_side_code << "," << "'" << p_release_qty << "'," << "'" << p_limit_prc << "'," << "'" << p_average_prc << "'," << "'" << p_executed_val << "'," << "'" << p_executed_qty << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_RELEASE" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void CLEAR_MARKET_DATA_MAP( quantmodel::DatabaseEngineRequest& request, const string& p_session_id, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call CLEAR_MARKET_DATA_MAP( " << "'" << p_session_id << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "CLEAR_MARKET_DATA_MAP" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void DELETE_CLIENT_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_client_order_id, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call DELETE_CLIENT_ORDER_ID( " << "'" << p_client_order_id << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "DELETE_CLIENT_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void DELETE_CLIENT_ORDER_ID_BY_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_order_id, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call DELETE_CLIENT_ORDER_ID_BY_ORDER_ID( " << "'" << p_order_id << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "DELETE_CLIENT_ORDER_ID_BY_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void DELETE_MARKET_DATA_MAP( quantmodel::DatabaseEngineRequest& request, const string& p_market_data_map_id, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call DELETE_MARKET_DATA_MAP( " << "'" << p_market_data_map_id << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "DELETE_MARKET_DATA_MAP" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void GET_CLIENT_ORDER_ID_MAP( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_CLIENT_ORDER_ID_MAP";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_CLIENT_ORDER_ID_MAP" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_CLIENT_ORDER_ID_MAP_BY_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_order_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_CLIENT_ORDER_ID_MAP_BY_ORDER_ID( " << "'" << p_order_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_CLIENT_ORDER_ID_MAP_BY_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_CLIENT_ORDER_ID_MAP_BY_SESSION_ID( quantmodel::DatabaseEngineRequest& request, const string& p_session_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_CLIENT_ORDER_ID_MAP_BY_SESSION_ID( " << "'" << p_session_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_CLIENT_ORDER_ID_MAP_BY_SESSION_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_FIX_ENGINE_SESSIONS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_FIX_ENGINE_SESSIONS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_FIX_ENGINE_SESSIONS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_MARKET_DATA_MAP_ID_BY_INSTRUMENT_ID( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_MARKET_DATA_MAP_ID_BY_INSTRUMENT_ID( " << p_instrument_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_MARKET_DATA_MAP_ID_BY_INSTRUMENT_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void INSERT_CLIENT_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_client_order_id, const string& p_correlation_order_id, const string& p_orig_client_order_id, const string& p_session_id, const string& p_release_id, const string& p_order_id, const string& p_instrument_id, const string& p_investment_system_id, const string& p_added_datetime, const string& p_added_by, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_CLIENT_ORDER_ID( " << "'" << p_client_order_id << "'," << "'" << p_correlation_order_id << "'," << "'" << p_orig_client_order_id << "'," << "'" << p_session_id << "'," << "'" << p_release_id << "'," << "'" << p_order_id << "'," << p_instrument_id << "," << "'" << p_investment_system_id << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_CLIENT_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_MARKET_DATA_EVENT( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id, const string& p_event_type, const string& p_event_action, const string& p_event_prc, const string& p_event_qty, const string& p_event_tm, const string& p_timebar_key, const string& p_arrival_tm ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_MARKET_DATA_EVENT( " << p_instrument_id << "," << "'" << p_event_type << "'," << "'" << p_event_action << "'," << "'" << p_event_prc << "'," << "'" << p_event_qty << "'," << "'" << p_event_tm << "'," << "'" << p_timebar_key << "'," << "'" << p_arrival_tm << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_MARKET_DATA_EVENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_MARKET_DATA_MAP( quantmodel::DatabaseEngineRequest& request, const string& p_market_data_map_id, const string& p_instrument_id, const string& p_session_id, const string& p_added_datetime, const string& p_added_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_MARKET_DATA_MAP( " << "'" << p_market_data_map_id << "'," << p_instrument_id << "," << "'" << p_session_id << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_MARKET_DATA_MAP" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_CLIENT_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_client_order_id, const string& p_release_id, const string& p_order_id, const string& p_instrument_id, const string& p_investment_system_id, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_CLIENT_ORDER_ID( " << "'" << p_client_order_id << "'," << "'" << p_release_id << "'," << "'" << p_order_id << "'," << p_instrument_id << "," << "'" << p_investment_system_id << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_CLIENT_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void GET_INSTRUMENT_TIMEBARS( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id, const string& p_timestep, const string& p_start, const string& p_stop ) 
    {
        std::ostringstream sql;
        sql << "call GET_INSTRUMENT_TIMEBARS( " << p_instrument_id << "," << p_timestep << "," << "'" << p_start << "'," << "'" << p_stop << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INSTRUMENT_TIMEBARS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_MARKET_DATA_EVENTS_BY_TIMEBAR_KEY( quantmodel::DatabaseEngineRequest& request, const string& p_timebar_key ) 
    {
        std::ostringstream sql;
        sql << "call GET_MARKET_DATA_EVENTS_BY_TIMEBAR_KEY( " << "'" << p_timebar_key << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_MARKET_DATA_EVENTS_BY_TIMEBAR_KEY" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_NEXT_MARKET_DATA_EVENT( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id, const string& p_begdate, const string& p_enddate ) 
    {
        std::ostringstream sql;
        sql << "call GET_NEXT_MARKET_DATA_EVENT( " << p_instrument_id << "," << "'" << p_begdate << "'," << "'" << p_enddate << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_NEXT_MARKET_DATA_EVENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ACCOUNT_REPORT_DATA( quantmodel::DatabaseEngineRequest& request, const string& p_account_id, const string& p_beg_dt, const string& p_end_dt ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACCOUNT_REPORT_DATA( " << p_account_id << "," << "'" << p_beg_dt << "'," << "'" << p_end_dt << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACCOUNT_REPORT_DATA" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void DELETE_ACCOUNT_STRATEGY_BY_ACCOUNT_ID( quantmodel::DatabaseEngineRequest& request, const string& p_account_id ) 
    {
        std::ostringstream sql;
        sql << "call DELETE_ACCOUNT_STRATEGY_BY_ACCOUNT_ID( " << p_account_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "DELETE_ACCOUNT_STRATEGY_BY_ACCOUNT_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void DELETE_INSTRUMENTS_BY_INVESTMENT_SYSTEM_ID( quantmodel::DatabaseEngineRequest& request, const string& p_investment_system_id ) 
    {
        std::ostringstream sql;
        sql << "call DELETE_INSTRUMENTS_BY_INVESTMENT_SYSTEM_ID( " << "'" << p_investment_system_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "DELETE_INSTRUMENTS_BY_INVESTMENT_SYSTEM_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void DELETE_STRATEGY( quantmodel::DatabaseEngineRequest& request, const string& p_strategy_id ) 
    {
        std::ostringstream sql;
        sql << "call DELETE_STRATEGY( " << p_strategy_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "DELETE_STRATEGY" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void GET_ACCOUNTS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACCOUNTS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACCOUNTS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ACCOUNTS_BY_INVESTMENT_SYSTEM( quantmodel::DatabaseEngineRequest& request, const string& p_investment_system_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACCOUNTS_BY_INVESTMENT_SYSTEM( " << "'" << p_investment_system_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACCOUNTS_BY_INVESTMENT_SYSTEM" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ACCOUNTS_BY_STRATEGY_ID( quantmodel::DatabaseEngineRequest& request, const string& p_strategy_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACCOUNTS_BY_STRATEGY_ID( " << p_strategy_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACCOUNTS_BY_STRATEGY_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ACCOUNT_BY_ACCOUNT_ID( quantmodel::DatabaseEngineRequest& request, const string& p_account_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACCOUNT_BY_ACCOUNT_ID( " << p_account_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACCOUNT_BY_ACCOUNT_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ACCOUNT_CONFIGURATION( quantmodel::DatabaseEngineRequest& request, const string& p_account_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACCOUNT_CONFIGURATION( " << p_account_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACCOUNT_CONFIGURATION" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ACCOUNT_INSTRUMENTS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACCOUNT_INSTRUMENTS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACCOUNT_INSTRUMENTS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ACCOUNT_STRATEGIES( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACCOUNT_STRATEGIES";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACCOUNT_STRATEGIES" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_ACCOUNT_STRATEGY_BY_INVESTMENT_SYSTEM_SIGNAL( quantmodel::DatabaseEngineRequest& request, const string& p_investment_system_id, const string& p_signal_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACCOUNT_STRATEGY_BY_INVESTMENT_SYSTEM_SIGNAL( " << "'" << p_investment_system_id << "'," << "'" << p_signal_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACCOUNT_STRATEGY_BY_INVESTMENT_SYSTEM_SIGNAL" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INSTRUMENT_BY_INVESTMENT_SYSTEM( quantmodel::DatabaseEngineRequest& request, const string& p_investment_system_id, const string& p_lookup_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_INSTRUMENT_BY_INVESTMENT_SYSTEM( " << "'" << p_investment_system_id << "'," << "'" << p_lookup_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INSTRUMENT_BY_INVESTMENT_SYSTEM" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INSTRUMENT_ID_TYPE_BY_INVESTMENT_SYSTEM( quantmodel::DatabaseEngineRequest& request, const string& p_investment_system_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_INSTRUMENT_ID_TYPE_BY_INVESTMENT_SYSTEM( " << "'" << p_investment_system_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INSTRUMENT_ID_TYPE_BY_INVESTMENT_SYSTEM" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INVESTMENT_SYSTEMS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_INVESTMENT_SYSTEMS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INVESTMENT_SYSTEMS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INVESTMENT_SYSTEMS_BY_ACCOUNT( quantmodel::DatabaseEngineRequest& request, const string& p_account_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_INVESTMENT_SYSTEMS_BY_ACCOUNT( " << p_account_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INVESTMENT_SYSTEMS_BY_ACCOUNT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INVESTMENT_SYSTEM_BY_INVESTMENT_SYSTEM_ID( quantmodel::DatabaseEngineRequest& request, const string& p_investment_system_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_INVESTMENT_SYSTEM_BY_INVESTMENT_SYSTEM_ID( " << "'" << p_investment_system_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INVESTMENT_SYSTEM_BY_INVESTMENT_SYSTEM_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_STRATEGIES( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_STRATEGIES";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_STRATEGIES" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_STRATEGIES_BY_ACCOUNT( quantmodel::DatabaseEngineRequest& request, const string& p_account_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_STRATEGIES_BY_ACCOUNT( " << p_account_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_STRATEGIES_BY_ACCOUNT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_STRATEGIES_BY_INVESTMENT_SYSTEM( quantmodel::DatabaseEngineRequest& request, const string& p_investment_system_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_STRATEGIES_BY_INVESTMENT_SYSTEM( " << "'" << p_investment_system_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_STRATEGIES_BY_INVESTMENT_SYSTEM" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_STRATEGY_BY_STRATEGY_ID( quantmodel::DatabaseEngineRequest& request, const string& p_strategy_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_STRATEGY_BY_STRATEGY_ID( " << p_strategy_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_STRATEGY_BY_STRATEGY_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_STRATEGY_CONFIGURATION( quantmodel::DatabaseEngineRequest& request, const string& p_account_id, const string& p_strategy_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_STRATEGY_CONFIGURATION( " << p_account_id << "," << p_strategy_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_STRATEGY_CONFIGURATION" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_STRATEGY_CONFIGURATION_BY_ACCOUNT( quantmodel::DatabaseEngineRequest& request, const string& p_account_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_STRATEGY_CONFIGURATION_BY_ACCOUNT( " << p_account_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_STRATEGY_CONFIGURATION_BY_ACCOUNT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_UNPROCESSED_SIGNALS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_UNPROCESSED_SIGNALS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_UNPROCESSED_SIGNALS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_UNPROCESSED_SIGNALS_BY_REQUEST( quantmodel::DatabaseEngineRequest& request, const string& p_request_id, const string& p_investment_system_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_UNPROCESSED_SIGNALS_BY_REQUEST( " << "'" << p_request_id << "'," << "'" << p_investment_system_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_UNPROCESSED_SIGNALS_BY_REQUEST" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void INSERT_ACCOUNT( quantmodel::DatabaseEngineRequest& request, const string& p_name, const string& p_market_account_id, const string& p_custody_account_id, const string& p_notional_val, const string& p_profit_target, const string& p_stop_loss, const string& p_signal_weight, const string& p_is_active, const string& p_added_datetime, const string& p_added_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_ACCOUNT( " << "'" << p_name << "'," << "'" << p_market_account_id << "'," << "'" << p_custody_account_id << "'," << "'" << p_notional_val << "'," << "'" << p_profit_target << "'," << "'" << p_stop_loss << "'," << p_signal_weight << "," << p_is_active << "," << "'" << p_added_datetime << "'," << "'" << p_added_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_ACCOUNT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_ACCOUNT_STRATEGY( quantmodel::DatabaseEngineRequest& request, const string& p_account_id, const string& p_strategy_id, const string& p_weight, const string& p_is_enabled ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_ACCOUNT_STRATEGY( " << p_account_id << "," << p_strategy_id << "," << p_weight << "," << p_is_enabled << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_ACCOUNT_STRATEGY" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_INVESTMENT_SYSTEM( quantmodel::DatabaseEngineRequest& request, const string& p_investment_system_id, const string& p_name ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_INVESTMENT_SYSTEM( " << "'" << p_investment_system_id << "'," << "'" << p_name << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_INVESTMENT_SYSTEM" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_INVESTMENT_SYSTEM_INSTRUMENT( quantmodel::DatabaseEngineRequest& request, const string& p_investment_system_id, const string& p_instrument_id ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_INVESTMENT_SYSTEM_INSTRUMENT( " << "'" << p_investment_system_id << "'," << p_instrument_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_INVESTMENT_SYSTEM_INSTRUMENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_STRATEGY( quantmodel::DatabaseEngineRequest& request, const string& p_investment_system_id, const string& p_signal_id, const string& p_name, const string& p_default_weight ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_STRATEGY( " << "'" << p_investment_system_id << "'," << "'" << p_signal_id << "'," << "'" << p_name << "'," << p_default_weight << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_STRATEGY" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_STRATEGY_CONFIGURATION( quantmodel::DatabaseEngineRequest& request, const string& p_account_id, const string& p_strategy_id, const string& p_name, const string& p_value ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_STRATEGY_CONFIGURATION( " << p_account_id << "," << p_strategy_id << "," << "'" << p_name << "'," << "'" << p_value << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_STRATEGY_CONFIGURATION" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_STRATEGY_SIGNAL( quantmodel::DatabaseEngineRequest& request, const string& p_strategy_id, const string& p_account_id, const string& p_investment_system_id, const string& p_request_id, const string& p_instrument_id, const string& p_instrument_name, const string& p_last_prc, const string& p_limit_prc, const string& p_signal_qty, const string& p_allocation_qty, const string& p_added_datetime, const string& p_added_by, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_STRATEGY_SIGNAL( " << p_strategy_id << "," << p_account_id << "," << "'" << p_investment_system_id << "'," << "'" << p_request_id << "'," << p_instrument_id << "," << "'" << p_instrument_name << "'," << "'" << p_last_prc << "'," << "'" << p_limit_prc << "'," << "'" << p_signal_qty << "'," << "'" << p_allocation_qty << "'," << "'" << p_added_datetime << "'," << "'" << p_added_by << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_STRATEGY_SIGNAL" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_ACCOUNT( quantmodel::DatabaseEngineRequest& request, const string& p_account_id, const string& p_name, const string& p_market_account_id, const string& p_custody_account_id, const string& p_giveup_account_id, const string& p_giveup_firm_id, const string& p_notional_val, const string& p_profit_target, const string& p_stop_loss, const string& p_signal_weight, const string& p_is_suppressed, const string& p_is_active, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_ACCOUNT( " << p_account_id << "," << "'" << p_name << "'," << "'" << p_market_account_id << "'," << "'" << p_custody_account_id << "'," << "'" << p_giveup_account_id << "'," << "'" << p_giveup_firm_id << "'," << "'" << p_notional_val << "'," << "'" << p_profit_target << "'," << "'" << p_stop_loss << "'," << p_signal_weight << "," << p_is_suppressed << "," << p_is_active << "," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_ACCOUNT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_INSTRUMENT_LOOKUP( quantmodel::DatabaseEngineRequest& request, const string& p_investment_system_id, const string& p_instrument_id_type_id ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_INSTRUMENT_LOOKUP( " << "'" << p_investment_system_id << "'," << "'" << p_instrument_id_type_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_INSTRUMENT_LOOKUP" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_INVESTMENT_SYSTEM( quantmodel::DatabaseEngineRequest& request, const string& p_investment_system_id, const string& p_name, const string& p_is_active ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_INVESTMENT_SYSTEM( " << "'" << p_investment_system_id << "'," << "'" << p_name << "'," << p_is_active << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_INVESTMENT_SYSTEM" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_STRATEGY( quantmodel::DatabaseEngineRequest& request, const string& p_strategy_id, const string& p_signal_id, const string& p_name, const string& p_default_weight, const string& p_is_active ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_STRATEGY( " << p_strategy_id << "," << "'" << p_signal_id << "'," << "'" << p_name << "'," << p_default_weight << "," << p_is_active << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_STRATEGY" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void GET_SYSTEM_PARAMETERS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_SYSTEM_PARAMETERS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_SYSTEM_PARAMETERS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_SYSTEM_PARAMETERS_BY_CATEGORY( quantmodel::DatabaseEngineRequest& request, const string& p_category ) 
    {
        std::ostringstream sql;
        sql << "call GET_SYSTEM_PARAMETERS_BY_CATEGORY( " << "'" << p_category << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_SYSTEM_PARAMETERS_BY_CATEGORY" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void UPDATE_PARAMETER( quantmodel::DatabaseEngineRequest& request, const string& p_category, const string& p_name, const string& p_value ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_PARAMETER( " << "'" << p_category << "'," << "'" << p_name << "'," << "'" << p_value << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_PARAMETER" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void VALIDATE_DEALING_ENGINE_PARAMETERS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call VALIDATE_DEALING_ENGINE_PARAMETERS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "VALIDATE_DEALING_ENGINE_PARAMETERS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void VALIDATE_MKTDATA_ENGINE_PARAMETERS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call VALIDATE_MKTDATA_ENGINE_PARAMETERS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "VALIDATE_MKTDATA_ENGINE_PARAMETERS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void VALIDATE_SIGNAL_ENGINE_PARAMETERS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call VALIDATE_SIGNAL_ENGINE_PARAMETERS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "VALIDATE_SIGNAL_ENGINE_PARAMETERS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void DIREXION_DUPLICATE_TEST( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id, const string& p_strategy_id, const string& p_order_type, const string& p_side, const string& p_price ) 
    {
        std::ostringstream sql;
        sql << "call DIREXION_DUPLICATE_TEST( " << p_instrument_id << "," << "'" << p_strategy_id << "'," << "'" << p_order_type << "'," << "'" << p_side << "'," << "'" << p_price << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "DIREXION_DUPLICATE_TEST" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void DIREXION_GET_TRADESTATION_ORDER( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id, const string& p_strategy_id, const string& p_order_type, const string& p_side, const string& p_price ) 
    {
        std::ostringstream sql;
        sql << "call DIREXION_GET_TRADESTATION_ORDER( " << p_instrument_id << "," << "'" << p_strategy_id << "'," << "'" << p_order_type << "'," << "'" << p_side << "'," << "'" << p_price << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "DIREXION_GET_TRADESTATION_ORDER" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_OPEN_TRADESTATION_ORDERS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_OPEN_TRADESTATION_ORDERS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_OPEN_TRADESTATION_ORDERS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_OPEN_TRADESTATION_ORDERS2( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id, const string& p_strategy_id, const string& p_open_close, const string& p_order_type ) 
    {
        std::ostringstream sql;
        sql << "call GET_OPEN_TRADESTATION_ORDERS2( " << p_instrument_id << "," << "'" << p_strategy_id << "'," << "'" << p_open_close << "'," << "'" << p_order_type << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_OPEN_TRADESTATION_ORDERS2" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_ID( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_ID( " << p_instrument_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_ID( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id, const string& p_strategy_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_ID( " << p_instrument_id << "," << "'" << p_strategy_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_OPEN_TRADESTATION_ORDERS_BY_STRATEGY_ID( quantmodel::DatabaseEngineRequest& request, const string& p_strategy_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_OPEN_TRADESTATION_ORDERS_BY_STRATEGY_ID( " << "'" << p_strategy_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_OPEN_TRADESTATION_ORDERS_BY_STRATEGY_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_TRADESTATION_INSTRUMENT( quantmodel::DatabaseEngineRequest& request, const string& p_symbol ) 
    {
        std::ostringstream sql;
        sql << "call GET_TRADESTATION_INSTRUMENT( " << "'" << p_symbol << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_TRADESTATION_INSTRUMENT" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_EXPIRATION( quantmodel::DatabaseEngineRequest& request, const string& p_instrument_id, const string& p_strategy_id, const string& p_expiration_dt ) 
    {
        std::ostringstream sql;
        sql << "call GET_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_EXPIRATION( " << p_instrument_id << "," << "'" << p_strategy_id << "'," << "'" << p_expiration_dt << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_EXPIRATION" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_TRADESTATION_ORDER_BY_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_order_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_TRADESTATION_ORDER_BY_ORDER_ID( " << "'" << p_order_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_TRADESTATION_ORDER_BY_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_TRADESTATION_ORDER_BY_TRADESTATION_ORDER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_tradestation_order_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_TRADESTATION_ORDER_BY_TRADESTATION_ORDER_ID( " << p_tradestation_order_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_TRADESTATION_ORDER_BY_TRADESTATION_ORDER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void INSERT_TRADESTATION_ORDER( quantmodel::DatabaseEngineRequest& request, const string& p_strategy_id, const string& p_order_data, const string& p_order_status, const string& p_order_type, const string& p_open_close, const string& p_signal, const string& p_timestamp, const string& p_side, const string& p_quantity, const string& p_symbol, const string& p_price, const string& p_instrument_id, const string& p_instrument_name, const string& p_signal_quantity, const string& p_signal_price, const string& p_expiration_dt, const string& p_is_executed, const string& p_is_open, const string& p_added_datetime, const string& p_added_by, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_TRADESTATION_ORDER( " << "'" << p_strategy_id << "'," << "'" << p_order_data << "'," << "'" << p_order_status << "'," << "'" << p_order_type << "'," << "'" << p_open_close << "'," << "'" << p_signal << "'," << "'" << p_timestamp << "'," << "'" << p_side << "'," << "'" << p_quantity << "'," << "'" << p_symbol << "'," << "'" << p_price << "'," << p_instrument_id << "," << "'" << p_instrument_name << "'," << "'" << p_signal_quantity << "'," << "'" << p_signal_price << "'," << "'" << p_expiration_dt << "'," << p_is_executed << "," << p_is_open << "," << "'" << p_added_datetime << "'," << "'" << p_added_by << "'," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_TRADESTATION_ORDER" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void UPDATE_TRADESTATION_ORDER( quantmodel::DatabaseEngineRequest& request, const string& p_tradestation_order_id, const string& p_order_id, const string& p_is_executed, const string& p_is_open, const string& p_updated_datetime, const string& p_updated_by ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_TRADESTATION_ORDER( " << p_tradestation_order_id << "," << "'" << p_order_id << "'," << p_is_executed << "," << p_is_open << "," << "'" << p_updated_datetime << "'," << "'" << p_updated_by << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_TRADESTATION_ORDER" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void DELETE_USER_APPLICATION_BY_USER_ID( quantmodel::DatabaseEngineRequest& request, const string& p_user_id ) 
    {
        std::ostringstream sql;
        sql << "call DELETE_USER_APPLICATION_BY_USER_ID( " << p_user_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "DELETE_USER_APPLICATION_BY_USER_ID" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void GET_ACCOUNTS_BY_USER( quantmodel::DatabaseEngineRequest& request, const string& p_user_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_ACCOUNTS_BY_USER( " << p_user_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_ACCOUNTS_BY_USER" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_APPLICATIONS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_APPLICATIONS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_APPLICATIONS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_APPLICATIONS_BY_USER( quantmodel::DatabaseEngineRequest& request, const string& p_user_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_APPLICATIONS_BY_USER( " << p_user_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_APPLICATIONS_BY_USER" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INSTRUMENTS_BY_USER( quantmodel::DatabaseEngineRequest& request, const string& p_user_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_INSTRUMENTS_BY_USER( " << p_user_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INSTRUMENTS_BY_USER" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_INVESTMENT_SYSTEMS_BY_USER( quantmodel::DatabaseEngineRequest& request, const string& p_user_id ) 
    {
        std::ostringstream sql;
        sql << "call GET_INVESTMENT_SYSTEMS_BY_USER( " << p_user_id << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_INVESTMENT_SYSTEMS_BY_USER" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void GET_USERS( quantmodel::DatabaseEngineRequest& request ) 
    {
        std::ostringstream sql;
        sql << "call GET_USERS";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "GET_USERS" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

    const inline static void INSERT_USER( quantmodel::DatabaseEngineRequest& request, const string& p_username, const string& p_password, const string& p_firstname, const string& p_lastname ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_USER( " << "'" << p_username << "'," << "'" << p_password << "'," << "'" << p_firstname << "'," << "'" << p_lastname << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_USER" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void INSERT_USER_APPLICATION( quantmodel::DatabaseEngineRequest& request, const string& p_user_id, const string& p_application_id ) 
    {
        std::ostringstream sql;
        sql << "call INSERT_USER_APPLICATION( " << p_user_id << "," << "'" << p_application_id << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "INSERT_USER_APPLICATION" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void UPDATE_USER( quantmodel::DatabaseEngineRequest& request, const string& p_user_id, const string& p_username, const string& p_password, const string& p_firstname, const string& p_lastname, const string& p_is_active ) 
    {
        std::ostringstream sql;
        sql << "call UPDATE_USER( " << p_user_id << "," << "'" << p_username << "'," << "'" << p_password << "'," << "'" << p_firstname << "'," << "'" << p_lastname << "'," << p_is_active << " )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "UPDATE_USER" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::UPDATE );
    }

    const inline static void USER_LOGIN( quantmodel::DatabaseEngineRequest& request, const string& p_username, const string& p_password, const string& p_application_id, const string& p_session ) 
    {
        std::ostringstream sql;
        sql << "call USER_LOGIN( " << "'" << p_username << "'," << "'" << p_password << "'," << "'" << p_application_id << "'," << "'" << p_session << "' )";
        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();
        query->set_query_id( "USER_LOGIN" );
        query->set_query_str( sql.str() );
        query->set_type( quantmodel::DatabaseEngineRequest::SELECT );
    }

  };
}

#endif // QUANTMODEL_PROCEDURES_H
