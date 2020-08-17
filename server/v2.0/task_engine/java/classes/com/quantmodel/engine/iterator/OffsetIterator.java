package com.quantmodel.engine.iterator;

import com.quantmodel.engine.ScheduleIterator;

import java.util.Date;
import java.util.Calendar;
import java.util.TimeZone;

import org.dom4j.Node;

public class OffsetIterator implements ScheduleIterator
{
	private int field;
	private int offset;

	private final Calendar calendar = Calendar.getInstance();

	public void init( Node node )
	    throws Exception
	{
		offset = Integer.parseInt( node.valueOf( "@offset" ) );
		calendar.setTimeZone( TimeZone.getTimeZone( node.valueOf( "@tz" ) ) );

		if( "HOUR".equals( node.valueOf( "@field" ) ) )
		{
		    field = Calendar.HOUR;
		}
		else if( "MINUTE".equals( node.valueOf( "@field" ) ) )
		{
		    field = Calendar.MINUTE;
		}
		else if( "SECOND".equals( node.valueOf( "@field" ) ) )
		{
		    field = Calendar.SECOND;
		}
		else if( "DATE".equals( node.valueOf( "@field" ) ) )
		{
		    field = Calendar.DATE;
		}
		else
		{
		    throw new Exception( "Unknown calendar field: " + node.valueOf( "@field" ) );
		}

        next();
	}

	public Date next()
	{
	    calendar.setTime( new Date() );
		calendar.add( field, offset );
		return calendar.getTime();
	}

	public Date current()
	{
	    return calendar.getTime();
	}
}
