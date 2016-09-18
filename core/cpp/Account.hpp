#ifndef QUANTMODEL_ACCOUNT_H
#define QUANTMODEL_ACCOUNT_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class Account : public DataObject
    {
        public:

            Account( const std::string& account_id )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();
                data.set_account_id( account_id );
            }

            Account( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                for( int k=0; k<row.column_size(); ++k )
                {
                    setAttribute( row.column( k ).name(), row.column( k ).value() );
                }
            }

            ~Account(){};

            const inline std::string getAccountId() const { return data.account_id(); }
            const inline bool isActive() const { return data.is_active() == "1"; }

        private:

            // prevent copying and assignment; not implemented
            Account (const Account &);
            Account & operator= (const Account &);

            quantmodel::AccountData data;
    };
}

#endif // QUANTMODEL_ACCOUNT_H
