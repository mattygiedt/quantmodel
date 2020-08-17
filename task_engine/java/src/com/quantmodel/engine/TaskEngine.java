package com.quantmodel.engine;

import java.io.File;

import java.util.Map;
import java.util.List;
import java.util.Date;
import java.util.Timer;
import java.util.HashMap;
import java.util.TimerTask;

import org.dom4j.Node;
import org.dom4j.Element;
import org.dom4j.Attribute;
import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.io.XPP3Reader;
import org.dom4j.io.XMLWriter;
import org.dom4j.io.OutputFormat;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

public class TaskEngine
{
    public static final String VERSION_ID = "1.0.0.0";
    private static final Logger log = Logger.getLogger( TaskEngine.class );

    public static final Map<String,String>
        NETWORK_MAP = new HashMap<String,String>();

    private final Scheduler scheduler = new Scheduler();

    private TaskEngine( final Document params )
        throws Exception
    {
        PropertyConfigurator.configure( params.valueOf( "/TaskEngine/@log4j" ) );

        //
        //  Load the network map
        //

        final Element networkNode = (Element)params.selectSingleNode( "/TaskEngine/network" );

        for( Object obj : networkNode.attributes() )
        {
            Attribute attr = (Attribute)obj;
            NETWORK_MAP.put( attr.getName(), attr.getValue() );

            log.info( attr.getName() + " --> " + attr.getValue() );
        }

        //
        //  Load the individual tasks
        //

        final List taskList = params.selectNodes( "/TaskEngine/task" );

        for( Object obj : taskList )
        {
            final Node node = (Node)obj;
            final String name = node.valueOf( "@name" );
            final String clazz = node.valueOf( "@class" );

            log.info( "Scheduling task: " + name );

            final SchedulerTask task = (SchedulerTask)
                Class.forName( clazz ).newInstance();
            task.init( node );
            scheduler.schedule( task );
        }
    }

    public static void main( String [] args )
        throws Exception
    {
        System.out.println( "QuantModel TaskEngine: " + VERSION_ID );

        final XPP3Reader reader = new XPP3Reader();

        File xml = null;

        if( args.length == 0 )
        {
            xml = new File( "../config/taskEngine.xml" );
        }
        else
        {
            xml = new File( args[ 0 ] );
        }

        if( xml.exists() )
        {
            System.out.println(
                "Loading TaskEngine configuration from: " +
                    xml.getAbsolutePath() );

            new TaskEngine( reader.read( xml ) );
        }
        else
        {
            System.out.println( "Error: cannot find configuration file: " + xml.getName() );
        }
    }
}
