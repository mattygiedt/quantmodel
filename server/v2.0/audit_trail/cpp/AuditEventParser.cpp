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

void parseOrderQualifier( const std::string& tif, std::string& out )
{
    if( tif == "0" ) { out = "DAY"; }
    else if( tif == "1" ) { out = "GTC"; }
    else if( tif == "4" ) { out = "FAK"; }
    else if( tif == "6" ) { out = "GTD"; }
    else { out = ""; }
}

void parseOrderType( const std::string& order_type, std::string& out )
{
    if( order_type == "1" ) { out = "MARKET"; }
    else if( order_type == "2" ) { out = "LIMIT"; }
    else if( order_type == "3" ) { out = "STOP"; }
    else if( order_type == "4" ) { out = "STOP LIMIT"; }
    else if( order_type == "K" ) { out = "MARKET LIMIT"; }
    else { out = ""; }
}

void parseBuySellIndicator( const std::string& side, std::string& out )
{
    if( side == "1" ) out = "B";
    else if( side == "2" ) out = "S";
    else out = "";
}

void parseMessageType( const std::string& message_type, const std::string& exec_type, std::string& out )
{
    if( message_type == "7" ) { out = "ADVERTISEMENT"; }
    else if( message_type == "J" ) { out = "ALLOCATION"; }
    else if( message_type == "P" ) { out = "ALLOCATION ACK"; }
    else if( message_type == "k" ) { out = "BID REQUEST"; }
    else if( message_type == "l" ) { out = "BID RESPONSE"; }
    else if( message_type == "j" ) { out = "BUSINESS MESSAGE REJECT"; }
    else if( message_type == "Q" ) { out = "DON'T KNOW TRADE"; }
    else if( message_type == "C" ) { out = "EMAIL"; }
    else if( message_type == "0" ) { out = "HEARTBEAT"; }
    else if( message_type == "6" ) { out = "INDICATION OF INTEREST"; }
    else if( message_type == "K" ) { out = "LIST CANCEL REQUEST"; }
    else if( message_type == "L" ) { out = "LIST EXECUTE"; }
    else if( message_type == "N" ) { out = "LIST STATUS"; }
    else if( message_type == "M" ) { out = "LIST STATUS REQUEST"; }
    else if( message_type == "m" ) { out = "LIST STRIKE PRICE"; }
    else if( message_type == "A" ) { out = "LOGON"; }
    else if( message_type == "5" ) { out = "LOGOUT"; }
    else if( message_type == "X" ) { out = "MARKET DATA INCREMENTAL REFRESH"; }
    else if( message_type == "V" ) { out = "MARKET DATA REQUEST"; }
    else if( message_type == "Y" ) { out = "MARKET DATA REJECT"; }
    else if( message_type == "W" ) { out = "MARKET DATA FULL REFRESH"; }
    else if( message_type == "i" ) { out = "MASS QUOTE"; }
    else if( message_type == "B" ) { out = "NEWS"; }
    else if( message_type == "G" ) { out = "ORDER CANCEL/REPLACE REQUEST"; }
    else if( message_type == "9" ) { out = "ORDER CANCEL REJECT"; }
    else if( message_type == "F" ) { out = "ORDER CANCEL REQUEST"; }
    else if( message_type == "E" ) { out = "ORDER - LIST"; }
    else if( message_type == "D" ) { out = "NEW ORDER SINGLE"; }
    else if( message_type == "H" ) { out = "ORDER STATUS REQUEST"; }
    else if( message_type == "S" ) { out = "QUOTE"; }
    else if( message_type == "b" ) { out = "QUOTE ACKNOWLEDGEMENT"; }
    else if( message_type == "Z" ) { out = "QUOTE CANCEL"; }
    else if( message_type == "R" ) { out = "QUOTE REQUEST"; }
    else if( message_type == "a" ) { out = "QUOTE STATUS REQUEST"; }
    else if( message_type == "3" ) { out = "REJECT"; }
    else if( message_type == "2" ) { out = "RESEND REQUEST"; }
    else if( message_type == "d" ) { out = "SECURITY DEFINITION"; }
    else if( message_type == "c" ) { out = "SECURITY DEFINITION REQUEST"; }
    else if( message_type == "f" ) { out = "SECURITY STATUS"; }
    else if( message_type == "e" ) { out = "SECURITY STATUS REQUEST"; }
    else if( message_type == "4" ) { out = "SEQUENCE RESET"; }
    else if( message_type == "T" ) { out = "SETTLEMENT INSTRUCTIONS"; }
    else if( message_type == "1" ) { out = "TEST REQUEST"; }
    else if( message_type == "h" ) { out = "TRADING SESSION STATUS"; }
    else if( message_type == "g" ) { out = "TRADING SESSION STATUS REQUEST"; }
    else if( message_type == "8" )
    {
        if( exec_type == "0" )
        {
            out = "NEW ACK";
        }
        else if( exec_type == "1" )
        {
            out = "PARIAL FILL";
        }
        else if( exec_type == "2" )
        {
            out = "FILL";
        }
        else if( exec_type == "3" )
        {
            out = "DONE";
        }
        else if( exec_type == "4" )
        {
            out = "CANCEL ACK";
        }
        else if( exec_type == "5" )
        {
            out = "REPLACE ACK";
        }
        else if( exec_type == "8" )
        {
            out = "REJECT";
        }
        else if( exec_type == "C" )
        {
            out = "EXPIRED";
        }
        else
        {
            out = "EXECUTION REPORT";
        }
    }
    else { out = "UNKNOWN"; }
}

void parseMessageDirection( const std::string& request_type, std::string& out, std::string& source, std::string& sink )
{
    if( request_type == "CREATE_DRAFT_ORDER" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "CREATE_ORDER" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "UPDATE_ORDER" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "CANCEL_ORDER" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "RELEASE_ORDER" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "CREATE_RELEASE" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "VALIDATE_RELEASE" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "CANCEL_RELEASE" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "REPLACE_RELEASE" ) { out = "FROM FIX ENGINE"; source = "FIX_ENGINE"; sink = "DEALING_ENGINE"; }
    else if( request_type == "RELEASE_REJECTED" ) { out = "FROM FIX ENGINE"; source = "FIX_ENGINE"; sink = "DEALING_ENGINE"; }
    else if( request_type == "CANCEL_REJECTED" ) { out = "FROM FIX ENGINE"; source = "FIX_ENGINE"; sink = "DEALING_ENGINE"; }
    else if( request_type == "RELEASE_RESTATED" ) { out = "FROM FIX ENGINE"; source = "FIX_ENGINE"; sink = "DEALING_ENGINE"; }
    else if( request_type == "RELEASE_CANCELED" ) { out = "FROM FIX ENGINE"; source = "FIX_ENGINE"; sink = "DEALING_ENGINE"; }
    else if( request_type == "RELEASE_REPLACED" ) { out = "FROM FIX ENGINE"; source = "FIX_ENGINE"; sink = "DEALING_ENGINE"; }
    else if( request_type == "CREATE_EXECUTION" ) { out = "FROM FIX ENGINE"; source = "FIX_ENGINE"; sink = "DEALING_ENGINE"; }
    else if( request_type == "EXECUTION_CANCELED" ) { out = "FROM FIX ENGINE"; source = "FIX_ENGINE"; sink = "DEALING_ENGINE"; }
    else if( request_type == "BROADCAST_DATA" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "ACTIVE_SET" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "QUERY_ORDER" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "QUERY_RELEASE" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "QUERY_EXECUTION" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "ROLLOVER" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "CURRENT_STATUS" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "UPDATE_STATUS" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "MARKETDATA_SUBSCRIBE" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "MARKETDATA_UNSUBSCRIBE" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "FIXENGINE_LOGIN" ) { out = "FROM FIX ENGINE"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "FIXENGINE_LOGOUT" ) { out = "FROM FIX ENGINE"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "LOGOUT_FIXENGINE" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "LOGIN_FIXENGINE" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else if( request_type == "RELOAD_STATIC_DATA" ) { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
    else { out = "FROM CLIENT"; source = "CLIENT"; sink = "DEALING_ENGINE"; }
}

void parseEventDatetime( const std::string& added_datetime, std::string& date, std::string& time )
{
    //
    //  1301584455.072294
    //

    char buf[ 32 ];
    const std::string epoch_str = added_datetime.substr( 0, 10 );
    const std::string millis_str = added_datetime.substr( 11, 6 );

    const time_t epoch = atol( epoch_str.c_str() );
    tm *epoch_tm = gmtime( &epoch );

    strftime( buf, 32, "%Y-%m-%d", epoch_tm );
    date = buf;

    strftime( buf, 32, "%H:%M:%S.", epoch_tm );
    time = buf;
    time.append( millis_str );
}

void createHeader( std::string& header )
{
    header.append( "Server Transaction Number\t" )
          .append( "Server Process Date\t" )
          .append( "Server Timestamp\t" )
          .append( "Sender Location ID\t" )
          .append( "Manual Order Identifier\t" )
          .append( "Exchange Code\t" )
          .append( "Message Direction\t" )
          .append( "Message Source\t" )
          .append( "Message Sink\t" )
          .append( "Status\t" )
          .append( "Reason Code / Error Code\t" )
          .append( "Tag 50 ID\t" )
          .append( "Account Number\t" )
          .append( "Executing Firm Number\t" )
          .append( "Session ID\t" )
          .append( "Client Order ID\t" )
          .append( "CorrelationClOrdID\t" )
          .append( "Host Order Number\t" )
          .append( "Message Type\t" )
          .append( "Buy/Sell Indicator\t" )
          .append( "Quantity\t" )
          .append( "Max Show\t" )
          .append( "Instrument/Security Description\t" )
          .append( "Product/Instrument Group Code\t" )
          .append( "Maturity Date\t" )
          .append( "CFI Code\t" )
          .append( "Strike Price\t" )
          .append( "Limit Price\t" )
          .append( "Stop Price\t" )
          .append( "Fill Price\t" )
          .append( "Order Type\t" )
          .append( "Order Qualifier\t" )
          .append( "Customer Type Indicator\t" )
          .append( "Origin\t" )
          .append( "Give-Up Firm\t" )
          .append( "Give-Up Indicator\t" )
          .append( "Give-Up Account" );
}

void initLogging( const XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

int main( int argc, char *argv[] )
{
    int row_id = 0;
    log4cpp::Category* log;
    zmq::socket_t* database;
    quantmodel::DatabaseEngineRequest request_msg;
    quantmodel::DatabaseEngineResponse response_msg;

    bool use_dealing_audit, use_fix_audit;

    quantmodel::Order *order;
    quantmodel::Instrument *instrument;
    std::vector< std::string > audit_trail, order_vec;
    std::vector< std::string >::const_iterator audit_trail_iter, order_iter;
    std::vector< quantmodel::Release* > release_vec;
    std::vector< quantmodel::Release* >::const_iterator release_iter;
    std::vector< quantmodel::Execution* > execution_vec;
    std::vector< quantmodel::Execution* >::const_iterator execution_iter;

    std::vector< std::map< const std::string , std::string > > fix_event_vec, dealing_event_vec, clordid_vec;
    std::vector< std::map< const std::string , std::string > >::iterator column_iter, fix_event_iter, dealing_event_iter, clordid_iter;

    std::vector< std::pair< std::string , std::map< const std::string , std::string > > > event_vec;
    std::vector< std::pair< std::string , std::map< const std::string , std::string > > >::iterator event_iter;

    const XMLNode config = XMLNode::openFileHelper( argv[ 1 ] );

    use_dealing_audit = strcmp( "true", config.getAttribute( "use_dealing_audit" ) ) == 0;
    use_fix_audit = strcmp( "true", config.getAttribute( "use_fix_audit" ) ) == 0;

    //
    //  Setup logging
    //

    initLogging( config );
    log = &log4cpp::Category::getInstance( "AuditEventParser" );

    //
    //  Setup network
    //

    const XMLNode network_node = config.getChildNode( "network" );
    zmq::context_t ctx ( 1 );
    log->info( " connecting to database engine callforward: %s", network_node.getAttribute( "database_callforward" ) );
    database = new zmq::socket_t( ctx, ZMQ_REQ );
    database->connect( network_node.getAttribute( "database_callforward" ) );

    //
    //  Open the audit file
    //

    std::fstream last_audit_file;
    std::string last_audit_tm, curr_audit_tm;
    quantmodel::TimeStamp::getUTCTime( curr_audit_tm );

    last_audit_file.open( argv[ 2 ], fstream::in );
    if(! last_audit_file.good() )
    {
        log->fatal( "cannot open file: %s", argv[ 2 ] );
        exit( 1 );
    }

    //
    //  Read the last_audit_tm, set the curr_audit_tm
    //

    last_audit_file >> last_audit_tm;
    last_audit_file.close();

    log->info( "Loading orders from last_audit_tm: " + last_audit_tm );

    //
    //  Get all orders since last_audit_tm
    //

    request_msg.Clear();
    quantmodel::TimeStamp::getMessageId( *request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( *request_msg.mutable_timestamp( ) );
    quantmodel::QuantModelProcedures::GET_ORDERS_FROM_ADDED_DATETIME( request_msg, last_audit_tm );

    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, database );

    for( int i=0; i<response_msg.result_set_size(); ++i )
    {
        const quantmodel::DatabaseEngineResponse::ResultSet& rs = response_msg.result_set( i );

        for( int j=0; j<rs.row_size(); ++j )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet::Row& row = rs.row( j );
            std::map< const std::string , std::string > columns;

            for( int k=0; k<row.column_size(); ++k )
            {
                order_vec.push_back( row.column( k ).value() );
            }
        }
    }

    //
    //  Create the audit_trail header
    //

    std::string header;
    createHeader( header );
    audit_trail.push_back( header );

    //
    //  Iterate over each order, building the audit_trail
    //

    for( order_iter = order_vec.begin(); order_iter != order_vec.end(); ++order_iter )
    {
        const std::string order_id = (*order_iter);

        event_vec.clear();
        fix_event_vec.clear();
        dealing_event_vec.clear();
        clordid_vec.clear();

        log->info( "generating audit trail for order: " + order_id );

        //
        //  Get the order and it's allocations
        //

        order = NULL;
        request_msg.Clear();
        quantmodel::TimeStamp::getMessageId( *request_msg.mutable_request_id( ) );
        quantmodel::TimeStamp::getUTCTime( *request_msg.mutable_timestamp( ) );
        quantmodel::QuantModelProcedures::GET_ORDER_BY_ORDER_ID( request_msg, order_id );
        quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, database );

        for( int i=0; i<response_msg.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response_msg.result_set( i );

            for( int j=0; j<rs.row_size(); ++j )
            {
                order = new quantmodel::Order( rs.row( j ) );

                //
                //  Get the orders allocations
                //

                quantmodel::DatabaseEngineRequest alloc_request;
                quantmodel::DatabaseEngineResponse alloc_response;

                quantmodel::TimeStamp::getMessageId( *alloc_request.mutable_request_id( ) );
                quantmodel::TimeStamp::getUTCTime( *alloc_request.mutable_timestamp( ) );
                quantmodel::QuantModelProcedures::GET_ALLOCATIONS_BY_ORDER_ID( alloc_request, order->getOrderId() );

                quantmodel::ZMQMessage::sendRPC( &alloc_request, &alloc_response, database );

                for( int k=0; k<alloc_response.result_set_size(); ++k )
                {
                    const quantmodel::DatabaseEngineResponse::ResultSet& rs2 = alloc_response.result_set( k );

                    for( int l=0; l<rs2.row_size(); ++l )
                    {
                        order->addAllocation( new quantmodel::Allocation( rs2.row( l ) ) );
                    }
                }
            }
        }

        if( NULL == order )
        {
            log->error( "UNKNOWN ORDER: " + order_id );
            exit( 1 );
        }

        //
        //  Get the releases for this order
        //

        log->info( " loading releases..." );

        request_msg.Clear();
        quantmodel::TimeStamp::getMessageId( *request_msg.mutable_request_id( ) );
        quantmodel::TimeStamp::getUTCTime( *request_msg.mutable_timestamp( ) );
        quantmodel::QuantModelProcedures::GET_RELEASES_BY_ORDER_ID( request_msg, order_id );
        quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, database );

        for( int i=0; i<response_msg.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response_msg.result_set( i );

            for( int j=0; j<rs.row_size(); ++j )
            {
                release_vec.push_back( new quantmodel::Release( rs.row( j ) ) );
                log->info( "  release: " + release_vec.back()->getReleaseId() );
            }
        }

        //
        //  Get the executions for this order
        //

        log->info( " loading executions..." );

        request_msg.Clear();
        quantmodel::TimeStamp::getMessageId( *request_msg.mutable_request_id( ) );
        quantmodel::TimeStamp::getUTCTime( *request_msg.mutable_timestamp( ) );
        quantmodel::QuantModelProcedures::GET_EXECUTIONS_BY_ORDER_ID( request_msg, order_id );
        quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, database );

        for( int i=0; i<response_msg.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response_msg.result_set( i );

            for( int j=0; j<rs.row_size(); ++j )
            {
                execution_vec.push_back( new quantmodel::Execution( rs.row( j ) ) );
                log->info( "  execution: " + execution_vec.back()->getExecutionId() );
            }
        }

        //
        //  Get the instrument
        //

        log->info( " loading instrument..." );

        instrument = NULL;
        request_msg.Clear();
        quantmodel::TimeStamp::getMessageId( *request_msg.mutable_request_id( ) );
        quantmodel::TimeStamp::getUTCTime( *request_msg.mutable_timestamp( ) );
        quantmodel::QuantModelProcedures::GET_INSTRUMENT_BY_INSTRUMENT_ID( request_msg, order->getInstrumentId() );
        quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, database );

        for( int i=0; i<response_msg.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response_msg.result_set( i );

            for( int j=0; j<rs.row_size(); ++j )
            {
                instrument = new quantmodel::Instrument( rs.row( j ) );
                log->info( "  instrument: " + instrument->getInstrumentId() );
            }
        }

        if( NULL == instrument )
        {
            log->error( "UNKNOWN INSTRUMENT: " + order->getInstrumentId() );
            exit( 1 );
        }

        if( use_dealing_audit == true )
        {
            //
            //  Get the dealing engine audit events
            //

            log->info( " loading dealing engine audit events..." );

            request_msg.Clear();
            quantmodel::TimeStamp::getMessageId( *request_msg.mutable_request_id( ) );
            quantmodel::TimeStamp::getUTCTime( *request_msg.mutable_timestamp( ) );
            quantmodel::QuantModelProcedures::GET_DEALING_EVENTS_BY_ORDER_ID( request_msg, order_id );
            quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, database );

            for( int i=0; i<response_msg.result_set_size(); ++i )
            {
                const quantmodel::DatabaseEngineResponse::ResultSet& rs = response_msg.result_set( i );

                for( int j=0; j<rs.row_size(); ++j )
                {
                    const quantmodel::DatabaseEngineResponse::ResultSet::Row& row = rs.row( j );
                    std::map< const std::string , std::string > columns;

                    for( int k=0; k<row.column_size(); ++k )
                    {
                        columns.insert(
                            std::pair< const std::string , std::string >(
                                row.column( k ).name(),
                                row.column( k ).value() ) );
                    }

                    dealing_event_vec.push_back( columns );
                }
            }
        }

        log->info( "  dealing engine audit events: %ld", dealing_event_vec.size() );

        if( use_fix_audit == true )
        {
            //
            //  Get the fix engine audit events
            //

            log->info( " loading fix engine audit events..." );

            request_msg.Clear();
            quantmodel::TimeStamp::getMessageId( *request_msg.mutable_request_id( ) );
            quantmodel::TimeStamp::getUTCTime( *request_msg.mutable_timestamp( ) );
            quantmodel::QuantModelProcedures::GET_FIX_EVENTS_BY_ORDER_ID( request_msg, order_id );
            quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, database );

            for( int i=0; i<response_msg.result_set_size(); ++i )
            {
                const quantmodel::DatabaseEngineResponse::ResultSet& rs = response_msg.result_set( i );

                for( int j=0; j<rs.row_size(); ++j )
                {
                    const quantmodel::DatabaseEngineResponse::ResultSet::Row& row = rs.row( j );
                    std::map< const std::string , std::string > columns;

                    for( int k=0; k<row.column_size(); ++k )
                    {
                        columns.insert(
                            std::pair< const std::string , std::string >(
                                row.column( k ).name(),
                                row.column( k ).value() ) );
                    }

                    fix_event_vec.push_back( columns );
                }
            }
        }

        log->info( "  fix engine audit events: %ld", fix_event_vec.size() );

        //
        //  Get the client order id records
        //

        log->info( " loading client order id records..." );

        request_msg.Clear();
        quantmodel::TimeStamp::getMessageId( *request_msg.mutable_request_id( ) );
        quantmodel::TimeStamp::getUTCTime( *request_msg.mutable_timestamp( ) );
        quantmodel::QuantModelProcedures::GET_CLIENT_ORDER_ID_MAP_BY_ORDER_ID( request_msg, order_id );
        quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, database );

        for( int i=0; i<response_msg.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response_msg.result_set( i );

            for( int j=0; j<rs.row_size(); ++j )
            {
                const quantmodel::DatabaseEngineResponse::ResultSet::Row& row = rs.row( j );
                std::map< const std::string , std::string > columns;

                for( int k=0; k<row.column_size(); ++k )
                {
                    columns.insert(
                        std::pair< const std::string , std::string >(
                            row.column( k ).name(),
                            row.column( k ).value() ) );
                }

                clordid_vec.push_back( columns );
            }
        }

        //
        //  Create the event vector as a sorted combination, based on
        //  added datetime of the dealing and fix events.
        //

        const size_t total_event_sz = dealing_event_vec.size() + fix_event_vec.size();

        for( dealing_event_iter = dealing_event_vec.begin(); dealing_event_iter != dealing_event_vec.end(); /*ERASE*/ )
        {
            std::map< const std::string , std::string > dealing_event = (*dealing_event_iter);

            for( fix_event_iter = fix_event_vec.begin(); fix_event_iter != fix_event_vec.end(); /*ERASE*/ )
            {
                std::map< const std::string , std::string > fix_event = (*fix_event_iter);

                //std::cout << fix_event[ "added_datetime" ]
                //          << " < "
                //          << dealing_event[ "added_datetime" ]
                //          << " ? ";

                if( fix_event[ "added_datetime" ] < dealing_event[ "added_datetime" ] )
                {
                    //std::cout << " TRUE --> ADDING FIX EVENT" << std::endl;

                    event_vec.push_back(
                        std::pair< std::string , std::map< const std::string , std::string > >(
                            "FIX_EVENT" , fix_event ) );

                    fix_event_iter = fix_event_vec.erase( fix_event_iter );
                }
                else
                {
                    //std::cout << " FALSE --> ";
                    break;
                }
            }

            //std::cout << " ADDING DEALING EVENT" << std::endl;

            event_vec.push_back(
                std::pair< std::string , std::map< const std::string , std::string > >(
                    "DEALING_EVENT" , dealing_event ) );

            dealing_event_iter = dealing_event_vec.erase( dealing_event_iter );
        }

        for( fix_event_iter = fix_event_vec.begin(); fix_event_iter != fix_event_vec.end(); /*ERASE*/ )
        {
            std::map< const std::string , std::string > fix_event = (*fix_event_iter);
            event_vec.push_back(
                std::pair< std::string , std::map< const std::string , std::string > >(
                    "FIX_EVENT" , fix_event ) );

            //std::cout << " ADDING FIX EVENT" << std::endl;
            fix_event_iter = fix_event_vec.erase( fix_event_iter );
        }

        //
        //  Iterate over the event vector, building the order's audit trail
        //

        log->info( " total combined audit events: %ld", event_vec.size() );

        if( total_event_sz != event_vec.size() )
        {
            log->error( " total_event_sz != event_vec.size()" );
            exit( 1 );
        }

        char buf[ 512 ];

        for( event_iter = event_vec.begin(); event_iter != event_vec.end(); ++event_iter )
        {
            ++row_id;
            std::map< const std::string , std::string > column_map = (*event_iter).second;
            std::string server_transaction_number, server_process_date, server_timestamp,
                        sender_location_id, manual_order_identifier, exchange_code,
                        message_direction, status, error_code, sender_sub_id,
                        account_number, executing_firm_number, session_id, client_order_id,
                        correlation_client_order_id, host_order_number, message_type,
                        buy_sell_indicator, quantity, max_show, security_description,
                        product, maturity_date, cfi_code, strike_price, limit_price,
                        stop_price, fill_price, order_type, order_qualifier,
                        customer_type_indicator, origin, give_up_firm, give_up_indicator,
                        give_up_account, message_source, message_sink, row;

            //
            //  Set the server transaction number
            //

            memset( buf, 0, 512 );
            sprintf( buf, "%d", row_id );
            server_transaction_number = buf;

            //
            //  Set the instrument values
            //

            product = instrument->getAttribute( quantmodel::SYMBOL );
            security_description = instrument->getAttribute( quantmodel::SECURITY_DESC );
            exchange_code = instrument->getAttribute( quantmodel::SECURITY_EXCHANGE );
            maturity_date = instrument->getAttribute( quantmodel::MATURITY_MONTH_YEAR );
            cfi_code = instrument->getAttribute( quantmodel::CFI_CODE );

            //
            //  Set the server_process_date and server_timestamp
            //

            parseEventDatetime( column_map[ "added_datetime" ], server_process_date, server_timestamp );

            if( (*event_iter).first == "DEALING_EVENT" )
            {
                sender_location_id = "US,IL,Illinois";
                manual_order_identifier = "N";

                parseMessageDirection( column_map[ "request_type" ],
                    message_direction, message_source, message_sink );
                status = column_map[ "response_type" ];
                error_code = column_map[ "error_msg" ];
                host_order_number = "";
                message_type = column_map[ "request_type" ];
                parseBuySellIndicator( order->getAttribute( quantmodel::SIDE_CODE ), buy_sell_indicator );

                if( column_map[ "order_qty" ] != "" ) quantity = column_map[ "order_qty" ];
                if( column_map[ "release_qty" ] != "" ) quantity = column_map[ "release_qty" ];
                if( column_map[ "execution_qty" ] != "" ) quantity = column_map[ "execution_qty" ];

                limit_price = column_map[ "limit_prc" ];
                stop_price = column_map[ "stop_prc" ];
                fill_price = column_map[ "execution_prc" ];
            }
            else
            {
                sender_location_id = column_map[ "senderlocationid" ];
                manual_order_identifier = column_map[ "manualorderindicator" ];

                if( column_map[ "message_direction" ] == "INCOMING" )
                {
                    message_direction = "FROM CME";
                    message_source = "GLOBEX";
                    message_sink = "FIX_ENGINE";
                }

                else
                {
                    message_direction = "TO CME";
                    message_source = "FIX_ENGINE";
                    message_sink = "GLOBEX";
                }

                if( column_map[ "msgtype" ] == "3" ||
                    column_map[ "msgtype" ] == "j" ||
                    column_map[ "msgtype" ] == "9" ||
                    ( column_map[ "msgtype" ] == "8" && column_map[ "ordstatus" ] == "8" ) )
                {
                    status = "REJECT";
                }
                else
                {
                    status = "OK";
                }

                error_code = column_map[ "text" ];
                sender_sub_id = column_map[ "sendersubid" ];
                account_number = column_map[ "account" ];
                executing_firm_number = column_map[ "sendercompid" ];
                session_id = column_map[ "sendercompid" ];
                client_order_id = column_map[ "clordid" ];
                correlation_client_order_id = column_map[ "correlationclordid" ];

                if( correlation_client_order_id == "" )
                {
                    correlation_client_order_id = client_order_id;
                }

                host_order_number = column_map[ "orderid" ];
                parseMessageType( column_map[ "msgtype" ], column_map[ "exectype" ], message_type );
                parseBuySellIndicator( column_map[ "side" ], buy_sell_indicator );

                if( column_map[ "lastshares" ] != "" ) quantity = column_map[ "lastshares" ];
                else quantity = column_map[ "orderqty" ];

                max_show = column_map[ "maxshow" ];

                limit_price = column_map[ "price" ];
                stop_price = column_map[ "stoppx" ];
                fill_price = column_map[ "lastpx" ];

                parseOrderType( column_map[ "ordtype" ], order_type );
                parseOrderQualifier( column_map[ "timeinforce" ], order_qualifier );
                customer_type_indicator = column_map[ "cticode" ];
                origin = column_map[ "customerorfirm" ];
            }

            //
            //  Create the row
            //

            row.append( server_transaction_number ).append( "\t" )
               .append( server_process_date ).append( "\t" )
               .append( server_timestamp ).append( "\t" )
               .append( sender_location_id ).append( "\t" )
               .append( manual_order_identifier ).append( "\t" )
               .append( exchange_code ).append( "\t" )
               .append( message_direction ).append( "\t" )
               .append( message_source ).append( "\t" )
               .append( message_sink ).append( "\t" )
               .append( status ).append( "\t" )
               .append( error_code ).append( "\t" )
               .append( sender_sub_id ).append( "\t" )
               .append( account_number ).append( "\t" )
               .append( executing_firm_number ).append( "\t" )
               .append( session_id ).append( "\t" )
               .append( client_order_id ).append( "\t" )
               .append( correlation_client_order_id ).append( "\t" )
               .append( host_order_number ).append( "\t" )
               .append( message_type ).append( "\t" )
               .append( buy_sell_indicator ).append( "\t" )
               .append( quantity ).append( "\t" )
               .append( max_show ).append( "\t" )
               .append( security_description ).append( "\t" )
               .append( product ).append( "\t" )
               .append( maturity_date ).append( "\t" )
               .append( cfi_code ).append( "\t" )
               .append( strike_price ).append( "\t" )
               .append( limit_price ).append( "\t" )
               .append( stop_price ).append( "\t" )
               .append( fill_price ).append( "\t" )
               .append( order_type ).append( "\t" )
               .append( order_qualifier ).append( "\t" )
               .append( customer_type_indicator ).append( "\t" )
               .append( origin ).append( "\t" )
               .append( give_up_firm ).append( "\t" )
               .append( give_up_indicator ).append( "\t" )
               .append( give_up_account );

            //
            //  Add the row to the audit trail
            //

            audit_trail.push_back( row );
        }
    }

    //
    //  Print out the audit_trail rows to the daily file
    //

    log->info( "Writing file: %s", argv[ 3 ] );

    FILE *file = NULL;
    file = fopen( argv[ 3 ], "w" );

    for( audit_trail_iter = audit_trail.begin();
         audit_trail_iter != audit_trail.end();
         ++audit_trail_iter )
    {
        fprintf( file, "%s\n", (*audit_trail_iter).c_str() );
    }

    fclose( file );

    //
    //  Open the last_audit_file
    //

    last_audit_file.open( argv[ 2 ], fstream::out | fstream::trunc );
    if(! last_audit_file.good() )
    {
        log->fatal( "cannot open file: %s", argv[ 2 ] );
        exit( 1 );
    }

    last_audit_file << curr_audit_tm;
    last_audit_file.close();

    exit( 0 );
}
