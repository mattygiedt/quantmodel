
#include <log4cpp/Category.hh>

#include <gmpxx.h>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include <quickfix/NullStore.h>

#include <quickfix/fix42/MarketDataRequest.h>
#include <quickfix/fix42/MarketDataRequestReject.h>
#include <quickfix/fix42/SecurityDefinitionRequest.h>

#include "TradingTechnologiesPrice.hpp"

fix_engine::TradingTechnologiesPrice::TradingTechnologiesPrice(
    const XMLNode& fix_config,
    const XMLNode& network_config )
{
    log = &log4cpp::Category::getInstance( fix_config.getAttribute( "fix_engine_id" ) );

    //
    //  We need to be able to handle TT intraday logouts
    //

    send_market_data_request_required = false;

    //
    //  Copy the network configuration
    //

    fix = fix_config.deepCopy();
    network = network_config.deepCopy();

    //
    //  Ensure that our zmq sockets are NULL
    //

    ctx = NULL;
    dealing = NULL;
    broadcast = NULL;
    persistence = NULL;

    //
    //  Create the SocketInitiator
    //

    conn = NULL;

    stringstream sstream;
    const XMLNode session_node = fix.getChildNode( "session" );

    for( int i=0; i<session_node.nChildNode( "param" ); ++i )
    {
        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " parameter: %s", session_node.getChildNode( "param", i ).getClear( 0 ).lpszValue );
        }

        sstream << session_node.getChildNode( "param", i ).getClear( 0 ).lpszValue << endl;
    }

    const FIX::SessionSettings sessionSettings ( sstream );
    FIX::NullStoreFactory storeFactory;

    //
    //  Set the session_id
    //

    session_id = *(sessionSettings.getSessions()).begin();

    //
    //  Create the socket initiator
    //

    conn = new FIX::SocketInitiator( (*this), storeFactory, sessionSettings );
}

void fix_engine::TradingTechnologiesPrice::start()
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

void fix_engine::TradingTechnologiesPrice::shutdown()
{
    log->warn( " Stopping FIX Engine: %s", session_id.toString().c_str() );
    conn->stop( false );
}

void fix_engine::TradingTechnologiesPrice::login()
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

void fix_engine::TradingTechnologiesPrice::logout()
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

void fix_engine::TradingTechnologiesPrice::toAdmin( FIX::Message& message, const FIX::SessionID& sessionID )
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
}

void fix_engine::TradingTechnologiesPrice::toApp( FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::DoNotSend )
{
    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( "<<<  toApp: %s", printMessage( message ).c_str() );
    }
}

void fix_engine::TradingTechnologiesPrice::fromAdmin( const FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon )
{
    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( ">>> fromAdmin: %s", printMessage( message ).c_str() );
    }

    handleMessage( message, sessionID );
}

void fix_engine::TradingTechnologiesPrice::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{
    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( ">>>  fromApp: %s", printMessage( message ).c_str() );
    }

    handleMessage( message, sessionID );
}

void fix_engine::TradingTechnologiesPrice::onCreate( const FIX::SessionID& sessionID )
{
    log->info( " created session: %s", sessionID.toString().c_str() );
    initNetwork();
}

void fix_engine::TradingTechnologiesPrice::onLogon( const FIX::SessionID& sessionID )
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

    //if( send_market_data_request_required )
    //{
    //    quantmodel::FixEngineRequest fix_request;
    //    quantmodel::FixEngineResponse fix_response;
    //
    //    for( int i=0; i<fix.nChildNode( "instrument" ); ++i )
    //    {
    //        fix_request.Clear();
    //
    //        fix_request.set_type( quantmodel::FixEngineRequest::MARKETDATA_SUBSCRIBE );
    //        fix_request.set_session_id( session_id.toString() );
    //        quantmodel::TimeStamp::getMessageId( fix_request.mutable_request_id( ) );
    //        quantmodel::TimeStamp::getUTCTime( fix_request.mutable_timestamp( ) );
    //
    //        const XMLNode& instrument_node = fix.getChildNode( "instrument", i );
    //
    //        fix_request.add_instrument_data()->set_instrument_id(
    //            instrument_node.getAttribute( "instrument_id" ) );
    //
    //        sendMarketDataRequest( fix_request, fix_response );
    //    }
    //
    //    send_market_data_request_required = false;
    //}
}

void fix_engine::TradingTechnologiesPrice::onLogout( const FIX::SessionID& sessionID )
{
    log->warn( " session logout: %s", sessionID.toString().c_str() );
    market_data_map.clear();

    //
    //  Tell the dealing engine that we've logged out
    //

    dealing_request.Clear();
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

    //
    //  Set the flag to resend market_data requests on the next login.
    //

    market_data_map.clear();
    send_market_data_request_required = true;
}

void fix_engine::TradingTechnologiesPrice::onMessage( const FIX42::MarketDataSnapshotFullRefresh& message, const FIX::SessionID& session )
{
    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( "MarketDataSnapshotFullRefresh: " + printMessage( message ) );
    }

    FIX::MDReqID mdReqID;
    FIX::SendingTime sendingTime;
    FIX::NoMDEntries noMDEntries;

    quantmodel::FixMarketData *fix_market_data = NULL;
    std::map< const std::string , const std::string >::const_iterator market_data_iter;

    //
    //  Get the market data details
    //

    message.get( mdReqID );
    message.get( noMDEntries );
    message.getHeader().get( sendingTime );

    //
    //  Get the instrument ID from the market_data_map cache
    //

    if( ( market_data_iter = market_data_map.find( mdReqID.getString() ) ) != market_data_map.end() )
    {
        fix_engine_broadcast.Clear();
        fix_engine_broadcast.set_instrument_id( market_data_iter->second );

        //
        //  Build the market data message groups
        //

        std::vector< std::string > mdEntryType;
        std::vector< std::string > mdEntryPx;
        std::vector< std::string > mdEntrySize;

        FIX::FieldMap::const_iterator field_iter;
        for( field_iter = message.begin(); field_iter != message.end(); ++field_iter )
        {
            //std::cout << field_iter->first << "=" << field_iter->second.getString() << std::endl;

            if( field_iter->first == 269 )
            {
                mdEntryType.push_back( field_iter->second.getString() );
            }
            else if( field_iter->first == 270 )
            {
                mdEntryPx.push_back( field_iter->second.getString() );
            }
            else if( field_iter->first == 271 )
            {
                mdEntrySize.push_back( field_iter->second.getString() );
            }
        }

        //
        //  Build the full updates
        //

        const unsigned int group_count = noMDEntries.getValue();
        for( unsigned int i=0; i<group_count; ++i )
        {
            fix_market_data = fix_engine_broadcast.add_fix_market_data();
            fix_market_data->set_instrument_id( market_data_iter->second );
            fix_market_data->set_sending_tm( sendingTime.getString() );

            if( mdEntryType[ i ] == "0" )
            {
                fix_market_data->set_entry_type(
                    quantmodel::FixMarketData::BID );
            }
            else if( mdEntryType[ i ] == "1" )
            {
                fix_market_data->set_entry_type(
                    quantmodel::FixMarketData::ASK );
            }
            else if( mdEntryType[ i ] == "2" )
            {
                fix_market_data->set_entry_type(
                    quantmodel::FixMarketData::TRADE );
            }
            else
            {
                fix_market_data->set_entry_type(
                    quantmodel::FixMarketData::UNKNOWN );
            }

            fix_market_data->set_entry_px( mdEntryPx[ i ] );
            fix_market_data->set_entry_size( mdEntrySize[ i ] );
            fix_market_data->set_update_action( "0" ); // NEW
        }

        //
        //  Send the market data message to the market data engine
        //

        //quantmodel::TimeStamp::getMessageId( *fix_engine_broadcast.mutable_broadcast_id( ) );
        quantmodel::TimeStamp::getUTCTime( fix_engine_broadcast.mutable_timestamp( ) );

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " <-- " + fix_engine_broadcast.ShortDebugString() );
        }

        quantmodel::ZMQMessage::send( &fix_engine_broadcast, broadcast );
    }
    else
    {
        log->warn( "Unknown instrument mapping: " +  mdReqID.getString() );
    }
}

void fix_engine::TradingTechnologiesPrice::onMessage( const FIX42::MarketDataIncrementalRefresh& message, const FIX::SessionID& session )
{
    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( "MarketDataIncrementalRefresh: " + printMessage( message ) );
    }

    FIX::MDReqID mdReqID;
    FIX::SendingTime sendingTime;
    FIX::NoMDEntries noMDEntries;

    quantmodel::FixMarketData *fix_market_data = NULL;
    std::map< const std::string , const std::string >::const_iterator market_data_iter;

    //
    //  Get the market data details
    //

    message.get( mdReqID );
    message.get( noMDEntries );
    message.getHeader().get( sendingTime );

    //
    //  Get the instrument ID from the market_data_map cache
    //

    if( ( market_data_iter = market_data_map.find( mdReqID.getString() ) ) != market_data_map.end() )
    {
        fix_engine_broadcast.Clear();
        fix_engine_broadcast.set_instrument_id( market_data_iter->second );

        //
        //  Build the market data message groups
        //

        std::vector< std::string > mdEntryType;
        std::vector< std::string > mdEntryPx;
        std::vector< std::string > mdEntrySize;
        std::vector< std::string > mdUpdateAction;

        FIX::FieldMap::const_iterator field_iter;
        for( field_iter = message.begin(); field_iter != message.end(); ++field_iter )
        {
            //std::cout << field_iter->first << "=" << field_iter->second.getString() << std::endl;

            if( field_iter->first == 269 )
            {
                mdEntryType.push_back( field_iter->second.getString() );
            }
            else if( field_iter->first == 270 )
            {
                mdEntryPx.push_back( field_iter->second.getString() );
            }
            else if( field_iter->first == 271 )
            {
                mdEntrySize.push_back( field_iter->second.getString() );
            }
            else if( field_iter->first == 279 )
            {
                mdUpdateAction.push_back( field_iter->second.getString() );
            }
        }

        //
        //  Build the incremental updates
        //

        const unsigned int group_count = noMDEntries.getValue();
        for( unsigned int i=0; i<group_count; ++i )
        {
            //
            //  Only publish NEW or UPDATE actions
            //

            if( mdUpdateAction[ i ] == "2" ) continue;

            fix_market_data = fix_engine_broadcast.add_fix_market_data();
            fix_market_data->set_instrument_id( market_data_iter->second );
            fix_market_data->set_sending_tm( sendingTime.getString() );

            if( mdEntryType[ i ] == "0" )
            {
                fix_market_data->set_entry_type(
                    quantmodel::FixMarketData::BID );
            }
            else if( mdEntryType[ i ] == "1" )
            {
                fix_market_data->set_entry_type(
                    quantmodel::FixMarketData::ASK );
            }
            else if( mdEntryType[ i ] == "2" )
            {
                fix_market_data->set_entry_type(
                    quantmodel::FixMarketData::TRADE );
            }
            else
            {
                fix_market_data->set_entry_type(
                    quantmodel::FixMarketData::UNKNOWN );
            }

            fix_market_data->set_entry_px( mdEntryPx[ i ] );
            fix_market_data->set_entry_size( mdEntrySize[ i ] );
            fix_market_data->set_update_action( mdUpdateAction[ i ]  );
        }

        //
        //  Send the market data message to the market data engine
        //

        //quantmodel::TimeStamp::getMessageId( *fix_engine_broadcast.mutable_broadcast_id( ) );
        quantmodel::TimeStamp::getUTCTime( fix_engine_broadcast.mutable_timestamp( ) );

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " <-- " + fix_engine_broadcast.ShortDebugString() );
        }

        quantmodel::ZMQMessage::send( &fix_engine_broadcast, broadcast );
    }
    else
    {
        log->warn( "Unknown instrument mapping: " +  mdReqID.getString() );
    }
}

void fix_engine::TradingTechnologiesPrice::onMessage( const FIX42::MarketDataRequestReject& message, const FIX::SessionID& session )
{
    FIX::Text text;
    FIX::MDReqID mdReqID;

    log->error( "MarketDataRequestReject: " + printMessage( message ) );

    message.get( mdReqID );
    if( message.isSet( text ) )
    {
        message.get( text );
        log->error( " text: " + text.getString() );
    }

    //
    //  Remove request_id from market_data_map
    //

    market_data_map.erase( mdReqID.getString() );
}

void fix_engine::TradingTechnologiesPrice::onMessage( const FIX42::SecurityDefinition& message, const FIX::SessionID& session )
{
    //
    //  Print out the following fields:
    //
    //      48=00A0FM00ESZ
    //      55=ES
    //      200=201212
    //      207=CME
    //      10455=ESZ2
    //
    //  Note that 200 won't be set on spread contracts.
    //

    if( message.isSetField( 200 ) )
    {
        double tick_value = atof( message.getField( 16552 ).c_str() ) *
                            atof( message.getField( 16554 ).c_str() );

        log->info( "SecurityDefinition: 48=%s 55=%s 167=%s 200=%s 207=%s 10455=%s [ tick_size=%s tick_value=%f ]",
            message.getField( 48 ).c_str(),
            message.getField( 55 ).c_str(),
            message.getField( 167 ).c_str(),
            message.getField( 200 ).c_str(),
            message.getField( 207 ).c_str(),
            message.getField( 10455 ).c_str(),
            message.getField( 16552 ).c_str(),
            tick_value );
    }
}

void fix_engine::TradingTechnologiesPrice::sendMarketDataRequest( const quantmodel::FixEngineRequest& request_msg, quantmodel::FixEngineResponse& response_msg )
{
    std::string key, val, request_id;

    std::map< const std::string , const std::string >::const_iterator market_data_iter;

    const quantmodel::InstrumentData& instrument_data = request_msg.instrument_data( );

    //
    //  Test to see if we're already subscribed
    //

    market_data_iter = market_data_map.begin();
    while( market_data_iter != market_data_map.end() )
    {
        if( market_data_iter->second.compare( instrument_data.instrument_id() ) == 0 )
        {
            log->warn( "Instrument %s is already subscribed with request_id %s",
                market_data_iter->second.c_str(), market_data_iter->first.c_str() );

            return;
        }

        ++market_data_iter;
    }

    quantmodel::Instrument *instrument = new quantmodel::Instrument( instrument_data );

    //
    //  Create a new mdReqID
    //

    request_id.clear();
    quantmodel::TimeStamp::getClientOrderId( request_id );

    log->info( " market data subscribe: %s --> %s %s",
        request_id.c_str(),
        instrument->getInstrumentId().c_str(),
        instrument->getName().c_str() );

    //
    //  Add the request_id to the market_data_map
    //

    market_data_map.insert(
        pair< const std::string, const std::string >
            ( request_id, instrument_data.instrument_id() ) );

    //
    //  Subscribe to market data
    //

    FIX42::MarketDataRequest marketDataRequest;

    marketDataRequest.set( FIX::MDReqID( request_id ) ); // 262
    marketDataRequest.set( FIX::SubscriptionRequestType( FIX::SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES ) ); // 263
    marketDataRequest.set( FIX::MarketDepth( 1 ) );      // 264 -- Top of Book
    marketDataRequest.set( FIX::MDUpdateType( 1 ) );     // 265 -- Incremental
    marketDataRequest.set( FIX::AggregatedBook( 'Y' ) ); // 266 -- Y = One book entry per side per price
                                                         //     -- N = Multiple entries per side per price allowed
    //
    //  NoMDEntryTypes group
    //

    FIX42::MarketDataRequest::NoMDEntryTypes entryGroup;

    //
    //  Type Market Data entry
    //      Valid values:
    //      0 = Bid
    //      1 = Offer
    //      2 = Trade
    //      3 = Index Value
    //      4 = Opening Price
    //      5 = Closing Price
    //      6 = Settlement Price
    //      7 = Trading Session High Price
    //      8 = Trading Session Low Price
    //      9 = Trading Session VWAP Price
    //

    entryGroup.set( FIX::MDEntryType( FIX::MDEntryType_BID ) );
    marketDataRequest.addGroup( entryGroup );
    entryGroup.set( FIX::MDEntryType( FIX::MDEntryType_OFFER ) );
    marketDataRequest.addGroup( entryGroup );
    entryGroup.set( FIX::MDEntryType( FIX::MDEntryType_TRADE ) );
    marketDataRequest.addGroup( entryGroup );

    //
    //  NoRelatedSym group
    //

    FIX42::MarketDataRequest::NoRelatedSym symbolGroup;
    symbolGroup.set( FIX::Symbol( instrument->getAttribute( quantmodel::SYMBOL ) ) ); // 55
    symbolGroup.set( FIX::SecurityID( instrument->getAttribute( quantmodel::SECURITY_ID ) ) ); // 48
    symbolGroup.set( FIX::SecurityExchange( instrument->getAttribute( quantmodel::SECURITY_EXCHANGE ) ) ); // 207

    marketDataRequest.addGroup( symbolGroup );

    //
    //  Cleanup
    //

    delete instrument;

    //
    //  Send to price session
    //

    FIX::Session::sendToTarget( marketDataRequest, session_id );

    response_msg.set_type( quantmodel::FixEngineResponse::ACKNOWLEDGEMENT );
}

void fix_engine::TradingTechnologiesPrice::sendSecurityDefinitionRequest( const quantmodel::FixEngineRequest& request_msg, quantmodel::FixEngineResponse& response_msg )
{
    std::string request_id;

    quantmodel::Instrument *instrument =
        new quantmodel::Instrument( request_msg.instrument_data() );

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