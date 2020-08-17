package com.quantmodel.engine.iterator;

import com.quantmodel.engine.ScheduleIterator;

import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;

import org.dom4j.Node;

public class RestrictedDailyIterator implements ScheduleIterator
{
	private int[] days;
	private final Calendar calendar = Calendar.getInstance();

	public void init( Node node )
	    throws Exception
	{
		//this.hourOfDay = hourOfDay;
		//this.minute = minute;
		//this.second = second;
		//this.days = (int[]) days.clone();
		//Arrays.sort(this.days);
        //
		//calendar.setTime(date);
		//calendar.set(Calendar.HOUR_OF_DAY, hourOfDay);
		//calendar.set(Calendar.MINUTE, minute);
		//calendar.set(Calendar.SECOND, second);
		//calendar.set(Calendar.MILLISECOND, 0);
		//if (!calendar.getTime().before(date)) {
		//	calendar.add(Calendar.DATE, -1);
		//}
	}

	public Date next()
	{
		do
		{
			calendar.add( Calendar.DATE, 1 );
		}
		while( Arrays.binarySearch( days, calendar.get( Calendar.DAY_OF_WEEK ) ) < 0 );

		return calendar.getTime();
	}

	public Date current()
	{
	    return calendar.getTime();
	}
}
