package com.quantmodel.engine.message;

import quantmodel.Quantmodel.DealingEngineRequest;

import java.util.UUID;
import java.util.Calendar;
import java.util.TimeZone;

import java.text.DecimalFormat;

import org.apache.log4j.Logger;

public abstract class BaseMessage
{
    private static final DecimalFormat decimalFormat = new DecimalFormat( "0000000000.000000" );

    public String getTimestamp()
    {
        return decimalFormat.format( Calendar.getInstance(
            TimeZone.getTimeZone( "GMT" ) ).getTimeInMillis() * 0.001 );
    }

    public abstract com.google.protobuf.Message getMessage();
}
