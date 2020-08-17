#ifndef QUICKFAST_CME_SETTLEMENTCONSUMER_H
#define QUICKFAST_CME_SETTLEMENTCONSUMER_H

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <quantmodel.hpp>

#include <Codecs/MessageConsumer.h>
#include <Messages/MessageFormatter.h>

namespace fast_engine
{
    class CMESettlementConsumer : public QuickFAST::Codecs::MessageConsumer
    {
        public:

            CMESettlementConsumer( zmq::socket_t* , log4cpp::Category* );
            ~CMESettlementConsumer( );

            bool consumeMessage( QuickFAST::Messages::Message & );

            bool wantLog( unsigned short ) { return true; }
            bool logMessage( unsigned short , const std::string& ) { return true; }
            bool reportDecodingError( const std::string& ) { return true; }
            bool reportCommunicationError( const std::string& ) { return true; }
            void decodingStarted(){}
            void decodingStopped(){}

            void loadInstrumentMap( const XMLNode& config )
            {
                quantmodel::DatabaseEngineRequest request;
                quantmodel::DatabaseEngineResponse response;

                const XMLNode& instruments = config.getChildNode( "instruments" );

                //
                //  Load each configured instrument by symbol group
                //

                for( int i=0; i<instruments.nChildNode( ); ++i )
                {
                    const XMLNode& symbol = instruments.getChildNode( i );

                    request.Clear();
                    quantmodel::TimeStamp::getMessageId( *request.mutable_request_id( ) );
                    quantmodel::TimeStamp::getUTCTime( *request.mutable_timestamp( ) );
                    quantmodel::QuantModelProcedures::GET_ACTIVE_INSTRUMENTS_BY_SYMBOL(
                        request, symbol.getAttribute( "name" ) );

                    if( unlikely( log->isDebugEnabled() ) )
                    {
                        log->debug( " <-- " + request.ShortDebugString() );
                    }

                    if( quantmodel::ZMQMessage::sendRPC( &request, &response, database_callforward ) )
                    {
                        for( int i=0; i<response.result_set_size(); ++i )
                        {
                            const quantmodel::DatabaseEngineResponse::ResultSet& rs = response.result_set( i );

                            for( int j=0; j<rs.row_size(); ++j )
                            {
                                if( unlikely( log->isDebugEnabled() ) )
                                {
                                    log->debug( " loading instrument: %s", rs.row( j ).ShortDebugString().c_str() );
                                }

                                quantmodel::Instrument *instrument = new quantmodel::Instrument( rs.row( j ) );
                                instrument_map.insert( std::pair< const std::string , quantmodel::Instrument* >(
                                    instrument->getInstrumentId(), instrument ) );

                                log->info( " loaded instrument: %s %s %s",
                                    instrument->getInstrumentId().c_str(),
                                    instrument->getAttribute( quantmodel::SYMBOL ).c_str(),
                                    instrument->getAttribute( quantmodel::NAME ).c_str() );
                            }
                        }
                    }
                }
            }

        private:

            zmq::socket_t *database_callforward;
            log4cpp::Category* log;
            QuickFAST::Messages::MessageFormatter* formatter;
            std::map< const std::string , quantmodel::Instrument* > instrument_map;
            std::map< const std::string , quantmodel::Instrument* >::const_iterator instrument_iter;
    };
}

#endif //   QUICKFAST_CME_SETTLEMENTCONSUMER_H
