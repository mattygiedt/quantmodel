#include <stdio.h>
#include <stdlib.h>

#include <log4cpp/Category.hh>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "RequestHandler.hpp"

const void dealing::RequestHandler::loginFixEngine(
    const quantmodel::DealingEngineRequest& request_msg,
    quantmodel::DealingEngineResponse& response_msg,
    quantmodel::DealingEngineBroadcast& broadcast_msg,
    quantmodel::DatabaseEngineRequest& database_msg )
{
    quantmodel::FixEngineRequest fix_request;
    quantmodel::FixEngineResponse fix_response;

    fix_request.set_type( quantmodel::FixEngineRequest::FIXENGINE_LOGIN );
    fix_request.set_session_id( request_msg.fix_engine_data().session_id() );
    quantmodel::TimeStamp::getMessageId( fix_request.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( fix_request.mutable_timestamp( ) );

    if( quantmodel::ZMQMessage::sendRPC(
        &fix_request,
        &fix_response,
        socket_map[ "fix_callforward" ] ) )
    {
        if( fix_response.type() == quantmodel::FixEngineResponse::REJECT ||
            fix_response.type() == quantmodel::FixEngineResponse::ERROR )
        {
            response_msg.set_request_id( request_msg.request_id( ) );
            quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
            quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
            response_msg.set_type( quantmodel::DealingEngineResponse::ERROR );
            response_msg.set_error_msg( fix_response.error_msg() );

            return;
        }
    }

    response_msg.set_request_id( request_msg.request_id( ) );
    quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
    quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
    response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );
}

