#ifndef QUANTMODEL_CLIENTORDERIDDATA_H
#define QUANTMODEL_CLIENTORDERIDDATA_H

using namespace std;

namespace fix_engine
{
    class ClientOrderIdData
    {
        public:

            ClientOrderIdData( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row, log4cpp::Category *log )
            {
                for( int k=0; k<row.column_size(); ++k )
                {
                    if( row.column( k ).name() == "client_order_id" )
                    {
                        internal_client_order_id = row.column( k ).value();
                    }
                    else if( row.column( k ).name() == "orig_client_order_id" )
                    {
                        internal_orig_client_order_id = row.column( k ).value();
                    }
                    else if( row.column( k ).name() == "correlation_order_id" )
                    {
                        internal_correlation_order_id = row.column( k ).value();
                    }
                    else if( row.column( k ).name() == "session_id" )
                    {
                        internal_session_id = row.column( k ).value();
                    }
                    else if( row.column( k ).name() == "release_id" )
                    {
                        internal_release_id = row.column( k ).value();
                    }
                    else if( row.column( k ).name() == "order_id" )
                    {
                        internal_order_id = row.column( k ).value();
                    }
                    else if( row.column( k ).name() == "instrument_id" )
                    {
                        internal_instrument_id = row.column( k ).value();
                    }
                    else if( row.column( k ).name() == "investment_system_id" )
                    {
                        internal_investment_system_id = row.column( k ).value();
                    }
                    else if( row.column( k ).name() == "is_active" )
                    {
                        internal_investment_system_id = row.column( k ).value();
                    }
                    else if( row.column( k ).name() == "added_datetime" )
                    {
                        internal_added_datetime = row.column( k ).value();
                    }
                    else if( row.column( k ).name() == "added_by" )
                    {
                        internal_added_by = row.column( k ).value();
                    }
                    else if( row.column( k ).name() == "updated_datetime" )
                    {
                        internal_added_by = row.column( k ).value();
                    }
                    else if( row.column( k ).name() == "updated_by" )
                    {
                        internal_added_by = row.column( k ).value();
                    }
                    else
                    {
                        log->error( "Unknown client_order_id_map row[ %d ] column[ %s ] = %s",
                            k, row.column( k ).name().c_str(), row.column( k ).value().c_str() );
                    }
                }
            }

            ClientOrderIdData(  const std::string& client_order_id,
                                const std::string& correlation_order_id,
                                const std::string& orig_client_order_id,
                                const std::string& session_id,
                                const std::string& release_id,
                                const std::string& order_id,
                                const std::string& instrument_id,
                                const std::string& investment_system_id,
                                const std::string& added_datetime,
                                const std::string& added_by ) :
                internal_client_order_id( client_order_id ),
                internal_correlation_order_id( correlation_order_id ),
                internal_orig_client_order_id( orig_client_order_id ),
                internal_session_id( session_id ),
                internal_release_id( release_id ),
                internal_order_id( order_id ),
                internal_instrument_id( instrument_id ),
                internal_investment_system_id( investment_system_id ),
                internal_is_active( "1" ),
                internal_added_datetime( added_datetime ),
                internal_added_by( added_by ),
                internal_updated_datetime( added_datetime ),
                internal_updated_by( added_by )
            {   }

            ~ClientOrderIdData(){}

            inline const std::string& client_order_id() { return internal_client_order_id; }
            inline const std::string& correlation_order_id() { return internal_correlation_order_id; }
            inline const std::string& orig_client_order_id() { return internal_orig_client_order_id; }
            inline const std::string& session_id() { return internal_session_id; }
            inline const std::string& release_id() { return internal_release_id; }
            inline const std::string& order_id() { return internal_order_id; }
            inline const std::string& instrument_id() { return internal_instrument_id; }
            inline const std::string& investment_system_id() { return internal_investment_system_id; }
            inline const std::string& is_active() { return internal_is_active; }
            inline const std::string& added_datetime() { return internal_added_datetime; }
            inline const std::string& added_by() { return internal_added_by; }
            inline const std::string& updated_datetime() { return internal_updated_datetime; }
            inline const std::string& updated_by() { return internal_updated_by; }

            void insert( quantmodel::DatabaseEngineRequest& request_msg ) const
            {
                quantmodel::QuantModelProcedures::INSERT_CLIENT_ORDER_ID(
                    request_msg,
                    internal_client_order_id,
                    internal_correlation_order_id,
                    internal_orig_client_order_id,
                    internal_session_id,
                    internal_release_id,
                    internal_order_id,
                    internal_instrument_id,
                    internal_investment_system_id,
                    internal_added_datetime,
                    internal_added_by,
                    internal_updated_datetime,
                    internal_updated_by );
            }

        private:
            std::string internal_client_order_id,
                        internal_correlation_order_id,
                        internal_orig_client_order_id,
                        internal_session_id,
                        internal_release_id,
                        internal_order_id,
                        internal_instrument_id,
                        internal_investment_system_id,
                        internal_is_active,
                        internal_added_datetime,
                        internal_added_by,
                        internal_updated_datetime,
                        internal_updated_by;
    };
}

#endif // QUANTMODEL_CLIENTORDERIDDATA_H
