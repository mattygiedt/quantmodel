#ifndef QUANTMODEL_ORDER_H
#define QUANTMODEL_ORDER_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class Order : public DataObject
    {
        public:

            enum SideCode {
                BUY = 1,
                SELL,
                BUY_MINUS,
                SELL_PLUS,
                SELL_SHORT,
                SELL_SHORT_EXEMPT,
                UNDISCLOSED,
                CROSS,
                CROSS_SHORT
            };

            enum OrderType {
                MARKET = 1,
                LIMIT,
                STOP,
                STOP_LIMIT,
                MARKET_ON_CLOSE,
                MARKET_LIMIT
            };

            //
            //  07.25.2011 -- note in next release, set closed as fixed value,
            //  large enough such that we can insert new order status's and
            //  the CLOSED value will always be at the botton.
            //

            enum OrderStatus {
                SUPPRESSED = 0,
                NEW,
                PENDING_RELEASE,
                RELEASED,
                PARTIALLY_FILLED,
                FILLED,
                CANCELED,
                CLOSED,
                REJECTED
            };

            enum TimeInForce {
                DAY = 0,    //0: Day
                GTC = 1,    //1: Good Till Cancel (GTC)
                OPG = 2,    //2: At The Opening (OPG)
                IOC = 3,    //3: Immediate or Cancel (IOC)
                FOK = 4,    //4: Fill Or Kill (FOK)
                GTD = 6,    //6: Good Till Date (GTD)
                GIS = 8     //8: Good in Session (GIS)
            };

            Order( const std::string& order_id )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();
                data.set_order_id( order_id );
                setAttribute( quantmodel::ORDER_STATUS, NEW );
            }

            Order( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                for( int k=0; k<row.column_size(); ++k )
                {
                    setAttribute( row.column( k ).name(), row.column( k ).value() );
                }
            }

            ~Order()
            {
                std::vector< quantmodel::Allocation* >::const_iterator iter;
                for( iter = alloc_vec.begin(); iter != alloc_vec.end(); ++iter )
                {
                    delete (*iter);
                }

                alloc_vec.clear();
            };

            inline const std::string getOrderId() { return data.order_id(); }
            inline const std::string getExternalOrderId() { return data.external_order_id(); }
            inline const std::string getInstrumentId() { return data.instrument_id(); }
            inline void getAllocations( std::vector< quantmodel::Allocation* >& vec ) const { vec.assign( alloc_vec.begin(), alloc_vec.end() ); }
            inline void addAllocation( quantmodel::Allocation * const alloc ) { alloc_vec.push_back( alloc ); }

            inline quantmodel::Allocation* getAllocation( const std::string& alloc_id ) const
            {
                std::vector< quantmodel::Allocation* >::const_iterator alloc_iter;
                for( alloc_iter = alloc_vec.begin(); alloc_iter != alloc_vec.end(); ++alloc_iter )
                {
                    if( (*alloc_iter)->getAllocationId().compare( alloc_id ) == 0 )
                    {
                        return *alloc_iter;
                    }
                }

                return NULL;
            }

            void reject( )
            {
                setAttribute( quantmodel::ORDER_STATUS, quantmodel::Order::REJECTED );
                setAttribute( quantmodel::ORDER_QTY, "0" );
                setAttribute( quantmodel::OPEN_QTY, "0" );

                std::vector< quantmodel::Allocation* >::const_iterator alloc_iter;
                for( alloc_iter = alloc_vec.begin(); alloc_iter != alloc_vec.end(); ++alloc_iter )
                {
                    (*alloc_iter)->setAttribute( quantmodel::ALLOCATION_QTY, "0" );
                }
            }

            inline void insert( quantmodel::DatabaseEngineRequest& request, const std::string& id ) const
            {
                quantmodel::QuantModelProcedures::INSERT_ORDER(
                    request,
                    id,
                    data.order_id(),
                    data.external_order_id(),
                    data.investment_system_id(),
                    data.order_type(),
                    data.order_status(),
                    data.instrument_id(),
                    data.symbol(),
                    data.tif(),
                    data.side_code(),
                    data.base_ccy(),
                    data.order_qty(),
                    data.open_qty(),
                    data.released_qty(),
                    data.executed_qty(),
                    data.limit_prc(),
                    data.stop_prc(),
                    data.mark_prc(),
                    data.average_prc(),
                    data.entry_prc(),
                    data.released_val(),
                    data.executed_val(),
                    data.added_datetime(),
                    data.added_by(),
                    data.updated_datetime(),
                    data.updated_by() );
            }

            inline void update( quantmodel::DatabaseEngineRequest& request ) const
            {
                quantmodel::QuantModelProcedures::UPDATE_ORDER(
                    request,
                    data.order_id(),
                    data.order_type(),
                    data.order_status(),
                    data.instrument_id(),
                    data.symbol(),
                    data.tif(),
                    data.side_code(),
                    data.base_ccy(),
                    data.order_qty(),
                    data.open_qty(),
                    data.released_qty(),
                    data.executed_qty(),
                    data.limit_prc(),
                    data.stop_prc(),
                    data.mark_prc(),
                    data.average_prc(),
                    data.released_val(),
                    data.executed_val(),
                    data.updated_datetime(),
                    data.updated_by() );
            }

            void asOrderMessage( quantmodel::OrderMessage * const order_msg ) const
            {
                copyTo( order_msg->mutable_order_data() );

                std::vector< quantmodel::Allocation* >::const_iterator iter;
                for( iter = alloc_vec.begin(); iter != alloc_vec.end(); ++iter )
                {
                    (*iter)->copyTo( order_msg->add_allocation_data() );
                }
            }

        private:
            std::vector< quantmodel::Allocation* > alloc_vec;
            quantmodel::OrderData data;


            // prevent copying and assignment; not implemented
            Order (const Order &);
            Order & operator= (const Order &);
  };
}

#endif // QUANTMODEL_ORDER_H


