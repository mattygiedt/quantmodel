package com.quantmodel.engine.iterator;

import com.quantmodel.engine.ScheduleIterator;

import java.util.Date;

import org.dom4j.Node;

public class ImmediateIterator implements ScheduleIterator
{
	public void init( Node node )
	    throws Exception
	{	}

	public Date next()
	{
		return null;
	}

	public Date current()
	{
	    return new Date();
	}
}
