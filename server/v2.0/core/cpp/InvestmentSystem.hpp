#ifndef QUANTMODEL_INVESTMENT_SYSTEM_H
#define QUANTMODEL_INVESTMENT_SYSTEM_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class InvestmentSystem : public DataObject
    {
        public:

            InvestmentSystem( const std::string& investment_system_id )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();
                data.set_investment_system_id( investment_system_id );
            }

            InvestmentSystem( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                for( int k=0; k<row.column_size(); ++k )
                {
                    setAttribute( row.column( k ).name(), row.column( k ).value() );
                }
            }

            ~InvestmentSystem()
            {
                //
                //  Note that the strategy pointers within the strategy_vec
                //  are not deleted here!
                //
            };

            inline const std::string getInvestmentSystemId() const { return data.investment_system_id(); }
            inline const bool hasStrategy( const std::string& strategy_id )
            {
                for( std::vector< quantmodel::Strategy* >::const_iterator iter = strategy_vec.begin();
                     iter != strategy_vec.end(); ++iter )
                {
                    if( strategy_id == (*iter)->getStrategyId() ) return true;
                }
                return false;
            }

            inline void getStrategies( std::vector< quantmodel::Strategy* >& vec ) const { vec.assign( strategy_vec.begin(), strategy_vec.end() ); }
            inline void addStrategy( quantmodel::Strategy *strategy ) { strategy_vec.push_back( strategy ); }

        private:

            // prevent copying and assignment; not implemented
            InvestmentSystem (const InvestmentSystem &);
            InvestmentSystem & operator= (const InvestmentSystem &);

            std::vector< quantmodel::Strategy* > strategy_vec;
            quantmodel::InvestmentSystemData data;
    };
}

#endif // QUANTMODEL_INVESTMENT_SYSTEM_H
