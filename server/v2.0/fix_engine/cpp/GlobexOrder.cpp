
#include <log4cpp/Category.hh>

#include <gmpxx.h>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include <quickfix/FileLog.h>
#include <quickfix/FileStore.h>

#include <quickfix/fix42/TestRequest.h>
#include <quickfix/fix42/SequenceReset.h>
#include <quickfix/fix42/ResendRequest.h>
#include <quickfix/fix42/NewOrderSingle.h>
#include <quickfix/fix42/OrderCancelRequest.h>
#include <quickfix/fix42/OrderCancelReplaceRequest.h>
#include <quickfix/fix42/SecurityDefinitionRequest.h>

#include "GlobexOrder.hpp"

fix_engine::GlobexOrder::GlobexOrder(
    const XMLNode& fix_config,
    const XMLNode& network_config,
    zmq::socket_t * socket )
{
    log = &log4cpp::Category::getInstance( fix_config.getAttribute( "fix_engine_id" ) );

    //
    //  Copy the network configuration
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

    resend_more = false;
    resend_begin_num = 0;

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

void fix_engine::GlobexOrder::onCreate( const FIX::SessionID& sessionID )
{
    log->info( " created session: %s", sessionID.toString().c_str() );
    initNetwork();

    //
    //  Set the lastMsgSeqNum
    //

    lastMsgSeqNum.setValue( FIX::Session::lookupSession( sessionID )->getExpectedTargetNum() - 1 );
}

void fix_engine::GlobexOrder::start()
{
    log->info( " starting session: %s", session_id.toString().c_str() );
    conn->start();
}

void fix_engine::GlobexOrder::shutdown()
{
    log->warn( "Shutdown FIX Engine: %s", session_id.toString().c_str() );
    conn->stop( true );
}

void fix_engine::GlobexOrder::onLogon( const FIX::SessionID& sessionID )
{
    log->warn( " session logon: %s", sessionID.toString().c_str() );
}

void fix_engine::GlobexOrder::onLogout( const FIX::SessionID& sessionID )
{
    log->warn( " session logout: %s", sessionID.toString().c_str() );
}

void fix_engine::GlobexOrder::toAdmin( FIX::Message& message, const FIX::SessionID& sessionID )
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
            log->info( " resetSeqNumFlag = 'Y'" );

            //
            //  Note: reset_on_connect attribute MUST equal 'true'
            //

            message.removeField( 141 );
            message.setField( 141, "Y" );
        }
        else
        {
            log->info( " resetSeqNumFlag = 'N'" );

            message.removeField( 141 );
            message.setField( 141, "N" );
        }

        //
        //  Add the CME header tags:
        //
        //      ApplicationSystemName (1603)
        //      TradingSystemVersion (1604)
        //      ApplicationSystemVendor (1605)
        //

        message.getHeader().setField( 1603, "QuantModelExecutionPlatform" );
        message.getHeader().setField( 1604, "1.0.0.0" );
        message.getHeader().setField( 1605, "QuantModel" );
    }

    if( msgType == FIX42::ResendRequest::MsgType() )
    {
        FIX::EndSeqNo endSeqNo;
        FIX::BeginSeqNo beginSeqNo;

        //
        //  Ensure that the resend request gap is not greater than 2500.
        //

        message.getField( endSeqNo );
        message.getField( beginSeqNo );

        log->info( "Resend request: lastMsgSeqNum = %d, beginSeqNo = %d, endSeqNo = %d",
            lastMsgSeqNum.getValue(),
            beginSeqNo.getValue(),
            endSeqNo.getValue() );

        if( endSeqNo.getValue() > lastMsgSeqNum.getValue() )
        {
            lastMsgSeqNum.setValue( endSeqNo.getValue() );
        }

        if( endSeqNo.getValue() - beginSeqNo.getValue() > 2000 )
        {
            log->warn( " RESEND REQUEST GAP > 2000" );

            endSeqNo.setValue( beginSeqNo.getValue() + 2000 );
            message.removeField( 16 );
            message.setField( endSeqNo );

            resend_more = true;
            resend_begin_num = endSeqNo.getValue();
        }
        else
        {
            endSeqNo.setValue( 0 );
            message.removeField( 16 );
            message.setField( endSeqNo );
        }
    }

    setCMEHeader( message.getHeader() );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( "<<< toAdmin: %s", printMessage( message ).c_str() );
    }

    audit( "OUTGOING", message, sessionID );
}

void fix_engine::GlobexOrder::toApp( FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::DoNotSend )
{
    setCMEHeader( message.getHeader() );
    setCMEBody( message );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( "<<<  toApp: %s", printMessage( message ).c_str() );
    }

    audit( "OUTGOING", message, sessionID );
}

void fix_engine::GlobexOrder::fromAdmin( const FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon )
{
    audit( "INCOMING", message, sessionID );
    handleMessage( message, sessionID );
}

void fix_engine::GlobexOrder::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{
    audit( "INCOMING", message, sessionID );
    handleMessage( message, sessionID );
}

void fix_engine::GlobexOrder::onMessage( const FIX42::ExecutionReport& message, const FIX::SessionID& session )
{
    quantmodel::ExecutionReportData *er_data = NULL;
    std::map< const std::string , fix_engine::ClientOrderIdData * >::const_iterator client_order_id_iter;

    FIX::SendingTime sendingTime;
    FIX::ClOrdID clOrdID;
    FIX::OrigClOrdID origClOrdID;
    FIX::OrderQty orderQty;
    FIX::OrderID orderID;
    FIX::ExecID execID;
    FIX::SecurityID securityID;
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
    FIX::MultiLegReportingType multiLegReportingType;

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( "ExecutionReport: " + printMessage( message ) );
    }

    //
    //  Get the fields from the execution report
    //

    message.get( clOrdID );
    message.get( orderID );
    message.get( execID );
    message.get( securityID );
    message.get( execType );
    message.get( execTransType );
    message.get( ordStatus );
    message.get( symbol );
    message.get( side );
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
    er_data->set_orderid( orderID.getString() );
    er_data->set_execid( execID.getString() );
    er_data->set_securityid( securityID.getString() );
    er_data->set_exectype( execType.getString() );
    er_data->set_exectranstype( execTransType.getString() );
    er_data->set_ordstatus( ordStatus.getString() );
    er_data->set_symbol( symbol.getString() );
    er_data->set_side( side.getString() );
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

    if( message.isSet( leavesQty ) )
    {
        message.get( leavesQty );
        er_data->set_leavesqty( leavesQty.getString() );
    }

    if( message.isSet( orderQty ) )
    {
        message.get( orderQty );
        er_data->set_orderqty( orderQty.getString() );
    }

    if( message.isSet( multiLegReportingType ) )
    {
        message.get( multiLegReportingType );
        er_data->set_multilegreportingtype( multiLegReportingType.getString() );
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
            dealing_request.set_type( quantmodel::DealingEngineRequest::RELEASE_REJECTED );
        }
        else if( execType == FIX::ExecType_RESTATED )
        {
            dealing_request.set_type( quantmodel::DealingEngineRequest::RELEASE_RESTATED );
        }
        else if( execType == FIX::ExecType_EXPIRED )
        {
            dealing_request.set_type( quantmodel::DealingEngineRequest::RELEASE_CANCELED );
        }
        else
        {
            log->error( " Unknown execType: " + execType.getValue() );
            return;
        }
    }
    else if( execTransType == FIX::ExecTransType_CANCEL )
    {
        log->warn( "Received ExecTransType of CANCEL" );
        dealing_request.set_type( quantmodel::DealingEngineRequest::EXECUTION_CANCELED );
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
    quantmodel::ZMQMessage::sendRPC( &dealing_request, dealing );
}

void fix_engine::GlobexOrder::onMessage( const FIX42::Reject& message, const FIX::SessionID& session )
{
    log->error( "Reject: " + printMessage( message ) );

    FIX::Text text;
    FIX::ClOrdID clOrdID;
    FIX::RefSeqNum refSeqNum;
    std::vector< std::string > message_vec;

    //
    //  Find the client order id from the ref seq num and reject it.
    //

    message.getField( text );
    message.getField( refSeqNum );
    const int msg_id = atoi( refSeqNum.getString().c_str() );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " retrieving messsage_id: %d", msg_id );
    }

    FIX::Session::lookupSession( session )->getStore()->get( msg_id, msg_id, message_vec );

    if( message_vec.size() == 1 )
    {
        FIX::Message rejected_message;
        quantmodel::ExecutionReportData *er_data = NULL;
        std::map< const std::string , fix_engine::ClientOrderIdData * >::const_iterator client_order_id_iter;

        rejected_message.setString( message_vec[ 0 ], false );

        if( rejected_message.isSetField( clOrdID ) )
        {
            rejected_message.getField( clOrdID );
        }
        else
        {
            log->error( "Rejected message has no client order id: " + printMessage( rejected_message ) );
            return;
        }

        //
        //  Get the ClientOrderId from the rejected message
        //

        client_order_id_iter = client_order_id_map.find( clOrdID.getValue() );

        if( client_order_id_iter == client_order_id_map.end() )
        {
            log->error( "REJECT ERROR, UNKNOWN CLIENT ORDER ID: " + clOrdID.getValue() );
            return;
        }

        //
        //  Set the release_id on a new execution report data and reject it
        //

        dealing_request.Clear();
        dealing_request.set_type( quantmodel::DealingEngineRequest::RELEASE_REJECTED );

        er_data = dealing_request.mutable_execution_report_data();
        er_data->set_release_id( client_order_id_iter->second->release_id() );
        er_data->set_text( text.getString() );

        //
        //  Send the execution report to the dealing engine
        //

        quantmodel::TimeStamp::getMessageId( dealing_request.mutable_request_id( ) );
        quantmodel::TimeStamp::getUTCTime( dealing_request.mutable_timestamp( ) );
        quantmodel::ZMQMessage::sendRPC( &dealing_request, dealing );
    }
}

void fix_engine::GlobexOrder::onMessage( const FIX42::BusinessMessageReject& message, const FIX::SessionID& session )
{
    log->error( "BusinessMessageReject: " + printMessage( message ) );

    FIX::Text text;
    FIX::ClOrdID clOrdID;
    FIX::RefSeqNum refSeqNum;
    std::vector< std::string > message_vec;

    //
    //  Find the client order id from the ref seq num and reject it.
    //

    message.getField( text );
    message.getField( refSeqNum );
    const int msg_id = atoi( refSeqNum.getString().c_str() );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " retrieving messsage_id: %d", msg_id );
    }

    FIX::Session::lookupSession( session )->getStore()->get( msg_id, msg_id, message_vec );

    if( message_vec.size() == 1 )
    {
        FIX::Message rejected_message;
        quantmodel::ExecutionReportData *er_data = NULL;
        std::map< const std::string , fix_engine::ClientOrderIdData * >::const_iterator client_order_id_iter;

        rejected_message.setString( message_vec[ 0 ], false );

        if( rejected_message.isSetField( clOrdID ) )
        {
            rejected_message.getField( clOrdID );
        }
        else
        {
            log->error( "Rejected message has no client order id: " + printMessage( rejected_message ) );
            return;
        }

        //
        //  Get the ClientOrderId from the rejected message
        //

        client_order_id_iter = client_order_id_map.find( clOrdID.getValue() );

        if( client_order_id_iter == client_order_id_map.end() )
        {
            log->error( "REJECT ERROR, UNKNOWN CLIENT ORDER ID: " + clOrdID.getValue() );
            return;
        }

        //
        //  Set the release_id on a new execution report data and reject it
        //

        dealing_request.Clear();
        dealing_request.set_type( quantmodel::DealingEngineRequest::RELEASE_REJECTED );

        er_data = dealing_request.mutable_execution_report_data();
        er_data->set_release_id( client_order_id_iter->second->release_id() );
        er_data->set_text( text.getString() );

        //
        //  Send the execution report to the dealing engine
        //

        quantmodel::TimeStamp::getMessageId( dealing_request.mutable_request_id( ) );
        quantmodel::TimeStamp::getUTCTime( dealing_request.mutable_timestamp( ) );
        quantmodel::ZMQMessage::sendRPC( &dealing_request, dealing );
    }
}

void fix_engine::GlobexOrder::onMessage( const FIX42::OrderCancelReject& message, const FIX::SessionID& session )
{
    log->error( "OrderCancelReject: " + printMessage( message ) );

    quantmodel::ExecutionReportData *er_data = NULL;
    std::map< const std::string , fix_engine::ClientOrderIdData * >::const_iterator client_order_id_iter;

    FIX::ClOrdID clOrdID;
    message.get( clOrdID );

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
    //  Set the release_id on a new execution report data and reject it
    //

    dealing_request.Clear();
    dealing_request.set_type( quantmodel::DealingEngineRequest::CANCEL_REJECTED );

    er_data = dealing_request.mutable_execution_report_data();
    er_data->set_release_id( client_order_id_iter->second->release_id() );

    //
    //  Send the execution report to the dealing engine
    //

    quantmodel::TimeStamp::getMessageId( dealing_request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( dealing_request.mutable_timestamp( ) );
    quantmodel::ZMQMessage::sendRPC( &dealing_request, dealing );
}

void fix_engine::GlobexOrder::onMessage( const FIX42::SecurityDefinition& message, const FIX::SessionID& session )
{
    log->info( "NOP SecurityDefinition..." );
    log->info( printMessage( message ) );
}

void fix_engine::GlobexOrder::sendNewOrderSingle( const quantmodel::FixEngineRequest& request_msg, quantmodel::FixEngineResponse& response_msg, quantmodel::DatabaseEngineRequest& database_msg )
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
    newOrderSingle.set( FIX::TransactTime( true ) ); // FIX::TransactTime
    newOrderSingle.setField( 1, account_data.market_account_id() ); // FIX::Account
    newOrderSingle.setField( 21, "1" ); // FIX::HandlInst
    newOrderSingle.setField( 204, "0" ); // FIX::CustomerOrFirm [ 0 = Customer, 1 = Firm ]
    newOrderSingle.setField( 9702, "4" ); // FIX::CtiCode [ 4 = Broker/trader trading for any other customer's account ]
    newOrderSingle.setField( 1028, "N" ); // FIX::ManualOrderIndicator [ Y=manual, N=automated ]

    //
    //  Instrument symbology
    //

    newOrderSingle.setField( 55, instrument_data.symbol() ); // FIX::Symbol
    newOrderSingle.setField( 107, instrument_data.security_desc() ); // FIX::SecurityDesc
    newOrderSingle.setField( 167, instrument_data.security_type() ); // FIX::SecurityType

    //
    //  Order quantity -- always set as long
    //

    //log->info( "sendNewOrderSingle: release_data = %s", release_data.ShortDebugString() );

    char quantity_buf[ MAX_DIGITS ];
    memset( quantity_buf, 0, MAX_DIGITS );
    const mpf_class order_qty ( release_data.release_qty() );
    gmp_snprintf( quantity_buf, MAX_DIGITS, INT_FORMAT, &order_qty );
    newOrderSingle.setField( 38, quantity_buf ); // FIX::OrderQty

    //log->info( "sendNewOrderSingle: quantity_buf = %s", quantity_buf );

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
            gmp_snprintf( limit_buf, MAX_DIGITS, INT_FORMAT, &release_prc );
            newOrderSingle.setField( 44, limit_buf ); // FIX::Price

            //log->info( "sendNewOrderSingle: release_data.limit_prc() = %s : limit_buf = %s",
            //    release_data.limit_prc().c_str(), limit_buf );
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
            gmp_snprintf( stop_buf, MAX_DIGITS, INT_FORMAT, &release_prc );
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
            gmp_snprintf( stop_limit_buf, MAX_DIGITS, INT_FORMAT, &release_limit_prc );
            newOrderSingle.setField( 44, stop_limit_buf ); // FIX::Price

            memset( stop_limit_buf, 0, MAX_DIGITS );
            gmp_snprintf( stop_limit_buf, MAX_DIGITS, INT_FORMAT, &release_stop_prc );
            newOrderSingle.setField( 99, stop_limit_buf ); // FIX::StopPx
        }
    }
    else if( release_data.release_type() == "5" ) // MARKET_ON_CLOSE
    {
        newOrderSingle.setField( FIX::OrdType( FIX::OrdType_MARKET_ON_CLOSE ) ); // 40
    }
    else if( release_data.release_type() == "6" ) // MARKET_LIMIT
    {
        newOrderSingle.setField( 40, "K" ); // 40
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
    //  Send to globex
    //

    FIX::Session::sendToTarget( newOrderSingle, session_id );
    response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
}

void fix_engine::GlobexOrder::sendOrderCancelRequest( const quantmodel::FixEngineRequest& request_msg, quantmodel::FixEngineResponse& response_msg, quantmodel::DatabaseEngineRequest& database_msg )
{
    FIX42::OrderCancelRequest orderCancelRequest;
    std::string ts, key, val, next_client_order_id;

    const quantmodel::ReleaseData& release_data = request_msg.release_data();
    const quantmodel::AccountData& account_data = request_msg.account_data();
    const quantmodel::InstrumentData& instrument_data = request_msg.instrument_data();

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

    next_client_order_id.clear();
    quantmodel::TimeStamp::getClientOrderId( next_client_order_id );

    //
    //  Add the cancel client order id to the client order id map and database
    //

    quantmodel::TimeStamp::getUTCTime( ts );

    fix_engine::ClientOrderIdData *cancel_data =
        new fix_engine::ClientOrderIdData(
            next_client_order_id,
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
    //  Create the order cancel request
    //

    orderCancelRequest.setField( 1, account_data.market_account_id() ); // FIX::Account
    orderCancelRequest.setField( 54, release_data.side_code() ); // FIX::Side
    orderCancelRequest.setField( 55, instrument_data.symbol() ); // FIX::Symbol
    orderCancelRequest.setField( 107, instrument_data.security_desc() ); // FIX::SecurityDesc
    orderCancelRequest.setField( 167, instrument_data.security_type() ); // FIX::SecurityType
    orderCancelRequest.setField( 1028, "N" ); // FIX::ManualOrderIndicator [ Y=manual, N=automated ]
    orderCancelRequest.setField( 9717, client_order_id_iter->second->correlation_order_id() ); // FIX::CorrelationClOrdID

    orderCancelRequest.set( FIX::TransactTime( true ) ); // FIX::TransactTime
    orderCancelRequest.set( FIX::ClOrdID( cancel_data->client_order_id() ) ); // 11
    orderCancelRequest.set( FIX::OrderID( release_data.counterparty_order_id() ) ); // 37
    orderCancelRequest.set( FIX::OrigClOrdID( cancel_data->orig_client_order_id() ) ); // 41

    orderCancelRequest.setField( FIX::TransactTime( true ) ); // FIX::TransactTime

    //
    //  Send the order cancel request to globex
    //

    FIX::Session::sendToTarget( orderCancelRequest, session_id );
    response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
}

void fix_engine::GlobexOrder::sendOrderCancelReplaceRequest( const quantmodel::FixEngineRequest& request_msg, quantmodel::FixEngineResponse& response_msg, quantmodel::DatabaseEngineRequest& database_msg )
{
    std::string ts, key, val, next_client_order_id;
    FIX42::OrderCancelReplaceRequest orderCancelReplaceRequest;

    const quantmodel::ReleaseData& release_data = request_msg.release_data();
    const quantmodel::AccountData& account_data = request_msg.account_data();
    const quantmodel::InstrumentData& instrument_data = request_msg.instrument_data();

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

    next_client_order_id.clear();
    quantmodel::TimeStamp::getClientOrderId( next_client_order_id );

    //
    //  Add the cancel client order id to the client order id map and database
    //

    quantmodel::TimeStamp::getUTCTime( ts );

    fix_engine::ClientOrderIdData *cancel_data =
        new fix_engine::ClientOrderIdData(
            next_client_order_id,
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
        std::pair< const std::string, fix_engine::ClientOrderIdData* >
            ( cancel_data->client_order_id(), cancel_data ) );

    cancel_data->insert( database_msg );

    //
    //  Create the order cancel replace request
    //

    orderCancelReplaceRequest.set( FIX::TransactTime( true ) ); // FIX::TransactTime
    orderCancelReplaceRequest.set( FIX::ClOrdID( cancel_data->client_order_id() ) ); // 11
    orderCancelReplaceRequest.set( FIX::OrderID( release_data.counterparty_order_id() ) ); // 37
    orderCancelReplaceRequest.set( FIX::OrigClOrdID( cancel_data->orig_client_order_id() ) ); // 41

    orderCancelReplaceRequest.setField( 1, account_data.market_account_id() ); // FIX::Account
    orderCancelReplaceRequest.setField( 21, "1" ); // FIX::HandlInst
    orderCancelReplaceRequest.setField( 204, "0" ); // FIX::CustomerOrFirm [ 0 = Customer, 1 = Firm ]
    orderCancelReplaceRequest.setField( 1028, "N" ); // FIX::ManualOrderIndicator [ Y=manual, N=automated ]
    orderCancelReplaceRequest.setField( 9702, "4" ); // FIX::CtiCode [ 4 = Broker/trader trading for any other customer's account ]
    orderCancelReplaceRequest.setField( 9717, client_order_id_iter->second->correlation_order_id() ); // FIX::CorrelationClOrdID

    //
    //  Instrument symbology
    //

    orderCancelReplaceRequest.setField( 55, instrument_data.symbol( ) ); // FIX::Symbol
    orderCancelReplaceRequest.setField( 107, instrument_data.security_desc( ) ); // FIX::SecurityDesc
    orderCancelReplaceRequest.setField( 167, instrument_data.security_type( ) ); // FIX::SecurityType

    //
    //  Order quantity -- always set as long
    //

    char quantity_buf[ MAX_DIGITS ];
    memset( quantity_buf, 0, MAX_DIGITS );
    const mpf_class order_qty ( release_data.release_qty() );
    gmp_snprintf( quantity_buf, MAX_DIGITS, INT_FORMAT, &order_qty );
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
            gmp_snprintf( limit_buf, MAX_DIGITS, INT_FORMAT, &release_prc );
            orderCancelReplaceRequest.setField( 44, limit_buf ); // FIX::Price

            //log->info( "sendOrderCancelReplaceRequest: release_data.limit_prc() = %s : limit_buf = %s",
            //    release_data.limit_prc().c_str(), limit_buf );
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
            gmp_snprintf( stop_buf, MAX_DIGITS, INT_FORMAT, &release_prc );
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
            gmp_snprintf( stop_limit_buf, MAX_DIGITS, INT_FORMAT, &release_limit_prc );
            orderCancelReplaceRequest.setField( 44, stop_limit_buf ); // FIX::Price

            memset( stop_limit_buf, 0, MAX_DIGITS );
            gmp_snprintf( stop_limit_buf, MAX_DIGITS, INT_FORMAT, &release_stop_prc );
            orderCancelReplaceRequest.setField( 99, stop_limit_buf ); // FIX::StopPx
        }
    }
    else if( release_data.release_type() == "5" ) // MARKET_ON_CLOSE
    {
        orderCancelReplaceRequest.setField( FIX::OrdType( FIX::OrdType_MARKET_ON_CLOSE ) ); // 40
    }
    else if( release_data.release_type() == "6" ) // MARKET_LIMIT
    {
        orderCancelReplaceRequest.setField( 40, "K" ); // 40
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
    //  Send to globex
    //

    FIX::Session::sendToTarget( orderCancelReplaceRequest, session_id );
    response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
}

void fix_engine::GlobexOrder::sendSecurityDefinitionRequest( const quantmodel::FixEngineRequest& request_msg, quantmodel::FixEngineResponse& response_msg ) const
{
    //
    //  NOP
    //

    response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
}

void fix_engine::GlobexOrder::sendSequenceResetRequest() const
{
    char buf[ 32 ];
    FIX42::SequenceReset sequenceReset;

    sprintf( buf, "%ld", FIX::Session::lookupSession( session_id )->getExpectedTargetNum() - 1 );
    sequenceReset.setField( 36, buf ); // NewSeqNo
    sequenceReset.setField( 123, "Y" ); // GapFillFlag

    //
    //  Send to globex
    //

    FIX::Session::sendToTarget( sequenceReset, session_id );
}

void fix_engine::GlobexOrder::sendResendRequest( const long& begin, const long& end ) const
{
    FIX42::ResendRequest resendRequest;
    FIX::EndSeqNo endSeqNo ( end );
    FIX::BeginSeqNo beginSeqNo ( begin );

    resendRequest.setField( beginSeqNo ); // BeginSeqNo
    resendRequest.setField( endSeqNo ); // EndSeqNo

    //
    //  Send to globex
    //

    FIX::Session::sendToTarget( resendRequest, session_id );
}

void fix_engine::GlobexOrder::sendTestRequest( ) const
{
    FIX42::TestRequest testRequest;
    testRequest.setField( FIX::TestReqID( "TEST" ) );

    //
    //  Send to globex
    //

    FIX::Session::sendToTarget( testRequest, session_id );
}
