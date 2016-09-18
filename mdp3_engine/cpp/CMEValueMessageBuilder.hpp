#ifndef QUICKFAST_CME_VALUEMESSAGEBUILDER_H
#define QUICKFAST_CME_VALUEMESSAGEBUILDER_H

#include <Messages/ValueMessageBuilder.h>

#include <quantmodel.hpp>

#include "CMEMarketDataProtobuf.pb.h"

namespace fast_engine
{
    class CMEFieldValues
    {
        public:

            CMEFieldValues() :
                MDUpdateAction( "279" ),
                MDPriceLevel( "1023" ),
                MDEntryType( "269" ),
                SecurityIDSource( "22" ),
                SecurityID( "48" ),
                RptSeq( "83" ),
                QuoteCondition( "276" ),
                MDEntryPx( "270" ),
                NumberOfOrders( "346" ),
                MDEntryTime( "273" ),
                MDEntrySize( "271" ),
                TradingSessionID( "336" ),
                NetChgPrevDay( "451" ),
                TickDirection( "274" ),
                OpenCloseSettleFlag( "286" ),
                SettlDate( "64" ),
                TradeVolume( "1020" ),
                TradeCondition( "277" ),
                AggressorSide( "5797" ),
                MDQuoteType( "1070" ),
                TradeID( "1003" ),
                ApplVerID( "1128" ),
                MessageType( "35" ),
                SenderCompID( "49" ),
                MsgSeqNum( "34" ),
                SendingTime( "52" ),
                PosDupFlag( "43" ),
                TradeDate( "75" )
            {}

            //
            //  CMEMarketDataEntry
            //

            const std::string MDUpdateAction;
            const std::string MDPriceLevel;
            const std::string MDEntryType;
            const std::string SecurityIDSource;
            const std::string SecurityID;
            const std::string RptSeq;
            const std::string QuoteCondition;
            const std::string MDEntryPx;
            const std::string NumberOfOrders;
            const std::string MDEntryTime;
            const std::string MDEntrySize;
            const std::string TradingSessionID;
            const std::string NetChgPrevDay;
            const std::string TickDirection;
            const std::string OpenCloseSettleFlag;
            const std::string SettlDate;
            const std::string TradeVolume;
            const std::string TradeCondition;
            const std::string AggressorSide;
            const std::string MDQuoteType;
            const std::string TradeID;

            //
            //  CMEMarketDataProtobuf
            //

            const std::string ApplVerID;
            const std::string MessageType;
            const std::string SenderCompID;
            const std::string MsgSeqNum;
            const std::string SendingTime;
            const std::string PosDupFlag;
            const std::string TradeDate;
    };

    //
    //  Interface definition heavily borrowed from:
    //
    //      quickfast_1_3\src\Examples\Examples\ValueToFix.h
    //

    enum ValueMessageBuilderState
    {
        INIT,
        START_MESSAGE,
        START_GROUP,
        START_SEQUENCE,
        END_MESSAGE
    };

    class CMEValueMessageBuilder : public QuickFAST::Messages::ValueMessageBuilder
    {
        public:

            CMEValueMessageBuilder();
            virtual ~CMEValueMessageBuilder();

            bool getFastEngineBroadcast( quantmodel::FastEngineBroadcast& );

            //
            //  Implement ValueMessageBuilder
            //

            virtual const std::string & getApplicationType() const;
            virtual const std::string & getApplicationTypeNs() const;
            virtual void addValue( QuickFAST::Messages::FieldIdentityCPtr & identity, QuickFAST::ValueType::Type type, const QuickFAST::int64 value);
            virtual void addValue( QuickFAST::Messages::FieldIdentityCPtr & identity, QuickFAST::ValueType::Type type, const QuickFAST::uint64 value);
            virtual void addValue( QuickFAST::Messages::FieldIdentityCPtr & identity, QuickFAST::ValueType::Type type, const QuickFAST::int32 value);
            virtual void addValue( QuickFAST::Messages::FieldIdentityCPtr & identity, QuickFAST::ValueType::Type type, const QuickFAST::uint32 value);
            virtual void addValue( QuickFAST::Messages::FieldIdentityCPtr & identity, QuickFAST::ValueType::Type type, const QuickFAST::int16 value);
            virtual void addValue( QuickFAST::Messages::FieldIdentityCPtr & identity, QuickFAST::ValueType::Type type, const QuickFAST::uint16 value);
            virtual void addValue( QuickFAST::Messages::FieldIdentityCPtr & identity, QuickFAST::ValueType::Type type, const QuickFAST::int8 value);
            virtual void addValue( QuickFAST::Messages::FieldIdentityCPtr & identity, QuickFAST::ValueType::Type type, const QuickFAST::uchar value);
            virtual void addValue( QuickFAST::Messages::FieldIdentityCPtr & identity, QuickFAST::ValueType::Type type, const QuickFAST::Decimal& value);
            virtual void addValue( QuickFAST::Messages::FieldIdentityCPtr & identity, QuickFAST::ValueType::Type type, const unsigned char * value, size_t length);
            virtual QuickFAST::Messages::ValueMessageBuilder & startMessage(
                const std::string & applicationType,
                const std::string & applicationTypeNamespace,
                size_t fieldCount );
            virtual bool endMessage( QuickFAST::Messages::ValueMessageBuilder & messageBuilder );
            virtual bool ignoreMessage( QuickFAST::Messages::ValueMessageBuilder & messageBuilder );
            virtual QuickFAST::Messages::ValueMessageBuilder & startSequence(
                QuickFAST::Messages::FieldIdentityCPtr & identity,
                const std::string & applicationType,
                const std::string & applicationTypeNamespace,
                size_t fieldCount,
                QuickFAST::Messages::FieldIdentityCPtr & lengthIdentity,
                size_t length );
            virtual void endSequence(
                QuickFAST::Messages::FieldIdentityCPtr & identity,
                QuickFAST::Messages::ValueMessageBuilder & sequenceBuilder);
            virtual QuickFAST::Messages::ValueMessageBuilder & startSequenceEntry(
                const std::string & applicationType,
                const std::string & applicationTypeNamespace,
                size_t size ) ;
            virtual void endSequenceEntry( QuickFAST::Messages::ValueMessageBuilder & entry );
            virtual QuickFAST::Messages::ValueMessageBuilder & startGroup(
                QuickFAST::Messages::FieldIdentityCPtr & identity,
                const std::string & applicationType,
                const std::string & applicationTypeNamespace,
                size_t size );
            virtual void endGroup(
                QuickFAST::Messages::FieldIdentityCPtr & identity,
                QuickFAST::Messages::ValueMessageBuilder & groupBuilder );

            //
            //  Implement Logger (because we have to)
            //

            void setLogLevel( QuickFAST::Common::Logger::LogLevel level );
            virtual bool wantLog( unsigned short level );
            virtual bool logMessage( unsigned short level, const std::string & logMessage );
            virtual bool reportDecodingError( const std::string & errorMessage );
            virtual bool reportCommunicationError( const std::string & errorMessage );

        private:

            QuickFAST::Common::Logger::LogLevel logLevel;
            size_t fieldCount;
            std::string applType;
            std::string applTypeNamespace;
            CMEMarketDataEntry *marketDataEntryPtr;
            CMEFieldValues fieldValues;
            ValueMessageBuilderState state;

            //
            //  Variables used for fastEngineBroadcast()
            //

            std::vector<CMEMarketDataProtobuf> marketData;
            struct tm timeinfo;
            char get_cme_time_buf[ 8 ];
            char cme_timestamp_buf[ 32 ];
            char broadcast_timestamp_buf[ 16 ];
            long long unsigned int last_sendingtime;
    };
}

#endif /* QUICKFAST_CME_VALUEMESSAGEBUILDER_H */
