package com.quantmodel.engine.task;

import com.quantmodel.engine.TaskEngine;
import com.quantmodel.engine.SchedulerTask;
import com.quantmodel.engine.message.MarketDataSubscribeMessage;

import quantmodel.Quantmodel.DealingEngineResponse;

import java.io.ByteArrayOutputStream;

import java.util.Date;
import java.util.TimerTask;
import java.util.zip.Deflater;
import java.util.zip.Inflater;

import org.dom4j.Node;

import org.zeromq.ZMQ;

import org.apache.log4j.Logger;

public class MarketDataSubscribe extends SchedulerTask
{
    private static final Logger log = Logger.getLogger( MarketDataSubscribe.class );

    private String session_id;
    private String instrument_id;

    public void init( Node node )
        throws Exception
    {
        name = node.valueOf( "@name" );

        session_id = node.valueOf( "instrument/@session_id" );
        instrument_id = node.valueOf( "instrument/@instrument_id" );

        //
        //  <task name="MarketDataSubscribe" class="com.quantmodel.engine.task.MarketDataSubscribe" >
        //      <instrument session_id="" instrument_id="" />
        //      <iterator class="com.quantmodel.task.iterator.DailyIterator" hour="02" minute="00" second="00" tz="CST"/>
        //  </task>
        //

        setScheduleIterator( node );
        log.info( " next execution time: " + iterator.current() );
    }

    public void run()
    {
        log.info( "Rollover started!!!" );

        try
        {
            ZMQ.Context ctx = ZMQ.context( 1 );
            final ZMQ.Socket zmq = ctx.socket( ZMQ.REQ );
            zmq.connect( TaskEngine.NETWORK_MAP.get( "dealing_callforward" ) );

            DealingEngineResponse.Builder response = DealingEngineResponse.newBuilder();
            response.mergeFrom( doZMQ( new MarketDataSubscribeMessage(
                session_id, instrument_id ).getMessage(), zmq ) );

            log.info( response.build().toString() );

            //
            //  Cleanup our ZMQ Context.
            //

            ctx.term();
        }
        catch( Exception ex )
        {
            log.error( "Rollover error:", ex );
        }

        log.info( "Rollover complete." );
    }
}
