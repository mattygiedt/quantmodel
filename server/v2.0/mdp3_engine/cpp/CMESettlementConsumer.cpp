

#include <Application/QuickFAST.h>
#include <Messages/Message.h>
#include <Messages/Field.h>

#include "CMESettlementConsumer.hpp"

fast_engine::CMESettlementConsumer::CMESettlementConsumer( zmq::socket_t *socket, log4cpp::Category* logger )
{
    database_callforward = socket;
    log = logger;
    formatter = new QuickFAST::Messages::MessageFormatter( std::cout );
}

fast_engine::CMESettlementConsumer::~CMESettlementConsumer( )
{
    delete formatter;
}

bool fast_engine::CMESettlementConsumer::consumeMessage( QuickFAST::Messages::Message & message )
{
    QuickFAST::Messages::FieldCPtr field;

    //
    //  MessageType 'd' == security definition
    //

    message.getField( "MessageType", field );
    const std::string& message_type_str = field->toAscii();

    if( message_type_str[ 0 ] == 'd' )
    {
        char buf[ 32 ];
        message.getField( "SecurityID", field );
        const QuickFAST::uint32& securityID = field->toUInt32();
        sprintf( buf, "%u", securityID );

        if( ( instrument_iter = instrument_map.find( buf ) ) != instrument_map.end() )
        {
            if( message.isPresent( QuickFAST::Messages::FieldIdentity( "SettlePriceType" ) ) &&
                message.isPresent( QuickFAST::Messages::FieldIdentity( "TradingReferencePrice" ) ) &&
                message.isPresent( QuickFAST::Messages::FieldIdentity( "TradingReferenceDate" ) ) )
            {
                message.getField( "SettlePriceType", field );
                const std::string& settle_price_type = field->toString();

                message.getField( "TradingReferenceDate", field );
                const QuickFAST::uint32& reference_date = field->toUInt32();
                sprintf( buf, "%u", reference_date );
                const std::string& trading_reference_date( buf );

                if( settle_price_type == "1" &&
                    trading_reference_date.compare( instrument_iter->second->getAttribute( quantmodel::MARK_DATETIME ) ) != 0 )
                {
                    std::string ts, mark_prc, trading_reference_price;
                    message.getField( "TradingReferencePrice", field );
                    const QuickFAST::Decimal& referencePrice = field->toDecimal();
                    referencePrice.toString( trading_reference_price );

                    instrument_iter->second->displayPrice( trading_reference_price, mark_prc );

                    log->info( "MARKING INSTRUMENT: %s, %s --> %s",
                        instrument_iter->first.c_str(),
                        trading_reference_date.c_str(),
                        mark_prc.c_str() );

                    //
                    //  Set the trading reference date and mark_prc so we don't
                    //  mark more than once.
                    //

                    instrument_iter->second->setAttribute( quantmodel::MARK_PRC, mark_prc );
                    instrument_iter->second->setAttribute( quantmodel::MARK_DATETIME, trading_reference_date );

                    quantmodel::TimeStamp::getUTCTime( ts );
                    quantmodel::DatabaseEngineRequest request;
                    quantmodel::QuantModelProcedures::MARK_INSTRUMENT(
                        request,
                        instrument_iter->first,
                        mark_prc,
                        trading_reference_date,
                        ts,
                        "SETTLMNT_ENGINE" );

                    quantmodel::ZMQMessage::sendRPC( &request, database_callforward );
                }
            }
        }
    }

    return true;
}
