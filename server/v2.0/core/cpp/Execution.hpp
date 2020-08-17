#ifndef QUANTMODEL_EXECUTION_H
#define QUANTMODEL_EXECUTION_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class Execution : public DataObject
    {
        public:

            //
            //  07.25.2011 -- note in next release, set closed as fixed value,
            //  large enough such that we can insert new order status's and
            //  the CLOSED value will always be at the botton.
            //

            enum ExecutionStatus {
                NEW = 0,
                UNALLOCATED,
                ALLOCATED,
                CANCELED,
                CLOSED
            };

            Execution( const std::string& execution_id )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();
                data.set_execution_id( execution_id );
                setAttribute( quantmodel::EXECUTION_STATUS, NEW );
            }

            Execution( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                for( int k=0; k<row.column_size(); ++k )
                {
                    setAttribute( row.column( k ).name(), row.column( k ).value() );
                }
            }

            ~Execution(){};

            inline const std::string& getExecutionId() { return data.execution_id(); }
            inline const std::string& getOrderId() { return data.order_id(); }
            inline const std::string& getReleaseId() { return data.release_id(); }
            inline const std::string& getAccountId() { return data.account_id(); }
            inline const std::string& getInvestmentSystemId() { return data.investment_system_id(); }
            inline const std::string& getInstrumentId() { return data.instrument_id(); }

            inline const bool isSpreadExecution() { return data.leg_type() == "3"; }

            inline void insert( quantmodel::DatabaseEngineRequest& request, const std::string& id ) const
            {
                quantmodel::QuantModelProcedures::INSERT_EXECUTION(
                    request,
                    id,
                    data.execution_id(),
                    data.session_id(),
                    data.account_id(),
                    data.order_id(),
                    data.release_id(),
                    data.investment_system_id(),
                    data.instrument_id(),
                    data.client_order_id(),
                    data.counterparty_execution_id(),
                    data.side_code(),
                    data.execution_ccy(),
                    data.execution_status(),
                    data.execution_prc(),
                    data.execution_qty(),
                    data.execution_val(),
                    data.leaves_qty(),
                    data.average_prc(),
                    data.leg_type(),
                    data.added_datetime(),
                    data.added_by(),
                    data.updated_datetime(),
                    data.updated_by() );
            }

            inline void update( quantmodel::DatabaseEngineRequest& request ) const
            {
                quantmodel::QuantModelProcedures::UPDATE_EXECUTION(
                    request,
                    data.execution_id(),
                    data.session_id(),
                    data.account_id(),
                    data.order_id(),
                    data.release_id(),
                    data.investment_system_id(),
                    data.instrument_id(),
                    data.client_order_id(),
                    data.counterparty_execution_id(),
                    data.side_code(),
                    data.execution_ccy(),
                    data.execution_status(),
                    data.execution_prc(),
                    data.execution_qty(),
                    data.execution_val(),
                    data.leaves_qty(),
                    data.average_prc(),
                    data.updated_datetime(),
                    data.updated_by() );
            }

            void asOrderMessage( quantmodel::OrderMessage * const order_msg ) const
            {
                copyTo( order_msg->add_execution_data() );
            }

        private:

            // prevent copying and assignment; not implemented
            Execution (const Execution &);
            Execution & operator= (const Execution &);

            quantmodel::ExecutionData data;
    };
}

#endif // QUANTMODEL_EXECUTION_H
