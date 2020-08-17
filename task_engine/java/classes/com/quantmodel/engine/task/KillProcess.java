package com.quantmodel.engine.task;

import com.quantmodel.engine.TaskEngine;
import com.quantmodel.engine.SchedulerTask;

import java.io.File;
import java.io.Reader;
import java.io.InputStream;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;

import java.util.Date;
import java.util.List;
import java.util.TimerTask;
import java.util.ArrayList;
import java.util.StringTokenizer;

import java.text.CharacterIterator;
import java.text.StringCharacterIterator;

import org.dom4j.Node;

import org.zeromq.ZMQ;

import org.apache.log4j.Logger;

public class KillProcess extends SchedulerTask
{
    private static final Logger log = Logger.getLogger( KillProcess.class );

    private String cmd_id = null;

    public void init( Node node )
        throws Exception
    {
        name = node.valueOf( "@name" );
        cmd_id = node.valueOf( "process/@cmd_id" );

        //
        //  <task name="KillProcess" class="com.quantmodel.engine.task.KillProcess" >
        //      <iterator class="com.quantmodel.task.iterator.DailyIterator" hour="02" minute="00" second="00" tz="CST"/>
        //      <process cmd_id="FastEngine PROD"/>
        //  </task>
        //

        setScheduleIterator( node );
        log.info( " next execution time: " + iterator.current() );
    }

    public void run()
    {
        InputStream in = null;
        InputStream buffer = null;
        Reader reader = null;

        try
        {
            Process p = Runtime.getRuntime().exec( "ps -eo pid,cmd" );
            in = p.getInputStream();
            buffer = new BufferedInputStream( in );
            reader = new InputStreamReader( buffer );

            char[] charr = new char[ 1024 ];
            StringBuffer strbuff = new StringBuffer();

            while( true )
            {
                int r = reader.read( charr );
                if( r <= 0 )
                {
                    break;
                }

                strbuff.append( charr, 0, r );
            }

            if( reader != null )
            {
                try { reader.close(); } catch( IOException ioe ) {}
            }

            if( buffer != null )
            {
                try { buffer.close(); } catch( IOException ioe ) {}
            }

            if( in != null )
            {
                try { in.close(); } catch( IOException ioe ) {}
            }

            //
            //  Parse the result of ps
            //

            int tokenCount = 0;
            StringTokenizer st = new StringTokenizer( strbuff.toString(), "\n" );

            while( st.hasMoreTokens() )
            {
                String token = st.nextToken();

                if( tokenCount > 0 )
                {
                    //
                    //  Find the PID + PROCESS CMD
                    //

                    char ch;
                    boolean pid_found = false;
                    StringBuilder pid = new StringBuilder();
                    StringBuilder cmd = new StringBuilder();
                    StringCharacterIterator sci = new StringCharacterIterator( token );

                    while( ( ch = sci.next() ) != CharacterIterator.DONE )
                    {
                        if( pid_found == false && ch == ' ' && pid.length() != 0 )
                        {
                            pid_found = true;
                        }
                        else if( pid_found == false && ch != ' ' )
                        {
                            pid.append( ch );
                        }
                        else
                        {
                            cmd.append( ch );
                        }
                    }

                    //
                    //  Try to find the match id in the cmd string
                    //

                    if( cmd.toString().indexOf( cmd_id ) != -1 )
                    {
                        log.info( "kill pid: " + pid.toString() + " cmd: " + cmd.toString() );
                        Runtime.getRuntime().exec( "kill -9 " + pid.toString() );
                    }
                }

                ++tokenCount;
            }

        }
        catch( Exception ex )
        {
            log.error( "KillProcess error:", ex );
        }

        log.info( "KillProcess " + name + " complete." );
    }
}
