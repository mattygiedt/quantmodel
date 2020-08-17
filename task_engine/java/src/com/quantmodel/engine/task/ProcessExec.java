package com.quantmodel.engine.task;

import com.quantmodel.engine.TaskEngine;
import com.quantmodel.engine.SchedulerTask;

import java.io.File;
import java.io.InputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.ByteArrayOutputStream;

import java.util.Date;
import java.util.List;
import java.util.TimerTask;
import java.util.ArrayList;

import org.dom4j.Node;

import org.zeromq.ZMQ;

import org.apache.log4j.Logger;

public class ProcessExec extends SchedulerTask
{
    private static final Logger log = Logger.getLogger( ProcessExec.class );

    private String directory = null;
    private ProcessBuilder processBuilder = null;
    private List<String> command = new ArrayList<String>();

    public void init( Node node )
        throws Exception
    {
        name = node.valueOf( "@name" );
        directory = node.valueOf( "exec/@dir" );
        command.add( node.valueOf( "exec/@command" ) );

        final List argumentList = node.selectNodes( "exec/arg" );

        for( Object obj : argumentList )
        {
            final Node arg = (Node)obj;
            command.add( arg.valueOf( "@value" ) );
        }

        processBuilder = new ProcessBuilder( command );

        if(! "".equals( directory ) )
        {
            processBuilder.directory( new File( directory ) );
        }

        //
        //  <task name="ProcessExec" class="com.quantmodel.engine.task.ProcessExec" >
        //      <iterator class="com.quantmodel.task.iterator.DailyIterator" hour="02" minute="00" second="00" tz="CST"/>
        //  </task>
        //

        setScheduleIterator( node );
        log.info( " next execution time: " + iterator.current() );
    }

    public void run()
    {
        try
        {
            log.info( "executing command: " + command.get( 0 ) +
                ", from: " + processBuilder.directory().getAbsolutePath() );

            final Process process = processBuilder.start();
            final InputStream is = process.getInputStream();
            final InputStreamReader isr = new InputStreamReader( is );
            final BufferedReader br = new BufferedReader( isr );

            String line;

            while( ( line = br.readLine() ) != null )
            {
                log.info( line );
            }
        }
        catch( Exception ex )
        {
            log.error( "ProcessExec error:", ex );
        }

        log.info( "ProcessExec " + command.get( 0 ) + " complete." );
    }
}
