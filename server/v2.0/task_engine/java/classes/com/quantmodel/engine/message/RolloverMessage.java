package com.quantmodel.engine.message;

import quantmodel.Quantmodel.DealingEngineRequest;

import java.util.UUID;
import java.util.Calendar;
import java.util.TimeZone;

import org.apache.log4j.Logger;

public class RolloverMessage extends BaseMessage
{
    private static final Logger log = Logger.getLogger( RolloverMessage.class );

    public com.google.protobuf.Message getMessage()
    {
        return
            DealingEngineRequest.newBuilder()
                .setRequestId( UUID.randomUUID().toString() )
                .setTimestamp( getTimestamp() )
                .setType( DealingEngineRequest.RequestType.ROLLOVER )
            .build();
    }
}
