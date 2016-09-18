#ifndef QUANTMODEL_MEMCACHEDCONNECTION_H
#define QUANTMODEL_MEMCACHEDCONNECTION_H

#include <string>
#include <algorithm>
#include <vector>

#include <libmemcached/memcached.hpp>

using namespace std;

namespace quantmodel
{
    class MemcachedConnection
    {
        public:

            MemcachedConnection( )
            {
                const char *connstr = getenv( "MEMCACHED_CONNECTION" );
                conn = new memcache::Memcache( connstr );
            }

            MemcachedConnection( const char* connstr )
            {
                conn = new memcache::Memcache( connstr );
            }

            MemcachedConnection( const std::string& connstr )
            {
                conn = new memcache::Memcache( connstr.c_str() );
            }

            const bool put( const std::string& key, std::string& value ) const
            {
                std::vector<char> vec;
                populate_vector( vec, value );
                return conn->set( key, vec, 0, 18 );
            }

            const bool get( const std::string& key, std::string& out ) const
            {
                bool flag = false;
                std::vector<char> vec;
                flag = conn->get( key, vec );
                copy_vec_to_string( vec, out );
                return flag;
            }

            ~MemcachedConnection()
            {
                if( NULL != conn )
                {
                    delete conn;
                }
            };

            inline const bool flush( unsigned int exp ) const { return conn->flush( exp ); }
            inline const std::string getServersList() const { return conn->getServersList(); }

        private:
            memcache::Memcache *conn;

            MemcachedConnection (const MemcachedConnection &);
            MemcachedConnection & operator= (const MemcachedConnection &);

            const inline void populate_vector( std::vector<char> &vec, const std::string &str ) const
            {
                vec.reserve( str.length() );
                vec.assign( str.begin(), str.end() );
            }

            const inline void copy_vec_to_string( std::vector<char> &vec, std::string &str ) const
            {
                if(! vec.empty() )
                {
                    str.reserve( vec.size() );
                    str.assign( vec.begin(), vec.end() );
                }
            }

    };
}

#endif // QUANTMODEL_MEMCACHEDCONNECTION_H
