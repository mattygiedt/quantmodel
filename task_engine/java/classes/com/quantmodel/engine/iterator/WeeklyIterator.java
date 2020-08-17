package com.quantmodel.engine.iterator;

import com.quantmodel.engine.ScheduleIterator;

import java.util.Date;
import java.util.Calendar;
import java.util.TimeZone;

import org.dom4j.Node;

public class WeeklyIterator implements ScheduleIterator
{
	private final Calendar calendar = Calendar.getInstance();

	public void init( Node node )
	    throws Exception
	{
	    final Date date = new Date();

		calendar.setTime( date );
		calendar.setTimeZone( TimeZone.getTimeZone( node.valueOf( "@tz" ) ) );

		if( "SUNDAY".equals( node.valueOf( "@day" ) ) )
		{
		    calendar.set( Calendar.DAY_OF_WEEK, Calendar.SUNDAY );
		}
		else if( "MONDAY".equals( node.valueOf( "@day" ) ) )
		{
		    calendar.set( Calendar.DAY_OF_WEEK, Calendar.MONDAY );
		}
		else if( "TUESDAY".equals( node.valueOf( "@day" ) ) )
		{
		    calendar.set( Calendar.DAY_OF_WEEK, Calendar.TUESDAY );
		}
		else if( "WEDNESDAY".equals( node.valueOf( "@day" ) ) )
		{
		    calendar.set( Calendar.DAY_OF_WEEK, Calendar.WEDNESDAY );
		}
		else if( "THURSDAY".equals( node.valueOf( "@day" ) ) )
		{
		    calendar.set( Calendar.DAY_OF_WEEK, Calendar.THURSDAY );
		}
		else if( "FRIDAY".equals( node.valueOf( "@day" ) ) )
		{
		    calendar.set( Calendar.DAY_OF_WEEK, Calendar.FRIDAY );
		}
		else if( "SATURDAY".equals( node.valueOf( "@day" ) ) )
		{
		    calendar.set( Calendar.DAY_OF_WEEK, Calendar.SATURDAY );
		}
		else
		{
		    throw new Exception( "Unknown day field: " + node.valueOf( "day" ) );
		}

		calendar.set( Calendar.HOUR_OF_DAY, Integer.parseInt( node.valueOf( "@hour" ) ) );
		calendar.set( Calendar.MINUTE, Integer.parseInt( node.valueOf( "@minute" ) ) );
		calendar.set( Calendar.SECOND, Integer.parseInt( node.valueOf( "@second" ) ) );
		calendar.set( Calendar.MILLISECOND, 0 );

		if( calendar.getTime().before( date ) )
		{
			calendar.add( Calendar.DATE, 7 );
		}
	}

	public Date next()
	{
		calendar.add( Calendar.DATE, 7 );
		return calendar.getTime();
	}

	public Date current()
	{
	    return calendar.getTime();
	}
}
