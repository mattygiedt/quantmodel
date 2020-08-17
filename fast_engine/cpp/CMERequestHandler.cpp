

#include <Application/QuickFAST.h>
#include <Messages/Message.h>
#include <Messages/Sequence.h>
#include <Messages/Field.h>

#include "CMERequestHandler.hpp"


const bool fast_engine::CMERequestHandler::loadSecurityDefinitionFile( const char *secdef )
{
    max_security_id = 0;

    cout << "  loading security definition file: " << secdef << endl;

    clearDefinitionMap();

    const XMLNode& instruments = XMLNode::openFileHelper( secdef );

    cout << "   importing " << instruments.nChildNode( ) << " instruments..." << endl;

    for( int i=0; i<instruments.nChildNode( ); ++i )
    {
        fast_engine::InstrumentDefinition *def =
            new fast_engine::InstrumentDefinition(
                instruments.getChildNode( i ),
                    fast_engine::InstrumentDefinition::CME );

        definition_map.insert(
            std::pair< const QuickFAST::uint32 , fast_engine::InstrumentDefinition * >
            ( def->getSecurityID(), def ) );

        if( max_security_id < def->getSecurityID() )
        {
            max_security_id = def->getSecurityID();
        }

        if( i%10000 == 0 )
        {
            cout << "   " << i << endl;
        }
    }

    //
    //  Create the book_array
    //

    cout << "   max_security_id: " << max_security_id << endl;

    book_array = new fast_engine::InstrumentOrderBook*[ max_security_id+1 ];

    for( unsigned int i=0; i<max_security_id+1; ++i )
    {
        book_array[ i ] = NULL;
    }

    cout << "  security definition import complete." << endl;

    return true;
}

const void fast_engine::CMERequestHandler::incrementalRefresh( const QuickFAST::Messages::Message& message )
{
    QuickFAST::Messages::FieldCPtr field;
    QuickFAST::Messages::FieldSetCPtr field_set;
    QuickFAST::Messages::SequenceCPtr sequence;

    fast_engine::InstrumentOrderBook *instrument_order_book;

    //
    //  Get the sending time from the CME header
    //

    message.getField( "SendingTime", field );
    const QuickFAST::uint64& sending_tm = field->toUInt64();

    //
    //  Process all MDEntries in the message
    //

    message.getField( "MDEntries", field );
    sequence = field->toSequence();

    for( sequence_iter = sequence->begin(); sequence_iter != sequence->end(); ++sequence_iter )
    {
        field_set = *sequence_iter;

        //
        //  Update the InstrumentOrderBook found by securityID
        //

        field_set->getField( "SecurityID", field );
        const QuickFAST::uint32 securityID = field->toUInt32();

        if( likely( ( instrument_order_book = getInstrumentOrderBook( securityID ) ) != unknown_instrument_book ) )
        {
            instrument_order_book->update( sending_tm, field_set );

            //
            //  Add the security id to the notify set
            //

            //printf( "  notify_set.insert( %u )\n", securityID );
            notify_set.insert( securityID );
        }
    }
}

/*
const void fast_engine::CMERequestHandler::securityDefinition( const QuickFAST::Messages::Message &message )
{
    QuickFAST::Messages::FieldCPtr field;
    message.getField( "SecurityID", field );
    const QuickFAST::uint32& securityID = field->toUInt32();

    if(! hasInstrumentDefinition( securityID ) )
    {
        printf( "UPDATING SECURITY DEFINITION ID: %u\n", securityID );

        QuickFAST::uint32 marketDepth;

        QuickFAST::Messages::FieldIdentity feedTypes( "MDFeedTypes" );
        QuickFAST::Messages::FieldSetCPtr field_set;
        QuickFAST::Messages::SequenceCPtr sequence;
        QuickFAST::Messages::Sequence::const_iterator sequence_iter;

        if( message.isPresent( feedTypes ) )
        {
            marketDepth = 0;
            message.getField( "MDFeedTypes", field );
            sequence = field->toSequence();

            for( sequence_iter = sequence->begin(); sequence_iter != sequence->end(); ++sequence_iter )
            {
                field_set = *sequence_iter;
                field_set->getField( "MarketDepth", field );

                if( field->toUInt32() > marketDepth )
                    marketDepth = field->toUInt32();
            }
        }
        else
        {
            marketDepth = 0;
        }

        printf( " MARKET DEPTH %u\n", marketDepth );

        //
        //  Create a new instrument definition from the message
        //

        message.getField( "Symbol", field );
        const std::string& symbol = field->toAscii();

        message.getField( "SecurityDesc", field );
        const std::string& securityDesc = field->toAscii();

        message.getField( "MaturityMonthYear", field );
        const QuickFAST::uint64& maturityMonthYear = field->toUInt64();

        message.getField( "MinPriceIncrement", field );
        const QuickFAST::Decimal& minPriceIncrement = field->toDecimal();

        message.getField( "DisplayFactor", field );
        const QuickFAST::Decimal& displayFactor = field->toDecimal();

        updateInstrumentDefinition(
            new fast_engine::InstrumentDefinition(
                symbol,
                securityDesc,
                securityID,
                marketDepth,
                maturityMonthYear,
                double( minPriceIncrement ),
                double( displayFactor ) ) );

        //printf( "\n ORDERBOOK %u\n", securityID );
        //getInstrumentOrderBook( securityID )->printOrderBook();
    }
}
*/

const void fast_engine::CMERequestHandler::securityDefinition( const QuickFAST::Messages::Message &message ){}
const void fast_engine::CMERequestHandler::quoteRequest( const QuickFAST::Messages::Message &message ){}
const void fast_engine::CMERequestHandler::securityStatus( const QuickFAST::Messages::Message &message ){}
const void fast_engine::CMERequestHandler::snapshotFullRequest( const QuickFAST::Messages::Message &message ){}
const void fast_engine::CMERequestHandler::newsMessage( const QuickFAST::Messages::Message &message ){}
const void fast_engine::CMERequestHandler::heartbeat( const QuickFAST::Messages::Message &message ){}
const void fast_engine::CMERequestHandler::logon( const QuickFAST::Messages::Message &message ){}
const void fast_engine::CMERequestHandler::logout( const QuickFAST::Messages::Message &message ){}
