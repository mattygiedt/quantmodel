
#include <Application/QuickFAST.h>

#include "CMEValueMessageBuilder.hpp"

fast_engine::CMEValueMessageBuilder::CMEValueMessageBuilder() : state( INIT ), last_sendingtime( 0 )
{
}

fast_engine::CMEValueMessageBuilder::~CMEValueMessageBuilder()
{
}

void fast_engine::CMEValueMessageBuilder::setLogLevel( QuickFAST::Common::Logger::LogLevel value )
{
    logLevel = value;
}

bool fast_engine::CMEValueMessageBuilder::wantLog( unsigned short level )
{
    return level <= logLevel;
}

bool fast_engine::CMEValueMessageBuilder::logMessage( unsigned short level, const std::string & logMessage )
{
    if( level <= logLevel )
    {
        std::cerr << logMessage << std::endl;
    }

    return true;
}

bool fast_engine::CMEValueMessageBuilder::reportDecodingError( const std::string & errorMessage )
{
    std::cerr << "Decoding error: " << errorMessage << std::endl;
    return false;
}

bool fast_engine::CMEValueMessageBuilder::reportCommunicationError( const std::string & errorMessage )
{
    std::cerr << "Communication error: " << errorMessage << std::endl;
    return false;
}

const std::string & fast_engine::CMEValueMessageBuilder::getApplicationType() const
{
    return applType;
}

const std::string & fast_engine::CMEValueMessageBuilder::getApplicationTypeNs() const
{
    return applTypeNamespace;
}

void fast_engine::CMEValueMessageBuilder::addValue(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    QuickFAST::ValueType::Type type,
    const QuickFAST::int64 value )
{
    std::cout << " addValue( int64 ): " << identity->id() << '=' << value << std::endl;
}

void fast_engine::CMEValueMessageBuilder::addValue(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    QuickFAST::ValueType::Type type,
    const QuickFAST::uint64 value )
{
    //std::cout << " addValue( uint64 ): " << identity->id() << '=' << value << std::endl;

    if( state == START_MESSAGE )
    {
        //
        //  Valid CMEMarketDataProtobuf uint64 fields:
        //
        //      CMEMarketDataProtobuf.SendingTime
        //

        if( marketData.back().has_sendingtime() == false &&
            fieldValues.SendingTime == identity->id() )
        {
            marketData.back().set_sendingtime( value );
            return;
        }
    }
}

void fast_engine::CMEValueMessageBuilder::addValue(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    QuickFAST::ValueType::Type type,
    const QuickFAST::int32 value )
{
    //std::cout << " addValue( int32 ): " << identity->id() << '=' << value << std::endl;

    if( state == START_SEQUENCE )
    {
        //
        //  Valid CMEMarketDataEntry int32 fields:
        //
        //      CMEMarketDataEntry.MDEntrySize
        //

        if( marketDataEntryPtr->has_mdentrysize( ) == false &&
            fieldValues.MDEntrySize == identity->id() )
        {
            marketDataEntryPtr->set_mdentrysize( value );
            return;
        }
    }
}

void fast_engine::CMEValueMessageBuilder::addValue(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    QuickFAST::ValueType::Type type,
    const QuickFAST::uint32 value )
{
    //std::cout << " addValue( uint32 ): " << identity->id() << '=' << value << std::endl;

    if( state == START_MESSAGE )
    {
        //
        //  Valid CMEMarketDataProtobuf uint32 fields:
        //
        //      CMEMarketDataProtobuf.MsgSeqNum
        //      CMEMarketDataProtobuf.TradeDate
        //

        if( marketData.back().has_msgseqnum( ) == false &&
            fieldValues.MsgSeqNum == identity->id() )
        {
            marketData.back().set_msgseqnum( value );
            return;
        }

        if( marketData.back().has_tradedate( ) == false &&
            fieldValues.TradeDate == identity->id() )
        {
            marketData.back().set_tradedate( value );
            return;
        }
    }
    else if( state == START_SEQUENCE )
    {
        //
        //  Valid CMEMarketDataEntry uint32 fields:
        //
        //      CMEMarketDataEntry.MDUpdateAction
        //      CMEMarketDataEntry.MDPriceLevel
        //      CMEMarketDataEntry.SecurityIDSource
        //      CMEMarketDataEntry.SecurityID
        //      CMEMarketDataEntry.RptSeq
        //      CMEMarketDataEntry.MDEntryTime
        //      CMEMarketDataEntry.NumberOfOrders
        //      CMEMarketDataEntry.OpenCloseSettleFlag
        //      CMEMarketDataEntry.SettlDate
        //      CMEMarketDataEntry.TradeVolume
        //      CMEMarketDataEntry.AggressorSide
        //      CMEMarketDataEntry.MDQuoteType
        //      CMEMarketDataEntry.TradeID
        //

        if( marketDataEntryPtr->has_mdupdateaction( ) == false &&
            fieldValues.MDUpdateAction == identity->id() )
        {
            marketDataEntryPtr->set_mdupdateaction( value );
            return;
        }

        if( marketDataEntryPtr->has_mdpricelevel( ) == false &&
            fieldValues.MDPriceLevel == identity->id() )
        {
            marketDataEntryPtr->set_mdpricelevel( value );
            return;
        }

        if( marketDataEntryPtr->has_securityidsource( ) == false &&
            fieldValues.SecurityIDSource == identity->id() )
        {
            marketDataEntryPtr->set_securityidsource( value );
            return;
        }

        if( marketDataEntryPtr->has_securityid( ) == false &&
            fieldValues.SecurityID == identity->id() )
        {
            marketDataEntryPtr->set_securityid( value );
            return;
        }

        if( marketDataEntryPtr->has_mdentrytime( ) == false &&
            fieldValues.MDEntryTime == identity->id() )
        {
            marketDataEntryPtr->set_mdentrytime( value );
            return;
        }

        if( marketDataEntryPtr->has_rptseq( ) == false &&
            fieldValues.RptSeq == identity->id() )
        {
            marketDataEntryPtr->set_rptseq( value );
            return;
        }

        if( marketDataEntryPtr->has_numberoforders( ) == false &&
            fieldValues.NumberOfOrders == identity->id() )
        {
            marketDataEntryPtr->set_numberoforders( value );
            return;
        }

        if( marketDataEntryPtr->has_aggressorside( ) == false &&
            fieldValues.AggressorSide == identity->id() )
        {
            marketDataEntryPtr->set_aggressorside( value );
            return;
        }

        if( marketDataEntryPtr->has_openclosesettleflag( ) == false &&
            fieldValues.OpenCloseSettleFlag == identity->id() )
        {
            marketDataEntryPtr->set_openclosesettleflag( value );
            return;
        }

        if( marketDataEntryPtr->has_settldate( ) == false &&
            fieldValues.SettlDate == identity->id() )
        {
            marketDataEntryPtr->set_settldate( value );
            return;
        }

        if( marketDataEntryPtr->has_tradevolume( ) == false &&
            fieldValues.TradeVolume == identity->id() )
        {
            marketDataEntryPtr->set_tradevolume( value );
            return;
        }

        if( marketDataEntryPtr->has_mdquotetype( ) == false &&
            fieldValues.MDQuoteType == identity->id() )
        {
            marketDataEntryPtr->set_mdquotetype( value );
            return;
        }

        if( marketDataEntryPtr->has_tradeid( ) == false &&
            fieldValues.TradeID == identity->id() )
        {
            marketDataEntryPtr->set_tradeid( value );
            return;
        }
    }
}

void fast_engine::CMEValueMessageBuilder::addValue(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    QuickFAST::ValueType::Type type,
    const QuickFAST::int16 value )
{
    //std::cout << " addValue( int16 ): " << identity->id() << '=' << value << std::endl;
}

void fast_engine::CMEValueMessageBuilder::addValue(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    QuickFAST::ValueType::Type type,
    const QuickFAST::uint16 value )
{
    //std::cout << " addValue( uint16 ): " << identity->id() << '=' << value << std::endl;
}

void fast_engine::CMEValueMessageBuilder::addValue(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    QuickFAST::ValueType::Type type,
    const QuickFAST::int8 value )
{
    //std::cout << " addValue( int8 ): " << identity->id() << '=' << (short)(value) << std::endl;
}

void fast_engine::CMEValueMessageBuilder::addValue(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    QuickFAST::ValueType::Type type,
    const QuickFAST::uchar value )
{
    //std::cout << " addValue( uchar ): " << identity->id() << '=' << (unsigned short)(value) << std::endl;
}

void fast_engine::CMEValueMessageBuilder::addValue(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    QuickFAST::ValueType::Type type,
    const QuickFAST::Decimal& value )
{
    //std::string v;
    //value.toString(v);
    //std::cout << " addValue( decimal ): " << identity->id() << '=' << v << std::endl;

    if( state == START_SEQUENCE )
    {
        //
        //  Valid CMEMarketDataEntry decimal fields:
        //
        //      CMEMarketDataEntry.MDEntryPx
        //      CMEMarketDataEntry.NetChgPrevDay
        //

        if( marketDataEntryPtr->has_mdentrypxmantissa( ) == false &&
            fieldValues.MDEntryPx == identity->id() )
        {
            marketDataEntryPtr->set_mdentrypxmantissa( value.getMantissa() );
            marketDataEntryPtr->set_mdentrypxexponent( value.getExponent() );
            return;
        }

        if( marketDataEntryPtr->has_netchgprevdaymantissa( ) == false &&
            fieldValues.NetChgPrevDay == identity->id() )
        {
            marketDataEntryPtr->set_netchgprevdaymantissa( value.getMantissa() );
            marketDataEntryPtr->set_netchgprevdayexponent( value.getExponent() );
            return;
        }
    }
}

void fast_engine::CMEValueMessageBuilder::addValue(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    QuickFAST::ValueType::Type type,
    const unsigned char * value,
    size_t length )
{
    //std::cout << " addValue( string ): " << identity->id() << '='
    //          << std::string(reinterpret_cast<const char *>(value), length)
    //          << std::endl;

    if( state == START_MESSAGE )
    {
        //
        //  Valid CMEMarketDataProtobuf string fields:
        //
        //      CMEMarketDataProtobuf.MessageType
        //      CMEMarketDataProtobuf.PosDupFlag
        //

        if( marketData.back().has_messagetype( ) == false &&
            fieldValues.MessageType == identity->id() )
        {
            marketData.back().set_messagetype( (CMEMarketDataProtobuf::CMEMessageType)value[ 0 ] );
                //reinterpret_cast<const char *>(value), length );
            return;
        }

        if( marketData.back().has_posdupflag( ) == false &&
            fieldValues.PosDupFlag == identity->id() )
        {
            marketData.back().set_posdupflag( (CMEMarketDataProtobuf::CMEPosDupFlag)value[ 0 ] );
                //reinterpret_cast<const char *>(value), length );
            return;
        }
    }
    else if( state == START_SEQUENCE )
    {
        //
        //  Valid CMEMarketDataEntry string fields:
        //
        //      CMEMarketDataEntry.MDEntryType
        //      CMEMarketDataEntry.QuoteCondition
        //      CMEMarketDataEntry.TradingSessionID
        //      CMEMarketDataEntry.TickDirection
        //      CMEMarketDataEntry.TradeCondition
        //

        if( marketDataEntryPtr->has_mdentrytype( ) == false &&
            fieldValues.MDEntryType == identity->id() )
        {
            marketDataEntryPtr->set_mdentrytype( (CMEMarketDataEntry::CMEEntryType)value[ 0 ] );
                //reinterpret_cast<const char *>(value), length );
            return;
        }

        if( marketDataEntryPtr->has_quotecondition( ) == false &&
            fieldValues.QuoteCondition == identity->id() )
        {
            marketDataEntryPtr->set_quotecondition( (CMEMarketDataEntry::CMEQuoteCondition)value[ 0 ] );
                //reinterpret_cast<const char *>(value), length );
            return;
        }

        if( marketDataEntryPtr->has_tradingsessionid( ) == false &&
            fieldValues.TradingSessionID == identity->id() )
        {
            marketDataEntryPtr->set_tradingsessionid( (CMEMarketDataEntry::CMETradingSessionID)value[ 0 ] );
                //reinterpret_cast<const char *>(value), length );
            return;
        }

        if( marketDataEntryPtr->has_tickdirection( ) == false &&
            fieldValues.TickDirection == identity->id() )
        {
            marketDataEntryPtr->set_tickdirection( (CMEMarketDataEntry::CMETickDirection)value[ 0 ] );
                //reinterpret_cast<const char *>(value), length );
            return;
        }

        if( marketDataEntryPtr->has_tradecondition( ) == false &&
            fieldValues.TradeCondition == identity->id() )
        {
            marketDataEntryPtr->set_tradecondition( (CMEMarketDataEntry::CMETradeCondition)value[ 0 ] );
                //reinterpret_cast<const char *>(value), length );
            return;
        }
    }
}

QuickFAST::Messages::ValueMessageBuilder & fast_engine::CMEValueMessageBuilder::startMessage(
    const std::string & applicationType,
    const std::string & applicationTypeNamespace,
    size_t count )
{
    state = START_MESSAGE;
    CMEMarketDataProtobuf marketDataProtobuf;
    marketData.push_back( std::move( marketDataProtobuf ) );
    
    //applType = applicationType;
    //applTypeNamespace = applicationTypeNamespace;
    //fieldCount = count;
    //
    //std::cout << "startMessage: " << applType << ", " << applTypeNamespace << ", " << fieldCount << std::endl;

    return *this;
}

bool fast_engine::CMEValueMessageBuilder::endMessage( QuickFAST::Messages::ValueMessageBuilder & messageBuilder )
{
    state = END_MESSAGE;

    //std::cout << "endMessage: " << std::endl << marketDataProtobuf.ShortDebugString() << std::endl;

    return true;
}

bool fast_engine::CMEValueMessageBuilder::ignoreMessage( QuickFAST::Messages::ValueMessageBuilder & messageBuilder )
{
    //std::cout << "ignoreMessage:" << std::endl;

    return true;
}

QuickFAST::Messages::ValueMessageBuilder & fast_engine::CMEValueMessageBuilder::startSequence(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    const std::string & applicationType,
    const std::string & applicationTypeNamespace,
    size_t fieldCount,
    QuickFAST::Messages::FieldIdentityCPtr & lengthIdentity,
    size_t length )
{
    //std::cout << "startSequence: "
    //          << applicationType << ", "
    //          << applicationTypeNamespace << ", "
    //          << fieldCount << std::endl;
    return *this;
}

void fast_engine::CMEValueMessageBuilder::endSequence(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    QuickFAST::Messages::ValueMessageBuilder & sequenceBuilder)
{
    //std::cout << "endSequence:" << std::endl;
    state = START_MESSAGE;
}

QuickFAST::Messages::ValueMessageBuilder & fast_engine::CMEValueMessageBuilder::startSequenceEntry(
    const std::string & applicationType,
    const std::string & applicationTypeNamespace,
    size_t size )
{
    //std::cout << "startSequenceEntry: "
    //          << applicationType << ", "
    //          << applicationTypeNamespace << ", "
    //          << size << std::endl;

    state = START_SEQUENCE;
    marketDataEntryPtr = marketData.back().add_mdentries();
    return *this;
}

void fast_engine::CMEValueMessageBuilder::endSequenceEntry( QuickFAST::Messages::ValueMessageBuilder & entry )
{
    //std::cout << "endSequenceEntry:" << std::endl;
}

QuickFAST::Messages::ValueMessageBuilder & fast_engine::CMEValueMessageBuilder::startGroup(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    const std::string & applicationType,
    const std::string & applicationTypeNamespace,
    size_t size )
{
    //std::cout << "startGroup:" << std::endl;
    state = START_GROUP;
    return *this;
}

void fast_engine::CMEValueMessageBuilder::endGroup(
    QuickFAST::Messages::FieldIdentityCPtr & identity,
    QuickFAST::Messages::ValueMessageBuilder & groupBuilder)
{
    //std::cout << "endGroup:" << std::endl;
    state = START_MESSAGE;
}

bool fast_engine::CMEValueMessageBuilder::getFastEngineBroadcast( quantmodel::FastEngineBroadcast& fastEngineBroadcast )
{
    if( marketData.size() > 0 )
    {
        std::vector<CMEMarketDataProtobuf>::const_iterator marketDataIter;
        
        for( marketDataIter = marketData.begin(); marketDataIter != marketData.end(); ++marketDataIter )
        {
            if( (*marketDataIter).messagetype() == CMEMarketDataProtobuf_CMEMessageType_MESSAGE_TYPE_MARKET_DATA_INCREMENTAL_REFRESH )
            {
                if( unlikely( fastEngineBroadcast.has_timestamp() == false ) )
                {
                    //
                    //  If the last_sendingtime is equal to this sendingtime, simply
                    //  copy over the previous values.
                    //

                    if( last_sendingtime != (*marketDataIter).sendingtime() )
                    {
                        last_sendingtime = (*marketDataIter).sendingtime();
                        sprintf( cme_timestamp_buf, "%llu", last_sendingtime );
                        quantmodel::TimeStamp::getCMETime(
                            timeinfo,
                            cme_timestamp_buf,
                            get_cme_time_buf,
                            broadcast_timestamp_buf );
                    }

                    fastEngineBroadcast.set_timestamp( broadcast_timestamp_buf );
                    fastEngineBroadcast.set_broadcast_type( quantmodel::FastEngineBroadcast::CME );
                }

                for( int i=0; i<(*marketDataIter).mdentries_size(); ++i )
                {
                    const fast_engine::CMEMarketDataEntry& marketDataEntry = (*marketDataIter).mdentries( i );

                    //
                    //  only process if update type == CME_MULTIPLE_DEPTH, or
                    //  entry type is not [ BID / ASK / TRADE ]
                    //

                    if( ( marketDataEntry.mdentrytype() > CMEMarketDataEntry::ENTRY_TYPE_TRADE ) ||
                        ( marketDataEntry.has_quotecondition() ) ||
                        ( marketDataEntry.mdupdateaction() == 5 && marketDataEntry.has_mdquotetype() ) )
                    {
                        continue;
                    }

                    //
                    //  Create a new quantmodel::FastEngineBroadcast::CMEMarketData
                    //

                    quantmodel::CMEMarketData *cme_market_data =
                        fastEngineBroadcast.add_cme_market_data();

                    //
                    //  Set the sending time
                    //

                    cme_market_data->set_sending_tm( (*marketDataIter).sendingtime() );

                    //
                    //  Set the security_id
                    //

                    cme_market_data->set_security_id( marketDataEntry.securityid() );

                    //
                    //  Set the update_action
                    //

                    cme_market_data->set_update_action(
                        (quantmodel::CMEMarketData_UpdateAction)
                            marketDataEntry.mdupdateaction() );

                    //
                    //  Populate the rest based on entry type
                    //

                    if( marketDataEntry.mdentrytype() == CMEMarketDataEntry::ENTRY_TYPE_BID )
                    {
                        cme_market_data->set_entry_type( quantmodel::CMEMarketData::BID );

                        //
                        //  Set the price level (1023)
                        //

                        cme_market_data->set_price_level( marketDataEntry.mdpricelevel() );

                        if( marketDataEntry.mdupdateaction() == quantmodel::CMEMarketData::ADD ||
                            marketDataEntry.mdupdateaction() == quantmodel::CMEMarketData::CHANGE )
                        {
                            //
                            //  Set the entry size (271)
                            //          price (270),
                            //          number of orders (346)
                            //

                            cme_market_data->set_entry_size( marketDataEntry.mdentrysize() );
                            cme_market_data->set_mantissa( marketDataEntry.mdentrypxmantissa() );
                            cme_market_data->set_exponent( marketDataEntry.mdentrypxexponent() );
                            cme_market_data->set_orders( marketDataEntry.numberoforders() );
                        }
                    }
                    else if( marketDataEntry.mdentrytype() == CMEMarketDataEntry::ENTRY_TYPE_ASK )
                    {
                        cme_market_data->set_entry_type( quantmodel::CMEMarketData::ASK );

                        //
                        //  Set the price level (1023)
                        //

                        cme_market_data->set_price_level( marketDataEntry.mdpricelevel() );

                        if( marketDataEntry.mdupdateaction() == quantmodel::CMEMarketData::ADD ||
                            marketDataEntry.mdupdateaction() == quantmodel::CMEMarketData::CHANGE )
                        {
                            //
                            //  Set the entry size (271)
                            //          price (270),
                            //          number of orders (346)
                            //

                            cme_market_data->set_entry_size( marketDataEntry.mdentrysize() );
                            cme_market_data->set_mantissa( marketDataEntry.mdentrypxmantissa() );
                            cme_market_data->set_exponent( marketDataEntry.mdentrypxexponent() );
                            cme_market_data->set_orders( marketDataEntry.numberoforders() );
                        }
                    }
                    else // if( marketDataEntry.mdentrytype() == CMEMarketDataEntry::ENTRY_TYPE_TRADE )
                    {
                        cme_market_data->set_entry_type( quantmodel::CMEMarketData::TRADE );

                        //
                        //  Set the MDEntryPx (270)
                        //

                        cme_market_data->set_mantissa( marketDataEntry.mdentrypxmantissa() );
                        cme_market_data->set_exponent( marketDataEntry.mdentrypxexponent() );

                        //
                        //  Test for implied price (277)
                        //

                        if( unlikely( marketDataEntry.has_tradecondition() == true ) )
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

                        if( likely( marketDataEntry.has_mdentrytime() == true ) )
                        {
                            cme_market_data->set_entry_tm( marketDataEntry.mdentrytime() );
                        }

                        //
                        //  Set the MDEntrySize (271) if present
                        //

                        if( likely( marketDataEntry.has_mdentrysize() == true ) )
                        {
                            cme_market_data->set_entry_size( marketDataEntry.mdentrysize() );
                        }

                        //
                        //  Set the AggressorSide (5797) if present
                        //

                        if( likely( marketDataEntry.has_aggressorside() == true ) )
                        {
                            cme_market_data->set_aggressor(
                                (quantmodel::CMEMarketData_AggressorType)
                                    marketDataEntry.aggressorside() );
                        }
                    }
                }
            }
        }
        
        marketData.clear();
    }
    
    return fastEngineBroadcast.cme_market_data_size() > 0;
}

