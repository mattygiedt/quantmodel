package com.quantmodel.engine;

import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.log4j.Logger;

public class Scheduler
{
    private static final Logger log = Logger.getLogger( Scheduler.class );

    class SchedulerTimerTask extends TimerTask
    {
        private SchedulerTask schedulerTask;
        private ScheduleIterator iterator;

        public SchedulerTimerTask( SchedulerTask schedulerTask )
        {
            this( schedulerTask, schedulerTask.getIterator() );
        }

        public SchedulerTimerTask( SchedulerTask schedulerTask, ScheduleIterator iterator )
        {
            this.schedulerTask = schedulerTask;
            this.iterator = iterator;
        }

        public void run()
        {
            schedulerTask.run();
            reschedule( schedulerTask, iterator );
        }
    }

    private final Timer timer = new Timer();

    public void cancel()
    {
        timer.cancel();
    }

    public void schedule( SchedulerTask schedulerTask )
    {
        schedule( schedulerTask, schedulerTask.getIterator() );
    }

    public void schedule( SchedulerTask schedulerTask, ScheduleIterator iterator )
    {
        Date time = iterator.current();

        if (time == null)
        {
            schedulerTask.cancel();
        }
        else
        {
            synchronized( schedulerTask.lock )
            {
                if( schedulerTask.state != SchedulerTask.VIRGIN )
                {
                    throw new IllegalStateException( "Task already scheduled or cancelled" );
                }

                schedulerTask.state = SchedulerTask.SCHEDULED;
                schedulerTask.timerTask = new SchedulerTimerTask( schedulerTask, iterator );
                timer.schedule( schedulerTask.timerTask, time );
            }
        }
    }

    private void reschedule( SchedulerTask schedulerTask, ScheduleIterator iterator )
    {
        Date time = iterator.next();
        if( time == null )
        {
            schedulerTask.cancel();
        }
        else
        {
            synchronized( schedulerTask.lock )
            {
                if( schedulerTask.state != SchedulerTask.CANCELLED )
                {
                    log.info( "Scheduling task: " + schedulerTask.getName() + " --> " + time );

                    schedulerTask.timerTask = new SchedulerTimerTask( schedulerTask, iterator );
                    timer.schedule( schedulerTask.timerTask, time );
                }
            }
        }
    }
}

