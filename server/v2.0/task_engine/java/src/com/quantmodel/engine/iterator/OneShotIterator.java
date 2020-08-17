package com.quantmodel.engine.iterator;

import com.quantmodel.engine.ScheduleIterator;

import java.util.Date;
import java.util.Calendar;
import java.util.TimeZone;

import org.dom4j.Node;

public class OneShotIterator implements ScheduleIterator
{
	private final Calendar calendar = Calendar.getInstance();

	public void init( Node node )
	    throws Exception
	{
        final Date date = new Date();

		calendar.setTime( date );
		calendar.setTimeZone( TimeZone.getTimeZone( node.valueOf( "@tz" ) ) );
		calendar.set( Calendar.HOUR_OF_DAY, Integer.parseInt( node.valueOf( "@hour" ) ) );
		calendar.set( Calendar.MINUTE, Integer.parseInt( node.valueOf( "@minute" ) ) );
		calendar.set( Calendar.SECOND, Integer.parseInt( node.valueOf( "@second" ) ) );
		calendar.set( Calendar.MILLISECOND, 0 );

		if( calendar.getTime().before( date ) )
		{
			calendar.add( Calendar.DATE, 1 );
		}
	}

	public Date next()
	{
		return null;
	}

	public Date current()
	{
	    return calendar.getTime();
	}
}
