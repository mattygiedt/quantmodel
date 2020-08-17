package com.quantmodel.engine.task;

import com.quantmodel.engine.TaskEngine;
import com.quantmodel.engine.SchedulerTask;

import java.io.BufferedReader;
import java.io.InputStreamReader;

import java.net.URL;
import java.net.URLConnection;

import java.util.Date;
import java.util.List;
import java.util.ArrayList;
import java.util.TimerTask;
import java.util.StringTokenizer;

import java.sql.Statement;
import java.sql.Connection;

import org.dom4j.Node;

import org.apache.log4j.Logger;

public class MarkCMEInstruments extends SchedulerTask
{
    private static final Logger log = Logger.getLogger( MarkCMEInstruments.class );
    private String ftp_address;
    private String database = null;
    private List<Node> instruments = new ArrayList<Node>();

    public void init( Node node )
        throws Exception
    {
        name = node.valueOf( "@name" );
        database = node.valueOf( "params/@db" );
        Class.forName( "com.mysql.jdbc.Driver" ).newInstance();

        //
        //  <task name="MARK: CME Equity Products" class="com.quantmodel.engine.task.MarkCMEInstruments" >
        //      <ftp address="ftp://ftp.cmegroup.com/pub/settle/stleqt" />
        //      <instrument header="ES CME E-MINI S&P 500 FUTURES" contract="MAR11" instrument_id="2000003" />
        //      <instrument header="NQ CME E-MINI NASDAQ-100 FUTURES" contract="MAR11" instrument_id="2003002" />
        //      <iterator class="com.quantmodel.engine.iterator.DailyIterator" days="MON,TUE,WED,THU,FRI" hour="19" minute="00" second="00" tz="CST" />
        //  </task>
        //

        ftp_address = node.valueOf( "ftp/@address" );
        final List instrumentList = node.selectNodes( "instrument" );

        for( Object obj : instrumentList )
        {
            instruments.add( (Node)obj );
        }

        setScheduleIterator( node );
        log.info( " next execution time: " + iterator.current() );
    }

    public void run()
    {
        log.info( "MarkCMEInstruments: " + name + " started." );

        try
        {
            //
            //  Get the Settlement Data from the CME FTP Server
            //

            log.info( " ftp: " + ftp_address );

            String line = null;
            final List<String> ftp_data = new ArrayList<String>();
            final URL url = new URL( ftp_address );
            final URLConnection ftp_conn = url.openConnection( );
            final BufferedReader in = new BufferedReader(
                new InputStreamReader( ftp_conn.getInputStream( ) ) );

            while( ( line = in.readLine( ) ) != null )
            {
                ftp_data.add( line.trim() );
            }
            in.close();

            log.info( " ftp: " + ftp_address + " complete." );

            //
            //  Get the settlement date for this file
            //

            String settlement_date = null;
            final String [] header_tok = ftp_data.get( 0 ).split( " " );
            for( int i=0; i<header_tok.length; i++ )
            {
                if( log.isDebugEnabled() )
                {
                    log.debug( " header_tok[ " + i + " ]: '" + header_tok[ i ] + "'" );
                }

                if( header_tok[ i ].contains( "/" ) )
                {
                    settlement_date = header_tok[ i ];
                }
            }

            log.info( " settlement date: " + settlement_date );

            //
            //  For each instrument...
            //

            for( Node instrument : instruments )
            {
                String mark_prc = "0.0";
                boolean found_header = false;
                boolean found_contract = false;

                final String header = instrument.valueOf( "@header" );
                final String contract = instrument.valueOf( "@contract" );
                final String mark_tm = instrument.valueOf( "@mark_tm" );
                final String instrument_id = instrument.valueOf( "@instrument_id" );

                log.info( " marking contract: " + contract + " " + header );

                //
                //  Find the header row in the ftp_data
                //

                for( String row : ftp_data )
                {
                    if( header.equals( row ) )
                    {
                        found_header = true;
                    }

                    if( found_header == true && row.contains( contract ) )
                    {
                        found_contract = true;

                        log.info( "  contract data: " + row );

                        //
                        //  We found the appropriate row, parse the settlement price
                        //

                        int pos = 0;
                        final StringTokenizer tokens = new StringTokenizer( row );

                        //
                        //  MAR11   2249.75   2262.50   2236.50   2246.50   2245.50
                        //  JUN11   2247.75   2258.75B  2234.00   2243.25A  2242.25
                        //

                        while( tokens.hasMoreTokens() )
                        {
                            mark_prc = tokens.nextToken();
                            pos++;

                            if( pos == 6 )
                            {
                                break;
                            }
                        }

                        break;
                    }
                }

                if( found_header == false )
                {
                    log.error( "Could not find header row: " + header );
                }
                else if( found_header == true && found_contract == false )
                {
                    log.error( "Could not find contract: " + contract + " for header: " + header );
                }
                else
                {
                    //
                    //  Ensure that mark_prc is in fact a price
                    //

                    if( mark_prc.contains( "'" ) )
                    {
                        //
                        //  Need to convert commodity price.
                        //
                        //      EX: 393'6 --> 393 + 6 * .125 --> 393.75
                        //

                        final String [] commodity_prc = mark_prc.split( "'" );

                        if( "0".equals( commodity_prc[ 1 ] ) )
                        {
                            mark_prc = commodity_prc[ 0 ] + ".0";
                        }
                        else if( "1".equals( commodity_prc[ 1 ] ) )
                        {
                            mark_prc = commodity_prc[ 0 ] + ".125";
                        }
                        else if( "2".equals( commodity_prc[ 1 ] ) )
                        {
                            mark_prc = commodity_prc[ 0 ] + ".25";
                        }
                        else if( "3".equals( commodity_prc[ 1 ] ) )
                        {
                            mark_prc = commodity_prc[ 0 ] + ".375";
                        }
                        else if( "4".equals( commodity_prc[ 1 ] ) )
                        {
                            mark_prc = commodity_prc[ 0 ] + ".5";
                        }
                        else if( "5".equals( commodity_prc[ 1 ] ) )
                        {
                            mark_prc = commodity_prc[ 0 ] + ".625";
                        }
                        else if( "6".equals( commodity_prc[ 1 ] ) )
                        {
                            mark_prc = commodity_prc[ 0 ] + ".75";
                        }
                        else if( "7".equals( commodity_prc[ 1 ] ) )
                        {
                            mark_prc = commodity_prc[ 0 ] + ".875";
                        }
                        else
                        {
                            log.error( "Invalid commodity_prc: " + mark_prc );
                            continue;
                        }

                        try
                        {
                            mark_prc =
                                Double.toString(
                                    Double.parseDouble( mark_prc ) *
                                    Double.parseDouble(
                                        instrument.valueOf( "@price_factor" ) ) );
                        }
                        catch( Throwable t )
                        {
                            log.error( "Invalid mark_prc: " + mark_prc );
                            continue;
                        }
                    }
                    else
                    {
                        try
                        {
                            mark_prc =
                                Double.toString(
                                    Double.parseDouble( mark_prc ) *
                                    Double.parseDouble(
                                        instrument.valueOf( "@price_factor" ) ) );
                        }
                        catch( Throwable t )
                        {
                            log.error( "Invalid mark_prc: " + mark_prc );
                            continue;
                        }
                    }

                    //
                    //  Calculate the mark_datetime
                    //

                    final String mark_datetime = getMarkTime( settlement_date, mark_tm );

                    log.info( " mark instrument: " + instrument_id +
                        " [ " + settlement_date + " " + mark_tm + " ] " +
                            mark_datetime + " " + mark_prc );

                    //
                    //  Mark the instrument with the mark_prc and mark_datetime
                    //

                    final Connection conn = getConnection( database );
                    final Statement stmt = conn.createStatement();

                    stmt.executeUpdate( "UPDATE instrument SET mark_prc = " + mark_prc + ", mark_datetime = '" + mark_datetime + "' WHERE instrument_id = " + instrument_id );
                    stmt.executeUpdate( "UPDATE instrument_mark_price SET is_current = 0 WHERE is_current = 1 AND instrument_id = " + instrument_id );
                    stmt.executeUpdate( "INSERT INTO instrument_mark_price ( instrument_id, mark_prc, mark_datetime, is_current ) VALUES ( " + instrument_id + ", " + mark_prc + ", '" + mark_datetime + "', 1 )" );

                    //
                    //  Release the database connection
                    //

                    stmt.close();
                    conn.close();
                }
            }
        }
        catch( Throwable t )
        {
            log.error( "MarkCMEInstruments error:", t );
        }

        log.info( "MarkCMEInstruments: " + name + " complete." );
    }
}
