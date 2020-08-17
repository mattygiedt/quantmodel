package com.quantmodel.engine.task;

import com.quantmodel.engine.TaskEngine;
import com.quantmodel.engine.SchedulerTask;

import java.io.ByteArrayOutputStream;

import java.util.Date;
import java.util.TimerTask;
import java.util.TimeZone;
import java.util.Calendar;

import java.text.SimpleDateFormat;

import org.dom4j.Node;

import java.sql.Statement;
import java.sql.ResultSet;
import java.sql.Connection;

import org.apache.log4j.Logger;

public class RolloverInstrument extends SchedulerTask
{
    private static final Logger log = Logger.getLogger( RolloverInstrument.class );

    private String hour;
    private String minute;
    private String second;
    private String tz;
    private String database;
    private String symbol;

    public void init( Node node )
        throws Exception
    {
        name = node.valueOf( "@name" );
        database = node.valueOf( "params/@db" );
        symbol = node.valueOf( "instrument/@symbol" );
        hour = node.valueOf( "instrument/@hour" );
        minute = node.valueOf( "instrument/@minute" );
        second = node.valueOf( "instrument/@second" );
        tz = node.valueOf( "instrument/@tz" );

        Class.forName( "com.mysql.jdbc.Driver" ).newInstance();

        //
        //  <task name="ES CME E-MINI S&amp;P 500 FUTURES" class="com.quantmodel.engine.task.RolloverInstrument" >
        //      <params db="dev" />
        //      <instrument symbol="ES" rollover_tm="1515 CST" />
        //      <iterator class="com.quantmodel.engine.iterator.DailyIterator" days="MON,TUE,WED,THU,FRI" hour="15" minute="15" second="0" tz="CST"/>
        //  </task>
        //

        setScheduleIterator( node );
        log.info( " next execution time: " + iterator.current() );
    }

    public void run()
    {
        log.info( "RolloverInstrument: " + name + " started." );

        try
        {
            //
            //  Calculate the GMT rollover_datetime
            //

            Calendar local = Calendar.getInstance();
            local.setTime( new Date() );
    		local.setTimeZone( TimeZone.getTimeZone( tz ) );
    		local.set( Calendar.HOUR_OF_DAY, Integer.parseInt( hour ) );
    		local.set( Calendar.MINUTE, Integer.parseInt( minute ) );
    		local.set( Calendar.SECOND, Integer.parseInt( second ) );
    		local.set( Calendar.MILLISECOND, 0 );

            final String rollover_datetime = "" + local.getTime().getTime() / 1000;

            if( log.isDebugEnabled() )
            {
                log.debug( "rollover   DATE: " + local.getTime().toString() );
                log.debug( "rollover   HOUR: " + local.get( Calendar.HOUR_OF_DAY ) );
                log.debug( "rollover MINUTE: " + local.get( Calendar.MINUTE ) );
                log.debug( "rollover SECOND: " + local.get( Calendar.SECOND ) );
            }

            final Connection conn = getConnection( database );
            final Statement stmt = conn.createStatement();

            log.info( " rollover " + symbol + ": " + rollover_datetime );

            //
            //  Update the instrument table
            //

            stmt.executeUpdate( "UPDATE instrument SET rollover_datetime = '" + rollover_datetime + "' WHERE symbol = '" + symbol + "'" );

            //
            //  Release the database connection
            //

            stmt.close();
            conn.close();
        }
        catch( Throwable t )
        {
            log.error( "RolloverInstrument " + symbol + " error:", t );
        }

        log.info( "RolloverInstrument: " + name + " complete." );
    }
}
