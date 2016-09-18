#ifndef QUANTMODEL_POSITION_MANAGER_H
#define QUANTMODEL_POSITION_MANAGER_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class PositionManager
    {
        public:

            PositionManager() {}
            ~PositionManager()
            {
                for( position_iter = position_map.begin(); position_iter != position_map.end(); ++position_iter )
                {
                    delete position_iter->second;
                }
            }

            void getPositions( std::vector< std::string >& vec )
            {
                for( position_iter = position_map.begin(); position_iter != position_map.end(); ++position_iter )
                {
                    vec.push_back( position_iter->first );
                }
            }

            void getPositions( std::vector< quantmodel::AccountPosition* >& vec )
            {
                for( position_iter = position_map.begin(); position_iter != position_map.end(); ++position_iter )
                {
                    vec.push_back( position_iter->second );
                }
            }

            quantmodel::AccountPosition* createOrGet( const std::string& account_id )
            {
                AccountPosition *position;
                if( ( position_iter = position_map.find( account_id ) ) == position_map.end() )
                {
                    position = new AccountPosition( account_id );
                    position_map.insert( std::pair< const std::string , AccountPosition* >( account_id, position ) );
                }
                else
                {
                    position = position_iter->second;
                }

                return position;
            }

            void add( quantmodel::Execution *execution )
            {
                if( execution->isSpreadExecution() == false )
                {
                    createOrGet( execution->getAccountId() )->
                        createOrGet( execution->getInvestmentSystemId() )->
                            createOrGet( execution->getInstrumentId() )->
                                addFill( new Fill(
                                    execution->getAttribute( quantmodel::EXECUTION_PRC ),
                                    execution->getAttribute( quantmodel::EXECUTION_QTY ),
                                    execution->getAttribute( quantmodel::SIDE_CODE ) ) );
                }
            }

            void add( const quantmodel::ExecutionData& execution )
            {
                if( execution.leg_type() != "3" )
                {
                    createOrGet( execution.account_id() )->
                        createOrGet( execution.investment_system_id() )->
                            createOrGet( execution.instrument_id() )->
                                addFill( new Fill(
                                    execution.execution_prc(),
                                    execution.execution_qty(),
                                    execution.side_code() ) );
                }
            }

            void add( quantmodel::PositionOvernight *position )
            {
                createOrGet( position->getAccountId() )->
                    createOrGet( position->getInvestmentSystemId() )->
                        createOrGet( position->getInstrumentId() )->
                            addFill( new Fill(
                                position->getAttribute( quantmodel::OPEN_PRC ),
                                position->getAttribute( quantmodel::OPEN_POS ),
                                position->getPositionType() ) );
            }

            void add( const quantmodel::PositionOvernightData& position )
            {
                createOrGet( position.account_id() )->
                    createOrGet( position.investment_system_id() )->
                        createOrGet( position.instrument_id() )->
                            addFill( new Fill(
                                position.open_prc(),
                                position.open_pos(),
                                position.position_type() == quantmodel::PositionOvernightData::LONG ? "1" : "2" ) );
            }

            void clear()
            {
                for( position_iter = position_map.begin(); position_iter != position_map.end(); ++position_iter )
                {
                    delete position_iter->second;
                }

                position_map.clear();
            }

        private:

            std::map< const std::string , AccountPosition* > position_map;
            std::map< const std::string , AccountPosition* >::const_iterator position_iter;

            // prevent copying and assignment; not implemented
            PositionManager (const PositionManager &);
            PositionManager & operator= (const PositionManager &);
    };

}

#endif // QUANTMODEL_POSITION_MANAGER_H