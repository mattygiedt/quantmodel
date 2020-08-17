#ifndef QUANTMODEL_TIMESTAMP_H
#define QUANTMODEL_TIMESTAMP_H

#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

using namespace std;

#define MESSAGE_PRELOAD_SZ 8192

namespace quantmodel
{
    //static pthread_mutex_t QUANTMODEL_TIMESTAMP_msg_mutex = PTHREAD_MUTEX_INITIALIZER;
    //static pthread_mutex_t QUANTMODEL_TIMESTAMP_cl_ord_mutex = PTHREAD_MUTEX_INITIALIZER;
    //static std::vector< std::string > QUANTMODEL_TIMESTAMP_msg_ids, QUANTMODEL_TIMESTAMP_cl_ord_ids;
    //static unsigned int QUANTMODEL_TIMESTAMP_msg_id_pos = 0, QUANTMODEL_TIMESTAMP_cl_ord_pos = 0;

    const static char QUANTMODEL_TIMESTAMP_alphanum[ 62 ] =
    {
        '0','1','2','3','4','5','6','7','8',
        '9','A','B','C','D','E','F','G','H',
        'I','J','K','L','M','N','O','P','Q',
        'R','S','T','U','V','W','X','Y','Z',
        'a','b','c','d','e','f','g','h','i',
        'j','k','l','m','n','o','p','q','r',
        's','t','u','v','w','x','y','z'
    };

    class TimeStamp
    {
        public:

            //
            // http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
            //
            //  Note that nano_diff can only be used for sub-second timing
            //

            const inline static long int nano_diff( const timespec& end , const timespec& start )
            {
                return end.tv_sec == start.tv_sec ?
                    end.tv_nsec - start.tv_nsec :
                    1000000000 + end.tv_nsec - start.tv_nsec;

                //if( end.tv_nsec > start.tv_nsec )
                //{
                //    //std::cout << end.tv_nsec
                //    //          << " - " << start.tv_nsec
                //    //          << " = " << end.tv_nsec - start.tv_nsec
                //    //          << std::endl;
                //
                //    return end.tv_nsec - start.tv_nsec;
                //}
                //else
                //{
                //    //std::cout << " end < start --> "
                //    //          << 1000000000L + end.tv_nsec
                //    //          << " - " << start.tv_nsec
                //    //          << " = " << 1000000000L + end.tv_nsec - start.tv_nsec
                //    //          << std::endl;
                //
                //    return 1000000000L + end.tv_nsec - start.tv_nsec;
                //}
            }

            const inline static void getUTCTime( double& time )
            {
                struct timeval tv;
                gettimeofday( &tv, NULL );
                time = double( tv.tv_sec ) + ( tv.tv_usec * 0.000001 );
            }

            const inline static void getUTCTime( std::string* const str, const bool assign = true )
            {
                char buffer[ 32 ];
                struct timeval tv;
                gettimeofday( &tv, NULL );
                sprintf( buffer, "%ld.%06ld", tv.tv_sec, tv.tv_usec );

                if( assign )
                    str->assign( buffer );
                else
                    str->append( buffer );
            }

            const inline static void getUTCTime( std::string& str, const bool assign = true )
            {
                char buffer[ 32 ];
                struct timeval tv;
                gettimeofday( &tv, NULL );
                sprintf( buffer, "%ld.%06ld", tv.tv_sec, tv.tv_usec );

                if( assign )
                    str.assign( buffer );
                else
                    str.append( buffer );
            }

            const inline static void getUTCTime( std::string& str, double& time, const bool assign = true )
            {
                char buffer[ 32 ];
                struct timeval tv;
                gettimeofday( &tv, NULL );
                time = double( tv.tv_sec ) + ( tv.tv_usec * 0.000001 );
                sprintf( buffer, "%ld.%06ld", tv.tv_sec, tv.tv_usec );

                if( assign )
                    str.assign( buffer );
                else
                    str.append( buffer );
            }

            const inline static void getUTCDay( std::string& str, const bool assign = true )
            {
                //
                //  Return a UTC (gmtime) string in form of yyyyMMdd
                //

                char buffer[ 16 ];
                struct timeval tv;
                gettimeofday( &tv, NULL );
                const time_t curtime = tv.tv_sec;
                strftime( buffer, 16, "%Y%m%d", gmtime( &curtime ) );

                if( assign )
                    str.assign( buffer );
                else
                    str.append( buffer );
            }

            const inline static void getLocalDay( std::string& str, const bool assign = true )
            {
                //
                //  Return a localtime string in form of yyyyMMdd
                //

                char buffer[ 16 ];
                struct timeval tv;
                gettimeofday( &tv, NULL );
                const time_t curtime = tv.tv_sec;
                strftime( buffer, 16, "%Y%m%d", localtime( &curtime ) );

                if( assign )
                    str.assign( buffer );
                else
                    str.append( buffer );
            }

            const inline static void getUTCHMS( std::string& str, const bool assign = true )
            {
                char buffer[ 32 ];
                const time_t& curtime = time( NULL );
                strftime( buffer, 32, "%H:%M:%S", gmtime( &curtime ) );

                if( assign )
                    str.assign( buffer );
                else
                    str.append( buffer );
            }

            const inline static void getLocalTime( std::string& str, const bool append = true )
            {
                //
                //  Return a localtime string in form of yyyyMMddhhmmssSSSSSS
                //

                char buffer[ 32 ];
                struct timeval tv;
                gettimeofday( &tv, NULL );
                const time_t curtime = tv.tv_sec;
                strftime( buffer, 15, "%Y%m%d%H%M%S", localtime( &curtime ) );
                sprintf( buffer + 14, "%06ld", tv.tv_usec );

                if( append )
                    str.append( buffer );
                else
                    str.assign( buffer );
            }

            const inline static void getTimeBarKey( std::string& str, const bool append = true )
            {
                //
                //  Return a UTC (gmtime) string in form of yyyyMMddhhmm
                //

                char buffer[ 16 ];
                struct timeval tv;
                gettimeofday( &tv, NULL );
                const time_t curtime = tv.tv_sec;
                strftime( buffer, 16, "%Y%m%d%H%M", gmtime( &curtime ) );

                if( append )
                    str.append( buffer );
                else
                    str.assign( buffer );
            }

            const inline static bool getFixTime( const char* const fix, time_t& tm )
            {
                int millis = 0;
                return getFixTime( fix, tm, millis );
            }

            const static bool getFixTime( const char* const fix, time_t& tm, int& millis )
            {
                tm = 0;
                const int str_len = strlen( fix );

                //
                //  Format the FIX timestamp string into a GMT time_t + millis
                //
                //      20100621-18:10:48.289
                //      20101213-18:26:13
                //

                if( str_len >= 17 )
                {
                    char buf[ 8 ];
                    struct tm * timeinfo;
                    time( &tm );
                    timeinfo = localtime( &tm );
                    //timeinfo = gmtime( &tm );

                    buf[ 0 ] = fix[ 0 ];
                    buf[ 1 ] = fix[ 1 ];
                    buf[ 2 ] = fix[ 2 ];
                    buf[ 3 ] = fix[ 3 ];
                    buf[ 4 ] = '\0';

                    const int year = atoi( buf );

                    buf[ 0 ] = fix[ 4 ];
                    buf[ 1 ] = fix[ 5 ];
                    buf[ 2 ] = '\0';

                    const int month = atoi( buf );

                    buf[ 0 ] = fix[ 6 ];
                    buf[ 1 ] = fix[ 7 ];
                    buf[ 2 ] = '\0';

                    const int day = atoi( buf );

                    buf[ 0 ] = fix[  9 ];
                    buf[ 1 ] = fix[ 10 ];
                    buf[ 2 ] = '\0';

                    const int hour = atoi( buf );

                    buf[ 0 ] = fix[ 12 ];
                    buf[ 1 ] = fix[ 13 ];
                    buf[ 2 ] = '\0';

                    const int minute = atoi( buf );

                    buf[ 0 ] = fix[ 15 ];
                    buf[ 1 ] = fix[ 16 ];
                    buf[ 2 ] = '\0';

                    const int second = atoi( buf );

                    timeinfo->tm_year = year - 1900;
                    timeinfo->tm_mon = month - 1;
                    timeinfo->tm_mday = day;
                    timeinfo->tm_hour = hour;
                    timeinfo->tm_min = minute;
                    timeinfo->tm_sec = second;
                    timeinfo->tm_isdst = -1;

                    tm = mktime( timeinfo );

                    if( str_len == 19 )
                    {
                        buf[ 0 ] = fix[ 18 ];
                        buf[ 1 ] = '\0';

                        millis = 100 * atoi( buf );
                    }
                    else if( str_len == 20 )
                    {
                        buf[ 0 ] = fix[ 18 ];
                        buf[ 1 ] = fix[ 19 ];
                        buf[ 2 ] = '\0';

                        millis = 10 * atoi( buf );
                    }
                    else if( str_len == 21 )
                    {
                        buf[ 0 ] = fix[ 18 ];
                        buf[ 1 ] = fix[ 19 ];
                        buf[ 2 ] = fix[ 20 ];
                        buf[ 3 ] = '\0';

                        millis = atoi( buf );
                    }
                    else
                    {
                        millis = 0;
                    }

                    return true;
                }
                else
                {
                    return false;
                }
            }

            const static void getCMETime(
                struct tm& timeinfo,
                char * const cme,
                char * buf,
                char * out )
            {
                buf[ 0 ] = cme[ 0 ];
                buf[ 1 ] = cme[ 1 ];
                buf[ 2 ] = cme[ 2 ];
                buf[ 3 ] = cme[ 3 ];
                buf[ 4 ] = '\0';

                const int year = atoi( buf );

                buf[ 0 ] = cme[ 4 ];
                buf[ 1 ] = cme[ 5 ];
                buf[ 2 ] = '\0';

                const int month = atoi( buf );

                buf[ 0 ] = cme[ 6 ];
                buf[ 1 ] = cme[ 7 ];
                buf[ 2 ] = '\0';

                const int day = atoi( buf );

                buf[ 0 ] = cme[ 8 ];
                buf[ 1 ] = cme[ 9 ];
                buf[ 2 ] = '\0';

                const int hour = atoi( buf );

                buf[ 0 ] = cme[ 10 ];
                buf[ 1 ] = cme[ 11 ];
                buf[ 2 ] = '\0';

                const int minute = atoi( buf );

                buf[ 0 ] = cme[ 12 ];
                buf[ 1 ] = cme[ 13 ];
                buf[ 2 ] = '\0';

                const int second = atoi( buf );

                buf[ 0 ] = cme[ 14 ];
                buf[ 1 ] = cme[ 15 ];
                buf[ 2 ] = cme[ 16 ];
                buf[ 3 ] = '\0';

                const int millis = atoi( buf );

                timeinfo.tm_year = year - 1900;
                timeinfo.tm_mon = month - 1;
                timeinfo.tm_mday = day;
                timeinfo.tm_hour = hour;
                timeinfo.tm_min = minute;
                timeinfo.tm_sec = second;
                timeinfo.tm_isdst = -1;

                sprintf( out, "%ld.%03d", timegm( &timeinfo ), millis );
            }

            const static void getCMETime( const long long int& sending_tm, std::string& out )
            {
                //
                //  Convert CME SendingTime into GMT time_t + millis
                //
                //      20120129223227492 --> 1327876347.492
                //

                char buf[ 8 ];
                char cme[ 32 ];
                struct tm timeinfo;

                sprintf( cme, "%lld", sending_tm );

                buf[ 0 ] = cme[ 0 ];
                buf[ 1 ] = cme[ 1 ];
                buf[ 2 ] = cme[ 2 ];
                buf[ 3 ] = cme[ 3 ];
                buf[ 4 ] = '\0';

                const int year = atoi( buf );

                buf[ 0 ] = cme[ 4 ];
                buf[ 1 ] = cme[ 5 ];
                buf[ 2 ] = '\0';

                const int month = atoi( buf );

                buf[ 0 ] = cme[ 6 ];
                buf[ 1 ] = cme[ 7 ];
                buf[ 2 ] = '\0';

                const int day = atoi( buf );

                buf[ 0 ] = cme[ 8 ];
                buf[ 1 ] = cme[ 9 ];
                buf[ 2 ] = '\0';

                const int hour = atoi( buf );

                buf[ 0 ] = cme[ 10 ];
                buf[ 1 ] = cme[ 11 ];
                buf[ 2 ] = '\0';

                const int minute = atoi( buf );

                buf[ 0 ] = cme[ 12 ];
                buf[ 1 ] = cme[ 13 ];
                buf[ 2 ] = '\0';

                const int second = atoi( buf );

                buf[ 0 ] = cme[ 14 ];
                buf[ 1 ] = cme[ 15 ];
                buf[ 2 ] = cme[ 16 ];
                buf[ 3 ] = '\0';

                const int millis = atoi( buf );

                timeinfo.tm_year = year - 1900;
                timeinfo.tm_mon = month - 1;
                timeinfo.tm_mday = day;
                timeinfo.tm_hour = hour;
                timeinfo.tm_min = minute;
                timeinfo.tm_sec = second;
                timeinfo.tm_isdst = -1;

                const time_t gmt = timegm( &timeinfo );
                sprintf( cme, "%ld.%03d", gmt, millis );
                out = cme;
            }

            const inline static bool getTimeBarKey( const char* const fix, std::string& out )
            {
                //
                //  Format the FIX timestamp string. Example:
                //
                //      20100621-18:10:48.289 --> 20100621181045
                //

                if( strlen( fix ) >= 17 )
                {
                    out.push_back( fix[ 0 ] );
                    out.push_back( fix[ 1 ] );
                    out.push_back( fix[ 2 ] );
                    out.push_back( fix[ 3 ] );
                    out.push_back( fix[ 4 ] );
                    out.push_back( fix[ 5 ] );
                    out.push_back( fix[ 6 ] );
                    out.push_back( fix[ 7 ] );

                    out.push_back( fix[ 9 ] );
                    out.push_back( fix[ 10 ] );

                    out.push_back( fix[ 12 ] );
                    out.push_back( fix[ 13 ] );

                    char second_arr[ 8 ];

                    second_arr[ 0 ] = fix[ 15 ];
                    second_arr[ 1 ] = fix[ 16 ];
                    second_arr[ 2 ] = '\0';

                    const int seconds = atoi( second_arr );

                    if( seconds < 15 )
                    {
                        out.append( "00" );
                    }
                    else if( seconds < 30 )
                    {
                        out.append( "15" );
                    }
                    else if( seconds < 45 )
                    {
                        out.append( "30" );
                    }
                    else
                    {
                        out.append( "45" );
                    }

                    return true;
                }
                else
                {
                    return false;
                }
            }

            const inline static double getMinuteFraction()
            {
                time_t rawtime;
                struct tm * timeinfo;

                time ( &rawtime );
                timeinfo = localtime ( &rawtime );

                if( timeinfo->tm_sec == 0 )
                {
                    return 0.016666;
                }
                else if( timeinfo->tm_sec >= 60 )
                {
                    return 1.0;
                }
                else
                {
                    return timeinfo->tm_sec / 60.0;
                }
            }

            static void getClientOrderId( std::string& str )
            {
                //static char buf[ 32 ];
                //pthread_mutex_lock( &QUANTMODEL_TIMESTAMP_cl_ord_mutex );
                //if( QUANTMODEL_TIMESTAMP_cl_ord_ids.size() == QUANTMODEL_TIMESTAMP_cl_ord_pos )
                //{
                //    QUANTMODEL_TIMESTAMP_cl_ord_pos = 0;
                //    genMessageIds( QUANTMODEL_TIMESTAMP_cl_ord_ids, 20 );
                //}
                //str = QUANTMODEL_TIMESTAMP_cl_ord_ids[ QUANTMODEL_TIMESTAMP_cl_ord_pos++ ];
                //pthread_mutex_unlock( &QUANTMODEL_TIMESTAMP_cl_ord_mutex );

                char buf[ 32 ];

                buf[  0 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  1 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  2 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  3 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                buf[  4 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  5 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  6 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  7 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                buf[  8 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  9 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 10 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 11 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                buf[ 12 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 13 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 14 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 15 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                buf[ 16 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 17 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 18 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 19 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                buf[ 20 ] = '\0';
                str = buf;
            }

            static void getMessageId( std::string* const str )
            {
                char buf[ 32 ];

                buf[  0 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  1 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  2 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  3 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                buf[  4 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  5 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  6 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  7 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                buf[  8 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  9 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 10 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 11 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                buf[ 12 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 13 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 14 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 15 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                buf[ 16 ] = '\0';
                str->assign( buf );
            }

            static void getMessageId( std::string& str )
            {
                //static char buf[ 32 ];
                //pthread_mutex_lock( &QUANTMODEL_TIMESTAMP_msg_mutex );
                //if( QUANTMODEL_TIMESTAMP_msg_ids.size() == QUANTMODEL_TIMESTAMP_msg_id_pos )
                //{
                //    QUANTMODEL_TIMESTAMP_msg_id_pos = 0;
                //    genMessageIds( QUANTMODEL_TIMESTAMP_msg_ids, 16 );
                //}
                //str = QUANTMODEL_TIMESTAMP_msg_ids[ QUANTMODEL_TIMESTAMP_msg_id_pos++ ];
                //pthread_mutex_unlock( &QUANTMODEL_TIMESTAMP_msg_mutex );

                char buf[ 32 ];

                buf[  0 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  1 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  2 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  3 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                buf[  4 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  5 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  6 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  7 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                buf[  8 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[  9 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 10 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 11 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                buf[ 12 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 13 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 14 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                buf[ 15 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                buf[ 16 ] = '\0';
                str = buf;
            }

        private:

            const static void genMessageIds( std::vector< std::string >& ids, const int length )
            {
                static char buf[ 32 ];

                if( ids.size() != MESSAGE_PRELOAD_SZ )
                {
                    ids.resize( MESSAGE_PRELOAD_SZ, "" );
                }

                for( int i=0; i<MESSAGE_PRELOAD_SZ; ++i )
                {
                    ids[ i ].clear();
                    ids[ i ].reserve( length );

                    if( length == 16 )
                    {
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );

                        buf[  0 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  1 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  2 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  3 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                        buf[  4 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  5 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  6 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  7 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                        buf[  8 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  9 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[ 10 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[ 11 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                        buf[ 12 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[ 13 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[ 14 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[ 15 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                        buf[ 16 ] = '\0';
                        ids[ i ] = buf;
                    }
                    else if( length == 20 )
                    {
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        //ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );

                        buf[  0 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  1 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  2 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  3 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                        buf[  4 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  5 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  6 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  7 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                        buf[  8 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[  9 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[ 10 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[ 11 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                        buf[ 12 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[ 13 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[ 14 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[ 15 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                        buf[ 16 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[ 17 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[ 18 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];
                        buf[ 19 ] = QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ];

                        buf[ 20 ] = '\0';
                        ids[ i ] = buf;
                    }
                    else
                    {
                        for( int j=0; j<length; ++j )
                        {
                            ids[ i ].push_back( QUANTMODEL_TIMESTAMP_alphanum[ rand() % (61) ] );
                        }
                    }
                }
            }

    };
}

#endif // QUANTMODEL_TIMESTAMP_H
