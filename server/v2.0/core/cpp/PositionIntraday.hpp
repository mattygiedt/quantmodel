#ifndef QUANTMODEL_POSITION_INTRADAY_H
#define QUANTMODEL_POSITION_INTRADAY_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class Fill
    {
        public:

            enum SideCode
            {
               BUY,
               SELL
            };

            Fill( const std::string& prc, const std::string& qty, const std::string& side_code ) :
                price( atof( prc.c_str() ) ),
                quantity( atof( qty.c_str() ) )
            {
                side = side_code == "1" ? Fill::BUY : Fill::SELL;
            }
            ~Fill() {}

            inline const double getPrice() const { return price; }
            inline const double getQuantity() const { return quantity; }
            inline const SideCode getSideCode() const { return side; }

        private:

            double price, quantity;
            SideCode side;

            // prevent copying and assignment; not implemented
            Fill (const Fill &);
            Fill & operator= (const Fill &);
    };

    class InstrumentPosition
    {
        public:

            enum PositionExposure
            {
               FLAT,
               LONG,
               SHORT
            };

            InstrumentPosition( const std::string& accnt_id, const std::string& inv_sys_id, const std::string& instr_id ) :
                account_id( accnt_id ),
                investment_system_id( inv_sys_id ),
                instrument_id( instr_id )
            {
                reset();
            }

            ~InstrumentPosition()
            {
                for( fill_iter = fill_vec.begin(); fill_iter != fill_vec.end(); ++fill_iter )
                {
                    delete (*fill_iter);
                }
            }

            const std::string getAccountId() const { return account_id; }
            const std::string getInvestmentSystemId() const { return investment_system_id; }
            const std::string getInstrumentId() const { return instrument_id; }

            inline const PositionExposure getExposure() const { return exposure; }
            inline const double getOpenPosition() const { return open_pos; }
            inline const double getOpenPrice() const { return open_prc; }
            inline const double getClosedPosition() const { return closed_pos; }
            inline const double getClosedValue() const { return closed_val; }
            inline const double getLongPosition() const { return exposure == InstrumentPosition::LONG ? open_pos : 0.0; }
            inline const double getShortPosition() const { return exposure == InstrumentPosition::SHORT ? -1*open_pos : 0.0; }

            inline const double getOpenPL() const { return open_pl; }
            inline const double getClosedPL() const { return closed_pl; }
            inline const double getTotalPL() const { return total_pl; }
            inline const double getNotionalPL() const { return notional_pl; }

            void addFill( Fill *fill )
            {
                fill_vec.push_back( fill );
                update( fill );
            }

            void pairOff()
            {
                reset();

                for( fill_iter = fill_vec.begin(); fill_iter != fill_vec.end(); ++fill_iter )
                {
                    update( (*fill_iter) );
                }
            }

            void profitLoss( quantmodel::Account *account, quantmodel::Instrument *instrument )
            {
                //
                //  TODO: have a think here about setting the account and
                //  instrument in the constructor? You'd never have an
                //  instrument position span multiple accounts, would you?
                //

                const double notional = atof( account->getAttribute( quantmodel::NOTIONAL_VAL ).c_str() );
                const double last_prc = atof( instrument->getAttribute( quantmodel::LAST_PRC ).c_str() );
                const double multiplier = atof( instrument->getAttribute( quantmodel::MULTIPLIER ).c_str() );

                if( exposure == InstrumentPosition::FLAT )
                {
                    open_pl = 0.0;
                }
                else if( exposure == InstrumentPosition::LONG )
                {
                    if( last_prc > 0.0 )
                    {
                        open_pl = ( ( last_prc * open_pos ) - ( open_prc * open_pos ) ) * multiplier;
                    }
                    else
                    {
                        open_pl = 0.0;
                    }
                }
                else
                {
                    if( last_prc > 0.0 )
                    {
                        open_pl = ( ( open_prc * open_pos ) - ( last_prc * open_pos ) ) * multiplier;
                    }
                    else
                    {
                        open_pl = 0.0;
                    }
                }

                closed_pl = closed_val * multiplier;
                total_pl = open_pl + closed_pl;
                if( notional > 0 ) notional_pl = total_pl / notional;
            }

        private:

            std::string account_id;
            std::string investment_system_id;
            std::string instrument_id;

            PositionExposure exposure;
            double open_pos, open_prc, closed_pos, closed_val, buy_qty, buy_val,
                   buy_avg, sell_qty, sell_val, sell_avg, open_pl, closed_pl,
                   total_pl, notional_pl;

            std::vector< Fill* > fill_vec;
            std::vector< Fill* >::const_iterator fill_iter;

            // prevent copying and assignment; not implemented
            InstrumentPosition (const InstrumentPosition &);
            InstrumentPosition & operator= (const InstrumentPosition &);

            inline void reset()
            {
                exposure = InstrumentPosition::FLAT;
                open_pos = open_prc = closed_pos = closed_val = buy_qty =
                buy_val = sell_qty = sell_val = buy_avg = sell_avg =
                open_pl = closed_pl = total_pl = notional_pl = 0.0;
            }

            void update( Fill *fill )
            {
                if( Fill::BUY == fill->getSideCode() )
                {
                    buy_qty += fill->getQuantity();
                    buy_val += fill->getQuantity() * fill->getPrice();
                }
                else
                {
                    sell_qty += fill->getQuantity();
                    sell_val += fill->getQuantity() * fill->getPrice();
                }

                if( buy_qty == sell_qty )
                {
                    exposure = InstrumentPosition::FLAT;
                    closed_pos += buy_qty;
                    closed_val += sell_val - buy_val;
                    open_pos = open_prc = buy_qty = buy_val = sell_qty = sell_val = 0.0;
                }
                else if( buy_qty > sell_qty )
                {
                    exposure = InstrumentPosition::LONG;

                    if( buy_val > 0 && buy_qty > 0 ) buy_avg = buy_val / buy_qty;

                    buy_qty -= sell_qty;
                    buy_val = buy_qty * buy_avg;

                    open_pos = buy_qty;
                    open_prc = buy_avg;

                    closed_pos += sell_qty;
                    closed_val += sell_val - ( sell_qty * buy_avg );

                    sell_qty = sell_val = 0.0;
                }
                else
                {
                    exposure = InstrumentPosition::SHORT;

                    if( sell_val > 0 && sell_qty > 0 ) sell_avg = sell_val / sell_qty;

                    sell_qty -= buy_qty;
                    sell_val = sell_qty * sell_avg;

                    open_pos = sell_qty;
                    open_prc = sell_avg;

                    closed_pos += buy_qty;
                    closed_val += ( buy_qty * sell_avg ) - buy_val;

                    buy_qty = buy_val = 0.0;
                }
            }
    };

    class InvestmentSystemPosition
    {
        public:

            InvestmentSystemPosition( const std::string& accnt_id , const std::string& inv_sys_id ) :
                account_id( accnt_id ),
                investment_system_id( inv_sys_id )
            {   }

            ~InvestmentSystemPosition()
            {
                for( position_iter = position_map.begin(); position_iter != position_map.end(); ++position_iter )
                {
                    delete position_iter->second;
                }
            }

            const std::string getAccountId() const { return account_id; }
            const std::string getInvestmentSystemId() const { return investment_system_id; }

            void getInvestmentSystemPositions( std::vector< quantmodel::InstrumentPosition* >& vec )
            {
                for( position_iter = position_map.begin(); position_iter != position_map.end(); ++position_iter )
                {
                    vec.push_back( position_iter->second );
                }
            }

            quantmodel::InstrumentPosition* createOrGet( const std::string& instrument_id )
            {
                InstrumentPosition *position;
                if( ( position_iter = position_map.find( instrument_id ) ) == position_map.end() )
                {
                    position = new InstrumentPosition( account_id, investment_system_id, instrument_id );
                    position_map.insert( std::pair< const std::string , InstrumentPosition* >( instrument_id, position ) );
                }
                else
                {
                    position = position_iter->second;
                }

                return position;
            }

        private:

            std::string account_id;
            std::string investment_system_id;

            std::map< const std::string , InstrumentPosition* > position_map;
            std::map< const std::string , InstrumentPosition* >::const_iterator position_iter;

            // prevent copying and assignment; not implemented
            InvestmentSystemPosition (const InvestmentSystemPosition &);
            InvestmentSystemPosition & operator= (const InvestmentSystemPosition &);
    };

    class AccountPosition
    {
        public:

            AccountPosition( const std::string& id ) :
                account_id( id )
            { }

            ~AccountPosition()
            {
                for( position_iter = position_map.begin(); position_iter != position_map.end(); ++position_iter )
                {
                    delete position_iter->second;
                }
            }

            const std::string getAccountId() const { return account_id; }

            void getInstrumentPositions( std::vector< InstrumentPosition* >& vec )
            {
                for( position_iter = position_map.begin(); position_iter != position_map.end(); ++position_iter )
                {
                    position_iter->second->getInvestmentSystemPositions( vec );
                }
            }

            void getAccountPositions( std::vector< std::pair< InvestmentSystemPosition* , std::vector< InstrumentPosition* > >* >& vec )
            {
                //
                //  DO NOT FORGET TO DELETE THE CONTENTS OF VEC IN CALLER!!!
                //

                for( position_iter = position_map.begin(); position_iter != position_map.end(); ++position_iter )
                {
                    std::pair< InvestmentSystemPosition* , std::vector< InstrumentPosition* > > *record =
                        new std::pair< InvestmentSystemPosition* , std::vector< InstrumentPosition* > >();
                    record->first = position_iter->second;
                    position_iter->second->getInvestmentSystemPositions( record->second );
                    vec.push_back( record );
                }
            }

            quantmodel::InvestmentSystemPosition* createOrGet( const std::string& investment_system_id )
            {
                InvestmentSystemPosition *position;
                if( ( position_iter = position_map.find( investment_system_id ) ) == position_map.end() )
                {
                    position = new InvestmentSystemPosition( account_id, investment_system_id );
                    position_map.insert( std::pair< const std::string , InvestmentSystemPosition* >( investment_system_id, position ) );
                }
                else
                {
                    position = position_iter->second;
                }

                return position;
            }


        private:

            std::string account_id;

            std::map< const std::string , InvestmentSystemPosition* > position_map;
            std::map< const std::string , InvestmentSystemPosition* >::const_iterator position_iter;

            // prevent copying and assignment; not implemented
            AccountPosition (const AccountPosition &);
            AccountPosition & operator= (const AccountPosition &);
    };
}

#endif // QUANTMODEL_POSITION_INTRADAY_H