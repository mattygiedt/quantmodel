#ifndef QUANTMODEL_ACCOUNT_LIMIT_H
#define QUANTMODEL_ACCOUNT_LIMIT_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class AccountLimit : public DataObject
    {
        public:

            AccountLimit( const std::string& account_id )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();
                data.set_account_id( account_id );
            }

            AccountLimit( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                for( int k=0; k<row.column_size(); ++k )
                {
                    setAttribute( row.column( k ).name(), row.column( k ).value() );
                }
            }

            ~AccountLimit(){};

            const std::string getAccountId() const { return data.account_id(); }
            const std::string getSymbol() const { return data.symbol(); }
            const std::string getSecurityExchange() const { return data.security_exchange(); }
            const std::string getSessionId() const { return data.session_id(); }
            const std::string getMaxLongQty() const { return data.max_open_long_qty(); }
            const std::string getMaxShortQty() const { return data.max_open_short_qty(); }
            const std::string getMaxReleaseQty() const { return data.max_release_qty(); }
            const bool isActive() const { return data.is_active() == "1"; }

            inline void insert( quantmodel::DatabaseEngineRequest& request ) const
            {
                quantmodel::QuantModelProcedures::INSERT_ACCOUNT_LIMIT(
                    request,
                    data.account_id(),
                    data.symbol(),
                    data.security_exchange(),
                    data.session_id(),
                    data.max_open_long_qty(),
                    data.max_open_short_qty(),
                    data.max_release_qty(),
                    data.is_active(),
                    data.added_datetime(),
                    data.added_by() );
            }

            inline void update( quantmodel::DatabaseEngineRequest& request ) const
            {
                quantmodel::QuantModelProcedures::UPDATE_ACCOUNT_LIMIT(
                    request,
                    data.account_id(),
                    data.symbol(),
                    data.security_exchange(),
                    data.session_id(),
                    data.max_open_long_qty(),
                    data.max_open_short_qty(),
                    data.max_release_qty(),
                    data.is_active(),
                    data.updated_datetime(),
                    data.updated_by() );
            }

        private:

            // prevent copying and assignment; not implemented
            AccountLimit (const AccountLimit &);
            AccountLimit & operator= (const AccountLimit &);

            quantmodel::AccountLimitData data;
    };
}

#endif // QUANTMODEL_ACCOUNT_LIMIT_H
