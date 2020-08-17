#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "DealingEngineParameters.hpp"

void dealing::DealingEngineParameters::initLogging()
{
    log = &log4cpp::Category::getInstance( "DealingEngineParameters" );
}

void dealing::DealingEngineParameters::getNextObjectId(
    std::string& id,
    std::string& object_id,
    dealing::DealingEngineParameters::ObjectType objtyp )
{
    char buf[ 16 ];
    int next_id = 0;
    std::string parameter_id;
    memset( buf, 0, 16 );

    if( objtyp == ORDER )
    {
        parameter_id = "curr_order_id";
        next_id = 1 + atoi( getParameter( parameter_id ).c_str() );
        sprintf( buf, "%d", next_id );
        id.assign( buf );
        object_id.assign( "O" ).append( buf );
    }
    else if( objtyp == ALLOCATION )
    {
        parameter_id = "curr_allocation_id";
        next_id = 1 + atoi( getParameter( parameter_id ).c_str() );
        sprintf( buf, "%d", next_id );
        id.assign( buf );
        object_id.assign( "A" ).append( buf );
    }
    else if( objtyp == RELEASE )
    {
        parameter_id = "curr_release_id";
        next_id = 1 + atoi( getParameter( parameter_id ).c_str() );
        sprintf( buf, "%d", next_id );
        id.assign( buf );
        object_id.assign( "R" ).append( buf );
    }
    else if( objtyp == EXECUTION )
    {
        parameter_id = "curr_execution_id";
        next_id = 1 + atoi( getParameter( parameter_id ).c_str() );
        sprintf( buf, "%d", next_id );
        id.assign( buf );
        object_id.assign( "E" ).append( buf );
    }
    else
    {
        log->error( "UNKNOWN OBJECT_TYPE" );
        return;
    }

    //
    //  Set the next_id for this parameter
    //

    setParameter( parameter_id, buf );
}

void dealing::DealingEngineParameters::loadParameters( zmq::socket_t* socket )
{
    quantmodel::DatabaseEngineRequest request;
    quantmodel::DatabaseEngineResponse response;

    quantmodel::TimeStamp::getMessageId( request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( request.mutable_timestamp( ) );
    quantmodel::QuantModelProcedures::VALIDATE_DEALING_ENGINE_PARAMETERS( request );

    if( unlikely( log->isDebugEnabled() ) )
    {
        log->debug( " <-- " + request.ShortDebugString() );
    }

    if( quantmodel::ZMQMessage::sendRPC( &request, &response, socket ) )
    {
        if( unlikely( log->isDebugEnabled() ) )
        {
            log->debug( " --> " + response.ShortDebugString() );
        }

        close();

        for( int i=0; i<response.result_set_size(); ++i )
        {
            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response.result_set( i );

            for( int j=0; j<rs.row_size(); ++j )
            {
                quantmodel::SystemParameter *parameter =
                    new quantmodel::SystemParameter( rs.row( j ) );

                parameter_map.insert(
                    pair< const std::string, quantmodel::SystemParameter* >
                        ( parameter->getName(), parameter ) );

                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( " dealing parameter: %s = %s",
                        parameter->getName().c_str(),
                        parameter->getValue().c_str() );
                }
            }
        }
    }
}

void dealing::DealingEngineParameters::updateParameter( const quantmodel::SystemParameterData& system_parameter, quantmodel::DatabaseEngineRequest& database_msg )
{
    quantmodel::QuantModelProcedures::UPDATE_PARAMETER(
        database_msg, "DEALING_ENGINE",
        system_parameter.name(), system_parameter.value() );

    //
    //  update the parameter map
    //

    setParameter( system_parameter.name(), system_parameter.value() );
}

void dealing::DealingEngineParameters::getParameters( quantmodel::DealingEngineResponse& response_msg )
{
    std::map< const std::string , quantmodel::SystemParameter* >::const_iterator parameter_iter;

    parameter_iter = parameter_map.begin();
    while( parameter_iter != parameter_map.end() )
    {
        parameter_iter->second->copyTo( response_msg.add_system_parameter() );
        ++parameter_iter;
    }
}

void dealing::DealingEngineParameters::getParameters( quantmodel::DealingEngineBroadcast& broadcast_msg )
{
    std::map< const std::string , quantmodel::SystemParameter* >::const_iterator parameter_iter;

    parameter_iter = parameter_map.begin();
    while( parameter_iter != parameter_map.end() )
    {
        parameter_iter->second->copyTo( broadcast_msg.add_system_parameter() );
        ++parameter_iter;
    }
}
          