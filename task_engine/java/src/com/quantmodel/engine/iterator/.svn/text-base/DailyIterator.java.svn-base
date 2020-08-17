package com.quantmodel.engine.iterator;

import com.quantmodel.engine.ScheduleIterator;

import java.util.Date;
import java.util.Calendar;
import java.util.TimeZone;
import java.text.SimpleDateFormat;

import org.dom4j.Node;

public class DailyIterator implements ScheduleIterator
{
    private String days;
	private final Calendar calendar = Calendar.getInstance();
	private final SimpleDateFormat sdf = new SimpleDateFormat( "EEE" );

	public void init( Node node )
	    throws Exception
	{
	    final Date date = new Date();
	    days = node.valueOf( "@days" ).toLowerCase();

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

		//
		//  Roll to the next execution day
		//

		int offset = 0;
		while(! isValidDay() )
		{
		    if( offset > 6 )
		    {
		        throw new Exception( "Invalid days field: " + days );
		    }

		    calendar.add( Calendar.DATE, 1 );
		    offset++;
		}
	}

	public Date next()
	{
	    calendar.add( Calendar.DATE, 1 );

		while(! isValidDay() )
		{
		    calendar.add( Calendar.DATE, 1 );
		}

		return calendar.getTime();
	}

	public Date current()
	{
	    return calendar.getTime();
	}

	private boolean isValidDay()
	{
	    //
		//  Is today a day we execute on?
		//

		if( "all".equals( days ) || "".equals( days ) )
		{
		    return true;
		}
		else
		{
		    return days.contains( sdf.format( calendar.getTime() ).toLowerCase() );
		}
	}
}
