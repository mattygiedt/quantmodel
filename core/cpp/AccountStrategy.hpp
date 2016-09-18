#ifndef QUANTMODEL_ACCOUNT_STRATEGY_H
#define QUANTMODEL_ACCOUNT_STRATEGY_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class AccountStrategy : public DataObject
    {
        public:

            AccountStrategy( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                for( int k=0; k<row.column_size(); ++k )
                {
                    setAttribute( row.column( k ).name(), row.column( k ).value() );
                }
            }

            ~AccountStrategy(){};

            const inline std::string getAccountId() const { return data.account_id(); }
            const inline std::string getStrategyId() const { return data.strategy_id(); }
            const inline std::string getWeight() const { return data.weight(); }
            const inline bool isEnabled() const { return data.is_enabled() == "1"; }

        private:

            // prevent copying and assignment; not implemented
            AccountStrategy (const AccountStrategy &);
            AccountStrategy & operator= (const AccountStrategy &);

            quantmodel::AccountStrategyData data;
    };
}

#endif // QUANTMODEL_ACCOUNT_STRATEGY_H
