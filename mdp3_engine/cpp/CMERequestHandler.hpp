#ifndef QUICKFAST_CME_REQUESTHANDLER_H
#define QUICKFAST_CME_REQUESTHANDLER_H

#include <set>

#include <Messages/Sequence.h>

#include <zmq.hpp>

#include <quantmodel.hpp>

#include "InstrumentDefinition.hpp"
#include "InstrumentOrderBook.hpp"

namespace fast_engine
{
    class CMERequestHandler
    {
        public:

            CMERequestHandler()
            {
                //
                //  Add the handler functions to the function map
                //
                //
                //  func_map.insert( std::pair< const std::string , const void (fast_engine::CMERequestHandler::*)(const QuickFAST::Messages::Message &) >( "X", &fast_engine::CMERequestHandler::incrementalRefresh ) );
                //  func_map.insert( std::pair< const std::string , const void (fast_engine::CMERequestHandler::*)(const QuickFAST::Messages::Message &) >( "d", &fast_engine::CMERequestHandler::securityDefinition ) );
                //  func_map.insert( std::pair< const std::string , const void (fast_engine::CMERequestHandler::*)(const QuickFAST::Messages::Message &) >( "R", &fast_engine::CMERequestHandler::quoteRequest ) );
                //  func_map.insert( std::pair< const std::string , const void (fast_engine::CMERequestHandler::*)(const QuickFAST::Messages::Message &) >( "f", &fast_engine::CMERequestHandler::securityStatus ) );
                //  func_map.insert( std::pair< const std::string , const void (fast_engine::CMERequestHandler::*)(const QuickFAST::Messages::Message &) >( "W", &fast_engine::CMERequestHandler::snapshotFullRequest ) );
                //  func_map.insert( std::pair< const std::string , const void (fast_engine::CMERequestHandler::*)(const QuickFAST::Messages::Message &) >( "B", &fast_engine::CMERequestHandler::newsMessage ) );
                //  func_map.insert( std::pair< const std::string , const void (fast_engine::CMERequestHandler::*)(const QuickFAST::Messages::Message &) >( "0", &fast_engine::CMERequestHandler::heartbeat ) );
                //  func_map.insert( std::pair< const std::string , const void (fast_engine::CMERequestHandler::*)(const QuickFAST::Messages::Message &) >( "A", &fast_engine::CMERequestHandler::logon ) );
                //  func_map.insert( std::pair< const std::string , const void (fast_engine::CMERequestHandler::*)(const QuickFAST::Messages::Message &) >( "5", &fast_engine::CMERequestHandler::logout ) );
                //

                //
                //  Create the unknown instrument order book
                //

                unknown_instrument_book =
                    new fast_engine::InstrumentOrderBook(
                        new fast_engine::InstrumentDefinition( ) );

                last_instrument_book =
                frequent_instrument_book =
                unknown_instrument_book;
           }

            ~CMERequestHandler(){}

            void printOrderBooks( )
            {
                printf( "\n/////////////////////////////////////  /////////////////////////////////////\n\n" );
                book_iter = book_map.begin();
                while( book_iter != book_map.end() )
                {
                    book_iter->second->printOrderBook();
                    ++book_iter;
                }
            }

            const inline bool hasInstrumentOrderBook( const QuickFAST::uint32& sec_id ) const { return book_map.count( sec_id ) > 0; }
            const inline bool hasInstrumentDefinition( const QuickFAST::uint32& sec_id ) const { return definition_map.count( sec_id ) > 0; }

            fast_engine::InstrumentOrderBook* const createInstrumentOrderBook( const QuickFAST::uint32& sec_id )
            {
                //
                //  Find the instrument definition
                //

                if( ( definition_iter = definition_map.find( sec_id ) ) != definition_map.end() )
                {
                    fast_engine::InstrumentOrderBook* book = new fast_engine::InstrumentOrderBook( definition_iter->second );
                    book_map.insert( std::pair< const QuickFAST::uint32& , fast_engine::InstrumentOrderBook* >( sec_id, book ) );
                    return book;
                }
                else
                {
                    fprintf( stderr, "UNKNOWN SECURITY DEFINITION ID: %u\n", sec_id );
                    //book_map.insert( std::pair< const QuickFAST::uint32& , fast_engine::InstrumentOrderBook* >( sec_id, unknown_instrument_book ) );
                    return unknown_instrument_book;
                }
            }

            //
            //  Uses book_array
            //

            fast_engine::InstrumentOrderBook* const getInstrumentOrderBook( const QuickFAST::uint32& sec_id )
            {
                if( unlikely( sec_id > max_security_id ) )
                {
                    printf( "sec_id( %u ) > max_security_id( %u )\n", sec_id, max_security_id );
                    return unknown_instrument_book;
                }

                if( sec_id == last_instrument_book->getSecurityID() )
                {
                    return last_instrument_book;
                }

                if( unlikely( ( last_instrument_book = book_array[ sec_id ] ) == NULL ) )
                {
                    if( ( definition_iter = definition_map.find( sec_id ) ) != definition_map.end() )
                    {
                        //
                        //  Thus we lazily create instrument order books loaded
                        //  by the instruments.xml file into the book array.
                        //

                        printf( "  fast_engine::InstrumentOrderBook( %u ) [ %u ]\n",
                            sec_id, definition_iter->second->getMarketDepth() );
                        last_instrument_book = new fast_engine::InstrumentOrderBook( definition_iter->second );
                    }
                    else
                    {
                        printf( "UNKNOWN INSTRUMENT BOOK: %u\n", sec_id );
                        last_instrument_book = unknown_instrument_book;
                    }

                    book_array[ sec_id ] = last_instrument_book;
                }

                return last_instrument_book;
            }

            inline void notify( quantmodel::FastEngineBroadcast& msg )
            {
                for( notify_iter = notify_set.begin(); notify_iter != notify_set.end(); ++notify_iter )
                {
                    book_array[ *notify_iter ]->publish( msg.add_book_data() );
                }
            }

/*
            //
            //  Uses book_map
            //

            fast_engine::InstrumentOrderBook* const getInstrumentOrderBook( const QuickFAST::uint32& sec_id )
            {
                ////////////////////////////////////////////////////////////////
                //  Four levels of caching here:
                //
                //      most frequent, last, mapped, everything
                //

                //
                //  MOST FREQUENT
                //

                if( sec_id == frequent_instrument_book->getSecurityID() )
                {
                    return frequent_instrument_book;
                }

                //
                //  LAST
                //

                if( sec_id == last_instrument_book->getSecurityID() )
                {
                    return last_instrument_book;
                }

                //
                //  MAPPED
                //

                if( ( book_iter = book_map.find( sec_id ) ) != book_map.end() )
                {
                    last_instrument_book = book_iter->second;

                    //
                    //  update the most frequency used book?
                    //

                    if( last_instrument_book->getHitCount() > frequent_instrument_book->getHitCount() )
                    {
                        frequent_instrument_book = last_instrument_book;
                    }

                    return last_instrument_book;
                }

                //
                //  EVERYTHING
                //

                if( ( definition_iter = definition_map.find( sec_id ) ) != definition_map.end() )
                {
                    //
                    //  Thus we lazily create instrument order books loaded
                    //  by the instruments.xml file into the definition map.
                    //

                    last_instrument_book = new fast_engine::InstrumentOrderBook( definition_iter->second );
                    book_map.insert(
                        std::pair< const unsigned int , fast_engine::InstrumentOrderBook* >(
                            sec_id, last_instrument_book ) );
                    return last_instrument_book;
                }
                else
                {
                    //printf( "UNKNOWN INSTRUMENT BOOK: %u\n", sec_id );
                    //fprintf( stderr, "UNKNOWN INSTRUMENT BOOK: %u\n", sec_id );

                    return unknown_instrument_book;
                }
            }

            void updateInstrumentDefinition( fast_engine::InstrumentDefinition* def )
            {
                const QuickFAST::uint32& sec_id = def->getSecurityID();

                //
                //  Use this method with EXTREME caution!
                //

                if( hasInstrumentDefinition( sec_id ) )
                {
                    if( hasInstrumentOrderBook( sec_id ) )
                    {
                        //
                        //  MAKE SURE YOU UNDERSTAND WHAT'S GOING ON HERE. THERE
                        //  WAS AN ORIGINAL DEFINITION FOR THIS SECURITY WHICH
                        //  HAD AN ORDER BOOK CREATED FOR IT. THIS ORDER BOOK
                        //  IS NOW GONE!!!
                        //

                        delete book_iter->second;
                        book_map.erase( sec_id );
                    }

                    delete definition_iter->second;
                    definition_map.erase( sec_id );
                }

                definition_map.insert(
                    std::pair< const QuickFAST::uint32 , fast_engine::InstrumentDefinition* >
                    ( def->getSecurityID(), def ) );

                book_map.insert(
                    std::pair< const QuickFAST::uint32 , fast_engine::InstrumentOrderBook* >(
                        sec_id, new fast_engine::InstrumentOrderBook( def ) ) );
            }

            inline void notify( quantmodel::FastEngineBroadcast& msg )
            {
                for( notify_iter = notify_set.begin(); notify_iter != notify_set.end(); ++notify_iter )
                {
                    if( ( book_iter = book_map.find( (*notify_iter) ) ) != book_map.end() )
                    {
                        book_iter->second->publish( msg.add_book_data() );
                        //getInstrumentOrderBook( (*notify_iter) )->publish( msg.add_book_data() );
                    }
                }
            }
*/

            void initNetwork( std::map< const std::string, zmq::socket_t* >& map )
            {
                std::map< const std::string, zmq::socket_t* >::iterator iter;
                iter = map.begin();
                while( iter != map.end() )
                {
                    socket_map.insert(
                        std::pair< const std::string , zmq::socket_t* >(
                            iter->first, iter->second ) );
                    ++iter;
                }
            }

            const bool handleMessage( const int message_type, const QuickFAST::Messages::Message & message )
            {
                if( likely( message_type == 88 ) ) { incrementalRefresh( message ); return true; } // X
                switch( message_type )
                {
                    case 102: { securityStatus( message ); return true; } // f
                    case 100: { securityDefinition( message ); return true; } // d
                    case 87: { snapshotFullRequest( message ); return true; } // W
                    case 82: { quoteRequest( message ); return true; } // R
                    case 66: { newsMessage( message ); return true; } // B
                    case 65: { logon( message ); return true; } // A
                    case 53: { logout( message ); return true; } // 5
                    case 48: { heartbeat( message ); return true; } // 0
                    default: return false;
                };

                //if( ( func_iter = func_map.find( message_type ) ) != func_map.end() ) { (this->*func_iter->second)( message ); return true; }
                //else return false;
            }

            const bool loadSecurityDefinitionFile( const char * );

            inline void reset() { notify_set.clear(); }

        private:

            // prevent copying and assignment; not implemented
            CMERequestHandler (const CMERequestHandler &);
            CMERequestHandler & operator= (const CMERequestHandler &);

            double start, stop;
            QuickFAST::uint32 max_security_id;

            fast_engine::InstrumentOrderBook *last_instrument_book;
            fast_engine::InstrumentOrderBook *frequent_instrument_book;
            fast_engine::InstrumentOrderBook *unknown_instrument_book;

            QuickFAST::Messages::Sequence::const_iterator sequence_iter;

            std::set< unsigned int > notify_set;
            std::set< unsigned int >::const_iterator notify_iter;

            std::map< const std::string , zmq::socket_t* > socket_map;
            std::map< const std::string , zmq::socket_t* >::const_iterator socket_iter;

            std::map< const unsigned int , fast_engine::InstrumentDefinition* > definition_map;
            std::map< const unsigned int , fast_engine::InstrumentDefinition* >::const_iterator definition_iter;

            fast_engine::InstrumentOrderBook** book_array;

            std::map< const unsigned int , fast_engine::InstrumentOrderBook* > book_map;
            std::map< const unsigned int , fast_engine::InstrumentOrderBook* >::const_iterator book_iter;

            std::vector< std::pair< unsigned int , fast_engine::InstrumentOrderBook* > > frequent_vec;
            std::vector< std::pair< unsigned int , fast_engine::InstrumentOrderBook* > >::const_iterator recent_iter;

            //std::map< const std::string , const void (fast_engine::CMERequestHandler::*)(const QuickFAST::Messages::Message &) > func_map;
            //std::map< const std::string , const void (fast_engine::CMERequestHandler::*)(const QuickFAST::Messages::Message &) >::const_iterator func_iter;

            const void incrementalRefresh( const QuickFAST::Messages::Message & );
            const void securityDefinition( const QuickFAST::Messages::Message & );
            const void quoteRequest( const QuickFAST::Messages::Message & );
            const void securityStatus( const QuickFAST::Messages::Message & );
            const void snapshotFullRequest( const QuickFAST::Messages::Message & );
            const void newsMessage( const QuickFAST::Messages::Message & );
            const void heartbeat( const QuickFAST::Messages::Message & );
            const void logon( const QuickFAST::Messages::Message & );
            const void logout( const QuickFAST::Messages::Message & );

            void clearDefinitionMap()
            {
                definition_iter = definition_map.begin();
                while( definition_iter != definition_map.end() )
                {
                    delete definition_iter->second;
                    ++definition_iter;
                }

                definition_map.clear();
            }

            void clearBookMap()
            {
                book_iter = book_map.begin();
                while( book_iter != book_map.end() )
                {
                    delete book_iter->second;
                    ++book_iter;
                }

                book_map.clear();
            }

    };
}

#endif // QUICKFAST_CME_REQUESTHANDLER_H
