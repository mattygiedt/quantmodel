#ifndef QUICKFAST_CME_MESSAGECONSUMER_H
#define QUICKFAST_CME_MESSAGECONSUMER_H

#include <Codecs/MessageConsumer.h>
#include <Messages/MessageFormatter.h>
#include <Messages/Sequence.h>

#include <log4cpp/Category.hh>

#include <quantmodel.hpp>

namespace fast_engine
{
    enum CMEUpdateType
    {
        CME_MULTIPLE_DEPTH = 0,
        CME_IMPLIED = 1,
        CME_INDICATIVE_PRICES = 2,
        CME_TOP_OF_BOOK = 4,
        CME_LAST_BEST_PRICE = 8,
        CME_UNKNOWN_UPDATE_TYPE = 256
    };

    class CMEMessageConsumer : public QuickFAST::Codecs::MessageConsumer
    {
        public:

            CMEMessageConsumer( );
            ~CMEMessageConsumer( );

            bool consumeMessage( QuickFAST::Messages::Message & );

            bool wantLog( unsigned short ) { return true; }
            bool logMessage( unsigned short , const std::string& ) { return true; }
            bool reportDecodingError( const std::string& ) { return true; }
            bool reportCommunicationError( const std::string& ) { return true; }
            void decodingStarted(){}
            void decodingStopped(){}

            inline void reset() { fast_engine_broadcast.Clear(); }

            bool getBroadcastMessage( quantmodel::FastEngineBroadcast& msg ) const
            {
                if( fast_engine_broadcast.cme_market_data_size() > 0 )
                {
                    msg.CopyFrom( fast_engine_broadcast );
                    return true;
                }
                else
                {
                    return false;
                }
            }

            inline void broadcast( zmq::socket_t* const socket ) const
            {
                if( likely( fast_engine_broadcast.cme_market_data_size() > 0 ) )
                {
                    //fast_engine_broadcast.set_timestamp( broadcast_timestamp );
                    //fast_engine_broadcast.set_broadcast_type( quantmodel::FastEngineBroadcast::CME );
                    quantmodel::ZMQMessage::send( &fast_engine_broadcast, socket );

                    //std::cout << fast_engine_broadcast.ShortDebugString() << "\n";
                }
            }

        private:

            CMEMessageConsumer (const CMEMessageConsumer &);
            CMEMessageConsumer & operator= (const CMEMessageConsumer &);

            const QuickFAST::Messages::FieldIdentity quoteTypeIdentity;
            const QuickFAST::Messages::FieldIdentity entrySizeIdentity;
            const QuickFAST::Messages::FieldIdentity entryTimeIdentity;
            const QuickFAST::Messages::FieldIdentity sendingTimeIdentity;
            const QuickFAST::Messages::FieldIdentity aggressorSideIdentity;
            const QuickFAST::Messages::FieldIdentity quoteConditionIdentity;
            const QuickFAST::Messages::FieldIdentity tradeConditionIdentity;

            void incrementalRefresh( const QuickFAST::Messages::Message & );
            void securityDefinition( const QuickFAST::Messages::Message & );
            void quoteRequest( const QuickFAST::Messages::Message & );
            void securityStatus( const QuickFAST::Messages::Message & );
            void snapshotFullRequest( const QuickFAST::Messages::Message & );
            void newsMessage( const QuickFAST::Messages::Message & );
            void heartbeat( const QuickFAST::Messages::Message & );
            void logon( const QuickFAST::Messages::Message & );
            void logout( const QuickFAST::Messages::Message & );

            const fast_engine::CMEUpdateType getUpdateType( const QuickFAST::uint32& , const QuickFAST::Messages::FieldSetCPtr& ) const;

            char ENTRY_TYPE_BID;
            char ENTRY_TYPE_ASK;
            char ENTRY_TYPE_TRADE;
            char ENTRY_TYPE_OPENING_PRICE;
            char ENTRY_TYPE_SETTLEMENT_PRICE;
            char ENTRY_TYPE_TRADING_SESSION_HIGH_PRICE;
            char ENTRY_TYPE_TRADING_SESSION_LOW_PRICE;
            char ENTRY_TYPE_TRADE_VOLUME;
            char ENTRY_TYPE_OPEN_INTEREST;
            char ENTRY_TYPE_SIMULATED_SELL;
            char ENTRY_TYPE_SIMULATED_BUY;
            char ENTRY_TYPE_EMPTY_BOOK;
            char ENTRY_TYPE_PRIOR;
            char ENTRY_TYPE_SESSION_HIGH_BID;
            char ENTRY_TYPE_SESSION_LOW_OFFER;
            char ENTRY_TYPE_FIXING_PRICE;
            char ENTRY_TYPE_CASH_NOTE;

            QuickFAST::Messages::Sequence::const_iterator sequence_iter;
            QuickFAST::Messages::MessageFormatter* formatter;
            quantmodel::FastEngineBroadcast fast_engine_broadcast;
            std::string broadcast_timestamp;

            struct tm timeinfo;
            char get_cme_time_buf[ 8 ];
            char cme_timestamp_buf[ 32 ];
            char broadcast_timestamp_buf[ 16 ];
    };
}

#endif //   QUICKFAST_CMEMESSAGECONSUMER_H
