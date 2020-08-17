#ifndef DEALING_ACTIVESET_H
#define DEALING_ACTIVESET_H

#include <zmq.hpp>

#include <quantmodel.hpp>

/*
    TODO: Need to add:
        order_release_map,
        order_execution_map,
        release_execution_map
*/

namespace dealing
{
    class ActiveSet
    {
        public:
            ActiveSet();
            ~ActiveSet();

            void initLogging();

            void loadOrders( zmq::socket_t* );
            void loadReleases( zmq::socket_t* );
            void loadExecutions( zmq::socket_t* );
            void loadOvernightPositions( zmq::socket_t* );

            void getActiveSet( quantmodel::DealingEngineResponse& ) const;
            void getActiveSet( quantmodel::DealingEngineBroadcast& ) const;

            void getOrdersByInstrumentId( const std::string& , std::vector< quantmodel::Order* >& ) const;
            void getAllocationsByAccountId( const std::string& , std::vector< quantmodel::Allocation* >& ) const;
            void getReleasesByOrderId( const std::string& , std::vector< quantmodel::Release* >& ) const;
            void getReleasesByAccountId( const std::string& , std::vector< quantmodel::Release* >& ) const;
            void getReleasesByAllocationId( const std::string& , std::vector< quantmodel::Release* >& ) const;
            void getExecutionsByOrderId( const std::string& , std::vector< quantmodel::Execution* >& ) const;
            void getExecutionsByReleaseId( const std::string& , std::vector< quantmodel::Execution* >& ) const;

            void rollover( zmq::socket_t* );
            void close();

            inline bool hasOrderByOrderId( const std::string& id ) const
            {
                return order_map.find( id ) != order_map.end();
            }

            inline bool hasOrderByExternalOrderId( const std::string& id ) const
            {
                return ext_order_map.find( id ) != ext_order_map.end();
            }

            inline void addOrder( quantmodel::Order* const order )
            {
                order_vec.push_back( order );
                order_map.insert( std::pair< const std::string , quantmodel::Order* >( order->getOrderId(), order ) );

                if( order->getExternalOrderId() != "" )
                {
                    ext_order_map.insert( std::pair< const std::string , quantmodel::Order* >( order->getExternalOrderId(), order ) );
                }
            }

            inline void addRelease( quantmodel::Release* const release )
            {
                release_map.insert( std::pair< const std::string , quantmodel::Release* >( release->getReleaseId(), release ) );
                release_vec.push_back( release );

                if( account_release_map.count( release->getAccountId() ) == 0 )
                {
                    account_release_map.insert(
                        std::pair< const std::string , std::vector< quantmodel::Release* > >
                        ( release->getAccountId(), std::vector< quantmodel::Release* >() ) );
                }

                account_release_map[ release->getAccountId() ].push_back( release );
            }

            inline void addExecution( quantmodel::Execution* const exec )
            {
                execution_map.insert( pair< const std::string , quantmodel::Execution* >( exec->getExecutionId(), exec ) );
                execution_vec.push_back( exec );
                position_manager->add( exec );
            }

            inline quantmodel::Order* const getOrderByOrderId( const std::string& id ) const
            {
                std::map< const std::string, quantmodel::Order* >::const_iterator order_iter;
                if( ( order_iter = order_map.find( id ) ) != order_map.end() ) return order_iter->second;
                return NULL;
            }

            inline quantmodel::Order* const getOrderByExternalOrderId( const std::string& id ) const
            {
                std::map< const std::string, quantmodel::Order* >::const_iterator order_iter;
                if( ( order_iter = ext_order_map.find( id ) ) != ext_order_map.end() ) return order_iter->second;
                return NULL;
            }

            inline quantmodel::Release* const getReleaseByReleaseId( const std::string& id ) const
            {
                std::map< const std::string, quantmodel::Release* >::const_iterator release_iter;
                if( ( release_iter = release_map.find( id ) ) != release_map.end() ) return release_iter->second;
                return NULL;
            }

            inline quantmodel::Execution* const getExecutionByExecutionId( const std::string& id ) const
            {
                std::map< const std::string, quantmodel::Execution* >::const_iterator execution_iter;
                if( ( execution_iter = execution_map.find( id ) ) != execution_map.end() ) return execution_iter->second;
                return NULL;
            }

            inline quantmodel::AccountPosition* const getAccountPosition( const std::string& account_id )
            {
                return position_manager->createOrGet( account_id );
            }

        private:

            // prevent copying and assignment; not implemented
            ActiveSet (const ActiveSet &);
            ActiveSet & operator= (const ActiveSet &);

            log4cpp::Category* log;
            quantmodel::PositionManager *position_manager;

            std::map< const std::string, quantmodel::Order* > order_map;
            std::map< const std::string, quantmodel::Order* > ext_order_map;
            //std::map< const std::string, quantmodel::Order* >::const_iterator order_iter;

            std::map< const std::string, quantmodel::Release* > release_map;
            //std::map< const std::string, quantmodel::Release* >::const_iterator release_iter;

            std::map< const std::string, quantmodel::Execution* > execution_map;
            //std::map< const std::string, quantmodel::Execution* >::const_iterator execution_iter;

            std::map< const std::string, std::vector< quantmodel::Release* > > account_release_map;

            std::vector< quantmodel::Order* > order_vec;
            std::vector< quantmodel::Release* > release_vec;
            std::vector< quantmodel::Execution* > execution_vec;
            std::vector< quantmodel::PositionOvernight* > position_vec;

            void removeOrder( const std::string& order_id );
            void removeRelease( const std::string& release_id );
            void removeExecution( const std::string& execution_id );
    };
}

#endif // DEALING_ACTIVESET_H
