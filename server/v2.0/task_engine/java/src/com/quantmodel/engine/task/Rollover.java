package com.quantmodel.engine.task;

import com.quantmodel.engine.TaskEngine;
import com.quantmodel.engine.SchedulerTask;
import com.quantmodel.engine.message.RolloverMessage;

import quantmodel.Quantmodel.DealingEngineResponse;

import java.io.ByteArrayOutputStream;

import java.util.Date;
import java.util.TimerTask;
import java.util.zip.Deflater;
import java.util.zip.Inflater;

import org.dom4j.Node;

import org.zeromq.ZMQ;

import org.apache.log4j.Logger;

public class Rollover extends SchedulerTask
{
    private static final Logger log = Logger.getLogger( Rollover.class );

    ZMQ.Socket zmq = null;

    public void init( Node node )
        throws Exception
    {
        name = node.valueOf( "@name" );

        //
        //  Create the ZMQ socket
        //

        ZMQ.Context ctx = ZMQ.context( 1 );
        zmq = ctx.socket( ZMQ.REQ );
        zmq.connect( TaskEngine.NETWORK_MAP.get( "dealing_callforward" ) );

        //
        //  <task name="Rollover" class="com.quantmodel.engine.task.Rollover" >
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
            DealingEngineResponse.Builder response = DealingEngineResponse.newBuilder();
            response.mergeFrom( doZMQ( new RolloverMessage().getMessage(), zmq ) );

            log.info( response.build().toString() );
        }
        catch( Exception ex )
        {
            log.error( "Rollover error:", ex );
        }

        log.info( "Rollover complete." );
    }
}
