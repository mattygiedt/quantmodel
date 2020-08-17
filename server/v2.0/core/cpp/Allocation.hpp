#ifndef QUANTMODEL_ALLOCATION_H
#define QUANTMODEL_ALLOCATION_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class Allocation : public DataObject
    {
        public:

            Allocation( const std::string& alloc_id )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();
                data.set_allocation_id( alloc_id );
            }

            Allocation( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                for( int k=0; k<row.column_size(); ++k )
                {
                    setAttribute( row.column( k ).name(), row.column( k ).value() );
                }
            }

            ~Allocation(){};

            inline const std::string& getAllocationId() { return data.allocation_id(); }
            inline const std::string& getOrderId() { return data.order_id(); }
            inline const std::string& getAccountId() { return data.account_id(); }
            inline const std::string& getInstrumentId() { return data.instrument_id(); }

            inline void insert( quantmodel::DatabaseEngineRequest& request, const std::string& id ) const
            {
                quantmodel::QuantModelProcedures::INSERT_ALLOCATION(
                    request,
                    id,
                    data.allocation_id(),
                    data.order_id(),
                    data.account_id(),
                    data.instrument_id(),
                    data.side_code(),
                    data.allocation_qty(),
                    data.released_qty(),
                    data.executed_qty(),
                    data.executed_val(),
                    data.average_prc(),
                    data.added_datetime(),
                    data.added_by(),
                    data.updated_datetime(),
                    data.updated_by() );
            }

            inline void update( quantmodel::DatabaseEngineRequest& request ) const
            {
                quantmodel::QuantModelProcedures::UPDATE_ALLOCATION(
                    request,
                    data.allocation_id(),
                    data.order_id(),
                    data.account_id(),
                    data.instrument_id(),
                    data.side_code(),
                    data.allocation_qty(),
                    data.released_qty(),
                    data.executed_qty(),
                    data.executed_val(),
                    data.average_prc(),
                    data.updated_datetime(),
                    data.updated_by() );
            }

        private:

            // prevent copying and assignment; not implemented
            Allocation (const Allocation &);
            Allocation & operator= (const Allocation &);

            quantmodel::AllocationData data;
    };
}

#endif // QUANTMODEL_ALLOCATION_H
