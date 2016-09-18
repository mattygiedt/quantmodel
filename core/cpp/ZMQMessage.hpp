#ifndef ZMQMESSAGE_H
#define ZMQMESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

//#include <log4cpp/Category.hh>
#include <zmq.hpp>
#include <zlib.h>

#include <list>

#include "xmlParser.hpp"

#define DESTINATION_LENGTH 8192
#define SIZE_OF_UINT32 sizeof( uint32_t )
#define COMPRESSION_LENGTH(n) (n+(((n)/1000)+1)+12)

namespace quantmodel
{
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
/*
        const static bool send( const char* msg_data, zmq::socket_t* socket )
        {
            if( memcmp( "<bdsq></bdsq>", msg_data, 13 ) == 0 )
            {
                static const Byte RPC_ACK[ 18 ] = { 120,-100,-77,73,74,41,46,-76,-77,-47,7,83,12,0,35,-115,4,120 };

                zmq::message_t msg ( 18 );
                memcpy( msg.data(), RPC_ACK, 18 );
                return socket->send( msg );
            }

            //
            //  ZLib compress
            //

            const long int str_len = strlen( msg_data ) ;
            uLong compr_len = (uLong) COMPRESSION_LENGTH( str_len + 1 );
            Byte* compr = (Byte *)malloc( (uInt)compr_len );
            memset( compr, 0, compr_len );
            const int err = compress( compr, &compr_len, (const Bytef*)msg_data, str_len );

            if( err != Z_OK )
            {
                log4cpp::Category::getInstance( "ZMQMessage" ).error( "ZLIB compression error %d", err );
                return false;
            }

            zmq::message_t msg ( compr_len + SIZE_OF_UINT32 );

            const uint32_t len = htonl( str_len );
            memcpy( msg.data(), &len, SIZE_OF_UINT32 );
            memcpy( (char *)msg.data() + SIZE_OF_UINT32, compr, compr_len );

            if(! socket->send( msg ) )
            {
                free( compr );
                return false;
            }

            free( compr );
            return true;
        }
*/
/*
        const static bool recv( std::string& msg_data, zmq::socket_t* socket )
        {
            static const Byte RPC_ACK[ 18 ] = { 120,-100,-77,73,74,41,46,-76,-77,-47,7,83,12,0,35,-115,4,120 };

            zmq::message_t msg;
            if(! socket->recv( &msg ) )
            {
                log4cpp::Category* log = &log4cpp::Category::getInstance( "ZMQMessage" );
                log->error( "ZMQ ERROR: %d", zmq_errno() );
                return false;
            }

            if( memcmp( RPC_ACK, msg.data(), 18 ) == 0 )
            {
                msg_data = "<bdsq></bdsq>";
                return true;
            }

            //
            //  Read the string length
            //

            uint32_t len;
            memcpy( &len, msg.data(), SIZE_OF_UINT32 );
            long int str_len = ntohl( len );

            //
            //  Create a new array large enough to hold the uncompressed
            //  string plus the null terminator.
            //

            char *str = new char[ str_len + 1 ];
            memset( str, 0, str_len + 1 );

            //
            //  ZLib uncompress
            //

            int err = msg_uncompress( str, &str_len,
                (char *)msg.data() + SIZE_OF_UINT32,
                    msg.size() - SIZE_OF_UINT32 );

            if( err == Z_BUF_ERROR )
            {
                log4cpp::Category::getInstance( "ZMQMessage" ).error(
                    "Z_BUF_ERROR ERROR: msg str_len %ld, z_stream str_len %ld ", ntohl( len ), str_len );

                delete[] str;
                str = new char[ str_len + 1 ];
                memset( str, 0, str_len + 1 );
                err = msg_uncompress( str, &str_len,
                    (char *)msg.data() + SIZE_OF_UINT32,
                        msg.size() - SIZE_OF_UINT32 );
            }

            if( err != Z_OK )
            {
                log4cpp::Category::getInstance( "ZMQMessage" ).error( "ZLIB ERROR: %d", err );
                return false;
            }

            if( str_len != (long)strlen( str ) )
            {
                log4cpp::Category::getInstance( "ZMQMessage" ).error(
                    " String Length Error: expected %ld != actual %ld", str_len, strlen( str ) );
            }

            msg_data.reserve( str_len + 1 );
            msg_data.assign( str );

            delete[] str;

            return true;
        }
*/


        private:

        const static int msg_uncompress( char* pDest, long* pnDestLen, void* pSrc, const long int nSrcLen )
        {
            //
            //  http://www.firstobject.com/easy-zlib-c++-xml-compression.htm
            //

            z_stream stream;
            int err;

        	int nExtraChunks;
        	uInt destlen;

            stream.next_in = (Bytef*)pSrc;
            stream.avail_in = (uInt)nSrcLen;
            /* Check for source > 64K on 16-bit machine: */
            if ((uLong)stream.avail_in != (uLong)nSrcLen) return Z_BUF_ERROR;

        	destlen = (uInt)*pnDestLen;
        	if ((uLong)destlen != (uLong)*pnDestLen) return Z_BUF_ERROR;

            stream.zalloc = (alloc_func)0;
            stream.zfree = (free_func)0;

            err = inflateInit(&stream);
            if (err != Z_OK) return err;

        	nExtraChunks = 0;
            do {
        		stream.next_out = (Bytef*)pDest;
        		stream.avail_out = destlen;
                err = inflate(&stream, Z_FINISH);
        		if (err == Z_STREAM_END )
        			break;
        		if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
        			err = Z_DATA_ERROR;
        		if (err != Z_BUF_ERROR) {
        			inflateEnd(&stream);
        			return err;
        		}
        		nExtraChunks += 1;
            } while (stream.avail_out == 0);

            *pnDestLen = stream.total_out;

            err = inflateEnd(&stream);
            if (err != Z_OK) return err;

            return nExtraChunks ? Z_BUF_ERROR : Z_OK;
        }

    };
}

#endif // ZMQMESSAGE_H
