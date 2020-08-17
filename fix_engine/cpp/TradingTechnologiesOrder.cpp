
#include <log4cpp/Category.hh>

#include <gmpxx.h>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include <quickfix/FileLog.h>
#include <quickfix/FileStore.h>

#include <quickfix/fix42/NewOrderSingle.h>
#include <quickfix/fix42/OrderCancelRequest.h>
#include <quickfix/fix42/OrderCancelReplaceRequest.h>
#include <quickfix/fix42/SecurityDefinitionRequest.h>

#include "TradingTechnologiesOrder.hpp"

fix_engine::TradingTechnologiesOrder::TradingTechnologiesOrder(
    const XMLNode& fix_config,
    const XMLNode& network_config,
    zmq::socket_t *socket )
{
    log = &log4cpp::Category::getInstance( fix_config.getAttribute( "fix_engine_id" ) );

    //
    //  Copy the xml configurations
    //

    fix = fix_config.deepCopy();
    network = network_config.deepCopy();

    //
    //  Set the zmq socket used for outgoing audits
    //

    client_persistence = socket;

    //
    //  Ensure that our internal zmq sockets are NULL
    //

    ctx = NULL;
    dealing = NULL;
    broadcast = NULL;
    persistence = NULL;

    //
    //  Load the session settings from the fix_config
    //

    stringstream sstream;
    const XMLNode session_node = fix_config.getChildNode( "session" );

    for( int i=0; i<session_node.nChildNode( "param" ); i++ )
    {
        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " parameter: %s", session_node.getChildNode( "param", i ).getClear( 0 ).lpszValue );
        }

        sstream << session_node.getChildNode( "param", i ).getClear( 0 ).lpszValue << endl;
    }

    const FIX::SessionSettings sessionSettings ( sstream );
    FIX::FileStoreFactory storeFactory( sessionSettings );
    FIX::FileLogFactory logFactory( sessionSettings );

    //
    //  Set the session_id
    //

    session_id = *(sessionSettings.getSessions()).begin();

    //
    //  Create the socket initiator
    //

    conn = new FIX::SocketInitiator( (*this),
        storeFactory, sessionSettings, logFactory );
}

void fix_engine::TradingTechnologiesOrder::start()
{
    if( conn->isStopped() )
    {
        log->warn( " Starting FIX Engine: %s", session_id.toString().c_str() );
        conn->start();
    }
    else
    {
        log->warn( " Ignoring request to start FIX Engine %s: already started.", session_id.toString().c_str() );
    }
}

void fix_engine::TradingTechnologiesOrder::shutdown()
{
    log->warn( " Stopping FIX Engine: %s", session_id.toString().c_str() );
    conn->stop( false );
}

void fix_engine::TradingTechnologiesOrder::login()
{
    FIX::Session *session = FIX::Session::lookupSession( session_id );

    if( session->isLoggedOn() == false )
    {
        log->warn( " Login FIX Engine: %s", session_id.toString().c_str() );
        session->logon();
    }
    else
    {
        log->warn( " Ignoring request to login FIX Engine %s: already logged in.", session_id.toString().c_str() );
    }
}

void fix_engine::TradingTechnologiesOrder::logout()
{
    FIX::Session *session = FIX::Session::lookupSession( session_id );

    if( session->isLoggedOn() == true )
    {
        log->warn( " Logout FIX Engine: %s", session_id.toString().c_str() );
        session->logout();
    }
    else
    {
        log->warn( " Ignoring request to logout FIX Engine %s: already logged out.", session_id.toString().c_str() );
    }
}

void fix_engine::TradingTechnologiesOrder::toAdmin( FIX::Message& message, const FIX::SessionID& sessionID )
{
    FIX::MsgType msgType;
    message.getHeader().getField( msgType );

    if( msgType == FIX42::Logon::MsgType() )
    {
        FIX::RawData password ( fix.getAttribute( "password" ) );
        FIX::RawDataLength password_len ( strlen( fix.getAttribute( "password" ) ) );

        message.setField( password_len );
        message.setField( password );

        if( strcmp( "true", fix.getAttribute( "reset_on_connect" ) ) == 0 )
        {
            //
            //  Note: reset_on_connect attribute MUST equal 'true'
            //

            FIX::ResetSeqNumFlag resetSeqNumFlag ( 'Y' );
            message.setField( resetSeqNumFlag );
        }
    }

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( "<<< toAdmin: %s", printMessage( message ).c_str() );
    }

    audit( "OUTGOING", message, sessionID );
}

void fix_engine::TradingTechnologiesOrder::toApp( FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::DoNotSend )
{
    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( "<<<  toApp: %s", printMessage( message ).c_str() );
    }

    audit( "OUTGOING", message, sessionID );
}

void fix_engine::TradingTechnologiesOrder::fromAdmin( const FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon )
{
    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( ">>> fromAdmin: %s", printMessage( message ).c_str() );
    }

    audit( "INCOMING", message, sessionID );
    handleMessage( message, sessionID );
}

void fix_engine::TradingTechnologiesOrder::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{
    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( ">>>  fromApp: %s", printMessage( message ).c_str() );
    }

    audit( "INCOMING", message, sessionID );
    handleMessage( message, sessionID );
}

void fix_engine::TradingTechnologiesOrder::onCreate( const FIX::SessionID& sessionID )
{
    log->info( " created session: %s", sessionID.toString().c_str() );
    initNetwork();
}

void fix_engine::TradingTechnologiesOrder::onLogon( const FIX::SessionID& sessionID )
{
    log->warn( " session logon: %s", sessionID.toString().c_str() );

    //
    //  Tell the dealing engine that we've logged in
    //

    dealing_request.Clear();
    dealing_request.set_type( quantmodel::DealingEngineRequest::FIXENGINE_LOGIN );
    quantmodel::FixEngineData *fix_engine_data = dealing_request.mutable_fix_engine_data();

    fix_engine_data->set_fix_engine_id( fix.getAttribute( "fix_engine_id" ) );
    fix_engine_data->set_session_id( session_id.toString() );

    quantmodel::TimeStamp::getMessageId( dealing_request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( dealing_request.mutable_timestamp( ) );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + dealing_request.ShortDebugString() );
    }

    quantmodel::ZMQMessage::sendRPC( &dealing_request, &dealing_response, dealing );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " --> " + dealing_response.ShortDebugString() );
    }
}

void fix_engine::TradingTechnologiesOrder::onLogout( const FIX::SessionID& sessionID )
{
    log->warn( " session logout: %s", sessionID.toString().c_str() );

    //
    //  Tell the dealing engine that we've logged out
    //

    quantmodel::DealingEngineRequest dealing_request;
    dealing_request.set_type( quantmodel::DealingEngineRequest::FIXENGINE_LOGOUT );
    quantmodel::FixEngineData *fix_engine_data = dealing_request.mutable_fix_engine_data();

    fix_engine_data->set_fix_engine_id( fix.getAttribute( "fix_engine_id" ) );
    fix_engine_data->set_session_id( session_id.toString() );

    quantmodel::TimeStamp::getMessageId( dealing_request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( dealing_request.mutable_timestamp( ) );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + dealing_request.ShortDebugString() );
    }

    quantmodel::ZMQMessage::sendRPC( &dealing_request, &dealing_response, dealing );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " --> " + dealing_response.ShortDebugString() );
    }
}

void fix_engine::TradingTechnologiesOrder::onMessage( const FIX42::ExecutionReport& message, const FIX::SessionID& session )
{
    quantmodel::ExecutionReportData *er_data = NULL;
    std::map< const std::string , fix_engine::ClientOrderIdData * >::const_iterator client_order_id_iter;

    FIX::SendingTime sendingTime;
    FIX::ClOrdID clOrdID;
    FIX::OrigClOrdID origClOrdID;
    FIX::OrderQty orderQty;
    FIX::OrderID orderID;
    FIX::ExecID execID;
    FIX::ExecTransType execTransType;
    FIX::ExecType execType;
    FIX::OrdType ordType;
    FIX::OrdStatus ordStatus;
    FIX::Symbol symbol;
    FIX::Side side;
    FIX::LeavesQty leavesQty;
    FIX::CumQty cumQty;
    FIX::AvgPx avgPx;
    FIX::Price price;
    FIX::StopPx stopPx;
    FIX::LastShares lastShares;
    FIX::LastPx lastPx;
    FIX::Text text;
    FIX::ExecRestatementReason reason;

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( "ExecutionReport: " + printMessage( message ) );
    }

    //
    //  Get the fields from the execution report
    //

    message.get( clOrdID );
    message.get( orderQty );
    message.get( orderID );
    message.get( execID );
    message.get( execType );
    message.get( execTransType );
    message.get( ordStatus );
    message.get( symbol );
    message.get( side );
    message.get( leavesQty );
    message.get( cumQty );
    message.get( avgPx );
    message.getHeader().get( sendingTime );

    //
    //  Get the ClientOrderIdMap for this execution report
    //

    client_order_id_iter = client_order_id_map.find( clOrdID.getValue() );

    if( client_order_id_iter == client_order_id_map.end() )
    {
        log->error( "UNKNOWN CLIENT ORDER ID: " + clOrdID.getValue() );
        return;
    }

    //
    //  Load the execution report data from the execution report message
    //

    dealing_request.Clear();
    er_data = dealing_request.mutable_execution_report_data();

    er_data->set_release_id( client_order_id_iter->second->release_id() );
    er_data->set_session_id( session_id.toString() );
    er_data->set_sending_tm( sendingTime.getString() );
    er_data->set_clordid( clOrdID.getString() );
    er_data->set_orderqty( orderQty.getString() );
    er_data->set_orderid( orderID.getString() );
    er_data->set_execid( execID.getString() );
    er_data->set_exectype( execType.getString() );
    er_data->set_exectranstype( execTransType.getString() );
    er_data->set_ordstatus( ordStatus.getString() );
    er_data->set_symbol( symbol.getString() );
    er_data->set_side( side.getString() );
    er_data->set_leavesqty( leavesQty.getString() );
    er_data->set_cumqty( cumQty.getString() );
    er_data->set_avgpx( avgPx.getString() );

    //
    //  Load the conditional fields
    //

    if( message.isSet( ordType ) )
    {
        message.get( ordType );
        er_data->set_ordtype( ordType.getString() );
    }

    if( message.isSet( price ) )
    {
        message.get( price );
        er_data->set_price( price.getString() );
    }

    if( message.isSet( stopPx ) )
    {
        message.get( stopPx );
        er_data->set_stoppx( stopPx.getString() );
    }

    if( message.isSet( origClOrdID ) )
    {
        message.get( origClOrdID );
        er_data->set_origclordid( origClOrdID.getString() );
    }

    if( message.isSet( text ) )
    {
        message.get( text );
        er_data->set_text( text.getString() );
    }

    if( message.isSet( reason ) )
    {
        message.get( reason );
        er_data->set_text( reason.getString() );
    }

    if( message.isSet( lastShares ) )
    {
        message.get( lastShares );
        er_data->set_lastshares( lastShares.getString() );
    }

    if( message.isSet( lastPx ) )
    {
        message.get( lastPx );
        er_data->set_lastpx( lastPx.getString() );
    }

    //
    //  Parse ExecType (150) for what to do.
    //
    //  Valid values:
    //      0 = New
    //      1 = Partial fill
    //      2 = Fill
    //      3 = Done for day
    //      4 = Canceled
    //      5 = Replace
    //      6 = Pending Cancel (e.g. result of Order Cancel Request)
    //      7 = Stopped
    //      8 = Rejected
    //      9 = Suspended
    //      A = Pending New
    //      B = Calculated
    //      C = Expired
    //      D = Restated (ExecutionRpt sent unsolicited by sellside, with ExecRestatementReason set)
    //      E = Pending Replace (e.g. result of Order Cancel/Replace Request)
    //

    if( execTransType == FIX::ExecTransType_NEW )
    {
        if( execType == FIX::ExecType_NEW )
        {
            dealing_request.set_type( quantmodel::DealingEngineRequest::RELEASE_RESTATED );
        }
        else if( execType == FIX::ExecType_FILL ||
                 execType == FIX::ExecType_PARTIAL_FILL )
        {
            dealing_request.set_type( quantmodel::DealingEngineRequest::CREATE_EXECUTION );
        }
        else if( execType == FIX::ExecType_CANCELED )
        {
            dealing_request.set_type( quantmodel::DealingEngineRequest::RELEASE_CANCELED );
        }
        else if( execType == FIX::ExecType_REPLACE )
        {
            dealing_request.set_type( quantmodel::DealingEngineRequest::RELEASE_REPLACED );
        }
        else if( execType == FIX::ExecType_REJECTED )
        {
            log->warn( "EXECUTION REPORT REJECT: " + printMessage( message ) );
            dealing_request.set_type( quantmodel::DealingEngineRequest::RELEASE_REJECTED );
        }
        else if( execType == FIX::ExecType_RESTATED )
        {
            dealing_request.set_type( quantmodel::DealingEngineRequest::RELEASE_RESTATED );
        }
        else
        {
            log->error( " Unknown execType: " + execType.getValue() );
            return;
        }
    }
    else if( execTransType == FIX::ExecTransType_CANCEL )
    {
        log->warn( "Received unhandled ExecTransType of CANCEL!!!" );
        return;
    }
    else if( execTransType == FIX::ExecTransType_CORRECT )
    {
        log->warn( "Received unhandled ExecTransType of CORRECT!!!" );
        return;
    }
    else if( execTransType == FIX::ExecTransType_STATUS )
    {
        log->warn( "Received unhandled ExecTransType of STATUS!!!" );
        return;
    }
    else
    {
        log->warn( "Received unknown ExecTransType: " + execTransType.getString() );
        return;
    }

    //
    //  Send the execution report to the dealing engine
    //

    quantmodel::TimeStamp::getMessageId( dealing_request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( dealing_request.mutable_timestamp( ) );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + dealing_request.ShortDebugString() );
    }

    quantmodel::ZMQMessage::sendRPC( &dealing_request, &dealing_response, dealing );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " --> " + dealing_response.ShortDebugString() );
    }
}

void fix_engine::TradingTechnologiesOrder::onMessage( const FIX42::Reject& message, const FIX::SessionID& session )
{
    log->error( "Reject: " + printMessage( message ) );

    FIX::Text text;
    if( message.isSet( text ) )
    {
        message.get( text );
        log->error( " text: " + text.getString() );
    }
}

void fix_engine::TradingTechnologiesOrder::onMessage( const FIX42::OrderCancelReject& message, const FIX::SessionID& session )
{
    log->error( "OrderCancelReject: " + printMessage( message ) );

    FIX::Text text;
    FIX::OrigClOrdID origClOrdID;
    quantmodel::ExecutionReportData *er_data = NULL;
    std::map< const std::string , fix_engine::ClientOrderIdData * >::const_iterator client_order_id_iter;

    message.get( origClOrdID );

    if( message.isSet( text ) )
    {
        message.get( text );
    }

    //
    //  Get the ClientOrderIdMap for this execution report
    //

    client_order_id_iter = client_order_id_map.find( origClOrdID.getValue() );

    if( client_order_id_iter == client_order_id_map.end() )
    {
        log->error( "UNKNOWN CLIENT ORDER ID: " + origClOrdID.getValue() );
        return;
    }

    //
    //  Create the dealing engine request
    //

    dealing_request.Clear();
    dealing_request.set_type( quantmodel::DealingEngineRequest::CANCEL_REJECTED );

    er_data = dealing_request.mutable_execution_report_data();
    er_data->set_release_id( client_order_id_iter->second->release_id() );
    er_data->set_session_id( session_id.toString() );
    er_data->set_text( text.getString() );

    //
    //  Send the execution report to the dealing engine
    //

    quantmodel::TimeStamp::getMessageId( dealing_request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( dealing_request.mutable_timestamp( ) );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + dealing_request.ShortDebugString() );
    }

    quantmodel::ZMQMessage::sendRPC( &dealing_request, &dealing_response, dealing );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " --> " + dealing_response.ShortDebugString() );
    }
}

void fix_engine::TradingTechnologiesOrder::onMessage( const FIX42::SecurityDefinition& message, const FIX::SessionID& session )
{
    log->info( "SecurityDefinition: " + printMessage( message ) );
}

void fix_engine::TradingTechnologiesOrder::sendNewOrderSingle( const quantmodel::FixEngineRequest& request_msg, quantmodel::FixEngineResponse& response_msg, quantmodel::DatabaseEngineRequest& database_msg )
{
    std::string ts, key, val;
    FIX42::NewOrderSingle newOrderSingle;

    const quantmodel::ReleaseData& release_data = request_msg.release_data();
    const quantmodel::AccountData& account_data = request_msg.account_data();
    const quantmodel::InstrumentData& instrument_data = request_msg.instrument_data();

    //
    //  Ensure that the client order id map doesn't already have
    //  this release...
    //

    if( client_order_id_map.count( release_data.client_order_id() ) > 0 )
    {
        //
        //  REJECT
        //

        response_msg.set_type( quantmodel::FixEngineResponse::ERROR );
        response_msg.set_error_msg( "DUPLICATE CLIENT ORDER ID" );
        return;
    }

    const mpf_class exchange_price_factor ( instrument_data.exchange_price_factor() );

    if( cmp( exchange_price_factor, 0 ) == 0 )
    {
        //
        //  REJECT
        //

        response_msg.set_type( quantmodel::FixEngineResponse::ERROR );
        response_msg.set_error_msg( "INVALID EXCHANGE PRICE FACTOR" );
        return;
    }

    //
    //  Add the release to the client order id map
    //

    quantmodel::TimeStamp::getUTCTime( ts );

    fix_engine::ClientOrderIdData* data =
        new fix_engine::ClientOrderIdData(
            release_data.client_order_id(),
            release_data.client_order_id(),
            release_data.client_order_id(),
            release_data.session_id(),
            release_data.release_id(),
            release_data.order_id(),
            release_data.instrument_id(),
            release_data.investment_system_id(),
            ts,
            fix.getAttribute( "fix_engine_id" ) );

    client_order_id_map.insert(
        pair< const std::string, fix_engine::ClientOrderIdData* >
            ( data->client_order_id(), data ) );

    data->insert( database_msg );

    //
    //  Assign static fields
    //

    newOrderSingle.set( FIX::ClOrdID( release_data.client_order_id() ) ); // 11
    newOrderSingle.setField( 1, account_data.market_account_id() ); // FIX::Account
    newOrderSingle.setField( FIX::TransactTime( ) ); // FIX::TransactTime

    //
    //  Instrument symbology
    //

    newOrderSingle.setField( 55, instrument_data.symbol( ) ); // FIX::Symbol
    newOrderSingle.setField( 48, instrument_data.security_id( ) ); // FIX::SecurityID
    newOrderSingle.setField( 207, instrument_data.security_exchange( ) ); // FIX::SecurityExchange

    //
    //  Order quantity -- always set as long
    //

    char quantity_buf[ MAX_DIGITS ];
    memset( quantity_buf, 0, MAX_DIGITS );
    const mpf_class order_qty ( release_data.release_qty() );
    sprintf( quantity_buf, "%ld", order_qty.get_si() );
    newOrderSingle.setField( 38, quantity_buf ); // FIX::OrderQty

    //
    //  Determine side from side_code field
    //

    if( release_data.side_code() == "1" ) // BUY
    {
        newOrderSingle.setField( FIX::Side( FIX::Side_BUY ) ); // 54
    }
    else
    {
        newOrderSingle.setField( FIX::Side(FIX::Side_SELL ) ); // 54
    }

    //
    //  Determine order type from order_type field
    //

    if( release_data.release_type() == "1" ) // MARKET
    {
        newOrderSingle.setField( FIX::OrdType( FIX::OrdType_MARKET ) ); // 40
    }
    else if( release_data.release_type() == "2" ) // LIMIT
    {
        newOrderSingle.setField( FIX::OrdType( FIX::OrdType_LIMIT ) ); // 40

        //
        //  If the exchange_price_factor is '1', simply use the actual limit price
        //

        if( cmp( exchange_price_factor, 1 ) == 0 )
        {
            newOrderSingle.setField( 44, release_data.limit_prc() ); // FIX::Price
        }
        else
        {
            char limit_buf[ MAX_DIGITS ];
            memset( limit_buf, 0, MAX_DIGITS );
            const mpf_class limit_prc ( release_data.limit_prc() );
            const mpf_class release_prc = limit_prc / exchange_price_factor;
            sprintf( limit_buf, "%ld", release_prc.get_si() );
            newOrderSingle.setField( 44, limit_buf ); // FIX::Price
        }
    }
    else if( release_data.release_type() == "3"  ) // STOP
    {
        newOrderSingle.setField( FIX::OrdType( FIX::OrdType_STOP ) ); // 40

        //
        //  If the exchange_price_factor is '1', simply use the actual limit price
        //

        if( cmp( exchange_price_factor, 1 ) == 0 )
        {
            newOrderSingle.setField( 99, release_data.stop_prc() ); // FIX::StopPx
        }
        else
        {
            char stop_buf[ MAX_DIGITS ];
            memset( stop_buf, 0, MAX_DIGITS );
            const mpf_class stop_prc ( release_data.stop_prc() );
            const mpf_class release_prc = stop_prc / exchange_price_factor;
            sprintf( stop_buf, "%ld", release_prc.get_si() );
            newOrderSingle.setField( 99, stop_buf ); // FIX::StopPx
        }
    }
    else if( release_data.release_type() == "4"  ) // STOP_LIMIT
    {
        newOrderSingle.setField( FIX::OrdType( FIX::OrdType_STOP_LIMIT ) ); // 40

        //
        //  If the exchange_price_factor is '1', simply use the actual limit price
        //

        if( cmp( exchange_price_factor, 1 ) == 0 )
        {
            newOrderSingle.setField( 44, release_data.limit_prc() ); // FIX::Price
            newOrderSingle.setField( 99, release_data.stop_prc() ); // FIX::StopPx
        }
        else
        {
            char stop_limit_buf[ MAX_DIGITS ];
            const mpf_class limit_prc ( release_data.limit_prc() );
            const mpf_class stop_prc ( release_data.stop_prc() );
            const mpf_class release_limit_prc = limit_prc / exchange_price_factor;
            const mpf_class release_stop_prc = stop_prc / exchange_price_factor;

            memset( stop_limit_buf, 0, MAX_DIGITS );
            sprintf( stop_limit_buf, "%ld", release_limit_prc.get_si() );
            newOrderSingle.setField( 44, stop_limit_buf ); // FIX::Price

            memset( stop_limit_buf, 0, MAX_DIGITS );
            sprintf( stop_limit_buf, "%ld", release_stop_prc.get_si() );
            newOrderSingle.setField( 99, stop_limit_buf ); // FIX::StopPx
        }
    }
    else if( release_data.release_type() == "5" ) // MARKET_ON_CLOSE
    {
        newOrderSingle.setField( FIX::OrdType( FIX::OrdType_MARKET_ON_CLOSE ) ); // 40
    }
    else
    {
        log->error( "UNKNOWN ORDER TYPE: " + release_data.release_type() );

        response_msg.set_type( quantmodel::FixEngineResponse::ERROR );
        response_msg.set_error_msg( "UNKNOWN ORDER TYPE" );
        return;
    }

    //
    //  Set the TIF
    //

    if( release_data.tif() == "0" )
    {
        newOrderSingle.setField( FIX::TimeInForce( FIX::TimeInForce_DAY ) ); // 59
    }
    else if( release_data.tif() == "1" )
    {
        newOrderSingle.setField( FIX::TimeInForce( FIX::TimeInForce_GOOD_TILL_CANCEL ) ); // 59
    }
    else if( release_data.tif() == "2" )
    {
        newOrderSingle.setField( FIX::TimeInForce( FIX::TimeInForce_AT_THE_OPENING ) ); // 59
    }
    else if( release_data.tif() == "3" )
    {
        newOrderSingle.setField( FIX::TimeInForce( FIX::TimeInForce_IMMEDIATE_OR_CANCEL ) ); // 59
    }
    else if( release_data.tif() == "4" )
    {
        newOrderSingle.setField( FIX::TimeInForce( FIX::TimeInForce_FILL_OR_KILL ) ); // 59
    }
    else if( release_data.tif() == "6" )
    {
        newOrderSingle.setField( FIX::TimeInForce( FIX::TimeInForce_GOOD_TILL_DATE ) ); // 59
    }
    else if( release_data.tif() == "8" )
    {
        newOrderSingle.setField( 59, "8" ); // Good in Session (GIS)
    }
    else
    {
        //
        //  Unknown TIF, return error
        //

        log->error( "UNKNOWN TIME IN FORCE: " + release_data.tif() );
        response_msg.set_type( quantmodel::FixEngineResponse::ERROR );
        response_msg.set_error_msg( "UNKNOWN TIME IN FORCE" );
        return;
    }

    //
    //  Send to order session
    //

    FIX::Session::sendToTarget( newOrderSingle, session_id );

    response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
}

void fix_engine::TradingTechnologiesOrder::sendOrderCancelRequest( const quantmodel::FixEngineRequest& request_msg, quantmodel::FixEngineResponse& response_msg, quantmodel::DatabaseEngineRequest& database_msg )
{
    std::string ts, cancel_client_order_id;
    FIX42::OrderCancelRequest orderCancelRequest;

    const quantmodel::ReleaseData& release_data = request_msg.release_data();

    //
    //  Get the client order id data
    //

    std::map< const std::string, fix_engine::ClientOrderIdData* >::const_iterator client_order_id_iter;
    client_order_id_iter = client_order_id_map.find( release_data.client_order_id() );

    if( client_order_id_iter == client_order_id_map.end() )
    {
        //
        //  REJECT
        //

        response_msg.set_type( quantmodel::FixEngineResponse::ERROR );
        response_msg.set_error_msg( "UNKNOWN CLIENT ORDER ID" );
        return;
    }

    //
    //  Get the next client order id
    //

    cancel_client_order_id.clear();
    quantmodel::TimeStamp::getClientOrderId( cancel_client_order_id );

    //
    //  Add the cancel client order id to the client order id map and database
    //

    quantmodel::TimeStamp::getUTCTime( ts );

    fix_engine::ClientOrderIdData *cancel_data =
        new fix_engine::ClientOrderIdData(
            cancel_client_order_id,
            client_order_id_iter->second->correlation_order_id(),
            client_order_id_iter->second->client_order_id(),
            client_order_id_iter->second->session_id(),
            client_order_id_iter->second->release_id(),
            client_order_id_iter->second->order_id(),
            client_order_id_iter->second->instrument_id(),
            client_order_id_iter->second->investment_system_id(),
            ts,
            fix.getAttribute( "fix_engine_id" ) );

    client_order_id_map.insert(
        pair< const std::string, fix_engine::ClientOrderIdData* >
            ( cancel_data->client_order_id(), cancel_data ) );

    cancel_data->insert( database_msg );

    //
    //  Send the cancel request to the counterparty
    //

    orderCancelRequest.set( FIX::ClOrdID( cancel_data->client_order_id() ) ); // 11
    orderCancelRequest.set( FIX::OrigClOrdID( cancel_data->orig_client_order_id() ) ); // 41

    FIX::Session::sendToTarget( orderCancelRequest, session_id );

    response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
}

void fix_engine::TradingTechnologiesOrder::sendOrderCancelReplaceRequest( const quantmodel::FixEngineRequest& request_msg, quantmodel::FixEngineResponse& response_msg, quantmodel::DatabaseEngineRequest& database_msg )
{
    std::string ts, key, val, replace_client_order_id;
    FIX42::OrderCancelReplaceRequest orderCancelReplaceRequest;

    const quantmodel::ReleaseData& release_data = request_msg.release_data();
    const quantmodel::AccountData& account_data = request_msg.account_data();
    const quantmodel::InstrumentData& instrument_data = request_msg.instrument_data();

    //
    //  Get the client order id data for the release
    //

    std::map< const std::string, fix_engine::ClientOrderIdData* >::const_iterator client_order_id_iter;
    client_order_id_iter = client_order_id_map.find( release_data.client_order_id() );

    if( client_order_id_iter == client_order_id_map.end() )
    {
        //
        //  REJECT
        //

        response_msg.set_type( quantmodel::FixEngineResponse::ERROR );
        response_msg.set_error_msg( "UNKNOWN CLIENT ORDER ID" );
        return;
    }

    const mpf_class exchange_price_factor ( instrument_data.exchange_price_factor() );

    if( cmp( exchange_price_factor, 0 ) == 0 )
    {
        //
        //  REJECT
        //

        response_msg.set_type( quantmodel::FixEngineResponse::ERROR );
        response_msg.set_error_msg( "INVALID EXCHANGE PRICE FACTOR" );
        return;
    }

    //
    //  Get the next client order id
    //

    quantmodel::TimeStamp::getClientOrderId( replace_client_order_id );

    //
    //  Add the replace client order id to the client order id map and database
    //

    quantmodel::TimeStamp::getUTCTime( ts );

    fix_engine::ClientOrderIdData *replace_data =
        new fix_engine::ClientOrderIdData(
            replace_client_order_id,
            client_order_id_iter->second->correlation_order_id(),
            client_order_id_iter->second->client_order_id(),
            client_order_id_iter->second->session_id(),
            client_order_id_iter->second->release_id(),
            client_order_id_iter->second->order_id(),
            client_order_id_iter->second->instrument_id(),
            client_order_id_iter->second->investment_system_id(),
            ts,
            fix.getAttribute( "fix_engine_id" ) );

    client_order_id_map.insert(
        pair< const std::string, fix_engine::ClientOrderIdData* >
            ( replace_data->client_order_id(), replace_data ) );

    replace_data->insert( database_msg );

    //
    //  Assign static fields
    //

    orderCancelReplaceRequest.set( FIX::ClOrdID( replace_data->client_order_id() ) ); // 11
    orderCancelReplaceRequest.set( FIX::OrigClOrdID( replace_data->orig_client_order_id() ) ); // 41
    orderCancelReplaceRequest.setField( 1, account_data.market_account_id() ); // FIX::Account
    orderCancelReplaceRequest.setField( FIX::TransactTime( ) ); // FIX::TransactTime

    //
    //  Instrument symbology
    //

    orderCancelReplaceRequest.setField( 55, instrument_data.symbol( ) ); // FIX::Symbol
    orderCancelReplaceRequest.setField( 48, instrument_data.security_id( ) ); // FIX::SecurityID
    orderCancelReplaceRequest.setField( 207, instrument_data.security_exchange( ) ); // FIX::SecurityExchange

    //
    //  Order quantity -- always set as long
    //

    char quantity_buf[ MAX_DIGITS ];
    memset( quantity_buf, 0, MAX_DIGITS );
    const mpf_class order_qty ( release_data.release_qty() );
    sprintf( quantity_buf, "%ld", order_qty.get_si() );
    orderCancelReplaceRequest.setField( 38, quantity_buf ); // FIX::OrderQty

    //
    //  Determine side from side_code field
    //

    if( release_data.side_code() == "1" ) // BUY
    {
        orderCancelReplaceRequest.setField( FIX::Side( FIX::Side_BUY ) ); // 54
    }
    else
    {
        orderCancelReplaceRequest.setField( FIX::Side(FIX::Side_SELL ) ); // 54
    }

    //
    //  Determine order type from order_type field
    //

    if( release_data.release_type() == "1" ) // MARKET
    {
        orderCancelReplaceRequest.setField( FIX::OrdType( FIX::OrdType_MARKET ) ); // 40
    }
    else if( release_data.release_type() == "2" ) // LIMIT
    {
        orderCancelReplaceRequest.setField( FIX::OrdType( FIX::OrdType_LIMIT ) ); // 40

        //
        //  If the exchange_price_factor is '1', simply use the actual limit price
        //

        if( cmp( exchange_price_factor, 1 ) == 0 )
        {
            orderCancelReplaceRequest.setField( 44, release_data.limit_prc() ); // FIX::Price
        }
        else
        {
            char limit_buf[ MAX_DIGITS ];
            memset( limit_buf, 0, MAX_DIGITS );
            const mpf_class limit_prc ( release_data.limit_prc() );
            const mpf_class release_prc = limit_prc / exchange_price_factor;
            sprintf( limit_buf, "%ld", release_prc.get_si() );
            orderCancelReplaceRequest.setField( 44, limit_buf ); // FIX::Price
        }
    }
    else if( release_data.release_type() == "3"  ) // STOP
    {
        orderCancelReplaceRequest.setField( FIX::OrdType( FIX::OrdType_STOP ) ); // 40

        //
        //  If the exchange_price_factor is '1', simply use the actual limit price
        //

        if( cmp( exchange_price_factor, 1 ) == 0 )
        {
            orderCancelReplaceRequest.setField( 99, release_data.stop_prc() ); // FIX::StopPx
        }
        else
        {
            char stop_buf[ MAX_DIGITS ];
            memset( stop_buf, 0, MAX_DIGITS );
            const mpf_class stop_prc ( release_data.stop_prc() );
            const mpf_class release_prc = stop_prc / exchange_price_factor;
            sprintf( stop_buf, "%ld", release_prc.get_si() );
            orderCancelReplaceRequest.setField( 99, stop_buf ); // FIX::StopPx
        }
    }
    else if( release_data.release_type() == "4"  ) // STOP_LIMIT
    {
        orderCancelReplaceRequest.setField( FIX::OrdType( FIX::OrdType_STOP_LIMIT ) ); // 40

        //
        //  If the exchange_price_factor is '1', simply use the actual limit price
        //

        if( cmp( exchange_price_factor, 1 ) == 0 )
        {
            orderCancelReplaceRequest.setField( 44, release_data.limit_prc() ); // FIX::Price
            orderCancelReplaceRequest.setField( 99, release_data.stop_prc() ); // FIX::StopPx
        }
        else
        {
            char stop_limit_buf[ MAX_DIGITS ];
            const mpf_class limit_prc ( release_data.limit_prc() );
            const mpf_class stop_prc ( release_data.stop_prc() );
            const mpf_class release_limit_prc = limit_prc / exchange_price_factor;
            const mpf_class release_stop_prc = stop_prc / exchange_price_factor;

            memset( stop_limit_buf, 0, MAX_DIGITS );
            sprintf( stop_limit_buf, "%ld", release_limit_prc.get_si() );
            orderCancelReplaceRequest.setField( 44, stop_limit_buf ); // FIX::Price

            memset( stop_limit_buf, 0, MAX_DIGITS );
            sprintf( stop_limit_buf, "%ld", release_stop_prc.get_si() );
            orderCancelReplaceRequest.setField( 99, stop_limit_buf ); // FIX::StopPx
        }
    }
    else if( release_data.release_type() == "5" ) // MARKET_ON_CLOSE
    {
        orderCancelReplaceRequest.setField( FIX::OrdType( FIX::OrdType_MARKET_ON_CLOSE ) ); // 40
    }
    else
    {
        log->error( "UNKNOWN ORDER TYPE: " + release_data.release_type() );

        response_msg.set_type( quantmodel::FixEngineResponse::ERROR );
        response_msg.set_error_msg( "UNKNOWN ORDER TYPE" );
        return;
    }

    //
    //  Set the TIF
    //

    if( release_data.tif() == "0" )
    {
        orderCancelReplaceRequest.setField( FIX::TimeInForce( FIX::TimeInForce_DAY ) ); // 59
    }
    else if( release_data.tif() == "1" )
    {
        orderCancelReplaceRequest.setField( FIX::TimeInForce( FIX::TimeInForce_GOOD_TILL_CANCEL ) ); // 59
    }
    else if( release_data.tif() == "2" )
    {
        orderCancelReplaceRequest.setField( FIX::TimeInForce( FIX::TimeInForce_AT_THE_OPENING ) ); // 59
    }
    else if( release_data.tif() == "3" )
    {
        orderCancelReplaceRequest.setField( FIX::TimeInForce( FIX::TimeInForce_IMMEDIATE_OR_CANCEL ) ); // 59
    }
    else if( release_data.tif() == "4" )
    {
        orderCancelReplaceRequest.setField( FIX::TimeInForce( FIX::TimeInForce_FILL_OR_KILL ) ); // 59
    }
    else if( release_data.tif() == "6" )
    {
        orderCancelReplaceRequest.setField( FIX::TimeInForce( FIX::TimeInForce_GOOD_TILL_DATE ) ); // 59
    }
    else if( release_data.tif() == "8" )
    {
        orderCancelReplaceRequest.setField( 59, "8" ); // Good in Session (GIS)
    }
    else
    {
        //
        //  Unknown TIF, return error
        //

        log->error( "UNKNOWN TIME IN FORCE: " + release_data.tif() );
        response_msg.set_type( quantmodel::FixEngineResponse::ERROR );
        response_msg.set_error_msg( "UNKNOWN TIME IN FORCE" );
        return;
    }

    //
    //  Send to order session
    //

    FIX::Session::sendToTarget( orderCancelReplaceRequest, session_id );
    response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
}

void fix_engine::TradingTechnologiesOrder::sendSecurityDefinitionRequest( const quantmodel::FixEngineRequest& request_msg, quantmodel::FixEngineResponse& response_msg, quantmodel::DatabaseEngineRequest& database_msg )
{
    std::string key, val, request_id;
    quantmodel::Instrument *instrument =
        new quantmodel::Instrument( request_msg.instrument_data( ) );

    //
    //  Get a new client order id as the security request id
    //

    request_id.clear();
    quantmodel::TimeStamp::getClientOrderId( request_id );

    FIX42::SecurityDefinitionRequest securityDefinitionRequest;
    securityDefinitionRequest.set( FIX::SecurityReqID( request_id ) );
    securityDefinitionRequest.setField( 17000, "Y" ); // RequestTickTable

    if( instrument->getAttribute( quantmodel::SYMBOL ) != "" )
    {
        securityDefinitionRequest.set( FIX::Symbol(
            instrument->getAttribute( quantmodel::SYMBOL ) ) );
    }

    if( instrument->getAttribute( quantmodel::SECURITY_TYPE ) != "" )
    {
        securityDefinitionRequest.set( FIX::SecurityType(
            instrument->getAttribute( quantmodel::SECURITY_TYPE ) ) );
    }

    if( instrument->getAttribute( quantmodel::SECURITY_EXCHANGE )!= "" )
    {
        securityDefinitionRequest.set( FIX::SecurityExchange(
            instrument->getAttribute( quantmodel::SECURITY_EXCHANGE ) ) );
    }

    FIX::Session::sendToTarget( securityDefinitionRequest, session_id );

    //
    //  Cleanup
    //

    delete instrument;

    response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
}

