#ifndef PIVOT_H
#define PIVOT_H

struct Pivot;
struct PivotLeg;

enum PivotDirection
{
    UP_PIVOT = 0,
    DOWN_PIVOT = 1
};

struct Pivot
{
    PivotDirection direction;

    time_t timestamp;
    double price;

    Pivot( const time_t& time, const double& p, const PivotDirection d )
    {
        timestamp = time;
        price = p;
        direction = d;
    }

    bool isUpPivot()
    {
        return direction == UP_PIVOT;
    }

    bool isDownPivot()
    {
        return direction == DOWN_PIVOT;
    }

    void print( log4cpp::Category * const log )
    {
        if( isUpPivot() )
        {
            log->info( "   [ UP_PIVOT %ld %f ]", timestamp, price );
        }
        else
        {
            log->info( " [ DOWN_PIVOT %ld %f ]", timestamp, price );
        }
    }
};

struct PivotLeg
{
    time_t begin_tm;
    time_t end_tm;

    double begin_prc;
    double end_prc;

    PivotLeg( Pivot * const left, Pivot * const right )
    {
        begin_tm = left->timestamp;
        end_tm = right->timestamp;

        begin_prc = left->price;
        end_prc = right->price;
    }

    time_t duration()
    {
        return begin_tm - end_tm;
    }

    double height()
    {
        return begin_prc - end_prc;
    }

    double top()
    {
        return begin_prc > end_prc ? begin_prc : end_prc;
    }

    double bottom()
    {
        return begin_prc > end_prc ? end_prc : begin_prc;
    }

    double slope()
    {
        return height() / double( duration() );
    }

    void fibonacci( double * arr )
    {
        const double h = fabs( height() );

        arr[ 0 ] = bottom();
        arr[ 1 ] = 0.236 * h + arr[ 0 ];
        arr[ 2 ] = 0.382 * h + arr[ 0 ];
        arr[ 3 ] = 0.5 * h + arr[ 0 ];
        arr[ 4 ] = 0.618 * h + arr[ 0 ];
        arr[ 5 ] = 0.764 * h + arr[ 0 ];
        arr[ 6 ] = top();
        arr[ 7 ] = 0.0;
    }

    void print( log4cpp::Category * const log )
    {
        log->info( " [ %ld %f ] [ %ld %f ]",
            begin_tm, begin_prc, end_tm, end_prc );
    }
};

#endif // PIVOT_H