package com.quantmodel.engine.message;

import quantmodel.Quantmodel.FixEngineData;
import quantmodel.Quantmodel.InstrumentData;
import quantmodel.Quantmodel.DealingEngineRequest;

import java.util.UUID;
import java.util.Calendar;
import java.util.TimeZone;

import org.apache.log4j.Logger;

public class MarketDataSubscribeMessage extends BaseMessage
{
    private static final Logger log = Logger.getLogger( MarketDataSubscribeMessage.class );

    private final String session_id;
    private final String instrument_id;

    public MarketDataSubscribeMessage( final String session_id, final String instrument_id )
    {
        this.session_id = session_id;
        this.instrument_id = instrument_id;
    }

    public com.google.protobuf.Message getMessage()
    {
        return
            DealingEngineRequest.newBuilder()
                .setRequestId( UUID.randomUUID().toString() )
                .setTimestamp( getTimestamp() )
                .setType( DealingEngineRequest.RequestType.MARKETDATA_SUBSCRIBE )
                .setFixEngineData( FixEngineData.newBuilder()
                    .setSessionId( session_id ) )
                .addInstrumentData( InstrumentData.newBuilder()
                    .setInstrumentId( instrument_id ) )
            .build();
    }
}
