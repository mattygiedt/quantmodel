#ifndef QUANTMODEL_USER_H
#define QUANTMODEL_USER_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class User : public DataObject
    {
        public:

            User( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                for( int k=0; k<row.column_size(); ++k )
                {
                    setAttribute( row.column( k ).name(), row.column( k ).value() );
                }
            }

            ~User(){};

        private:

            // prevent copying and assignment; not implemented
            User (const User &);
            User & operator= (const User &);

            quantmodel::UserData data;
    };
}

#endif // QUANTMODEL_USER_H
