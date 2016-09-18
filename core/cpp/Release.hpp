#ifndef QUANTMODEL_RELEASE_H
#define QUANTMODEL_RELEASE_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class Release : public DataObject
    {
        public:


            //
            //  07.25.2011 -- note in next release, set closed as fixed value,
            //  large enough such that we can insert new order status's and
            //  the CLOSED value will always be at the botton.
            //

            enum ReleaseStatus {
                PENDING_NEW = 0,
                NEW,
                PARTIALLY_FILLED,
                FILLED,
                DONE_FOR_DAY,
                CANCELED,
                REPLACED,
                PENDING_CANCEL,
                PENDING_REPLACE,
                STOPPED,
                REJECTED,
                SUSPENDED,
                CLOSED,
                COMPLIANCE_REJECT
            };

            Release( const std::string& release_id )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();
                data.set_release_id( release_id );
                setAttribute( quantmodel::RELEASE_STATUS, PENDING_NEW );
                quantmodel::TimeStamp::getClientOrderId( *data.mutable_client_order_id() );
            }

            Release( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                for( int k=0; k<row.column_size(); ++k )
                {
                    setAttribute( row.column( k ).name(), row.column( k ).value() );
                }
            }

            ~Release(){};

            inline const std::string& getReleaseId() { return data.release_id(); }
            inline const std::string& getOrderId() { return data.order_id(); }
            inline const std::string& getAccountId() { return data.account_id(); }
            inline const std::string& getSessionId() { return data.session_id(); }
            inline const std::string& getAllocationId() { return data.allocation_id(); }
            inline const std::string& getInstrumentId() { return data.instrument_id(); }
            inline const std::string& getClientOrderId() { return data.client_order_id(); }

            inline void insert( quantmodel::DatabaseEngineRequest& request, const std::string& id ) const
            {
                quantmodel::QuantModelProcedures::INSERT_RELEASE(
                    request,
                    id,
                    data.release_id(),
                    data.session_id(),
                    data.account_id(),
                    data.order_id(),
                    data.allocation_id(),
                    data.investment_system_id(),
                    data.instrument_id(),
                    data.compliance_audit_id(),
                    data.client_order_id(),
                    data.counterparty_order_id(),
                    data.release_status(),
                    data.release_type(),
                    data.tif(),
                    data.side_code(),
                    data.release_qty(),
                    data.limit_prc(),
                    data.stop_prc(),
                    data.average_prc(),
                    data.executed_val(),
                    data.executed_qty(),
                    data.added_datetime(),
                    data.added_by(),
                    data.updated_datetime(),
                    data.updated_by() );
            }

            inline void update( quantmodel::DatabaseEngineRequest& request ) const
            {
                quantmodel::QuantModelProcedures::UPDATE_RELEASE(
                    request,
                    data.release_id(),
                    data.session_id(),
                    data.account_id(),
                    data.order_id(),
                    data.investment_system_id(),
                    data.instrument_id(),
                    data.client_order_id(),
                    data.counterparty_order_id(),
                    data.release_status(),
                    data.release_type(),
                    data.tif(),
                    data.side_code(),
                    data.release_qty(),
                    data.limit_prc(),
                    data.average_prc(),
                    data.executed_val(),
                    data.executed_qty(),
                    data.updated_datetime(),
                    data.updated_by() );
            }

            void asOrderMessage( quantmodel::OrderMessage * const order_msg ) const
            {
                copyTo( order_msg->add_release_data() );
            }

        private:

            // prevent copying and assignment; not implemented
            Release (const Release &);
            Release & operator= (const Release &);

            quantmodel::ReleaseData data;
  };
}

#endif // QUANTMODEL_RELEASE_H
