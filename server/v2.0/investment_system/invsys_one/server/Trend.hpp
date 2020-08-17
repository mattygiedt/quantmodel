#ifndef TREND_H
#define TREND_H

struct Trend;

enum TrendDirection
{
    UP_TREND = 0,
    DOWN_TREND = 1
};

struct Trend
{
    TrendDirection direction;

    time_t timestamp;
    double high;
    double low;

    Trend( const time_t& time, const double& h, const double& l, const TrendDirection d )
    {
        timestamp = time;
        high = h;
        low = l;
        direction = d;
    }

    double price()
    {
        return direction == UP_TREND ? high : low;
    }

    bool isUpTrend()
    {
        return direction == UP_TREND;
    }

    bool isDownTrend()
    {
        return direction == DOWN_TREND;
    }

    void print( log4cpp::Category * const log )
    {
        if( isUpTrend() )
        {
            log->info( "   [ UP_TREND %ld %f ]", timestamp, price() );
        }
        else
        {
            log->info( " [ DOWN_TREND %ld %f ]", timestamp, price() );
        }
    }
};

#endif // TREND_H