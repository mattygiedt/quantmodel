#ifndef QUANTMODEL_POSITION_OVERNIGHT_H
#define QUANTMODEL_POSITION_OVERNIGHT_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class PositionOvernight : public DataObject
    {
        public:

            PositionOvernight( quantmodel::InstrumentPosition * position )
            {
                char buf[ 32 ];
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                data.set_account_id( position->getAccountId() );
                data.set_instrument_id( position->getInstrumentId() );
                data.set_investment_system_id( position->getInvestmentSystemId() );
                data.set_position_type(
                    position->getExposure() == quantmodel::InstrumentPosition::LONG ?
                        quantmodel::PositionOvernightData::LONG :
                        quantmodel::PositionOvernightData::SHORT );

                sprintf( buf, "%f", position->getOpenPosition() );
                data.set_open_pos( buf );
                sprintf( buf, "%f", position->getOpenPrice() );
                data.set_open_prc( buf );
            }

            PositionOvernight( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                for( int k=0; k<row.column_size(); ++k )
                {
                    if( row.column( k ).name() == "position_type" )
                    {
                        data.set_position_type(
                            row.column( k ).value() == "1" ?
                                quantmodel::PositionOvernightData::LONG :
                                quantmodel::PositionOvernightData::SHORT );
                    }
                    else
                    {
                        setAttribute( row.column( k ).name(), row.column( k ).value() );
                    }
                }
            }

            ~PositionOvernight(){};

            void copyPositionOvernightData( quantmodel::PositionOvernightData *msg )
            {
                msg->CopyFrom( data );
            }

            inline const std::string& getAccountId() { return data.account_id(); }
            inline const std::string& getInvestmentSystemId() { return data.investment_system_id(); }
            inline const std::string& getInstrumentId() { return data.instrument_id(); }
            inline const std::string getPositionType() const { return data.position_type() == quantmodel::PositionOvernightData::LONG ? "1" : "2"; }

            inline void insert( quantmodel::DatabaseEngineRequest& request ) const
            {
                quantmodel::QuantModelProcedures::INSERT_POSITION_OVERNIGHT(
                    request,
                    data.account_id(),
                    data.instrument_id(),
                    data.investment_system_id(),
                    getPositionType(),
                    data.open_pos(),
                    data.open_prc(),
                    data.added_datetime(),
                    data.added_by() );
            }

        private:

            // prevent copying and assignment; not implemented
            PositionOvernight (const PositionOvernight &);
            PositionOvernight & operator= (const PositionOvernight &);

            quantmodel::PositionOvernightData data;
    };
}

#endif // QUANTMODEL_POSITION_OVERNIGHT_H
