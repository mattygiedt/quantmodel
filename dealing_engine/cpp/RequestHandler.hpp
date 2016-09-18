#ifndef DEALING_REQUESTHANDLER_H
#define DEALING_REQUESTHANDLER_H

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "ActiveSet.hpp"
#include "DealingEngineParameters.hpp"

namespace dealing
{
    class RequestHandler
    {
        public:

            RequestHandler( dealing::ActiveSet * const actv_set,
                            dealing::DealingEngineParameters * const params )
            {
                char buf[ MAX_DIGITS ];
                active_set = actv_set;
                parameters = params;

                //
                //  Set some string constants
                //

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Order::NEW );
                NEW_ORDER = buf;

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Order::SUPPRESSED );
                SUPPRESSED_ORDER = buf;

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Order::PENDING_RELEASE );
                PENDING_RELEASE_ORDER = buf;

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Order::RELEASED );
                RELEASED_ORDER = buf;

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Order::PARTIALLY_FILLED );
                PARTIALLY_FILLED_ORDER = buf;

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Order::FILLED );
                FILLED_ORDER = buf;

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Release::NEW );
                NEW_RELEASE = buf;

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Release::CANCELED );
                CANCELED_RELEASE = buf;

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Release::PENDING_CANCEL );
                PENDING_CANCEL_RELEASE = buf;

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Release::REJECTED );
                REJECTED_RELEASE = buf;

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Release::PENDING_REPLACE );
                PENDING_REPLACE_RELEASE = buf;

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Release::REPLACED );
                REPLACED_RELEASE = buf;

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Release::PARTIALLY_FILLED );
                PARTIALLY_FILLED_RELEASE = buf;

                memset( buf, 0, MAX_DIGITS );
                sprintf( buf, "%d", quantmodel::Release::FILLED );
                FILLED_RELEASE = buf;
            }

            ~RequestHandler(){};

            void initLogging()
            {
                log = &log4cpp::Category::getInstance( "RequestHandler" );
            }

            void initNetwork( std::map< const std::string, zmq::socket_t* >& map )
            {
                std::map< const std::string, zmq::socket_t* >::iterator iter;
                iter = map.begin();
                while( iter != map.end() )
                {
                    socket_map.insert(
                        pair< const std::string , zmq::socket_t* >(
                            iter->first, iter->second ) );
                    ++iter;
                }
            }

            const void executeSignal(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& );

            const void executeDraftOrder(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& );

            const void createDraftOrder(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void createOrder(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void updateOrder(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void cancelOrder(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void releaseOrder(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& );

            const void createRelease(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& );

            const void validateRelease(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void cancelRelease(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& );

            const void replaceRelease(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& );

            const void releaseRejected(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void cancelRejected(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void releaseRestated(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void releaseCanceled(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void releaseReplaced(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void createExecution(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void queryOrder(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void queryRelease(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void queryExecution(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void currentStatus(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void updateStatus(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void fixEngineLogin(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void fixEngineLogout(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void executionCanceled(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void marketDataSubscribe(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& );

            const void marketDataUnsubscribe(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& );

            const void logoutFixEngine(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& );

            const void loginFixEngine(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& );

            const void reloadStaticData(
                const quantmodel::DealingEngineRequest&,
                quantmodel::DealingEngineResponse&,
                quantmodel::DealingEngineBroadcast&,
                quantmodel::DatabaseEngineRequest& );

            const void createAuditEvent(
                const quantmodel::DealingEngineRequest&,
                const quantmodel::DealingEngineResponse&,
                quantmodel::DatabaseEngineRequest& ) const;

            const void rollover(
                const quantmodel::DealingEngineRequest& request_msg,
                quantmodel::DealingEngineResponse& response_msg,
                quantmodel::DealingEngineBroadcast& broadcast_msg,
                quantmodel::DatabaseEngineRequest& database_msg )
            {
                active_set->rollover( socket_map[ "database_callforward" ] );

                response_msg.set_request_id( request_msg.request_id( ) );
                quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

                quantmodel::TimeStamp::getMessageId( broadcast_msg.mutable_broadcast_id( ) );
                quantmodel::TimeStamp::getUTCTime( broadcast_msg.mutable_timestamp( ) );
                broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::ROLLOVER );
                active_set->getActiveSet( broadcast_msg );
            }

            const void getActiveSet(
                const quantmodel::DealingEngineRequest& request_msg,
                quantmodel::DealingEngineResponse& response_msg,
                quantmodel::DealingEngineBroadcast& broadcast_msg,
                quantmodel::DatabaseEngineRequest& database_msg ) const
            {
                active_set->getActiveSet( response_msg );
                response_msg.set_request_id( request_msg.request_id( ) );
                quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );
            }

            const void broadcastActiveSet(
                const quantmodel::DealingEngineRequest& request_msg,
                quantmodel::DealingEngineResponse& response_msg,
                quantmodel::DealingEngineBroadcast& broadcast_msg,
                quantmodel::DatabaseEngineRequest& database_msg ) const
            {
                response_msg.set_request_id( request_msg.request_id( ) );
                quantmodel::TimeStamp::getMessageId( response_msg.mutable_response_id( ) );
                quantmodel::TimeStamp::getUTCTime( response_msg.mutable_timestamp( ) );
                response_msg.set_type( quantmodel::DealingEngineResponse::ACKNOWLEDGEMENT );

                active_set->getActiveSet( broadcast_msg );
                quantmodel::TimeStamp::getMessageId( broadcast_msg.mutable_broadcast_id( ) );
                quantmodel::TimeStamp::getUTCTime( broadcast_msg.mutable_timestamp( ) );
                broadcast_msg.set_type( quantmodel::DealingEngineBroadcast::BROADCAST_DATA );
            }

        private:

            // prevent copying and assignment; not implemented
            RequestHandler (const RequestHandler &);
            RequestHandler & operator= (const RequestHandler &);

            //char buf[ MAX_DIGITS ];

            dealing::ActiveSet *active_set;
            dealing::DealingEngineParameters *parameters;
            log4cpp::Category *log;

            //quantmodel::MemcachedConnection memcache;

            std::map< const std::string, zmq::socket_t* > socket_map;

            std::vector< quantmodel::AccountStrategy* > account_strategy_vec;

            std::map< const std::string, quantmodel::Account* > account_map;
            std::map< const std::string, quantmodel::Strategy* > strategy_map;
            std::map< const std::string, quantmodel::Instrument* > instrument_map;
            std::map< const std::pair< const std::string, const std::string >, std::set< std::string > > symbol_exchange_map;
            std::map< const std::string, quantmodel::InvestmentSystem* > investment_system_map;
            std::map< const std::string, std::map< const std::pair< const std::string, const std::string > , quantmodel::AccountLimit* > > account_limit_map;

            std::string NEW_ORDER,
                        RELEASED_ORDER,
                        SUPPRESSED_ORDER,
                        PENDING_RELEASE_ORDER,
                        PARTIALLY_FILLED_ORDER,
                        FILLED_ORDER,
                        NEW_RELEASE,
                        REJECTED_RELEASE,
                        CANCELED_RELEASE,
                        REPLACED_RELEASE,
                        PARTIALLY_FILLED_RELEASE,
                        FILLED_RELEASE,
                        PENDING_REPLACE_RELEASE,
                        PENDING_CANCEL_RELEASE;

            const void rebuildOrder(
                quantmodel::Order *order,
                quantmodel::Allocation *allocation,
                quantmodel::DatabaseEngineRequest& database_msg,
                const std::string& ts ) const
            {
                std::vector< quantmodel::Allocation* > alloc_vec;
                std::vector< quantmodel::Allocation* >::const_iterator alloc_iter;

                std::vector< quantmodel::Release* > release_vec;
                std::vector< quantmodel::Release* >::const_iterator release_iter;

                mpf_class alloc_released_qty ( 0 );
                mpf_class alloc_executed_qty ( 0 );
                mpf_class alloc_executed_val ( 0 );

                mpf_class order_executed_qty ( 0 );
                mpf_class order_executed_val ( 0 );
                mpf_class order_released_qty ( 0 );

                //
                //  Get the releases for this allocation
                //

                release_vec.clear();
                active_set->getReleasesByAllocationId( allocation->getAllocationId(), release_vec );

                for( release_iter = release_vec.begin(); release_iter != release_vec.end(); ++release_iter )
                {
                    const mpf_class release_release_qty ( (*release_iter)->getAttribute( quantmodel::RELEASE_QTY ) );
                    const mpf_class release_executed_qty ( (*release_iter)->getAttribute( quantmodel::EXECUTED_QTY ) );
                    const mpf_class release_executed_val ( (*release_iter)->getAttribute( quantmodel::EXECUTED_VAL ) );

                    alloc_released_qty += release_release_qty;
                    alloc_executed_qty += release_executed_qty;
                    alloc_executed_val += release_executed_val;
                }

                if( cmp( alloc_executed_qty, 0 ) > 0 )
                {
                    const mpf_class alloc_avg_prc = alloc_executed_val / alloc_executed_qty;
                    allocation->setAttribute( quantmodel::AVERAGE_PRC, alloc_avg_prc );
                }

                allocation->setAttribute( quantmodel::RELEASED_QTY, alloc_released_qty );
                allocation->setAttribute( quantmodel::EXECUTED_QTY, alloc_executed_qty );
                allocation->setAttribute( quantmodel::EXECUTED_VAL, alloc_executed_val );

                allocation->update( database_msg );

                //
                //  Get all allocations for this order
                //

                alloc_vec.clear();
                order->getAllocations( alloc_vec );

                for( alloc_iter = alloc_vec.begin(); alloc_iter != alloc_vec.end(); ++alloc_iter )
                {
                    alloc_released_qty = ( (*alloc_iter)->getAttribute( quantmodel::RELEASED_QTY ) );
                    alloc_executed_qty = ( (*alloc_iter)->getAttribute( quantmodel::EXECUTED_QTY ) );
                    alloc_executed_val = ( (*alloc_iter)->getAttribute( quantmodel::EXECUTED_VAL ) );

                    order_released_qty += alloc_released_qty;
                    order_executed_qty += alloc_executed_qty;
                    order_executed_val += alloc_executed_val;
                }

                //
                //  Update the order
                //

                const mpf_class order_qty ( order->getAttribute( quantmodel::ORDER_QTY ) );
                const mpf_class order_entry_prc ( order->getAttribute( quantmodel::ENTRY_PRC ) );

                order->setAttribute( quantmodel::RELEASED_QTY, order_released_qty );
                order->setAttribute( quantmodel::RELEASED_VAL, order_released_qty * order_entry_prc );
                order->setAttribute( quantmodel::EXECUTED_QTY, order_executed_qty );
                order->setAttribute( quantmodel::EXECUTED_VAL, order_executed_val );
                order->setAttribute( quantmodel::OPEN_QTY, order_qty - order_executed_qty );

                if( cmp( order_executed_qty, 0 ) > 0 )
                {
                    order->setAttribute( quantmodel::AVERAGE_PRC, ( order_executed_val / order_executed_qty ) );
                }

                if( cmp( order_qty, order_executed_qty ) == 0 )
                {
                    order->setAttribute( quantmodel::ORDER_STATUS, FILLED_ORDER );
                }
                else if( cmp( order_executed_qty, 0 ) > 0 )
                {
                    order->setAttribute( quantmodel::ORDER_STATUS, PARTIALLY_FILLED_ORDER );
                }
                else if( cmp( order_released_qty, 0 ) > 0 )
                {
                    order->setAttribute( quantmodel::ORDER_STATUS, RELEASED_ORDER );
                }
                else
                {
                    order->setAttribute( quantmodel::ORDER_STATUS, NEW_ORDER );
                }

                order->setAttribute( quantmodel::UPDATED_DATETIME, ts );
                order->setAttribute( quantmodel::UPDATED_BY, "DEALING_ENGINE" );

                //
                //  Update the order in the database
                //

                order->update( database_msg );
            }
    };
}

#endif // DEALING_REQUESTHANDLER_H
