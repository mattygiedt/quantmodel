
#include <Application/QuickFAST.h>
#include <Messages/Message.h>
#include <Messages/Field.h>

#include "CMEMessageConsumer.hpp"

fast_engine::CMEMessageConsumer::CMEMessageConsumer( ) :
    quoteTypeIdentity( "MDQuoteType" ) ,
    entrySizeIdentity( "MDEntrySize" ),
    entryTimeIdentity( "MDEntryTime" ),
    sendingTimeIdentity( "SendingTime" ),
    aggressorSideIdentity( "AggressorSide" ),
    quoteConditionIdentity( "QuoteCondition" ),
    tradeConditionIdentity( "TradeCondition" )
{
    formatter = new QuickFAST::Messages::MessageFormatter( std::cout );

    ENTRY_TYPE_BID = '0';
    ENTRY_TYPE_ASK = '1';
    ENTRY_TYPE_TRADE = '2';
    ENTRY_TYPE_OPENING_PRICE = '4';
    ENTRY_TYPE_SETTLEMENT_PRICE = '6';
    ENTRY_TYPE_TRADING_SESSION_HIGH_PRICE = '7';
    ENTRY_TYPE_TRADING_SESSION_LOW_PRICE = '8';
    ENTRY_TYPE_TRADE_VOLUME = 'B';
    ENTRY_TYPE_OPEN_INTEREST = 'C';
    ENTRY_TYPE_SIMULATED_SELL = 'E';
    ENTRY_TYPE_SIMULATED_BUY = 'F';
    ENTRY_TYPE_EMPTY_BOOK = 'J';
    ENTRY_TYPE_PRIOR = 'M';
    ENTRY_TYPE_SESSION_HIGH_BID = 'N';
    ENTRY_TYPE_SESSION_LOW_OFFER = 'O';
    ENTRY_TYPE_FIXING_PRICE = 'W';
    ENTRY_TYPE_CASH_NOTE = 'X';

    broadcast_timestamp.clear();
}

fast_engine::CMEMessageConsumer::~CMEMessageConsumer( )
{

}

bool fast_engine::CMEMessageConsumer::consumeMessage( QuickFAST::Messages::Message& message )
{
    QuickFAST::Messages::FieldCPtr field;

    //
    //  Get the MessageType field
    //

    message.getField( "MessageType", field );

    //
    //  Currently we only support incremental updates
    //

    if( likely( (int)field->toAscii()[ 0 ] == 88 ) )
    {
        incrementalRefresh( message );
    }

    return true;
}

const fast_engine::CMEUpdateType fast_engine::CMEMessageConsumer::getUpdateType( const QuickFAST::uint32& updateAction, const QuickFAST::Messages::FieldSetCPtr& mdEntry ) const
{
    QuickFAST::Messages::FieldCPtr field;

    if( mdEntry->isPresent( quoteConditionIdentity ) )
    {
        mdEntry->getField( "QuoteCondition", field );
        const std::string& quoteCondition = field->toAscii();

        if( quoteCondition == "K" )
        {
            return CME_IMPLIED;
        }

        if( quoteCondition == "C" )
        {
            return CME_LAST_BEST_PRICE;
        }
    }
    else if( updateAction == 5 && mdEntry->isPresent( quoteTypeIdentity ) )
    {
        mdEntry->getField( "MDQuoteType", field );

        if( field->toInt32() == 0 )
        {
            return CME_INDICATIVE_PRICES;
        }

        return CME_TOP_OF_BOOK;
    }
    else
    {
        return CME_MULTIPLE_DEPTH;
    }

    return CME_UNKNOWN_UPDATE_TYPE;
}

void fast_engine::CMEMessageConsumer::incrementalRefresh( const QuickFAST::Messages::Message& message )
{
    QuickFAST::Messages::FieldCPtr field;
    QuickFAST::Messages::FieldSetCPtr field_set;
    QuickFAST::Messages::SequenceCPtr sequence;

    //
    //  Get the sending time from the CME header
    //

    message.getField( "SendingTime", field );
    //const QuickFAST::uint64& sending_tm = field->toUInt64();
    const long long unsigned int& sending_tm = field->toUInt64();

    //
    //  If this is the first message after a reset,
    //  initialize the fast_engine_broadcast.
    //

    if( unlikely( fast_engine_broadcast.has_timestamp() == false ) )
    {
        sprintf( cme_timestamp_buf, "%llu", sending_tm );
        quantmodel::TimeStamp::getCMETime(
            timeinfo,
            cme_timestamp_buf,
            get_cme_time_buf,
            broadcast_timestamp_buf );

        fast_engine_broadcast.set_timestamp( broadcast_timestamp_buf );
        fast_engine_broadcast.set_broadcast_type( quantmodel::FastEngineBroadcast::CME );

        //PRE 06.20.2013
        //quantmodel::TimeStamp::getCMETime( sending_tm, broadcast_timestamp );
        //fast_engine_broadcast.set_timestamp( broadcast_timestamp );
        //fast_engine_broadcast.set_broadcast_type( quantmodel::FastEngineBroadcast::CME );
    }

    //
    //  Process all MDEntries in the message
    //

    message.getField( "MDEntries", field );
    sequence = field->toSequence();

    for( sequence_iter = sequence->begin(); sequence_iter != sequence->end(); ++sequence_iter )
    {
        field_set = *sequence_iter;

        //
        //  Get the update action.
        //
        //      ADD -- (tag 279 MDUpdateAction=0)
        //      CHANGE -- (tag 279 MDUpdateAction=1)
        //      DELETE -- (tag 279 MDUpdateAction=2)
        //      OVERLAY -- (tag 279 MDUpdateAction=5)
        //

        field_set->getField( "MDUpdateAction", field );
        const QuickFAST::uint32& updateAction = field->toUInt32();

        //
        //  Based on the update action, determine if we can handle this message.
        //

        if( likely( getUpdateType( updateAction, field_set ) == CME_MULTIPLE_DEPTH ) )
        {
            //
            //  Get the entry_type
            //

            field_set->getField( "MDEntryType", field );
            const char entryType = field->toAscii()[ 0 ];

            if( entryType == ENTRY_TYPE_BID ||
                entryType == ENTRY_TYPE_ASK ||
                entryType == ENTRY_TYPE_TRADE )
            {
                //
                //  Create a new CMEMarketData
                //

                quantmodel::CMEMarketData *cme_market_data =
                    fast_engine_broadcast.add_cme_market_data();

                //
                //  Set the sending time
                //

                cme_market_data->set_sending_tm( sending_tm );

                //
                //  Set the security_id
                //

                field_set->getField( "SecurityID", field );
                cme_market_data->set_security_id( field->toUInt32() );

                //
                //  Set the update_action
                //

                cme_market_data->set_update_action(
                    (quantmodel::CMEMarketData_UpdateAction)updateAction );

                //
                //  Populate the rest based on entry type
                //

                if( entryType == ENTRY_TYPE_BID )
                {
                    cme_market_data->set_entry_type( quantmodel::CMEMarketData::BID );

                    //
                    //  Set the price level (1023)
                    //

                    field_set->getField( "MDPriceLevel", field );
                    cme_market_data->set_price_level( field->toUInt32() );

                    if( updateAction == quantmodel::CMEMarketData::ADD ||
                        updateAction == quantmodel::CMEMarketData::CHANGE )
                    {
                        //
                        //  Set the entry size (271)
                        //          price (270),
                        //          number of orders (346)
                        //

                        field_set->getField( "MDEntrySize", field );
                        cme_market_data->set_entry_size( field->toInt32() );

                        field_set->getField( "MDEntryPx", field );
                        const QuickFAST::Decimal& price = field->toDecimal();

                        //price.toString( *cme_market_data->mutable_entry_px( ) );
                        cme_market_data->set_mantissa( price.getMantissa() );
                        cme_market_data->set_exponent( price.getExponent() );

                        field_set->getField( "NumberOfOrders", field );
                        cme_market_data->set_orders( field->toUInt32() );
                    }
                }
                else if( entryType == ENTRY_TYPE_ASK )
                {
                    cme_market_data->set_entry_type( quantmodel::CMEMarketData::ASK );

                    //
                    //  Set the price level (1023)
                    //

                    field_set->getField( "MDPriceLevel", field );
                    cme_market_data->set_price_level( field->toUInt32() );

                    if( updateAction == quantmodel::CMEMarketData::ADD ||
                        updateAction == quantmodel::CMEMarketData::CHANGE )
                    {
                        //
                        //  Set the entry size (271)
                        //          price (270),
                        //          number of orders (346)
                        //

                        field_set->getField( "MDEntrySize", field );
                        cme_market_data->set_entry_size( field->toInt32() );

                        field_set->getField( "MDEntryPx", field );
                        const QuickFAST::Decimal& price = field->toDecimal();

                        //price.toString( *cme_market_data->mutable_entry_px( ) );
                        cme_market_data->set_mantissa( price.getMantissa() );
                        cme_market_data->set_exponent( price.getExponent() );

                        field_set->getField( "NumberOfOrders", field );
                        cme_market_data->set_orders( field->toUInt32() );
                    }
                }
                else // if( entryType == ENTRY_TYPE_TRADE )
                {
                    cme_market_data->set_entry_type( quantmodel::CMEMarketData::TRADE );

                    //
                    //  Set the MDEntryPx (270)
                    //

                    field_set->getField( "MDEntryPx", field );
                    const QuickFAST::Decimal& price = field->toDecimal();

                    //price.toString( *cme_market_data->mutable_entry_px( ) );
                    cme_market_data->set_mantissa( price.getMantissa() );
                    cme_market_data->set_exponent( price.getExponent() );

                    //
                    //  Test for implied price (277)
                    //

                    if( unlikely( field_set->isPresent( tradeConditionIdentity ) ) )
                    {
                        //
                        //  Sent for opening trade or trade at a
                        //  CME-Globex calculated price only.
                        //

                        cme_market_data->set_is_implied( true );
                    }
                    else
                    {
                        cme_market_data->set_is_implied( false );
                    }

                    //
                    //  Set the MDEntryTime (273) if present
                    //

                    if( likely( field_set->isPresent( entryTimeIdentity ) ) )
                    {
                        field_set->getField( "MDEntryTime", field );
                        cme_market_data->set_entry_tm( field->toUInt32() );
                    }

                    //
                    //  Set the MDEntrySize (271) if present
                    //

                    if( likely( field_set->isPresent( entrySizeIdentity ) ) )
                    {
                        field_set->getField( "MDEntrySize", field );
                        cme_market_data->set_entry_size( field->toInt32() );
                    }

                    //
                    //  Set the AggressorSide (5797) if present
                    //

                    if( likely( field_set->isPresent( aggressorSideIdentity ) ) )
                    {
                        field_set->getField( "AggressorSide", field );
                        cme_market_data->set_aggressor( (quantmodel::CMEMarketData_AggressorType)field->toUInt32() );
                    }
                }
            }
        }
    }
}

void fast_engine::CMEMessageConsumer::securityDefinition( const QuickFAST::Messages::Message &message ){}
void fast_engine::CMEMessageConsumer::quoteRequest( const QuickFAST::Messages::Message &message ){}
void fast_engine::CMEMessageConsumer::securityStatus( const QuickFAST::Messages::Message &message ){}
void fast_engine::CMEMessageConsumer::snapshotFullRequest( const QuickFAST::Messages::Message &message ){}
void fast_engine::CMEMessageConsumer::newsMessage( const QuickFAST::Messages::Message &message ){}
void fast_engine::CMEMessageConsumer::heartbeat( const QuickFAST::Messages::Message &message ){}
void fast_engine::CMEMessageConsumer::logon( const QuickFAST::Messages::Message &message ){}
void fast_engine::CMEMessageConsumer::logout( const QuickFAST::Messages::Message &message ){}