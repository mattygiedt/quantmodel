package com.quantmodel.engine.task;

import com.quantmodel.engine.TaskEngine;
import com.quantmodel.engine.SchedulerTask;

import java.io.ByteArrayOutputStream;

import java.util.Date;
import java.util.TimerTask;

import org.dom4j.Node;

import java.sql.Statement;
import java.sql.ResultSet;
import java.sql.Connection;

import org.apache.log4j.Logger;

public class MarkInstrument extends SchedulerTask
{
    private static final Logger log = Logger.getLogger( MarkInstrument.class );

    private String mark_tm;
    private String database;
    private String price_factor;
    private String instrument_id;

    public void init( Node node )
        throws Exception
    {
        name = node.valueOf( "@name" );
        database = node.valueOf( "params/@db" );
        mark_tm = node.valueOf( "instrument/@mark_tm" );
        price_factor = node.valueOf( "instrument/@price_factor" );
        instrument_id = node.valueOf( "instrument/@instrument_id" );

        Class.forName( "com.mysql.jdbc.Driver" ).newInstance();

        //
        //  <task name="MARK: DEC 2010 E-mini Russell 2000 Future" class="com.quantmodel.engine.task.MarkInstrument" >
        //      <params db="dev" />
        //      <instrument price_factor="1" instrument_id="2011000" mark_tm="1515 CST" />
        //      <iterator class="com.quantmodel.engine.iterator.DailyIterator" days="MON,TUE,WED,THU,FRI" hour="15" minute="15" second="0" tz="CST"/>
        //  </task>
        //

        setScheduleIterator( node );
        log.info( " next execution time: " + iterator.current() );
    }

    public void run()
    {
        log.info( "MarkInstrument: " + name + " started." );

        try
        {
            //
            //  Calculate the mark_datetime
            //

            final String mark_datetime = getMarkTime( mark_tm );

            //
            //  Mark the instrument with the mark_prc and mark_datetime
            //

            final Connection conn = getConnection( database );
            final Statement stmt = conn.createStatement();

            //
            //  Get the mark_prc as the current last_prc for this instrument
            //

            String mark_prc = "0.0";
            final ResultSet rs = stmt.executeQuery( "SELECT last_prc FROM instrument WHERE instrument_id = " + instrument_id );
            if( rs.next() )
            {
                mark_prc = rs.getString( "last_prc" );
            }
            rs.close();

            log.info( " mark instrument: " + instrument_id +
                " [ " + mark_tm + " ] " + mark_datetime + " " + mark_prc );

            //
            //  Update the instrument / instrument_mark_price tables accordingly
            //

            stmt.executeUpdate( "UPDATE instrument SET mark_prc = " + mark_prc + ", mark_datetime = '" + mark_datetime + "' WHERE instrument_id = " + instrument_id );
            stmt.executeUpdate( "UPDATE instrument_mark_price SET is_current = 0 WHERE is_current = 1 AND instrument_id = " + instrument_id );
            stmt.executeUpdate( "INSERT INTO instrument_mark_price ( instrument_id, mark_prc, mark_datetime, is_current ) VALUES ( " + instrument_id + ", " + mark_prc + ", '" + mark_datetime + "', 1 )" );

            //
            //  Release the database connection
            //

            stmt.close();
            conn.close();
        }
        catch( Throwable t )
        {
            log.error( "MarkInstrument " + instrument_id + " error:", t );
        }

        log.info( "MarkInstrument: " + name + " complete." );
    }
}
