package com.quantmodel.engine;

import java.util.Date;

import org.dom4j.Node;

public interface ScheduleIterator
{
    public Date next();
    public Date current();
    public void init( Node node ) throws Exception;
}
