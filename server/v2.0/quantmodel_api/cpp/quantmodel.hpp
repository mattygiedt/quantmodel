#ifndef QUANTMODEL_API_H
#define QUANTMODEL_API_H

#define MAX_DIGITS  32

#define FORCE_CACHE_LINE_ALIGNMENT __attribute__ ((aligned (64)))

#define likely(x)   __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)

#include <sys/time.h>
#include <time.h>

#include <zmq.hpp>
#include "xmlParser.hpp"

#include <google/protobuf/descriptor.h>
#include "quantmodel.pb.h"

namespace quantmodel
{
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

        static void getMessageId( std::string& str )
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
            str = buf;
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
    };

    class ZMQMessage
    {
        public:

        const inline static bool sendRPC( const google::protobuf::Message * const request,
                                                google::protobuf::Message *response,
                                                zmq::socket_t * const socket )
        {
            //
            //  ZMQ send + recv
            //

            if(! send( request, socket ) ) return false;
            return recv( response, socket );
        }

        const inline static bool sendRPC( const google::protobuf::Message * const protobuf,
                                                zmq::socket_t * const socket )
        {
            //
            //  ZMQ send + recv, used for when we don't care about the response
            //

            zmq::message_t msg;
            if(! send( protobuf, socket ) ) return false;
            return socket->recv( &msg );
        }

        const inline static bool sendRPC( std::string& msg_data, zmq::socket_t * const socket )
        {
            //
            //  ZMQ send + recv
            //

            if(! send( msg_data.data(), msg_data.size(), socket ) ) return false;
            return recv( msg_data, socket );
        }

        const inline static bool sendRPC( const char * const msg_data, zmq::socket_t * const socket )
        {
            //
            //  ZMQ send + recv, used for when we don't care about the response
            //

            zmq::message_t msg;
            if(! send( msg_data, strlen( msg_data ), socket ) ) return false;
            return socket->recv( &msg );
        }

        const inline static bool sendRPC( const XMLNode& xml, std::string& response_data, zmq::socket_t * const socket )
        {
            //
            //  ZMQ send + recv
            //

            if(! send( xml, socket ) ) return false;
            return recv( response_data, socket );
        }

        const inline static bool sendRPC( const char * const msg_data, std::string& response_data, zmq::socket_t * const socket )
        {
            //
            //  ZMQ send + recv
            //

            if(! send( msg_data, strlen( msg_data ), socket ) ) return false;
            return recv( response_data, socket );
        }

        const static bool send( const XMLNode& node , zmq::socket_t * const socket )
        {
            bool flag = true;
            char* xml_char = node.createXMLString();
            if(! send( xml_char, strlen( xml_char ), socket ) ) flag = false;
            freeXMLString( xml_char );
            return flag;
        }

        const inline static bool send( const std::string& msg_data, zmq::socket_t * const socket )
        {
            return send( msg_data.data(), msg_data.size(), socket );
        }

        const inline static bool send( const char* const msg_data, zmq::socket_t * const socket )
        {
            return send( msg_data, strlen( msg_data ), socket );
        }

        const inline static bool send( const char* const msg_data, const size_t& msg_len, zmq::socket_t * const socket )
        {
            zmq::message_t msg ( msg_len );
            memcpy( msg.data(), msg_data, msg_len );
            return socket->send( msg );
        }

        const inline static bool send( const google::protobuf::Message * const protobuf, zmq::socket_t * const socket )
        {
            zmq::message_t msg ( protobuf->ByteSize() );
            protobuf->SerializeWithCachedSizesToArray( (google::protobuf::uint8*)msg.data() );  //static_cast<google::protobuf::uint8*>( msg.data() ) );
            return socket->send( msg );
        }

        const static bool recv( std::string& msg_data, zmq::socket_t * const socket )
        {
            zmq::message_t msg;
            if( socket->recv( &msg ) == false )
            {
                fprintf( stderr, "ZMQ RECV ERROR: %s", zmq_strerror( zmq_errno() ) );
                return false;
            }

            msg_data.reserve( msg.size() );
            msg_data.assign( static_cast<char*>( msg.data() ), msg.size() );
            return true;
        }

        const inline static bool recv( google::protobuf::Message * const protobuf, zmq::socket_t * const socket )
        {
            zmq::message_t msg;
            socket->recv( &msg );
            return protobuf->ParsePartialFromArray( msg.data(), msg.size() );
        }
    };
}

#endif // QUANTMODEL_API_H
