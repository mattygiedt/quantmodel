package com.quantmodel.engine.message;

import quantmodel.Quantmodel.DealingEngineRequest;

import java.util.UUID;
import java.util.Calendar;
import java.util.TimeZone;

import org.apache.log4j.Logger;

public class DealingStatusMessage extends BaseMessage
{
    private static final Logger log = Logger.getLogger( DealingStatusMessage.class );

    public com.google.protobuf.Message getMessage()
    {
        return
            DealingEngineRequest.newBuilder()
                .setRequestId( UUID.randomUUID().toString() )
                .setTimestamp( getTimestamp() )
                .setType( DealingEngineRequest.RequestType.CURRENT_STATUS )
            .build();
    }
}
