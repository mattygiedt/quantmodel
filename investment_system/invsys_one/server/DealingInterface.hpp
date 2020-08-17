

void handleTradeInstruction( TradeInstruction *instruction, System& system )
{
    char buf[ MAX_DIGITS ];
    quantmodel::DealingEngineRequest request_msg;
    quantmodel::DealingEngineResponse response_msg;
    quantmodel::SignalData *signal_data = NULL;

    if( instruction == NULL )
    {
        system.log->warn( "INVALID TRADE INSTRUCTION: NULL"  );
        return;
    }

    //
    //  Format the signal quantity into buf
    //

    memset( buf, 0, MAX_DIGITS );
    sprintf( buf, "%u", instruction->quantity );

    //
    //  Create the new signal data
    //

    signal_data = request_msg.add_signal_data();
    signal_data->set_investment_system_id( system.investment_system_id );
    signal_data->set_signal_id( system.signal_id );
    signal_data->set_instrument_id( system.instrument_id );

    if( instruction->side == TRADE_INSTRUCTION_SIDE_LONG )
    {
        signal_data->set_side_code( "BUY" );

        if( instruction->type == TRADE_INSTRUCTION_TYPE_MARKET )
        {
            signal_data->set_order_type( "MARKET" );
            signal_data->set_signal_qty( buf );
        }
        else
        {
            system.log->warn( "INVALID TRADE INSTRUCTION: instruction->type != MARKET"  );
            return;
        }
    }
    else if( instruction->side == TRADE_INSTRUCTION_SIDE_SHORT )
    {
        signal_data->set_side_code( "SELL" );

        if( instruction->type == TRADE_INSTRUCTION_TYPE_MARKET )
        {
            signal_data->set_order_type( "MARKET" );
            signal_data->set_signal_qty( buf );
        }
        else
        {
            system.log->warn( "INVALID TRADE INSTRUCTION: instruction->type != MARKET"  );
            return;
        }
    }

    //
    //  Send the signals to the dealing engine
    //

    quantmodel::TimeStamp::getMessageId( *request_msg.mutable_request_id( ) );
    quantmodel::TimeStamp::getUTCTime( *request_msg.mutable_timestamp( ) );
    request_msg.set_type( quantmodel::DealingEngineRequest::EXECUTE_SIGNAL );

    quantmodel::ZMQMessage::sendRPC( &request_msg, &response_msg, system.socket_map[ "dealing_callforward" ] );

    if( response_msg.type() != quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT )
    {
        system.log->fatal( "UNABLE TO HANDLE TRADE INSTRUCTION." );
        system.log->fatal( response_msg.ShortDebugString() );
        exit( 1 );
    }
}
