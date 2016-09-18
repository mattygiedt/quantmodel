#ifndef QUANTMODEL_STRATEGY_H
#define QUANTMODEL_STRATEGY_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class Strategy : public DataObject
    {
        public:

            Strategy( const std::string& strategy_id )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();
                data.set_strategy_id( strategy_id );
            }

            Strategy( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                for( int k=0; k<row.column_size(); ++k )
                {
                    setAttribute( row.column( k ).name(), row.column( k ).value() );
                }
            }

            ~Strategy(){};

            const inline std::string getSignalId() const { return data.signal_id(); }
            const inline std::string getStrategyId() const { return data.strategy_id(); }
            const inline std::string getInvestmentSystemId() const { return data.investment_system_id(); }

        private:

            // prevent copying and assignment; not implemented
            Strategy (const Strategy &);
            Strategy & operator= (const Strategy &);

            quantmodel::StrategyData data;
    };
}

#endif // QUANTMODEL_STRATEGY_H
