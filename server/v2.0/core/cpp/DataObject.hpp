#ifndef QUANTMODEL_DATAOBJECT_H
#define QUANTMODEL_DATAOBJECT_H

#include <gmpxx.h>

#include <quantmodel.hpp>

namespace quantmodel
{
    class DataObject
    {
        public:

            inline const std::string getAttribute( const std::string& name ) const
            {
                return reflector->GetString( *message, descriptor->FindFieldByName( name ) );
            }

            inline const void setAttribute( const std::string& name, const std::string& value )
            {
                reflector->SetString( message, descriptor->FindFieldByName( name ), value );
            }

            inline const void setAttribute( const std::string& name, const int& value )
            {
                char buf[ MAX_DIGITS ];
                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", value );
                reflector->SetString( message, descriptor->FindFieldByName( name ), buf );
            }

            inline const void setAttribute( const std::string& name, const mpf_class& f )
            {
                char buf[ MAX_DIGITS ];
                memset( buf, 0, MAX_DIGITS );
                gmp_snprintf( buf, MAX_DIGITS, GMP_FORMAT, &f );
                reflector->SetString( message, descriptor->FindFieldByName( name ), buf );
            }

            inline const std::string toString( const mpf_class& f ) const
            {
                std::string str;
                char buf[ MAX_DIGITS ];
                memset( buf, 0, MAX_DIGITS );
                gmp_snprintf( buf, MAX_DIGITS, GMP_FORMAT, &f );
                str = buf;
                return str;
            }

            inline void toString( std::string& str, const mpf_class& f ) const
            {
                char buf[ MAX_DIGITS ];
                memset( buf, 0, MAX_DIGITS );
                gmp_snprintf( buf, MAX_DIGITS, GMP_FORMAT, &f );
                str = buf;
            }

            inline const void copyTo( google::protobuf::Message * const new_data ) const
            {
                new_data->CopyFrom( *message );
            }

            inline const std::string debugString() const
            {
                return message->ShortDebugString();
            }

        protected:

            google::protobuf::Message *message;
            const google::protobuf::Reflection * reflector;
            const google::protobuf::Descriptor * descriptor;

        private:
            //char buf[ MAX_DIGITS ];
    };
}

#endif // QUANTMODEL_DATAOBJECT_H