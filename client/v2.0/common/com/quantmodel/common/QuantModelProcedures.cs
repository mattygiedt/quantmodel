/*
 QuantModelProcedures.cs auto generated on:
 Fri Feb 17 21:15:16 CST 2012
 DO NOT EDIT
*/

using System;
using quantmodel;


namespace com.quantmodel.common
{
  public sealed class QuantModelProcedures
  {
    public static Google.ProtocolBuffers.IMessage VALIDATE_SIGNAL_ENGINE_PARAMETERS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call VALIDATE_SIGNAL_ENGINE_PARAMETERS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "VALIDATE_SIGNAL_ENGINE_PARAMETERS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage VALIDATE_MKTDATA_ENGINE_PARAMETERS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call VALIDATE_MKTDATA_ENGINE_PARAMETERS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "VALIDATE_MKTDATA_ENGINE_PARAMETERS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_SYSTEM_PARAMETERS_BY_CATEGORY( string session_id, string p_category ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_SYSTEM_PARAMETERS_BY_CATEGORY( " + "'" + p_category + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_SYSTEM_PARAMETERS_BY_CATEGORY" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage VALIDATE_DEALING_ENGINE_PARAMETERS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call VALIDATE_DEALING_ENGINE_PARAMETERS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "VALIDATE_DEALING_ENGINE_PARAMETERS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_PARAMETER( string session_id, string p_category, string p_name, string p_value ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_PARAMETER( " + "'" + p_category + "'," + "'" + p_name + "'," + "'" + p_value + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_PARAMETER" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_SYSTEM_PARAMETERS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_SYSTEM_PARAMETERS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_SYSTEM_PARAMETERS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACCOUNTS_BY_INVESTMENT_SYSTEM( string session_id, string p_investment_system_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACCOUNTS_BY_INVESTMENT_SYSTEM( " + "'" + p_investment_system_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACCOUNTS_BY_INVESTMENT_SYSTEM" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INSTRUMENT_BY_INVESTMENT_SYSTEM( string session_id, string p_investment_system_id, string p_lookup_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INSTRUMENT_BY_INVESTMENT_SYSTEM( " + "'" + p_investment_system_id + "'," + "'" + p_lookup_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INSTRUMENT_BY_INVESTMENT_SYSTEM" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_UNPROCESSED_SIGNALS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_UNPROCESSED_SIGNALS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_UNPROCESSED_SIGNALS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_STRATEGIES( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_STRATEGIES";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_STRATEGIES" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_INSTRUMENT_LOOKUP( string session_id, string p_investment_system_id, string p_instrument_id_type_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_INSTRUMENT_LOOKUP( " + "'" + p_investment_system_id + "'," + "'" + p_instrument_id_type_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_INSTRUMENT_LOOKUP" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_ACCOUNT( string session_id, string p_name, string p_market_account_id, string p_custody_account_id, string p_notional_val, string p_profit_target, string p_stop_loss, string p_signal_weight, string p_is_active, string p_added_datetime, string p_added_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_ACCOUNT( " + "'" + p_name + "'," + "'" + p_market_account_id + "'," + "'" + p_custody_account_id + "'," + "'" + p_notional_val + "'," + "'" + p_profit_target + "'," + "'" + p_stop_loss + "'," + p_signal_weight + "," + p_is_active + "," + "'" + p_added_datetime + "'," + "'" + p_added_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_ACCOUNT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INVESTMENT_SYSTEMS_BY_ACCOUNT( string session_id, string p_account_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INVESTMENT_SYSTEMS_BY_ACCOUNT( " + p_account_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INVESTMENT_SYSTEMS_BY_ACCOUNT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_STRATEGY_CONFIGURATION( string session_id, string p_account_id, string p_strategy_id, string p_name, string p_value ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_STRATEGY_CONFIGURATION( " + p_account_id + "," + p_strategy_id + "," + "'" + p_name + "'," + "'" + p_value + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_STRATEGY_CONFIGURATION" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_STRATEGY_BY_STRATEGY_ID( string session_id, string p_strategy_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_STRATEGY_BY_STRATEGY_ID( " + p_strategy_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_STRATEGY_BY_STRATEGY_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_STRATEGIES_BY_INVESTMENT_SYSTEM( string session_id, string p_investment_system_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_STRATEGIES_BY_INVESTMENT_SYSTEM( " + "'" + p_investment_system_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_STRATEGIES_BY_INVESTMENT_SYSTEM" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage DELETE_ACCOUNT_STRATEGY_BY_ACCOUNT_ID( string session_id, string p_account_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call DELETE_ACCOUNT_STRATEGY_BY_ACCOUNT_ID( " + p_account_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "DELETE_ACCOUNT_STRATEGY_BY_ACCOUNT_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_STRATEGY_CONFIGURATION_BY_ACCOUNT( string session_id, string p_account_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_STRATEGY_CONFIGURATION_BY_ACCOUNT( " + p_account_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_STRATEGY_CONFIGURATION_BY_ACCOUNT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_STRATEGY_SIGNAL( string session_id, string p_strategy_id, string p_account_id, string p_investment_system_id, string p_request_id, string p_instrument_id, string p_instrument_name, string p_last_prc, string p_limit_prc, string p_signal_qty, string p_allocation_qty, string p_added_datetime, string p_added_by, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_STRATEGY_SIGNAL( " + p_strategy_id + "," + p_account_id + "," + "'" + p_investment_system_id + "'," + "'" + p_request_id + "'," + p_instrument_id + "," + "'" + p_instrument_name + "'," + "'" + p_last_prc + "'," + "'" + p_limit_prc + "'," + "'" + p_signal_qty + "'," + "'" + p_allocation_qty + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_STRATEGY_SIGNAL" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_STRATEGY_CONFIGURATION( string session_id, string p_account_id, string p_strategy_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_STRATEGY_CONFIGURATION( " + p_account_id + "," + p_strategy_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_STRATEGY_CONFIGURATION" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_STRATEGIES_BY_ACCOUNT( string session_id, string p_account_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_STRATEGIES_BY_ACCOUNT( " + p_account_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_STRATEGIES_BY_ACCOUNT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INVESTMENT_SYSTEM_BY_INVESTMENT_SYSTEM_ID( string session_id, string p_investment_system_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INVESTMENT_SYSTEM_BY_INVESTMENT_SYSTEM_ID( " + "'" + p_investment_system_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INVESTMENT_SYSTEM_BY_INVESTMENT_SYSTEM_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_STRATEGY( string session_id, string p_investment_system_id, string p_signal_id, string p_name, string p_default_weight ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_STRATEGY( " + "'" + p_investment_system_id + "'," + "'" + p_signal_id + "'," + "'" + p_name + "'," + p_default_weight + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_STRATEGY" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_UNPROCESSED_SIGNALS_BY_REQUEST( string session_id, string p_request_id, string p_investment_system_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_UNPROCESSED_SIGNALS_BY_REQUEST( " + "'" + p_request_id + "'," + "'" + p_investment_system_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_UNPROCESSED_SIGNALS_BY_REQUEST" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACCOUNT_STRATEGIES( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACCOUNT_STRATEGIES";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACCOUNT_STRATEGIES" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_INVESTMENT_SYSTEM( string session_id, string p_investment_system_id, string p_name, string p_is_active ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_INVESTMENT_SYSTEM( " + "'" + p_investment_system_id + "'," + "'" + p_name + "'," + p_is_active + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_INVESTMENT_SYSTEM" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_ACCOUNT( string session_id, string p_account_id, string p_name, string p_market_account_id, string p_custody_account_id, string p_giveup_account_id, string p_giveup_firm_id, string p_notional_val, string p_profit_target, string p_stop_loss, string p_signal_weight, string p_is_suppressed, string p_is_active, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_ACCOUNT( " + p_account_id + "," + "'" + p_name + "'," + "'" + p_market_account_id + "'," + "'" + p_custody_account_id + "'," + "'" + p_giveup_account_id + "'," + "'" + p_giveup_firm_id + "'," + "'" + p_notional_val + "'," + "'" + p_profit_target + "'," + "'" + p_stop_loss + "'," + p_signal_weight + "," + p_is_suppressed + "," + p_is_active + "," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_ACCOUNT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_ACCOUNT_STRATEGY( string session_id, string p_account_id, string p_strategy_id, string p_weight, string p_is_enabled ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_ACCOUNT_STRATEGY( " + p_account_id + "," + p_strategy_id + "," + p_weight + "," + p_is_enabled + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_ACCOUNT_STRATEGY" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACCOUNT_STRATEGY_BY_INVESTMENT_SYSTEM_SIGNAL( string session_id, string p_investment_system_id, string p_signal_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACCOUNT_STRATEGY_BY_INVESTMENT_SYSTEM_SIGNAL( " + "'" + p_investment_system_id + "'," + "'" + p_signal_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACCOUNT_STRATEGY_BY_INVESTMENT_SYSTEM_SIGNAL" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage DELETE_INSTRUMENTS_BY_INVESTMENT_SYSTEM_ID( string session_id, string p_investment_system_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call DELETE_INSTRUMENTS_BY_INVESTMENT_SYSTEM_ID( " + "'" + p_investment_system_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "DELETE_INSTRUMENTS_BY_INVESTMENT_SYSTEM_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACCOUNT_INSTRUMENTS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACCOUNT_INSTRUMENTS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACCOUNT_INSTRUMENTS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_INVESTMENT_SYSTEM( string session_id, string p_investment_system_id, string p_name ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_INVESTMENT_SYSTEM( " + "'" + p_investment_system_id + "'," + "'" + p_name + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_INVESTMENT_SYSTEM" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_INVESTMENT_SYSTEM_INSTRUMENT( string session_id, string p_investment_system_id, string p_instrument_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_INVESTMENT_SYSTEM_INSTRUMENT( " + "'" + p_investment_system_id + "'," + p_instrument_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_INVESTMENT_SYSTEM_INSTRUMENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACCOUNTS_BY_STRATEGY_ID( string session_id, string p_strategy_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACCOUNTS_BY_STRATEGY_ID( " + p_strategy_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACCOUNTS_BY_STRATEGY_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INSTRUMENT_ID_TYPE_BY_INVESTMENT_SYSTEM( string session_id, string p_investment_system_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INSTRUMENT_ID_TYPE_BY_INVESTMENT_SYSTEM( " + "'" + p_investment_system_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INSTRUMENT_ID_TYPE_BY_INVESTMENT_SYSTEM" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACCOUNTS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACCOUNTS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACCOUNTS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INVESTMENT_SYSTEMS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INVESTMENT_SYSTEMS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INVESTMENT_SYSTEMS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACCOUNT_BY_ACCOUNT_ID( string session_id, string p_account_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACCOUNT_BY_ACCOUNT_ID( " + p_account_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACCOUNT_BY_ACCOUNT_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage DELETE_STRATEGY( string session_id, string p_strategy_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call DELETE_STRATEGY( " + p_strategy_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "DELETE_STRATEGY" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACCOUNT_CONFIGURATION( string session_id, string p_account_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACCOUNT_CONFIGURATION( " + p_account_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACCOUNT_CONFIGURATION" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_STRATEGY( string session_id, string p_strategy_id, string p_signal_id, string p_name, string p_default_weight, string p_is_active ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_STRATEGY( " + p_strategy_id + "," + "'" + p_signal_id + "'," + "'" + p_name + "'," + p_default_weight + "," + p_is_active + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_STRATEGY" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACCOUNT_REPORT_DATA( string session_id, string p_account_id, string p_beg_dt, string p_end_dt ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACCOUNT_REPORT_DATA( " + p_account_id + "," + "'" + p_beg_dt + "'," + "'" + p_end_dt + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACCOUNT_REPORT_DATA" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_EXECUTION( string session_id, string p_execution_id, string p_session_id, string p_account_id, string p_order_id, string p_release_id, string p_investment_system_id, string p_instrument_id, string p_client_order_id, string p_counterparty_execution_id, string p_side_code, string p_execution_ccy, string p_execution_status, string p_execution_prc, string p_execution_qty, string p_execution_val, string p_leaves_qty, string p_average_prc, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_EXECUTION( " + "'" + p_execution_id + "'," + "'" + p_session_id + "'," + p_account_id + "," + "'" + p_order_id + "'," + "'" + p_release_id + "'," + "'" + p_investment_system_id + "'," + p_instrument_id + "," + "'" + p_client_order_id + "'," + "'" + p_counterparty_execution_id + "'," + p_side_code + "," + "'" + p_execution_ccy + "'," + "'" + p_execution_status + "'," + "'" + p_execution_prc + "'," + "'" + p_execution_qty + "'," + "'" + p_execution_val + "'," + "'" + p_leaves_qty + "'," + "'" + p_average_prc + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_EXECUTION" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_OPEN_RELEASES( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_OPEN_RELEASES";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_OPEN_RELEASES" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INSTRUMENTS_BY_SYMBOL_MATURITY_MONTH_YEAR( string session_id, string p_symbol, string p_maturity_month_year ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INSTRUMENTS_BY_SYMBOL_MATURITY_MONTH_YEAR( " + "'" + p_symbol + "'," + "'" + p_maturity_month_year + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INSTRUMENTS_BY_SYMBOL_MATURITY_MONTH_YEAR" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_POSITION_OVERNIGHT( string session_id, string p_account_id, string p_instrument_id, string p_investment_system_id, string p_position_type, string p_open_pos, string p_open_prc, string p_added_datetime, string p_added_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_POSITION_OVERNIGHT( " + p_account_id + "," + p_instrument_id + "," + "'" + p_investment_system_id + "'," + p_position_type + "," + "'" + p_open_pos + "'," + "'" + p_open_prc + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_POSITION_OVERNIGHT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ORDER_BY_EXTERNAL_ORDER_ID( string session_id, string p_external_order_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ORDER_BY_EXTERNAL_ORDER_ID( " + "'" + p_external_order_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ORDER_BY_EXTERNAL_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INSTRUMENT_SYMBOLS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INSTRUMENT_SYMBOLS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INSTRUMENT_SYMBOLS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INSTRUMENTS_BY_INSTRUMENT_TYPE( string session_id, string p_instrument_type ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INSTRUMENTS_BY_INSTRUMENT_TYPE( " + p_instrument_type + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INSTRUMENTS_BY_INSTRUMENT_TYPE" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INSTRUMENTS_BY_INVESTMENT_SYSTEM( string session_id, string p_investment_system_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INSTRUMENTS_BY_INVESTMENT_SYSTEM( " + "'" + p_investment_system_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INSTRUMENTS_BY_INVESTMENT_SYSTEM" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_INSTRUMENT( string session_id, string p_instrument_id, string p_instrument_type, string p_name, string p_security_id, string p_symbol, string p_cusip, string p_sedol, string p_isin, string p_ric, string p_security_type, string p_security_desc, string p_security_exchange, string p_maturity_month_year, string p_base_ccy, string p_last_prc, string p_multiplier, string p_exchange_price_factor, string p_is_active, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_INSTRUMENT( " + p_instrument_id + "," + p_instrument_type + "," + "'" + p_name + "'," + "'" + p_security_id + "'," + "'" + p_symbol + "'," + "'" + p_cusip + "'," + "'" + p_sedol + "'," + "'" + p_isin + "'," + "'" + p_ric + "'," + "'" + p_security_type + "'," + "'" + p_security_desc + "'," + "'" + p_security_exchange + "'," + "'" + p_maturity_month_year + "'," + "'" + p_base_ccy + "'," + "'" + p_last_prc + "'," + "'" + p_multiplier + "'," + "'" + p_exchange_price_factor + "'," + p_is_active + "," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_INSTRUMENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_OPEN_EXECUTIONS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_OPEN_EXECUTIONS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_OPEN_EXECUTIONS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_ALLOCATION( string session_id, string p_id, string p_allocation_id, string p_order_id, string p_account_id, string p_instrument_id, string p_side_code, string p_allocation_qty, string p_released_qty, string p_executed_qty, string p_executed_val, string p_average_prc, string p_added_datetime, string p_added_by, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_ALLOCATION( " + p_id + "," + "'" + p_allocation_id + "'," + "'" + p_order_id + "'," + p_account_id + "," + p_instrument_id + "," + p_side_code + "," + "'" + p_allocation_qty + "'," + "'" + p_released_qty + "'," + "'" + p_executed_qty + "'," + "'" + p_executed_val + "'," + "'" + p_average_prc + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_ALLOCATION" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_INSTRUMENT_LAST_PRICE( string session_id, string p_instrument_id, string p_last_prc, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_INSTRUMENT_LAST_PRICE( " + p_instrument_id + "," + "'" + p_last_prc + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_INSTRUMENT_LAST_PRICE" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_RELEASES_BY_ORDER_ID( string session_id, string p_order_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_RELEASES_BY_ORDER_ID( " + "'" + p_order_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_RELEASES_BY_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage MARK_INSTRUMENT( string session_id, string p_instrument_id, string p_mark_prc, string p_mark_datetime, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call MARK_INSTRUMENT( " + p_instrument_id + "," + "'" + p_mark_prc + "'," + "'" + p_mark_datetime + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "MARK_INSTRUMENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INSTRUMENT_BY_EXCHANGE_SECURITY_ID( string session_id, string p_security_exchange, string p_security_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INSTRUMENT_BY_EXCHANGE_SECURITY_ID( " + "'" + p_security_exchange + "'," + "'" + p_security_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INSTRUMENT_BY_EXCHANGE_SECURITY_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_ORDER( string session_id, string p_id, string p_order_id, string p_external_order_id, string p_investment_system_id, string p_order_type, string p_order_status, string p_instrument_id, string p_symbol, string p_tif, string p_side_code, string p_base_ccy, string p_order_qty, string p_open_qty, string p_released_qty, string p_executed_qty, string p_limit_prc, string p_stop_prc, string p_mark_prc, string p_average_prc, string p_entry_prc, string p_released_val, string p_executed_val, string p_added_datetime, string p_added_by, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_ORDER( " + p_id + "," + "'" + p_order_id + "'," + "'" + p_external_order_id + "'," + "'" + p_investment_system_id + "'," + p_order_type + "," + p_order_status + "," + p_instrument_id + "," + "'" + p_symbol + "'," + p_tif + "," + p_side_code + "," + "'" + p_base_ccy + "'," + "'" + p_order_qty + "'," + "'" + p_open_qty + "'," + "'" + p_released_qty + "'," + "'" + p_executed_qty + "'," + "'" + p_limit_prc + "'," + "'" + p_stop_prc + "'," + "'" + p_mark_prc + "'," + "'" + p_average_prc + "'," + "'" + p_entry_prc + "'," + "'" + p_released_val + "'," + "'" + p_executed_val + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_ORDER" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_FIX_ENGINE_BY_SESSION_ID( string session_id, string p_session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_FIX_ENGINE_BY_SESSION_ID( " + "'" + p_session_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_FIX_ENGINE_BY_SESSION_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_FIX_ENGINE_BY_ACCOUNT_INSTRUMENT_ID( string session_id, string p_account_id, string p_instrument_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_FIX_ENGINE_BY_ACCOUNT_INSTRUMENT_ID( " + p_account_id + "," + p_instrument_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_FIX_ENGINE_BY_ACCOUNT_INSTRUMENT_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_ORDER( string session_id, string p_order_id, string p_order_type, string p_order_status, string p_instrument_id, string p_symbol, string p_tif, string p_side_code, string p_base_ccy, string p_order_qty, string p_open_qty, string p_released_qty, string p_executed_qty, string p_limit_prc, string p_stop_prc, string p_mark_prc, string p_average_prc, string p_released_val, string p_executed_val, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_ORDER( " + "'" + p_order_id + "'," + p_order_type + "," + p_order_status + "," + p_instrument_id + "," + "'" + p_symbol + "'," + p_tif + "," + p_side_code + "," + "'" + p_base_ccy + "'," + "'" + p_order_qty + "'," + "'" + p_open_qty + "'," + "'" + p_released_qty + "'," + "'" + p_executed_qty + "'," + "'" + p_limit_prc + "'," + "'" + p_stop_prc + "'," + "'" + p_mark_prc + "'," + "'" + p_average_prc + "'," + "'" + p_released_val + "'," + "'" + p_executed_val + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_ORDER" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_RELEASE( string session_id, string p_release_id, string p_session_id, string p_account_id, string p_order_id, string p_investment_system_id, string p_instrument_id, string p_client_order_id, string p_counterparty_order_id, string p_release_status, string p_release_type, string p_tif, string p_side_code, string p_release_qty, string p_limit_prc, string p_average_prc, string p_executed_val, string p_executed_qty, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_RELEASE( " + "'" + p_release_id + "'," + "'" + p_session_id + "'," + p_account_id + "," + "'" + p_order_id + "'," + "'" + p_investment_system_id + "'," + p_instrument_id + "," + "'" + p_client_order_id + "'," + "'" + p_counterparty_order_id + "'," + p_release_status + "," + p_release_type + "," + p_tif + "," + p_side_code + "," + "'" + p_release_qty + "'," + "'" + p_limit_prc + "'," + "'" + p_average_prc + "'," + "'" + p_executed_val + "'," + "'" + p_executed_qty + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_RELEASE" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INSTRUMENT_BY_FIX_ENGINE_ID( string session_id, string p_fix_engine_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INSTRUMENT_BY_FIX_ENGINE_ID( " + "'" + p_fix_engine_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INSTRUMENT_BY_FIX_ENGINE_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_EXECUTIONS_BY_ORDER_ID( string session_id, string p_order_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_EXECUTIONS_BY_ORDER_ID( " + "'" + p_order_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_EXECUTIONS_BY_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACTIVE_INSTRUMENTS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACTIVE_INSTRUMENTS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACTIVE_INSTRUMENTS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ALLOCATION_BY_ALLOCATION_ID( string session_id, string p_allocation_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ALLOCATION_BY_ALLOCATION_ID( " + "'" + p_allocation_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ALLOCATION_BY_ALLOCATION_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_ALLOCATION( string session_id, string p_allocation_id, string p_order_id, string p_account_id, string p_instrument_id, string p_side_code, string p_allocation_qty, string p_released_qty, string p_executed_qty, string p_executed_val, string p_average_prc, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_ALLOCATION( " + "'" + p_allocation_id + "'," + "'" + p_order_id + "'," + p_account_id + "," + p_instrument_id + "," + p_side_code + "," + "'" + p_allocation_qty + "'," + "'" + p_released_qty + "'," + "'" + p_executed_qty + "'," + "'" + p_executed_val + "'," + "'" + p_average_prc + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_ALLOCATION" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_RELEASE( string session_id, string p_id, string p_release_id, string p_session_id, string p_account_id, string p_order_id, string p_allocation_id, string p_investment_system_id, string p_instrument_id, string p_compliance_audit_id, string p_client_order_id, string p_counterparty_order_id, string p_release_status, string p_release_type, string p_tif, string p_side_code, string p_release_qty, string p_limit_prc, string p_stop_prc, string p_average_prc, string p_executed_val, string p_executed_qty, string p_added_datetime, string p_added_by, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_RELEASE( " + p_id + "," + "'" + p_release_id + "'," + "'" + p_session_id + "'," + p_account_id + "," + "'" + p_order_id + "'," + "'" + p_allocation_id + "'," + "'" + p_investment_system_id + "'," + p_instrument_id + "," + "'" + p_compliance_audit_id + "'," + "'" + p_client_order_id + "'," + "'" + p_counterparty_order_id + "'," + p_release_status + "," + p_release_type + "," + p_tif + "," + p_side_code + "," + "'" + p_release_qty + "'," + "'" + p_limit_prc + "'," + "'" + p_stop_prc + "'," + "'" + p_average_prc + "'," + "'" + p_executed_val + "'," + "'" + p_executed_qty + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_RELEASE" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ALLOCATIONS_BY_ORDER_ID( string session_id, string p_order_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ALLOCATIONS_BY_ORDER_ID( " + "'" + p_order_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ALLOCATIONS_BY_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INSTRUMENT_BY_INSTRUMENT_ID( string session_id, string p_instrument_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INSTRUMENT_BY_INSTRUMENT_ID( " + p_instrument_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INSTRUMENT_BY_INSTRUMENT_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_OVERNIGHT_POSITIONS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_OVERNIGHT_POSITIONS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_OVERNIGHT_POSITIONS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACTIVE_INSTRUMENT_IDS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACTIVE_INSTRUMENT_IDS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACTIVE_INSTRUMENT_IDS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_EXECUTION( string session_id, string p_id, string p_execution_id, string p_session_id, string p_account_id, string p_order_id, string p_release_id, string p_investment_system_id, string p_instrument_id, string p_client_order_id, string p_counterparty_execution_id, string p_side_code, string p_execution_ccy, string p_execution_status, string p_execution_prc, string p_execution_qty, string p_execution_val, string p_leaves_qty, string p_average_prc, string p_leg_type, string p_added_datetime, string p_added_by, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_EXECUTION( " + p_id + "," + "'" + p_execution_id + "'," + "'" + p_session_id + "'," + p_account_id + "," + "'" + p_order_id + "'," + "'" + p_release_id + "'," + "'" + p_investment_system_id + "'," + p_instrument_id + "," + "'" + p_client_order_id + "'," + "'" + p_counterparty_execution_id + "'," + p_side_code + "," + "'" + p_execution_ccy + "'," + "'" + p_execution_status + "'," + "'" + p_execution_prc + "'," + "'" + p_execution_qty + "'," + "'" + p_execution_val + "'," + "'" + p_leaves_qty + "'," + "'" + p_average_prc + "'," + p_leg_type + "," + "'" + p_added_datetime + "'," + "'" + p_added_by + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_EXECUTION" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INSTRUMENTS_BY_SYMBOL( string session_id, string p_symbol ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INSTRUMENTS_BY_SYMBOL( " + "'" + p_symbol + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INSTRUMENTS_BY_SYMBOL" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACTIVE_INSTRUMENTS_BY_SYMBOL( string session_id, string p_symbol ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACTIVE_INSTRUMENTS_BY_SYMBOL( " + "'" + p_symbol + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACTIVE_INSTRUMENTS_BY_SYMBOL" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_EXECUTION_BY_EXECUTION_ID( string session_id, string p_execution_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_EXECUTION_BY_EXECUTION_ID( " + "'" + p_execution_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_EXECUTION_BY_EXECUTION_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage DELETE_POSITION_OVERNIGHT( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call DELETE_POSITION_OVERNIGHT";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "DELETE_POSITION_OVERNIGHT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ORDER_BY_ORDER_ID( string session_id, string p_order_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ORDER_BY_ORDER_ID( " + "'" + p_order_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ORDER_BY_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage CLEARDOWN( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call CLEARDOWN";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "CLEARDOWN" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_RELEASE_BY_RELEASE_ID( string session_id, string p_release_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_RELEASE_BY_RELEASE_ID( " + "'" + p_release_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_RELEASE_BY_RELEASE_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_OPEN_ORDERS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_OPEN_ORDERS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_OPEN_ORDERS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_APPLICATIONS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_APPLICATIONS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_APPLICATIONS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_USER( string session_id, string p_username, string p_password, string p_firstname, string p_lastname ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_USER( " + "'" + p_username + "'," + "'" + p_password + "'," + "'" + p_firstname + "'," + "'" + p_lastname + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_USER" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_USER( string session_id, string p_user_id, string p_username, string p_password, string p_firstname, string p_lastname, string p_is_active ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_USER( " + p_user_id + "," + "'" + p_username + "'," + "'" + p_password + "'," + "'" + p_firstname + "'," + "'" + p_lastname + "'," + p_is_active + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_USER" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_USERS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_USERS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_USERS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACCOUNTS_BY_USER( string session_id, string p_user_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACCOUNTS_BY_USER( " + p_user_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACCOUNTS_BY_USER" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage USER_LOGIN( string session_id, string p_username, string p_password, string p_application_id, string p_session ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call USER_LOGIN( " + "'" + p_username + "'," + "'" + p_password + "'," + "'" + p_application_id + "'," + "'" + p_session + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "USER_LOGIN" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_USER_APPLICATION( string session_id, string p_user_id, string p_application_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_USER_APPLICATION( " + p_user_id + "," + "'" + p_application_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_USER_APPLICATION" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_APPLICATIONS_BY_USER( string session_id, string p_user_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_APPLICATIONS_BY_USER( " + p_user_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_APPLICATIONS_BY_USER" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INVESTMENT_SYSTEMS_BY_USER( string session_id, string p_user_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INVESTMENT_SYSTEMS_BY_USER( " + p_user_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INVESTMENT_SYSTEMS_BY_USER" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INSTRUMENTS_BY_USER( string session_id, string p_user_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INSTRUMENTS_BY_USER( " + p_user_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INSTRUMENTS_BY_USER" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage DELETE_USER_APPLICATION_BY_USER_ID( string session_id, string p_user_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call DELETE_USER_APPLICATION_BY_USER_ID( " + p_user_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "DELETE_USER_APPLICATION_BY_USER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_OPEN_TRADESTATION_ORDERS2( string session_id, string p_instrument_id, string p_strategy_id, string p_open_close, string p_order_type ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_OPEN_TRADESTATION_ORDERS2( " + p_instrument_id + "," + "'" + p_strategy_id + "'," + "'" + p_open_close + "'," + "'" + p_order_type + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_OPEN_TRADESTATION_ORDERS2" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_TRADESTATION_ORDER_BY_ORDER_ID( string session_id, string p_order_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_TRADESTATION_ORDER_BY_ORDER_ID( " + "'" + p_order_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_TRADESTATION_ORDER_BY_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage DIREXION_DUPLICATE_TEST( string session_id, string p_instrument_id, string p_strategy_id, string p_order_type, string p_side, string p_price ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call DIREXION_DUPLICATE_TEST( " + p_instrument_id + "," + "'" + p_strategy_id + "'," + "'" + p_order_type + "'," + "'" + p_side + "'," + "'" + p_price + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "DIREXION_DUPLICATE_TEST" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_OPEN_TRADESTATION_ORDERS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_OPEN_TRADESTATION_ORDERS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_OPEN_TRADESTATION_ORDERS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_TRADESTATION_ORDER( string session_id, string p_tradestation_order_id, string p_order_id, string p_is_executed, string p_is_open, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_TRADESTATION_ORDER( " + p_tradestation_order_id + "," + "'" + p_order_id + "'," + p_is_executed + "," + p_is_open + "," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_TRADESTATION_ORDER" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage DIREXION_GET_TRADESTATION_ORDER( string session_id, string p_instrument_id, string p_strategy_id, string p_order_type, string p_side, string p_price ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call DIREXION_GET_TRADESTATION_ORDER( " + p_instrument_id + "," + "'" + p_strategy_id + "'," + "'" + p_order_type + "'," + "'" + p_side + "'," + "'" + p_price + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "DIREXION_GET_TRADESTATION_ORDER" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_TRADESTATION_ORDER_BY_TRADESTATION_ORDER_ID( string session_id, string p_tradestation_order_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_TRADESTATION_ORDER_BY_TRADESTATION_ORDER_ID( " + p_tradestation_order_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_TRADESTATION_ORDER_BY_TRADESTATION_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_TRADESTATION_ORDER( string session_id, string p_strategy_id, string p_order_data, string p_order_status, string p_order_type, string p_open_close, string p_signal, string p_timestamp, string p_side, string p_quantity, string p_symbol, string p_price, string p_instrument_id, string p_instrument_name, string p_signal_quantity, string p_signal_price, string p_expiration_dt, string p_is_executed, string p_is_open, string p_added_datetime, string p_added_by, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_TRADESTATION_ORDER( " + "'" + p_strategy_id + "'," + "'" + p_order_data + "'," + "'" + p_order_status + "'," + "'" + p_order_type + "'," + "'" + p_open_close + "'," + "'" + p_signal + "'," + "'" + p_timestamp + "'," + "'" + p_side + "'," + "'" + p_quantity + "'," + "'" + p_symbol + "'," + "'" + p_price + "'," + p_instrument_id + "," + "'" + p_instrument_name + "'," + "'" + p_signal_quantity + "'," + "'" + p_signal_price + "'," + "'" + p_expiration_dt + "'," + p_is_executed + "," + p_is_open + "," + "'" + p_added_datetime + "'," + "'" + p_added_by + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_TRADESTATION_ORDER" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_EXPIRATION( string session_id, string p_instrument_id, string p_strategy_id, string p_expiration_dt ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_EXPIRATION( " + p_instrument_id + "," + "'" + p_strategy_id + "'," + "'" + p_expiration_dt + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_EXPIRATION" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_OPEN_TRADESTATION_ORDERS_BY_STRATEGY_ID( string session_id, string p_strategy_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_OPEN_TRADESTATION_ORDERS_BY_STRATEGY_ID( " + "'" + p_strategy_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_OPEN_TRADESTATION_ORDERS_BY_STRATEGY_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_ID( string session_id, string p_instrument_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_ID( " + p_instrument_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_ID( string session_id, string p_instrument_id, string p_strategy_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_ID( " + p_instrument_id + "," + "'" + p_strategy_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_TRADESTATION_INSTRUMENT( string session_id, string p_symbol ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_TRADESTATION_INSTRUMENT( " + "'" + p_symbol + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_TRADESTATION_INSTRUMENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_CLIENT_ORDER_ID( string session_id, string p_client_order_id, string p_correlation_order_id, string p_orig_client_order_id, string p_session_id, string p_release_id, string p_order_id, string p_instrument_id, string p_investment_system_id, string p_added_datetime, string p_added_by, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_CLIENT_ORDER_ID( " + "'" + p_client_order_id + "'," + "'" + p_correlation_order_id + "'," + "'" + p_orig_client_order_id + "'," + "'" + p_session_id + "'," + "'" + p_release_id + "'," + "'" + p_order_id + "'," + p_instrument_id + "," + "'" + p_investment_system_id + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_CLIENT_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_CLIENT_ORDER_ID_MAP_BY_ORDER_ID( string session_id, string p_order_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_CLIENT_ORDER_ID_MAP_BY_ORDER_ID( " + "'" + p_order_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_CLIENT_ORDER_ID_MAP_BY_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_MARKET_DATA_MAP( string session_id, string p_market_data_map_id, string p_instrument_id, string p_session_id, string p_added_datetime, string p_added_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_MARKET_DATA_MAP( " + "'" + p_market_data_map_id + "'," + p_instrument_id + "," + "'" + p_session_id + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_MARKET_DATA_MAP" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage DELETE_MARKET_DATA_MAP( string session_id, string p_market_data_map_id, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call DELETE_MARKET_DATA_MAP( " + "'" + p_market_data_map_id + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "DELETE_MARKET_DATA_MAP" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage CLEAR_MARKET_DATA_MAP( string session_id, string p_session_id, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call CLEAR_MARKET_DATA_MAP( " + "'" + p_session_id + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "CLEAR_MARKET_DATA_MAP" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_CLIENT_ORDER_ID_MAP( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_CLIENT_ORDER_ID_MAP";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_CLIENT_ORDER_ID_MAP" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage DELETE_CLIENT_ORDER_ID_BY_ORDER_ID( string session_id, string p_order_id, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call DELETE_CLIENT_ORDER_ID_BY_ORDER_ID( " + "'" + p_order_id + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "DELETE_CLIENT_ORDER_ID_BY_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_CLIENT_ORDER_ID( string session_id, string p_client_order_id, string p_release_id, string p_order_id, string p_instrument_id, string p_investment_system_id, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_CLIENT_ORDER_ID( " + "'" + p_client_order_id + "'," + "'" + p_release_id + "'," + "'" + p_order_id + "'," + p_instrument_id + "," + "'" + p_investment_system_id + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_CLIENT_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_CLIENT_ORDER_ID_MAP_BY_SESSION_ID( string session_id, string p_session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_CLIENT_ORDER_ID_MAP_BY_SESSION_ID( " + "'" + p_session_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_CLIENT_ORDER_ID_MAP_BY_SESSION_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_FIX_ENGINE_SESSIONS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_FIX_ENGINE_SESSIONS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_FIX_ENGINE_SESSIONS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_MARKET_DATA_MAP_ID_BY_INSTRUMENT_ID( string session_id, string p_instrument_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_MARKET_DATA_MAP_ID_BY_INSTRUMENT_ID( " + p_instrument_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_MARKET_DATA_MAP_ID_BY_INSTRUMENT_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage DELETE_CLIENT_ORDER_ID( string session_id, string p_client_order_id, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call DELETE_CLIENT_ORDER_ID( " + "'" + p_client_order_id + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "DELETE_CLIENT_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_MARKET_DATA_EVENT( string session_id, string p_instrument_id, string p_event_type, string p_event_action, string p_event_prc, string p_event_qty, string p_event_tm, string p_timebar_key, string p_arrival_tm ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_MARKET_DATA_EVENT( " + p_instrument_id + "," + "'" + p_event_type + "'," + "'" + p_event_action + "'," + "'" + p_event_prc + "'," + "'" + p_event_qty + "'," + "'" + p_event_tm + "'," + "'" + p_timebar_key + "'," + "'" + p_arrival_tm + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_MARKET_DATA_EVENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_AUDIT_EVENT( string session_id, string p_order_id, string p_element_id, string p_element_type, string p_audit_desc, string p_added_datetime, string p_added_by, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_AUDIT_EVENT( " + "'" + p_order_id + "'," + "'" + p_element_id + "'," + "'" + p_element_type + "'," + "'" + p_audit_desc + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_AUDIT_EVENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_DEALING_EXECUTION_AUDIT_EVENT( string session_id, string p_request_id, string p_request_type, string p_request_ts, string p_response_id, string p_response_type, string p_response_ts, string p_error_msg, string p_investment_system_id, string p_order_id, string p_release_id, string p_execution_id, string p_session_id, string p_account_id, string p_instrument_id, string p_side_code, string p_execution_status, string p_execution_qty, string p_execution_prc, string p_added_datetime, string p_added_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_DEALING_EXECUTION_AUDIT_EVENT( " + "'" + p_request_id + "'," + "'" + p_request_type + "'," + "'" + p_request_ts + "'," + "'" + p_response_id + "'," + "'" + p_response_type + "'," + "'" + p_response_ts + "'," + "'" + p_error_msg + "'," + "'" + p_investment_system_id + "'," + "'" + p_order_id + "'," + "'" + p_release_id + "'," + "'" + p_execution_id + "'," + "'" + p_session_id + "'," + "'" + p_account_id + "'," + "'" + p_instrument_id + "'," + "'" + p_side_code + "'," + "'" + p_execution_status + "'," + "'" + p_execution_qty + "'," + "'" + p_execution_prc + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_DEALING_EXECUTION_AUDIT_EVENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ORDERS_FROM_ADDED_DATETIME( string session_id, string p_added_datetime ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ORDERS_FROM_ADDED_DATETIME( " + "'" + p_added_datetime + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ORDERS_FROM_ADDED_DATETIME" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_DEALING_RELEASE_AUDIT_EVENT( string session_id, string p_request_id, string p_request_type, string p_request_ts, string p_response_id, string p_response_type, string p_response_ts, string p_error_msg, string p_investment_system_id, string p_order_id, string p_release_id, string p_session_id, string p_account_id, string p_instrument_id, string p_side_code, string p_tif, string p_limit_prc, string p_stop_prc, string p_release_status, string p_release_qty, string p_release_executed_qty, string p_release_average_prc, string p_added_datetime, string p_added_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_DEALING_RELEASE_AUDIT_EVENT( " + "'" + p_request_id + "'," + "'" + p_request_type + "'," + "'" + p_request_ts + "'," + "'" + p_response_id + "'," + "'" + p_response_type + "'," + "'" + p_response_ts + "'," + "'" + p_error_msg + "'," + "'" + p_investment_system_id + "'," + "'" + p_order_id + "'," + "'" + p_release_id + "'," + "'" + p_session_id + "'," + "'" + p_account_id + "'," + "'" + p_instrument_id + "'," + "'" + p_side_code + "'," + "'" + p_tif + "'," + "'" + p_limit_prc + "'," + "'" + p_stop_prc + "'," + "'" + p_release_status + "'," + "'" + p_release_qty + "'," + "'" + p_release_executed_qty + "'," + "'" + p_release_average_prc + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_DEALING_RELEASE_AUDIT_EVENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_DEALING_ORDER_AUDIT_EVENT( string session_id, string p_request_id, string p_request_type, string p_request_ts, string p_response_id, string p_response_type, string p_response_ts, string p_error_msg, string p_investment_system_id, string p_order_id, string p_instrument_id, string p_side_code, string p_tif, string p_limit_prc, string p_stop_prc, string p_order_status, string p_order_qty, string p_order_released_qty, string p_order_executed_qty, string p_order_average_prc, string p_added_datetime, string p_added_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_DEALING_ORDER_AUDIT_EVENT( " + "'" + p_request_id + "'," + "'" + p_request_type + "'," + "'" + p_request_ts + "'," + "'" + p_response_id + "'," + "'" + p_response_type + "'," + "'" + p_response_ts + "'," + "'" + p_error_msg + "'," + "'" + p_investment_system_id + "'," + "'" + p_order_id + "'," + "'" + p_instrument_id + "'," + "'" + p_side_code + "'," + "'" + p_tif + "'," + "'" + p_limit_prc + "'," + "'" + p_stop_prc + "'," + "'" + p_order_status + "'," + "'" + p_order_qty + "'," + "'" + p_order_released_qty + "'," + "'" + p_order_executed_qty + "'," + "'" + p_order_average_prc + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_DEALING_ORDER_AUDIT_EVENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_DEALING_AUDIT_EVENT( string session_id, string p_request_id, string p_request_type, string p_request_ts, string p_response_id, string p_response_type, string p_response_ts, string p_error_msg, string p_added_datetime, string p_added_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_DEALING_AUDIT_EVENT( " + "'" + p_request_id + "'," + "'" + p_request_type + "'," + "'" + p_request_ts + "'," + "'" + p_response_id + "'," + "'" + p_response_type + "'," + "'" + p_response_ts + "'," + "'" + p_error_msg + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_DEALING_AUDIT_EVENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_DEALING_EVENTS_BY_ORDER_ID( string session_id, string p_order_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_DEALING_EVENTS_BY_ORDER_ID( " + "'" + p_order_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_DEALING_EVENTS_BY_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_COMPLIANCE_AUDIT_EVENT( string session_id, string p_compliance_audit_id, string p_session_id, string p_account_id, string p_instrument_id, string p_order_id, string p_allocation_id, string p_release_id, string p_side_code, string p_release_qty, string p_worst_case_long_qty, string p_worst_case_short_qty, string p_curr_open_long_qty, string p_curr_open_short_qty, string p_max_open_long_qty, string p_max_open_short_qty, string p_max_release_qty, string p_result_code, string p_added_datetime, string p_added_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_COMPLIANCE_AUDIT_EVENT( " + p_compliance_audit_id + "," + "'" + p_session_id + "'," + p_account_id + "," + p_instrument_id + "," + "'" + p_order_id + "'," + "'" + p_allocation_id + "'," + "'" + p_release_id + "'," + p_side_code + "," + "'" + p_release_qty + "'," + p_worst_case_long_qty + "," + p_worst_case_short_qty + "," + p_curr_open_long_qty + "," + p_curr_open_short_qty + "," + p_max_open_long_qty + "," + p_max_open_short_qty + "," + p_max_release_qty + "," + "'" + p_result_code + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_COMPLIANCE_AUDIT_EVENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_DEALING_FIX_AUDIT_EVENT( string session_id, string p_request_id, string p_request_type, string p_request_ts, string p_response_id, string p_response_type, string p_response_ts, string p_error_msg, string p_fix_engine_id, string p_session_id, string p_added_datetime, string p_added_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_DEALING_FIX_AUDIT_EVENT( " + "'" + p_request_id + "'," + "'" + p_request_type + "'," + "'" + p_request_ts + "'," + "'" + p_response_id + "'," + "'" + p_response_type + "'," + "'" + p_response_ts + "'," + "'" + p_error_msg + "'," + "'" + p_fix_engine_id + "'," + "'" + p_session_id + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_DEALING_FIX_AUDIT_EVENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_FIX_EVENTS_BY_ORDER_ID( string session_id, string p_order_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_FIX_EVENTS_BY_ORDER_ID( " + "'" + p_order_id + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_FIX_EVENTS_BY_ORDER_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_FIX_AUDIT_EVENT( string session_id, string p_session_id, string p_message_direction, string p_account, string p_avgpx, string p_clordid, string p_cumqty, string p_execid, string p_execrefid, string p_exectranstype, string p_lastpx, string p_lastshares, string p_msgtype, string p_orderid, string p_orderqty, string p_ordstatus, string p_ordtype, string p_origclordid, string p_price, string p_securityid, string p_sendercompid, string p_sendersubid, string p_sendingtime, string p_shares, string p_side, string p_symbol, string p_targetcompid, string p_targetsubid, string p_text, string p_timeinforce, string p_transacttime, string p_stoppx, string p_senderlocationid, string p_exectype, string p_leavesqty, string p_securitytype, string p_maturitymonthyear, string p_customerorfirm, string p_securityexchange, string p_maxshow, string p_manualorderindicator, string p_cticode, string p_correlationclordid, string p_added_datetime, string p_added_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_FIX_AUDIT_EVENT( " + "'" + p_session_id + "'," + "'" + p_message_direction + "'," + "'" + p_account + "'," + "'" + p_avgpx + "'," + "'" + p_clordid + "'," + "'" + p_cumqty + "'," + "'" + p_execid + "'," + "'" + p_execrefid + "'," + "'" + p_exectranstype + "'," + "'" + p_lastpx + "'," + "'" + p_lastshares + "'," + "'" + p_msgtype + "'," + "'" + p_orderid + "'," + "'" + p_orderqty + "'," + "'" + p_ordstatus + "'," + "'" + p_ordtype + "'," + "'" + p_origclordid + "'," + "'" + p_price + "'," + "'" + p_securityid + "'," + "'" + p_sendercompid + "'," + "'" + p_sendersubid + "'," + "'" + p_sendingtime + "'," + "'" + p_shares + "'," + "'" + p_side + "'," + "'" + p_symbol + "'," + "'" + p_targetcompid + "'," + "'" + p_targetsubid + "'," + "'" + p_text + "'," + "'" + p_timeinforce + "'," + "'" + p_transacttime + "'," + "'" + p_stoppx + "'," + "'" + p_senderlocationid + "'," + "'" + p_exectype + "'," + "'" + p_leavesqty + "'," + "'" + p_securitytype + "'," + "'" + p_maturitymonthyear + "'," + "'" + p_customerorfirm + "'," + "'" + p_securityexchange + "'," + "'" + p_maxshow + "'," + "'" + p_manualorderindicator + "'," + "'" + p_cticode + "'," + "'" + p_correlationclordid + "'," + "'" + p_added_datetime + "'," + "'" + p_added_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_FIX_AUDIT_EVENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACCOUNT_LIMITS( string session_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACCOUNT_LIMITS";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACCOUNT_LIMITS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage INSERT_ACCOUNT_LIMIT( string session_id, string p_account_id, string p_symbol, string p_security_exchange, string p_session_id, string p_max_open_long_qty, string p_max_open_short_qty, string p_max_release_qty, string p_is_active, string p_added_datetime, string p_added_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call INSERT_ACCOUNT_LIMIT( " + p_account_id + "," + "'" + p_symbol + "'," + "'" + p_security_exchange + "'," + "'" + p_session_id + "'," + p_max_open_long_qty + "," + p_max_open_short_qty + "," + p_max_release_qty + "," + p_is_active + "," + "'" + p_added_datetime + "'," + "'" + p_added_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "INSERT_ACCOUNT_LIMIT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_ACCOUNT_LIMIT( string session_id, string p_account_id, string p_symbol, string p_security_exchange, string p_session_id, string p_max_open_long_qty, string p_max_open_short_qty, string p_max_release_qty, string p_is_active, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_ACCOUNT_LIMIT( " + p_account_id + "," + "'" + p_symbol + "'," + "'" + p_security_exchange + "'," + "'" + p_session_id + "'," + p_max_open_long_qty + "," + p_max_open_short_qty + "," + p_max_release_qty + "," + p_is_active + "," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_ACCOUNT_LIMIT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage UPDATE_ACCOUNT_LIMIT_SESSION_IDS( string session_id, string p_account_id, string p_session_id, string p_updated_datetime, string p_updated_by ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call UPDATE_ACCOUNT_LIMIT_SESSION_IDS( " + p_account_id + "," + "'" + p_session_id + "'," + "'" + p_updated_datetime + "'," + "'" + p_updated_by + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "UPDATE_ACCOUNT_LIMIT_SESSION_IDS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.UPDATE ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACCOUNT_LIMITS_BY_ACCOUNT_ID( string session_id, string p_account_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACCOUNT_LIMITS_BY_ACCOUNT_ID( " + p_account_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACCOUNT_LIMITS_BY_ACCOUNT_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_ACCOUNT_LIMIT_BY_ACCOUNT_INSTRUMENT_ID( string session_id, string p_account_id, string p_instrument_id ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_ACCOUNT_LIMIT_BY_ACCOUNT_INSTRUMENT_ID( " + p_account_id + "," + p_instrument_id + " )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_ACCOUNT_LIMIT_BY_ACCOUNT_INSTRUMENT_ID" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_NEXT_MARKET_DATA_EVENT( string session_id, string p_instrument_id, string p_begdate, string p_enddate ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_NEXT_MARKET_DATA_EVENT( " + p_instrument_id + "," + "'" + p_begdate + "'," + "'" + p_enddate + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_NEXT_MARKET_DATA_EVENT" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_MARKET_DATA_EVENTS_BY_TIMEBAR_KEY( string session_id, string p_timebar_key ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_MARKET_DATA_EVENTS_BY_TIMEBAR_KEY( " + "'" + p_timebar_key + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_MARKET_DATA_EVENTS_BY_TIMEBAR_KEY" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

    public static Google.ProtocolBuffers.IMessage GET_INSTRUMENT_TIMEBARS( string session_id, string p_instrument_id, string p_timestep, string p_start, string p_stop ) 
    {
        string request_id = Guid.NewGuid().ToString();
        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
        string sql = "call GET_INSTRUMENT_TIMEBARS( " + p_instrument_id + "," + p_timestep + "," + "'" + p_start + "'," + "'" + p_stop + "' )";
        return ClientAdapterRequest.CreateBuilder()
           .SetRequestId( request_id )
           .SetSessionId( session_id )
           .SetTimestamp( timestamp )
           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
           .SetDatabaseRequest(
               DatabaseEngineRequest.CreateBuilder()
                   .SetRequestId( request_id )
                   .SetTimestamp( timestamp )
                   .AddQuery(
                       DatabaseEngineRequest.Types.Query.CreateBuilder()
                           .SetQueryId( "GET_INSTRUMENT_TIMEBARS" )
                           .SetQueryStr( sql )
                           .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
           .Build();
    }

  }
}

