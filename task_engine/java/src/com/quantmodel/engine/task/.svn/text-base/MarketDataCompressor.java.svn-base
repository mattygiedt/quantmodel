package com.quantmodel.engine.task;

import com.quantmodel.engine.TaskEngine;
import com.quantmodel.engine.SchedulerTask;

import java.io.File;
import java.io.FileWriter;
import java.io.InputStream;
import java.io.BufferedWriter;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.ByteArrayOutputStream;

import java.util.Date;
import java.util.List;
import java.util.TimerTask;
import java.util.ArrayList;

import java.util.zip.DeflaterOutputStream;

import java.sql.Statement;
import java.sql.ResultSet;
import java.sql.Connection;
import java.sql.PreparedStatement;

import java.text.DateFormat;
import java.text.SimpleDateFormat;

import org.dom4j.Node;

import org.zeromq.ZMQ;

import org.apache.log4j.Logger;

public class MarketDataCompressor extends SchedulerTask
{
    private static final Logger log = Logger.getLogger( MarketDataCompressor.class );

    public static final int STEP_SIZE = 4096;
    private final DateFormat ddf = new SimpleDateFormat( "yyyyMMdd" );

    private String database = null;
    private String directory = null;

    public void init( Node node )
        throws Exception
    {
        name = node.valueOf( "@name" );
        database = node.valueOf( "params/@db" );
        directory = node.valueOf( "params/@dir" );

        Class.forName( "com.mysql.jdbc.Driver" ).newInstance();

        //
        //  <task name="MarketDataCompressor" class="com.quantmodel.engine.task.MarketDataCompressor" >
        //      <iterator class="com.quantmodel.task.iterator.DailyIterator" hour="02" minute="00" second="00" tz="CST"/>
        //  </task>
        //

        setScheduleIterator( node );
        log.info( " next execution time: " + iterator.current() );
    }

    public void run()
    {
        ResultSet rs = null;
        Statement stmt = null;
        PreparedStatement pstmt = null;

        final List<String> instruments = new ArrayList<String>();

        try
        {
            //
            //  Get a new database connection
            //

            if( log.isDebugEnabled() )
            {
                log.debug( " database conn: " + database );
            }

            int max_id = 0;
            final Connection conn = getConnection( database );

            //
            //  Get the instruments...
            //

            if( log.isDebugEnabled() )
            {
                log.debug( " executing: SELECT instrument_id FROM market_data_event GROUP BY instrument_id" );
            }

            stmt = conn.createStatement();
            stmt.executeQuery( "SELECT instrument_id FROM market_data_event GROUP BY instrument_id" );

            rs = stmt.getResultSet();

            while( rs.next() )
            {
                if( log.isDebugEnabled() )
                {
                    log.debug( " adding market data instrument: " + rs.getString( "instrument_id" ) );
                }

                instruments.add( rs.getString( "instrument_id" ) );
            }

            if( log.isDebugEnabled() )
            {
                log.debug( " instrument size: " + instruments.size() );
            }

            rs.close();
            stmt.close();

            //
            //  Iterate through each instrument
            //

            for( String instrument_id : instruments )
            {
                log.info( "Compressing market data for instrument: " + instrument_id );

                //
                //  What is the maximum market_data_event_id for this instrument?
                //

                if( log.isDebugEnabled() )
                {
                    log.debug( " executing: SELECT IFNULL( max( market_data_event_id ), 0 ) as max_id FROM market_data_event WHERE instrument_id = " + instrument_id );
                }

                stmt = conn.createStatement();
                rs = stmt.executeQuery( "SELECT IFNULL( max( market_data_event_id ), 0 ) as max_id FROM market_data_event WHERE instrument_id = " + instrument_id );

                if( rs.next() )
                {
                    max_id = rs.getInt( "max_id" );
                }

                rs.close();
                stmt.close();

                log.info( " max_id: " + max_id );

                if( max_id > 0 )
                {
                    //
                    //  Create the writer used to store the insert statements
                    //

                    final File f_dir = new File( directory );

                    if(! f_dir.exists() )
                    {
                        if(! f_dir.mkdirs() )
                        {
                            throw new Exception( "Cannot create directory: " +
                                f_dir.getAbsolutePath() );
                        }
                    }

                    final File f_sql = new File( f_dir, instrument_id +
                        "." + ddf.format( new Date() ) + ".sql" );

                    final BufferedWriter out =
                        new BufferedWriter(
                            new FileWriter( f_sql ) );

                    //
                    //  Loop from 0 -> max_id pulling market_data_event data
                    //  formatted as an insert statement.
                    //

                    int beg_id = 0;
                    final StringBuffer sb = new StringBuffer();

                    while( beg_id <= max_id )
                    {
                        sb.setLength( 0 );

                        //
                        //  Create the prepared statement
                        //

                        pstmt = conn.prepareStatement( "SELECT event_type, event_action, event_prc, event_qty, event_tm, timebar_key FROM market_data_event WHERE instrument_id = ? and market_data_event_id BETWEEN ? AND ?" );

                        pstmt.setString( 1, instrument_id );
                        pstmt.setInt( 2, beg_id );
                        pstmt.setInt( 3, beg_id + STEP_SIZE );

                        rs = pstmt.executeQuery();

                        while( rs.next() )
                        {
                            sb.append( "INSERT INTO market_data_event ( instrument_id, event_type, event_action, event_prc, event_qty, event_tm, timebar_key ) VALUES ( " )
                              .append( instrument_id ).append( ", '" )
                              .append( rs.getString( "event_type" ) ).append( "', '" )
                              .append( rs.getString( "event_action" ) ).append( "', " )
                              .append( rs.getString( "event_prc" ) ).append( ", " )
                              .append( rs.getString( "event_qty" ) ).append( ", '" )
                              .append( rs.getString( "event_tm" ) ).append( "', '" )
                              .append( rs.getString( "timebar_key" ) ).append( "' );\n" );
                        }

                        rs.close();
                        pstmt.close();

                        //
                        //  Write the new insert statements
                        //

                        if( sb.length() > 0 )
                        {
                            out.write( sb.toString() );
                        }

                        //
                        //  Move our beg_id up by the step size
                        //

                        beg_id += STEP_SIZE;
                    }

                    //
                    //  Close the output file
                    //

                    out.flush();
                    out.close();
/*
                    //
                    //  Now, zip the SQL file
                    //

                    final FileInputStream f_in = new FileInputStream( f_sql );

                    final File f_gzip = new File( f_dir, instrument_id +
                        "." + ddf.format( new Date() ) + ".sql.gz" );

                    final GZIPOutputStream gzip_os =
                        new GZIPOutputStream( new FileOutputStream( f_gzip ) );

                    for( int c = f_in.read(); c != -1; c = f_in.read( ) )
                    {
                        gzip_os.write( c );
                    }

                    gzip_os.finish( );
                    gzip_os.close( );

                    f_in.close( );

                    //
                    //  Delete the SQL file
                    //

                    if(! f_sql.delete( ) )
                    {
                        log.warn( "Could not delete SQL file: " + f_sql.getAbsolutePath( ) );
                    }
*/
                }
            }

            //
            //  Truncate the market_data_event table
            //

            stmt = conn.createStatement();
            stmt.executeUpdate( "TRUNCATE market_data_event" );
            stmt.close();

            //
            //  Release the database connection
            //

            conn.close();
        }
        catch( Exception ex )
        {
            log.error( "MarketDataCompressor error:", ex );
        }

        log.info( "MarketDataCompressor complete." );
    }
}
