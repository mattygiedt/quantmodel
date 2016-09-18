#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <quantmodel.hpp>

void initNetwork( const XMLNode& config, zmq::context_t& ctx, std::map< const std::string , zmq::socket_t* >& socket_map, log4cpp::Category* log )
{
    zmq::socket_t* s = NULL;
    const XMLNode network_node = config.getChildNode( "network" );

    if( network_node.isAttributeSet( "client_request" ) )
    {
        log->info( " client_request network address: %s", network_node.getAttribute( "client_request" ) );
        s = new zmq::socket_t( ctx, ZMQ_REQ );
        s->connect( network_node.getAttribute( "client_request" ) );
        socket_map.insert( pair< const std::string , zmq::socket_t* >( "client_request", s ) );
    }
    else
    {
        log->warn( " client_request network address NOT SET." );
    }

    if( network_node.isAttributeSet( "fix_request" ) )
    {
        log->info( " fix_request network address: %s", network_node.getAttribute( "fix_request" ) );
        s = new zmq::socket_t( ctx, ZMQ_REQ );
        s->connect( network_node.getAttribute( "fix_request" ) );
        socket_map.insert( pair< const std::string , zmq::socket_t* >( "fix_request", s ) );
    }
    else
    {
        log->warn( " fix_request network address NOT SET." );
    }

    if( network_node.isAttributeSet( "fix_callforward" ) )
    {
        log->info( " fix_callforward network address: %s", network_node.getAttribute( "fix_callforward" ) );
        s = new zmq::socket_t( ctx, ZMQ_REQ );
        s->connect( network_node.getAttribute( "fix_callforward" ) );
        socket_map.insert( pair< const std::string , zmq::socket_t* >( "fix_callforward", s ) );
    }
    else
    {
        log->warn( " fix_request network address NOT SET." );
    }

    if( network_node.isAttributeSet( "database_callforward" ) )
    {
        log->info( " connecting to database engine callforward: %s", network_node.getAttribute( "database_callforward" ) );
        s = new zmq::socket_t( ctx, ZMQ_REQ );
        s->connect( network_node.getAttribute( "database_callforward" ) );
        socket_map.insert( pair< const std::string , zmq::socket_t* >( "database_callforward", s ) );
    }
    else
    {
        log->warn( " database_callforward network address NOT SET." );
    }

    if( network_node.isAttributeSet( "broadcast" ) )
    {
        log->info( " connecting to broadcast: %s", network_node.getAttribute( "broadcast" ) );
        s = new zmq::socket_t( ctx, ZMQ_SUB );
        s->connect( network_node.getAttribute( "broadcast" ) );
        socket_map.insert( pair< const std::string , zmq::socket_t* >( "broadcast", s ) );
    }
    else
    {
        log->warn( " broadcast network address NOT SET." );
    }
}

quantmodel::DealingEngineResponse sendRolloverRequest(
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;

    request_msg.Clear();
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::ROLLOVER );

    log->info( "ROLLOVER" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendStatusRequest(
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;

    request_msg.Clear();
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::CURRENT_STATUS );

    log->info( "CURRENT_STATUS" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendActiveSetRequest(
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;

    request_msg.Clear();
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::ACTIVE_SET );

    log->info( "ACTIVE_SET" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendMarketDataSubscribe(
    //const char* session_id,
    //const char* instrument_id,
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;
    quantmodel::FixEngineData *fix_engine_data = NULL;
    quantmodel::InstrumentData *instrument_data = NULL;

    //
    //  Send a market data subscribe
    //

    request_msg.Clear();
    fix_engine_data = request_msg.mutable_fix_engine_data();
    instrument_data = request_msg.add_instrument_data();

    fix_engine_data->set_session_id( test.getAttribute( "session_id" ) );

    //
    //  The following are dummy default values
    //

    instrument_data->set_exchange_price_factor( "1" );

    //
    //  Fields required to subscribe to market data
    //

    instrument_data->set_instrument_id( test.getAttribute( "instrument_id" ) );
    instrument_data->set_symbol( test.getAttribute( "symbol" ) );
    instrument_data->set_security_id( test.getAttribute( "security_id" ) );
    instrument_data->set_security_exchange( test.getAttribute( "security_exchange" ) );

    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::MARKETDATA_SUBSCRIBE );

    log->info( "MARKETDATA_SUBSCRIBE" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendMarketDataUnsubscribe(
    //const char* session_id,
    //const char* instrument_id,
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;
    quantmodel::FixEngineData *fix_engine_data = NULL;
    quantmodel::InstrumentData *instrument_data = NULL;

    //
    //  Send a market data unsubscribe
    //

    request_msg.Clear();
    fix_engine_data = request_msg.mutable_fix_engine_data();
    instrument_data = request_msg.add_instrument_data();

    fix_engine_data->set_session_id( test.getAttribute( "session_id" ) );
    instrument_data->set_instrument_id( test.getAttribute( "instrument_id" ) );

    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::MARKETDATA_UNSUBSCRIBE );

    log->info( "MARKETDATA_UNSUBSCRIBE" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendLogoutFixEngine(
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;
    quantmodel::FixEngineData *fix_engine_data = NULL;

    request_msg.Clear();
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::LOGOUT_FIXENGINE );

    fix_engine_data = request_msg.mutable_fix_engine_data();
    fix_engine_data->set_session_id( test.getAttribute( "session_id" ) );

    log->info( "LOGOUT_FIXENGINE" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendLoginFixEngine(
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;
    quantmodel::FixEngineData *fix_engine_data = NULL;

    request_msg.Clear();
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::LOGIN_FIXENGINE );

    fix_engine_data = request_msg.mutable_fix_engine_data();
    fix_engine_data->set_session_id( test.getAttribute( "session_id" ) );

    log->info( "LOGIN_FIXENGINE" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::FixEngineResponse sendResendRequest(
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::FixEngineRequest request_msg;
    quantmodel::FixEngineResponse response_msg;

    request_msg.Clear();
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::FixEngineRequest::RESEND_REQUEST );
    request_msg.set_session_id( test.getAttribute( "session_id" ) );

    log->info( "RESEND_REQUEST" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::FixEngineResponse sendTestRequest(
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::FixEngineRequest request_msg;
    quantmodel::FixEngineResponse response_msg;

    request_msg.Clear();
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::FixEngineRequest::TEST_REQUEST );
    request_msg.set_session_id( test.getAttribute( "session_id" ) );

    log->info( "TEST_REQUEST" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::FixEngineResponse sendSequenceReset(
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::FixEngineRequest request_msg;
    quantmodel::FixEngineResponse response_msg;

    request_msg.Clear();
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::FixEngineRequest::SEQUENCE_RESET );
    request_msg.set_session_id( test.getAttribute( "session_id" ) );

    log->info( "SEQUENCE_RESET" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::FixEngineResponse sendDefineSecurity(
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::FixEngineRequest request_msg;
    quantmodel::FixEngineResponse response_msg;

    request_msg.Clear();
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::FixEngineRequest::DEFINE_SECURITY );
    request_msg.set_session_id( test.getAttribute( "session_id" ) );

    //
    //  The following are dummy default values
    //

    request_msg.mutable_instrument_data()->set_instrument_id( "PLACEHOLDER" );
    request_msg.mutable_instrument_data()->set_exchange_price_factor( "1" );

    //
    //  Set the parameters used to query for instruments
    //

    if( test.isAttributeSet( "symbol" ) )
    {
        request_msg.mutable_instrument_data()->set_symbol( test.getAttribute( "symbol" ) );
    }

    if( test.isAttributeSet( "security_type" ) )
    {
        request_msg.mutable_instrument_data()->set_security_type( test.getAttribute( "security_type" ) );
    }

    if( test.isAttributeSet( "security_exchange" ) )
    {
        request_msg.mutable_instrument_data()->set_security_exchange( test.getAttribute( "security_exchange" ) );
    }

    log->info( "DEFINE_SECURITY" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendQueryOrderRequest(
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;

    quantmodel::OrderData *order_data = NULL;
    quantmodel::OrderMessage *order_msg = NULL;

    request_msg.Clear();
    order_msg = request_msg.mutable_order();
    order_data = order_msg->mutable_order_data();

    //
    //  Query the dealing engine for the requested order
    //

    if( test.isAttributeSet( "order_id" ) )
    {
        order_data->set_order_id( test.getAttribute( "order_id" ) );
    }

    if( test.isAttributeSet( "external_order_id" ) )
    {
        order_data->set_external_order_id( test.getAttribute( "external_order_id" ) );
    }

    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::QUERY_ORDER );

    log->info( "QUERY_ORDER" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendQueryReleaseRequest(
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;

    quantmodel::OrderMessage *order_msg = NULL;
    quantmodel::ReleaseData *release_data = NULL;

    order_msg = request_msg.mutable_order();
    release_data = order_msg->add_release_data();

    //
    //  Query the dealing engine for the requested release
    //

    release_data->set_release_id( test.getAttribute( "release_id" ) );
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::QUERY_RELEASE );

    log->info( "QUERY_RELEASE" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendCancelReleaseRequest(
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;

    quantmodel::OrderMessage *order_msg = NULL;
    quantmodel::ReleaseData *release_data = NULL;

    order_msg = request_msg.mutable_order();
    release_data = order_msg->add_release_data();

    release_data->set_release_id( test.getAttribute( "release_id" ) );

    response_msg.Clear();
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::CANCEL_RELEASE );

    log->info( "CANCEL_RELEASE" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendReplaceReleaseRequest(
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;

    quantmodel::OrderMessage *order_msg = NULL;
    quantmodel::ReleaseData *release_data = NULL;

    order_msg = request_msg.mutable_order();
    release_data = order_msg->add_release_data();

    release_data->set_release_id( test.getAttribute( "release_id" ) );

    //
    //  Now, update the release_data with the replaced values
    //

    if( test.isAttributeSet( "release_qty" ) )
    {
        release_data->set_release_qty( test.getAttribute( "release_qty" ) );
    }

    if( test.isAttributeSet( "limit_prc" ) )
    {
        release_data->set_limit_prc( test.getAttribute( "limit_prc" ) );
    }

    if( test.isAttributeSet( "stop_prc" ) )
    {
        release_data->set_stop_prc( test.getAttribute( "stop_prc" ) );
    }

    response_msg.Clear();
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::REPLACE_RELEASE );

    log->info( "REPLACE_RELEASE" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendExecuteSignalRequest(
    //const char* investment_system_id,
    //const char* instrument_id,
    //const quantmodel::DraftOrderData::SideCode side_code,
    //const quantmodel::DraftOrderData::TimeInForce tif,
    //const quantmodel::DraftOrderData::OrderType order_type,
    //const char* order_qty,
    //const char* account_id,
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;

    quantmodel::SignalData *signal_data = NULL;

    signal_data = request_msg.add_signal_data();

    signal_data->set_investment_system_id( test.getAttribute( "investment_system_id" ) );
    signal_data->set_instrument_id( test.getAttribute( "instrument_id" ) );
    signal_data->set_side_code( test.getAttribute( "side_code" ) );
    signal_data->set_order_type( test.getAttribute( "order_type" ) );
    signal_data->set_signal_id( test.getAttribute( "signal_id" ) );
    signal_data->set_signal_qty( test.getAttribute( "signal_qty" ) );

    if( strcmp( "LIMIT", test.getAttribute( "order_type" ) ) == 0 )
    {
        signal_data->set_limit_prc( test.getAttribute( "limit_prc" ) );
    }
    else if( strcmp( "STOP", test.getAttribute( "order_type" ) ) == 0 )
    {
        signal_data->set_stop_prc( test.getAttribute( "stop_prc" ) );
    }
    else if( strcmp( "STOP_LIMIT", test.getAttribute( "order_type" ) ) == 0 )
    {
        signal_data->set_stop_prc( test.getAttribute( "stop_prc" ) );
        signal_data->set_limit_prc( test.getAttribute( "limit_prc" ) );
    }

    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::EXECUTE_SIGNAL );

    log->info( "EXECUTE_SIGNAL" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendCreateDraftOrderRequest(
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;

    quantmodel::SignalData *signal_data = NULL;

    signal_data = request_msg.add_signal_data();

    signal_data->set_investment_system_id( test.getAttribute( "investment_system_id" ) );
    signal_data->set_instrument_id( test.getAttribute( "instrument_id" ) );
    signal_data->set_side_code( test.getAttribute( "side_code" ) );
    signal_data->set_order_type( test.getAttribute( "order_type" ) );
    signal_data->set_signal_id( test.getAttribute( "signal_id" ) );
    signal_data->set_signal_qty( test.getAttribute( "signal_qty" ) );

    if( test.isAttributeSet( "external_order_id" ) )
    {
        signal_data->set_external_order_id( test.getAttribute( "external_order_id" ) );
    }

    if( strcmp( "LIMIT", test.getAttribute( "order_type" ) ) == 0 )
    {
        signal_data->set_limit_prc( test.getAttribute( "limit_prc" ) );
    }
    else if( strcmp( "STOP", test.getAttribute( "order_type" ) ) == 0 )
    {
        signal_data->set_stop_prc( test.getAttribute( "stop_prc" ) );
    }
    else if( strcmp( "STOP_LIMIT", test.getAttribute( "order_type" ) ) == 0 )
    {
        signal_data->set_stop_prc( test.getAttribute( "stop_prc" ) );
        signal_data->set_limit_prc( test.getAttribute( "limit_prc" ) );
    }

    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::CREATE_DRAFT_ORDER );

    log->info( "CREATE_DRAFT_ORDER" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendCreateOrderRequest(
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;

    quantmodel::DraftOrderData *draft_order_data = NULL;
    quantmodel::DraftOrderMessage *draft_order_msg = NULL;
    quantmodel::DraftAllocationData *draft_allocation_data = NULL;

    draft_order_msg = request_msg.mutable_draft_order();
    draft_order_data = draft_order_msg->mutable_draft_order_data();

    if( test.isAttributeSet( "external_order_id" ) )
    {
        draft_order_data->set_external_order_id( test.getAttribute( "external_order_id" ) );
    }

    draft_order_data->set_investment_system_id( test.getAttribute( "investment_system_id" ) );
    draft_order_data->set_instrument_id( test.getAttribute( "instrument_id" ) );

    draft_order_data->set_tif( quantmodel::DraftOrderData::DAY );

    if( strcmp( "BUY", test.getAttribute( "side_code" ) ) == 0 )
    {
        draft_order_data->set_side_code( quantmodel::DraftOrderData::BUY );
    }
    else
    {
        draft_order_data->set_side_code( quantmodel::DraftOrderData::SELL );
    }

    if( strcmp( "MARKET", test.getAttribute( "order_type" ) ) == 0 )
    {
        draft_order_data->set_order_type( quantmodel::DraftOrderData::MARKET );
    }
    else if( strcmp( "LIMIT", test.getAttribute( "order_type" ) ) == 0 )
    {
        draft_order_data->set_order_type( quantmodel::DraftOrderData::LIMIT );
        draft_order_data->set_limit_prc( test.getAttribute( "limit_prc" ) );
    }
    else if( strcmp( "STOP", test.getAttribute( "order_type" ) ) == 0 )
    {
        draft_order_data->set_order_type( quantmodel::DraftOrderData::STOP );
        draft_order_data->set_stop_prc( test.getAttribute( "stop_prc" ) );
    }
    else if( strcmp( "STOP_LIMIT", test.getAttribute( "order_type" ) ) == 0 )
    {
        draft_order_data->set_order_type( quantmodel::DraftOrderData::STOP_LIMIT );
        draft_order_data->set_stop_prc( test.getAttribute( "stop_prc" ) );
        draft_order_data->set_limit_prc( test.getAttribute( "limit_prc" ) );
    }
    else if( strcmp( "MARKET_LIMIT", test.getAttribute( "order_type" ) ) == 0 )
    {
        draft_order_data->set_order_type( quantmodel::DraftOrderData::MARKET_LIMIT );
    }
    else
    {
        log->error( "UNKNOWN order_type: %s", test.getAttribute( "order_type" ) );
        return response_msg;
    }

    draft_order_data->set_order_qty( test.getAttribute( "order_qty" ) );

    for( int i=0; i<test.nChildNode( "alloc" ); ++i )
    {
        const XMLNode& alloc = test.getChildNode( "alloc", i );
        draft_allocation_data = draft_order_msg->add_draft_allocation_data();

        draft_allocation_data->set_account_id( alloc.getAttribute( "account_id" ) );
        draft_allocation_data->set_allocation_qty( alloc.getAttribute( "allocation_qty" ) );
    }

    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::CREATE_ORDER );

    log->info( "CREATE_ORDER" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendCancelOrderRequest(
    //const char* order_id,
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;

    quantmodel::OrderData *order_data = NULL;
    quantmodel::OrderMessage *order_msg = NULL;

    request_msg.Clear();
    order_msg = request_msg.mutable_order();
    order_data = order_msg->mutable_order_data();

    order_data->set_order_id( test.getAttribute( "order_id" ) );
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::CANCEL_ORDER );

    log->info( "CANCEL_ORDER" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendReleaseOrderRequest(
    //const char* order_id,
    const XMLNode& test,
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;

    quantmodel::OrderData *order_data = NULL;
    quantmodel::OrderMessage *order_msg = NULL;

    order_msg = request_msg.mutable_order();
    order_data = order_msg->mutable_order_data();

    order_data->set_order_id( test.getAttribute( "order_id" ) );
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::RELEASE_ORDER );

    log->info( "RELEASE_ORDER" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

quantmodel::DealingEngineResponse sendReloadStaticDataRequest(
    zmq::socket_t* socket,
    log4cpp::Category* log )
{
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;

    request_msg.Clear();
    quantmodel::TimeStamp::getMessageId( request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::RELOAD_STATIC_DATA );

    log->info( "RELOAD_STATIC_DATA" );
    log->info( request_msg.ShortDebugString() );
    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, socket );
    log->info( response_msg.ShortDebugString() );

    return response_msg;
}

int main( int argc, char *argv[] )
{
    log4cpp::Category* log;
    std::map< const std::string, zmq::socket_t* > socket_map;

    //
    //  Load the XML configuration
    //

    cout << "Loading configuration file: " << argv[ 1 ] << endl;
    XMLNode config = XMLNode::openFileHelper( argv[ 1 ] );
    //XMLNode config = XMLNode::openFileHelper( "../config/dealingEngine.xml" );

    //
    //  Setup log4cpp
    //

    log4cpp::PropertyConfigurator::configure( "../config/log4cpp.tester.properties" );
    log = &log4cpp::Category::getInstance( "DealingEngineTester" );

    //
    //  Setup ZMQ
    //

    zmq::context_t ctx ( 1 );
    initNetwork( config, ctx, socket_map, log );

    for( int i=0; i<config.nChildNode( "test" ); ++i )
    {
        const XMLNode& test = config.getChildNode( "test", i );

        if( strcmp( "EXECUTE_SIGNAL", test.getAttribute( "action" ) ) == 0 ) { sendExecuteSignalRequest( test, socket_map[ "client_request" ], log ); }
        else if( strcmp( "CREATE_DRAFT_ORDER", test.getAttribute( "action" ) ) == 0 ) { sendCreateDraftOrderRequest( test, socket_map[ "client_request" ], log ); }
        else if( strcmp( "CREATE_ORDER", test.getAttribute( "action" ) ) == 0 ) { sendCreateOrderRequest( test, socket_map[ "client_request" ], log ); }
        else if( strcmp( "UPDATE_ORDER", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "CANCEL_ORDER", test.getAttribute( "action" ) ) == 0 ) { sendCancelOrderRequest( test, socket_map[ "client_request" ], log ); }
        else if( strcmp( "RELEASE_ORDER", test.getAttribute( "action" ) ) == 0 ) { sendReleaseOrderRequest( test, socket_map[ "client_request" ], log ); }
        else if( strcmp( "CANCEL_RELEASE", test.getAttribute( "action" ) ) == 0 ) { sendCancelReleaseRequest( test, socket_map[ "client_request" ], log ); }
        else if( strcmp( "REPLACE_RELEASE", test.getAttribute( "action" ) ) == 0 ) { sendReplaceReleaseRequest( test, socket_map[ "client_request" ], log ); }
        else if( strcmp( "RELEASE_REJECTED", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "CANCEL_REJECTED", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "RELEASE_RESTATED", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "RELEASE_CANCELED", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "RELEASE_REPLACED", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "CREATE_EXECUTION", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "FLATTEN_POSITIONS", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "BOD_POSITIONS", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "BROADCAST_DATA", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "ACTIVE_SET", test.getAttribute( "action" ) ) == 0 ) { sendActiveSetRequest( socket_map[ "client_request" ], log ); }
        else if( strcmp( "QUERY_ORDER", test.getAttribute( "action" ) ) == 0 ) { sendQueryOrderRequest( test, socket_map[ "client_request" ], log ); }
        else if( strcmp( "QUERY_RELEASE", test.getAttribute( "action" ) ) == 0 ) { sendQueryReleaseRequest( test, socket_map[ "client_request" ], log ); }
        else if( strcmp( "QUERY_EXECUTION", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "ROLLOVER", test.getAttribute( "action" ) ) == 0 ) { sendRolloverRequest( socket_map[ "client_request" ], log ); }
        else if( strcmp( "CURRENT_STATUS", test.getAttribute( "action" ) ) == 0 ) { sendStatusRequest( socket_map[ "client_request" ], log ); }
        else if( strcmp( "UPDATE_STATUS", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "MARKETDATA_SUBSCRIBE", test.getAttribute( "action" ) ) == 0 ) { sendMarketDataSubscribe( test, socket_map[ "client_request" ], log ); }
        else if( strcmp( "MARKETDATA_UNSUBSCRIBE", test.getAttribute( "action" ) ) == 0 ) { sendMarketDataUnsubscribe( test, socket_map[ "client_request" ], log ); }
        else if( strcmp( "FIXENGINE_LOGIN", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "FIXENGINE_LOGOUT", test.getAttribute( "action" ) ) == 0 ) {}
        else if( strcmp( "LOGOUT_FIXENGINE", test.getAttribute( "action" ) ) == 0 ) { sendLogoutFixEngine( test, socket_map[ "client_request" ], log ); }
        else if( strcmp( "LOGIN_FIXENGINE", test.getAttribute( "action" ) ) == 0 ) { sendLoginFixEngine( test, socket_map[ "client_request" ], log ); }
        else if( strcmp( "RESEND_REQUEST", test.getAttribute( "action" ) ) == 0 ) { sendResendRequest( test, socket_map[ "fix_callforward" ], log ); }
        else if( strcmp( "TEST_REQUEST", test.getAttribute( "action" ) ) == 0 ) { sendTestRequest( test, socket_map[ "fix_callforward" ], log ); }
        else if( strcmp( "SEQUENCE_RESET", test.getAttribute( "action" ) ) == 0 ) { sendSequenceReset( test, socket_map[ "fix_callforward" ], log ); }
        else if( strcmp( "DEFINE_SECURITY", test.getAttribute( "action" ) ) == 0 ) { sendDefineSecurity( test, socket_map[ "fix_callforward" ], log ); }
        else if( strcmp( "WAIT", test.getAttribute( "action" ) ) == 0 ) { sleep( atoi( test.getAttribute( "delay" ) ) ); }
        else if( strcmp( "RELOAD_STATIC_DATA", test.getAttribute( "action" ) ) == 0 ) { sendReloadStaticDataRequest( socket_map[ "client_request" ], log ); }
        else { log->warn( "UNKNOWN action: %s", test.getAttribute( "action" ) ); }
    }

    exit( 0 );
}
