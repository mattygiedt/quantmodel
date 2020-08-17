#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "ActiveSet.hpp"

dealing::ActiveSet::ActiveSet()
{
    position_manager = new quantmodel::PositionManager();
}

dealing::ActiveSet::~ActiveSet()
{
    delete position_manager;
    close();
}

void dealing::ActiveSet::initLogging()
{
    log = &log4cpp::Category::getInstance( "ActiveSet" );
}

void dealing::ActiveSet::loadOrders( zmq::socket_t* database_callforward )
{
    quantmodel::DatabaseEngineRequest request;
    quantmodel::DatabaseEngineResponse response;

    quantmodel::TimeStamp::getMessageId( request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request.mutable_timestamp( ) );
    quantmodel::QuantModelProcedures::GET_OPEN_ORDERS( request );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + request.ShortDebugString() );
    }

    if( quantmodel::ZMQMessage::sendRPC( &request, &response, database_callforward ) )
    {
        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " --> " + response.ShortDebugString() );
        }

        for( int i=0; i<response.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response.result_set( i );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " loading %d orders", rs.row_size() );
            }

            for( int j=0; j<rs.row_size(); ++j )
            {
                quantmodel::Order *order = new quantmodel::Order( rs.row( j ) );

                //
                //  Get the orders allocations
                //

                quantmodel::DatabaseEngineRequest alloc_request;
                quantmodel::DatabaseEngineResponse alloc_response;

                quantmodel::TimeStamp::getMessageId( alloc_request.mutable_request_id( ) );
                quantmodel::TimeStamp::getUTCTime( alloc_request.mutable_timestamp( ) );
                quantmodel::QuantModelProcedures::GET_ALLOCATIONS_BY_ORDER_ID(
                    alloc_request, order->getOrderId() );

                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( " <-- " + alloc_request.ShortDebugString() );
                }

                if( quantmodel::ZMQMessage::sendRPC( &alloc_request, &alloc_response, database_callforward ) )
                {
                    if( unlikely( log->isDebugEnabled() ) )
                    {
                        log->debug( " --> " + alloc_response.ShortDebugString() );
                    }

                    for( int k=0; k<alloc_response.result_set_size(); ++k )
                    {
                        const quantmodel::DatabaseEngineResponse::ResultSet& rs2 = alloc_response.result_set( k );

                        for( int l=0; l<rs2.row_size(); ++l )
                        {
                            order->addAllocation( new quantmodel::Allocation( rs2.row( l ) ) );
                        }
                    }
                }

                //
                //  Add the order internally
                //

                addOrder( order );
            }
        }
    }
}

void dealing::ActiveSet::loadReleases( zmq::socket_t* database_callforward )
{
    quantmodel::DatabaseEngineRequest request;
    quantmodel::DatabaseEngineResponse response;

    quantmodel::TimeStamp::getMessageId( request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request.mutable_timestamp( ) );
    quantmodel::QuantModelProcedures::GET_OPEN_RELEASES( request );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + request.ShortDebugString() );
    }

    if( quantmodel::ZMQMessage::sendRPC( &request, &response, database_callforward ) )
    {
        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " --> " + response.ShortDebugString() );
        }

        for( int i=0; i<response.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response.result_set( i );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " loading %d releases", rs.row_size() );
            }

            for( int j=0; j<rs.row_size(); ++j )
            {
                addRelease( new quantmodel::Release( rs.row( j ) ) );
            }
        }
    }
}

void dealing::ActiveSet::loadExecutions( zmq::socket_t* database_callforward )
{
    quantmodel::DatabaseEngineRequest request;
    quantmodel::DatabaseEngineResponse response;

    quantmodel::TimeStamp::getMessageId( request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request.mutable_timestamp( ) );
    quantmodel::QuantModelProcedures::GET_OPEN_EXECUTIONS( request );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + request.ShortDebugString() );
    }

    if( quantmodel::ZMQMessage::sendRPC( &request, &response, database_callforward ) )
    {

        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " --> " + response.ShortDebugString() );
        }

        for( int i=0; i<response.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response.result_set( i );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( " loading %d executions", rs.row_size() );
            }

            for( int j=0; j<rs.row_size(); ++j )
            {
                addExecution( new quantmodel::Execution( rs.row( j ) ) );
            }
        }
    }
}

void dealing::ActiveSet::loadOvernightPositions( zmq::socket_t* database_callforward )
{
    quantmodel::DatabaseEngineRequest request;
    quantmodel::DatabaseEngineResponse response;

    quantmodel::TimeStamp::getMessageId( request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request.mutable_timestamp( ) );
    quantmodel::QuantModelProcedures::GET_OVERNIGHT_POSITIONS( request );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + request.ShortDebugString() );
    }

    if( quantmodel::ZMQMessage::sendRPC( &request, &response, database_callforward ) )
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
                quantmodel::PositionOvernight *position_overnight =
                    new quantmodel::PositionOvernight( rs.row( j ) );

                position_vec.push_back( position_overnight );
                position_manager->add( position_overnight );
            }
        }
    }
}

void dealing::ActiveSet::getOrdersByInstrumentId( const std::string& instrument_id, std::vector< quantmodel::Order* >& vec ) const
{
    std::map< const std::string, quantmodel::Order* >::const_iterator order_iter;

    order_iter = order_map.begin();

    while( order_iter != order_map.end() )
    {
        if( instrument_id.compare( order_iter->second->getInstrumentId() ) == 0 )
        {
            vec.push_back( order_iter->second );
        }

        ++order_iter;
    }
}

void dealing::ActiveSet::getAllocationsByAccountId( const std::string& account_id, std::vector< quantmodel::Allocation* >& vec ) const
{
    std::vector< quantmodel::Allocation* > allocations;
    std::vector< quantmodel::Allocation* >::const_iterator alloc_iter;
    std::map< const std::string, quantmodel::Order* >::const_iterator order_iter;

    order_iter = order_map.begin();

    while( order_iter != order_map.end() )
    {
        //
        //  Get the order's allocations
        //

        allocations.clear();
        order_iter->second->getAllocations( allocations );

        for( alloc_iter = allocations.begin(); alloc_iter != allocations.end(); ++alloc_iter )
        {
            if( account_id.compare( (*alloc_iter)->getAccountId( ) ) == 0 )
            {
                vec.push_back( (*alloc_iter) );
            }
        }

        ++order_iter;
    }
}

void dealing::ActiveSet::getReleasesByOrderId( const std::string& order_id, std::vector< quantmodel::Release* >& vec ) const
{
    //std::map< const std::string, quantmodel::Release* >::const_iterator release_iter;
    //
    //for( release_iter = release_map.begin(); release_iter != release_map.end(); ++release_iter )
    //{
    //    if( order_id.compare( release_iter->second->getOrderId( ) ) == 0 )
    //    {
    //        vec.push_back( release_iter->second );
    //    }
    //}

    std::vector< quantmodel::Release* >::const_iterator release_iter;
    for( release_iter = release_vec.begin(); release_iter != release_vec.end(); ++release_iter )
    {
        if( order_id.compare( (*release_iter)->getOrderId( ) ) == 0 )
        {
            vec.push_back( (*release_iter) );
        }
    }
}

void dealing::ActiveSet::getReleasesByAccountId( const std::string& account_id, std::vector< quantmodel::Release* >& vec ) const
{
    std::vector< quantmodel::Release* >::const_iterator release_iter;
    std::map< const std::string, std::vector< quantmodel::Release* > >::const_iterator account_release_iter;

    if( ( account_release_iter = account_release_map.find( account_id ) ) == account_release_map.end() )
    {
        return;
    }

    for( release_iter = account_release_iter->second.begin(); release_iter != account_release_iter->second.end(); ++release_iter )
    {
        vec.push_back( (*release_iter) );
    }
}

void dealing::ActiveSet::getReleasesByAllocationId( const std::string& alloc_id, std::vector< quantmodel::Release* >& vec ) const
{
    std::vector< quantmodel::Release* >::const_iterator release_iter;

    for( release_iter = release_vec.begin(); release_iter != release_vec.end(); ++release_iter )
    {
        if( alloc_id.compare( (*release_iter)->getAllocationId( ) ) == 0 )
        {
            vec.push_back( (*release_iter) );
        }
    }
}

void dealing::ActiveSet::getExecutionsByOrderId( const std::string& order_id, std::vector< quantmodel::Execution* >& vec ) const
{
    std::vector< quantmodel::Execution* >::const_iterator execution_iter;

    for( execution_iter = execution_vec.begin(); execution_iter != execution_vec.end(); ++execution_iter )
    {
        if( order_id.compare( (*execution_iter)->getOrderId( ) ) == 0 )
        {
            vec.push_back( (*execution_iter) );
        }
    }
}

void dealing::ActiveSet::getExecutionsByReleaseId( const std::string& release_id, std::vector< quantmodel::Execution* >& vec ) const
{
    std::vector< quantmodel::Execution* >::const_iterator execution_iter;

    for( execution_iter = execution_vec.begin(); execution_iter != execution_vec.end(); ++execution_iter )
    {
        if( release_id.compare( (*execution_iter)->getReleaseId( ) ) == 0 )
        {
            vec.push_back( (*execution_iter) );
        }
    }
}

void dealing::ActiveSet::getActiveSet( quantmodel::DealingEngineResponse& response_msg ) const
{
    std::vector< quantmodel::Order* >::const_iterator order_iter;
    std::vector< quantmodel::Release* >::const_iterator release_iter;
    std::vector< quantmodel::Execution* >::const_iterator execution_iter;
    std::vector< quantmodel::PositionOvernight* >::const_iterator position_iter;

    for( order_iter = order_vec.begin(); order_iter != order_vec.end(); ++order_iter )
    {
        quantmodel::OrderMessage * const orderMessage = response_msg.add_order();

        (*order_iter)->asOrderMessage( orderMessage );

        for( release_iter = release_vec.begin(); release_iter != release_vec.end(); ++release_iter )
        {
            if( (*order_iter)->getOrderId().compare( (*release_iter)->getOrderId() ) == 0 )
            {
                (*release_iter)->asOrderMessage( orderMessage );
            }
        }

        for( execution_iter = execution_vec.begin(); execution_iter != execution_vec.end(); ++execution_iter )
        {
            if( (*order_iter)->getOrderId().compare( (*execution_iter)->getOrderId() ) == 0 )
            {
                (*execution_iter)->asOrderMessage( orderMessage );
            }
        }
    }

    for( position_iter = position_vec.begin(); position_iter != position_vec.end(); ++position_iter )
    {
        (*position_iter)->copyPositionOvernightData( response_msg.add_position() );
    }
}

void dealing::ActiveSet::getActiveSet( quantmodel::DealingEngineBroadcast& broadcast_msg ) const
{
    std::vector< quantmodel::Order* >::const_iterator order_iter;
    std::vector< quantmodel::Release* >::const_iterator release_iter;
    std::vector< quantmodel::Execution* >::const_iterator execution_iter;
    std::vector< quantmodel::PositionOvernight* >::const_iterator position_iter;

    for( order_iter = order_vec.begin(); order_iter != order_vec.end(); ++order_iter )
    {
        quantmodel::OrderMessage * const orderMessage = broadcast_msg.add_order();

        (*order_iter)->asOrderMessage( orderMessage );

        for( release_iter = release_vec.begin(); release_iter != release_vec.end(); ++release_iter )
        {
            if( (*order_iter)->getOrderId().compare( (*release_iter)->getOrderId() ) == 0 )
            {
                (*release_iter)->asOrderMessage( orderMessage );
            }
        }

        for( execution_iter = execution_vec.begin(); execution_iter != execution_vec.end(); ++execution_iter )
        {
            if( (*order_iter)->getOrderId().compare( (*execution_iter)->getOrderId() ) == 0 )
            {
                (*execution_iter)->asOrderMessage( orderMessage );
            }
        }
    }

    for( position_iter = position_vec.begin(); position_iter != position_vec.end(); ++position_iter )
    {
        (*position_iter)->copyPositionOvernightData( broadcast_msg.add_position() );
    }
}

void dealing::ActiveSet::rollover( zmq::socket_t* database_callforward )
{
    char buf[ MAX_DIGITS ];
    std::string ts, rollover_id;
    quantmodel::DatabaseEngineRequest database_request;

    //
    //  active set management objects
    //

    std::vector< quantmodel::Release* > releases_to_close;
    std::vector< quantmodel::Execution* > executions_to_close;

    std::vector< std::string > orders_to_erase;
    std::vector< std::string > releases_to_erase;
    std::vector< std::string > executions_to_erase;

    std::vector< std::string >::iterator erase_iter;
    std::map< const std::string, quantmodel::Order* >::iterator order_map_iter;
    std::map< const std::string, quantmodel::Release* >::iterator release_map_iter;
    std::map< const std::string, quantmodel::Execution* >::iterator execution_map_iter;

    std::vector< quantmodel::Order* >::iterator order_vec_iter;
    std::vector< quantmodel::Release* >::iterator release_vec_iter;
    std::vector< quantmodel::Execution* >::iterator execution_vec_iter;
    std::vector< quantmodel::PositionOvernight* >::const_iterator position_iter;

    //
    //  position management objects
    //

    std::vector< quantmodel::AccountPosition* > account_position_vec;
    std::vector< quantmodel::AccountPosition* >::const_iterator account_position_iter;

    std::vector< quantmodel::InstrumentPosition* >::const_iterator instrument_position_iter;

    std::vector< std::pair< quantmodel::InvestmentSystemPosition* , std::vector< quantmodel::InstrumentPosition* > >* > investment_system_position_vec;
    std::vector< std::pair< quantmodel::InvestmentSystemPosition* , std::vector< quantmodel::InstrumentPosition* > >* >::iterator investment_system_position_iter;

    //
    //  Get the final order states
    //

    memset( buf, 0, MAX_DIGITS );
    sprintf( buf, "%d", quantmodel::Order::DAY );
    const std::string day_order ( buf );

    memset( buf, 0, MAX_DIGITS );
    sprintf( buf, "%d", quantmodel::Order::FILLED );
    const std::string filled_order ( buf );

    memset( buf, 0, MAX_DIGITS );
    sprintf( buf, "%d", quantmodel::Order::CANCELED );
    const std::string canceled_order ( buf );

    memset( buf, 0, MAX_DIGITS );
    sprintf( buf, "%d", quantmodel::Order::CLOSED );
    const std::string closed_order ( buf );

    memset( buf, 0, MAX_DIGITS );
    sprintf( buf, "%d", quantmodel::Release::CANCELED );
    const std::string canceled_release ( buf );

    memset( buf, 0, MAX_DIGITS );
    sprintf( buf, "%d", quantmodel::Release::CLOSED );
    const std::string closed_release ( buf );

    memset( buf, 0, MAX_DIGITS );
    sprintf( buf, "%d", quantmodel::Execution::CLOSED );
    const std::string closed_execution ( buf );

    //
    //  Give all events the same timestamp
    //

    quantmodel::TimeStamp::getUTCTime( ts );
    rollover_id = "ROLLOVER";

    //
    //  Iterate over each order. If it should be rolled over, update the
    //  order, releases, executions, and the positions.
    //

    for( order_map_iter = order_map.begin(); order_map_iter != order_map.end(); ++order_map_iter )
    {
        quantmodel::Order* order = order_map_iter->second;

        const std::string& order_id = order->getOrderId();
        const std::string& order_tif = order->getAttribute( quantmodel::TIF );
        const std::string& order_status = order->getAttribute( quantmodel::ORDER_STATUS );
        const std::string& instrument_id = order->getAttribute( quantmodel::INSTRUMENT_ID );
        const std::string& added_datetime = order->getAttribute( quantmodel::ADDED_DATETIME );

        if( order_tif.compare( day_order ) == 0 ||
            order_status.compare( filled_order ) == 0 ||
            order_status.compare( canceled_order ) == 0 )
        {
            database_request.Clear();

            //
            //  Get the instrument on the order.
            //

            quantmodel::DatabaseEngineRequest instrument_req;
            quantmodel::DatabaseEngineResponse instrument_rep;

            quantmodel::TimeStamp::getMessageId( instrument_req.mutable_request_id( ) );
            quantmodel::TimeStamp::getUTCTime( instrument_req.mutable_timestamp( ) );
            quantmodel::QuantModelProcedures::GET_INSTRUMENT_BY_INSTRUMENT_ID( instrument_req, instrument_id );

            if( quantmodel::ZMQMessage::sendRPC( &instrument_req, &instrument_rep, database_callforward ) )
            {
                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( " --> " + instrument_rep.ShortDebugString() );
                }
            }

            //
            //  If the order is canceled, don't bother to check if
            //  the instrument has been marked, or if it was created
            //  prior to mark time.
            //

            if( order_status.compare( canceled_order ) != 0 )
            {
                quantmodel::Instrument instrument ( instrument_rep.result_set( 0 ).row( 0 ) );
                const std::string& mark_datetime = instrument.getAttribute( quantmodel::MARK_DATETIME );
                const std::string& rollover_datetime = instrument.getAttribute( quantmodel::ROLLOVER_DATETIME );

                //
                //  If the instrument has not been marked, do not close order.
                //

                if( mark_datetime.compare( "NULL" ) == 0 )
                {
                    log->warn( " SKIPPING ORDER: " + order_id );
                    log->warn( "  INSTRUMENT: %s has not been marked!", instrument_id.c_str() );

                    continue;
                }

                //
                //  If the order was added AFTER the instrument rollover date,
                //  DO NOT CLOSE the order.
                //

                if( rollover_datetime.compare( "NULL" ) == 0 ||
                    added_datetime.compare( rollover_datetime ) >= 0 )
                {
                    log->warn( " SKIPPING ORDER: " + order_id );
                    log->warn( "  ROLLOVER_DATETIME: %s == NULL || >= %s",
                        rollover_datetime.c_str(), added_datetime.c_str() );

                    continue;
                }
            }

            log->info( " CLOSING ORDER: " + order_id );

            //
            //  Update the order's attributes and add it to the database request
            //

            order->setAttribute( quantmodel::UPDATED_BY, rollover_id );
            order->setAttribute( quantmodel::UPDATED_DATETIME, ts );
            order->setAttribute( quantmodel::ORDER_STATUS, closed_order );

            order->update( database_request );

            //
            //  Add the order to the erase vector
            //

            orders_to_erase.push_back( order_id );

            //
            //  Close the releases on this order
            //

            releases_to_close.clear();
            getReleasesByOrderId( order_id, releases_to_close );

            for( release_vec_iter = releases_to_close.begin(); release_vec_iter != releases_to_close.end(); ++release_vec_iter )
            {
                quantmodel::Release* release = (*release_vec_iter);

                release->setAttribute( quantmodel::UPDATED_BY, rollover_id );
                release->setAttribute( quantmodel::UPDATED_DATETIME, ts );
                release->setAttribute( quantmodel::RELEASE_STATUS, closed_release );

                release->update( database_request );
                releases_to_erase.push_back( release->getReleaseId() );
            }

            //
            //  Close the executions on this order
            //

            executions_to_close.clear();
            getExecutionsByOrderId( order_id, executions_to_close );

            for( execution_vec_iter = executions_to_close.begin(); execution_vec_iter != executions_to_close.end(); ++execution_vec_iter )
            {
                quantmodel::Execution* execution = (*execution_vec_iter);

                execution->setAttribute( quantmodel::UPDATED_BY, rollover_id );
                execution->setAttribute( quantmodel::UPDATED_DATETIME, ts );
                execution->setAttribute( quantmodel::EXECUTION_STATUS, closed_execution );

                execution->update( database_request );
                executions_to_erase.push_back( execution->getExecutionId() );
            }

            //
            //  Close the client_order_ids on this order
            //

            quantmodel::QuantModelProcedures::DELETE_CLIENT_ORDER_ID_BY_ORDER_ID(
                database_request,
                order_id,
                ts,
                rollover_id );

            //
            //  Send the database update requests to the database engine
            //

            quantmodel::ZMQMessage::sendRPC( &database_request, database_callforward );
        }
    }

    //
    //  Clear (set all positions to in_active) the position overnight table
    //

    database_request.Clear();
    quantmodel::QuantModelProcedures::DELETE_POSITION_OVERNIGHT( database_request );
    quantmodel::ZMQMessage::sendRPC( &database_request, database_callforward );

    //
    //  Get all positions from the position manager
    //

    position_manager->getPositions( account_position_vec );

    //
    //  Iterate over each accout position, and if it has an open position,
    //  add it to the position overnight table.
    //

    database_request.Clear();

    for( account_position_iter = account_position_vec.begin(); account_position_iter != account_position_vec.end(); ++account_position_iter )
    {
        investment_system_position_vec.clear();
        (*account_position_iter)->getAccountPositions( investment_system_position_vec );

        investment_system_position_iter = investment_system_position_vec.begin();
        while( investment_system_position_iter != investment_system_position_vec.end() )
        {
            for( instrument_position_iter = (*investment_system_position_iter)->second.begin();
                 instrument_position_iter != (*investment_system_position_iter)->second.end();
                 ++instrument_position_iter )
            {
                if( (*instrument_position_iter)->getExposure() != quantmodel::InstrumentPosition::FLAT )
                {
                    quantmodel::InstrumentPosition *instrument_position = (*instrument_position_iter);
                    quantmodel::PositionOvernight position_overnight( instrument_position );
                    position_overnight.setAttribute( quantmodel::ADDED_BY, "DEALING_ENGINE" );
                    position_overnight.setAttribute( quantmodel::ADDED_DATETIME, ts );
                    position_overnight.insert( database_request );
                }
            }

            delete (*investment_system_position_iter);
            ++investment_system_position_iter;
        }
    }

    //
    //  Send the position overnight insert statements to the database engine
    //

    quantmodel::ZMQMessage::sendRPC( &database_request, database_callforward );

    //
    //  Iterate over each dealing object [ order, release, execution ]
    //  to erase, and erase it
    //

    for( erase_iter = orders_to_erase.begin(); erase_iter != orders_to_erase.end(); ++erase_iter )
    {
        removeOrder( (*erase_iter) );
    }

    for( erase_iter = releases_to_erase.begin(); erase_iter != releases_to_erase.end(); ++erase_iter )
    {
        removeRelease( (*erase_iter) );
    }

    for( erase_iter = executions_to_erase.begin(); erase_iter != executions_to_erase.end(); ++erase_iter )
    {
        removeExecution( (*erase_iter) );
    }

    orders_to_erase.clear();
    releases_to_erase.clear();
    executions_to_erase.clear();

    //
    //  Reload the overnight positions
    //

    for( position_iter = position_vec.begin(); position_iter != position_vec.end(); ++position_iter )
    {
        delete (*position_iter);
    }

    position_vec.clear();
    position_manager->clear();

    loadOvernightPositions( database_callforward );
}

void dealing::ActiveSet::close( )
{
    std::map< const std::string , quantmodel::Order* >::iterator order_iter;
    std::map< const std::string , quantmodel::Release* >::iterator release_iter;
    std::map< const std::string , quantmodel::Execution* >::iterator execution_iter;

    order_iter = order_map.begin();
    while( order_iter != order_map.end() )
    {
        delete order_iter->second;
        ++order_iter;
    }
    order_map.clear();
    order_vec.clear();

    release_iter = release_map.begin();
    while( release_iter != release_map.end() )
    {
        delete release_iter->second;
        ++release_iter;
    }
    release_map.clear();
    release_vec.clear();
    account_release_map.clear();

    execution_iter = execution_map.begin();
    while( execution_iter != execution_map.end() )
    {
        delete execution_iter->second;
        ++execution_iter;
    }
    execution_map.clear();
    execution_vec.clear();
}

void dealing::ActiveSet::removeOrder( const std::string& order_id )
{
    quantmodel::Order *order = NULL;
    std::vector< quantmodel::Order* >::iterator order_vec_iter;
    std::map< const std::string , quantmodel::Order* >::iterator order_map_iter;

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " deleting order: " + order_id );
    }

    //
    //  Find the order in the order_vec
    //

    order_vec_iter = order_vec.begin();
    while( order_vec_iter != order_vec.end() )
    {
        if( (*order_vec_iter)->getOrderId().compare( order_id ) == 0 )
        {
            order = (*order_vec_iter);
            order_vec.erase( order_vec_iter );
            break;
        }
        else
        {
            ++order_vec_iter;
        }
    }

    //
    //  If the order is not in the order_vec, return;
    //

    if( NULL == order )
    {
        return;
    }

    //
    //  Find the order in the order_map
    //

    order_map_iter = order_map.find( order_id );
    if( order_map_iter != order_map.end() )
    {
        order_map.erase( order_map_iter );
    }

    //
    //  Find the order in the ext_order_map
    //

    order_map_iter = ext_order_map.find( order->getExternalOrderId() );
    if( order_map_iter != ext_order_map.end() )
    {
        ext_order_map.erase( order_map_iter );
    }

    //
    //  Delete the order
    //

    delete order;
}

void dealing::ActiveSet::removeRelease( const std::string& release_id )
{
    quantmodel::Release *release = NULL;
    std::vector< quantmodel::Release* >::iterator release_vec_iter;
    std::map< const std::string , quantmodel::Release* >::iterator release_map_iter;
    std::map< const std::string, std::vector< quantmodel::Release* > >::iterator account_release_map_iter;

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " deleting release: " + release_id );
    }

    //
    //  Find the release in the release_vec
    //

    release_vec_iter = release_vec.begin();
    while( release_vec_iter != release_vec.end() )
    {
        if( (*release_vec_iter)->getReleaseId().compare( release_id ) == 0 )
        {
            release = (*release_vec_iter);
            release_vec.erase( release_vec_iter );

            if( unlikely( log->isDebugEnabled() ) )
            {
                log->debug( "  release_vec.erase..." );
            }

            break;
        }
        else
        {
            ++release_vec_iter;
        }
    }

    //
    //  If the release is not in the release_vec, return.
    //

    if( NULL == release )
    {
        log->warn( " removeRelease: UNKNOWN RELEASE " + release_id );
        return;
    }

    //
    //  Find the release in the account release map vector
    //

    account_release_map_iter = account_release_map.find( release->getAccountId() );
    if( account_release_map_iter != account_release_map.end() )
    {
        for( release_vec_iter = account_release_map_iter->second.begin();
             release_vec_iter != account_release_map_iter->second.end();
             ++release_vec_iter )
        {
            if( (*release_vec_iter)->getReleaseId().compare( release_id ) == 0 )
            {
                account_release_map_iter->second.erase( release_vec_iter );

                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( "  account_release_map.erase..." );
                }

                break;
            }
        }
    }

    //
    //  Find the release in the release map
    //

    release_map_iter = release_map.find( release_id );
    if( release_map_iter != release_map.end() )
    {
        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( "  release_map.erase..." );
        }

        release_map.erase( release_map_iter );
    }

    //
    //  Delete the release
    //

    delete release;
}

void dealing::ActiveSet::removeExecution( const std::string& execution_id )
{
    quantmodel::Execution *execution = NULL;
    std::vector< quantmodel::Execution* >::iterator execution_vec_iter;
    std::map< const std::string , quantmodel::Execution* >::iterator execution_map_iter;

    //
    //  Find the execution in the execution_vec
    //

    execution_vec_iter = execution_vec.begin();
    while( execution_vec_iter != execution_vec.end() )
    {
        if( (*execution_vec_iter)->getExecutionId().compare( execution_id ) == 0 )
        {
            execution = (*execution_vec_iter);
            execution_vec.erase( execution_vec_iter );
            break;
        }
        else
        {
            ++execution_vec_iter;
        }
    }

    if( NULL == execution )
    {
        return;
    }

    //
    //  Find the execution in the execution map
    //

    execution_map_iter = execution_map.find( execution_id );
    if( execution_map_iter != execution_map.end() )
    {
        execution_map.erase( execution_map_iter );
    }

    //
    //  Delete the execution
    //

    delete execution;
}
