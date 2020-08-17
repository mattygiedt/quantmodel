#ifndef QUANTMODEL_FIXAPPLICATION_H
#define QUANTMODEL_FIXAPPLICATION_H

#include <quickfix/Message.h>
#include <quickfix/SessionID.h>
#include <quickfix/Application.h>
#include <quickfix/SocketInitiator.h>

#include <quickfix/fix42/Logon.h>
#include <quickfix/fix42/Reject.h>
#include <quickfix/fix42/Logout.h>
#include <quickfix/fix42/ExecutionReport.h>
#include <quickfix/fix42/OrderCancelReject.h>
#include <quickfix/fix42/SecurityDefinition.h>
#include <quickfix/fix42/BusinessMessageReject.h>
#include <quickfix/fix42/MarketDataRequestReject.h>
#include <quickfix/fix42/MarketDataIncrementalRefresh.h>
#include <quickfix/fix42/MarketDataSnapshotFullRefresh.h>

#include "ClientOrderIdData.hpp"

using namespace std;

namespace fix_engine
{
    class FIXApplication : public FIX::Application
    {
        public:

            virtual ~FIXApplication(){};
            virtual void start() = 0;
            virtual void shutdown() = 0;
            virtual void handleMessage( const quantmodel::FixEngineRequest&, quantmodel::FixEngineResponse&, quantmodel::DatabaseEngineRequest& ) = 0;

            const FIX::SessionID getSessionID() const { return session_id; }

            const std::string printMessage( const FIX::Message& msg ) const
            {
                std::stringstream out;
                FIX::FieldMap::const_iterator iter;

                //
                //  Print the the header
                //

                out << "[[ ";
                for( iter = msg.getHeader().begin(); iter != msg.getHeader().end(); ++iter )
                {
                    out << iter->first << "=" << iter->second.getString() << " ";
                }
                out << "] [ ";

                //
                //  Print the body
                //

                for( iter = msg.begin(); iter != msg.end(); ++iter )
                {
                    out << iter->first << "=" << iter->second.getString() << " ";
                }
                out << "] [ ";

                //
                //  Print the the trailer
                //

                for( iter = msg.getTrailer().begin(); iter != msg.getTrailer().end(); ++iter )
                {
                    out << iter->first << "=" << iter->second.getString() << " ";
                }
                out << "]]";

                //std::string out;
                //const std::string str = msg.toString();
                //out.reserve( str.size() );
                //std::string::const_iterator iter;
                //for( iter = str.begin(); iter < str.end(); ++iter )
                //{
                //    if( (*iter) == 1 )
                //    {
                //        out.push_back( ' ' );
                //    }
                //    else
                //    {
                //        out.push_back( (*iter) );
                //    }
                //}

                return out.str();
            }

            const bool isLoggedOn() const
            {
                FIX::Session *session = FIX::Session::lookupSession( session_id );
                return NULL == session ? false : session->isLoggedOn();
            }

            void login()
            {
                log->info( " session login: %s", session_id.toString().c_str() );
                FIX::Session *session = FIX::Session::lookupSession( session_id );

                if( NULL != session && session->isLoggedOn() == false )
                {
                    session->logon();
                }
            }

            void logout()
            {
                log->warn( " session logout: %s", session_id.toString().c_str() );
                FIX::Session *session = FIX::Session::lookupSession( session_id );

                if( NULL != session && session->isLoggedOn() == true )
                {
                    session->logout();
                }
            }

            void initNetwork()
            {
                ctx = new zmq::context_t( 1 );

                if( network.isAttributeSet( "dealing_callforward" ) )
                {
                    log->info( " connecting to callforward: %s", network.getAttribute( "dealing_callforward" ) );
                    dealing = new zmq::socket_t( *ctx, ZMQ_REQ );
                    dealing->connect( network.getAttribute( "dealing_callforward" ) );
                }
                else
                {
                    log->fatal( " dealing_callforward network address NOT SET." );
                    exit( 1 );
                }

                if( network.isAttributeSet( "persistence_callforward" ) )
                {
                    log->info( " connecting to persistence_callforward: %s",
                        network.getAttribute( "persistence_callforward" ) );
                    persistence = new zmq::socket_t( *ctx, ZMQ_REQ );
                    persistence->connect( network.getAttribute( "persistence_callforward" ) );
                }
                else
                {
                    log->fatal( " persistence network address NOT SET." );
                    exit( 1 );
                }

                if( network.isAttributeSet( "broadcast" ) )
                {
                    log->info( " binding to broadcast: %s", network.getAttribute( "broadcast" ) );
                    broadcast = new zmq::socket_t( *ctx, ZMQ_PUB );
                    broadcast->bind( network.getAttribute( "broadcast" ) );
                }
                else
                {
                    log->warn( " broadcast network address NOT SET." );
                }
            }

            void loadClientOrderIdMap( zmq::socket_t *socket )
            {
                quantmodel::DatabaseEngineRequest request;
                quantmodel::DatabaseEngineResponse response;

                quantmodel::TimeStamp::getMessageId( *request.mutable_request_id( ) );
                quantmodel::TimeStamp::getUTCTime( *request.mutable_timestamp( ) );
                quantmodel::QuantModelProcedures::GET_CLIENT_ORDER_ID_MAP( request );

                if( unlikely( log->isDebugEnabled() ) )
                {
                    log->debug( " <-- " + request.ShortDebugString() );
                }

                if( quantmodel::ZMQMessage::sendRPC( &request, &response, socket ) )
                {
                    if( unlikely( log->isDebugEnabled() ) )
                    {
                        log->debug( " --> " + response.ShortDebugString() );
                    }

                    for( int i=0; i<response.result_set_size(); i++ )
                    {
                        const quantmodel::DatabaseEngineResponse::ResultSet& rs = response.result_set( i );

                        for( int j=0; j<rs.row_size(); j++ )
                        {
                            fix_engine::ClientOrderIdData *data =
                                new fix_engine::ClientOrderIdData( rs.row( j ), log );

                            client_order_id_map.insert(
                                std::pair< const std::string , fix_engine::ClientOrderIdData* >(
                                    data->client_order_id(), data ) );
                        }
                    }
                }
            }

        protected:

            log4cpp::Category *log;
            XMLNode fix, network;
            FIX::SocketInitiator *conn;
            FIX::SessionID session_id;
            std::map< const std::string , fix_engine::ClientOrderIdData* > client_order_id_map;
            //quantmodel::MemcachedConnection memcache;

            zmq::context_t *ctx;
            zmq::socket_t *dealing, *broadcast, *persistence, *client_persistence;

            quantmodel::DealingEngineRequest dealing_request;
            quantmodel::DealingEngineRequest dealing_response;

            void audit(
                const std::string& direction,
                const FIX::Message& message,
                const FIX::SessionID& sessionID )
            {
                quantmodel::DatabaseEngineRequest database_request;

                std::string ts, account, avgpx, clordid, cumqty, execid, execrefid,
                            exectranstype, lastpx, lastshares, msgtype, orderid,
                            orderqty, ordstatus, ordtype, origclordid, price,
                            securityid, sendercompid, sendersubid, sendingtime,
                            shares, side, symbol, targetcompid, targetsubid,
                            text, timeinforce, transacttime, stoppx, exectype,
                            leavesqty, securitytype, maturitymonthyear, cticode,
                            securityexchange, correlationclordid, senderlocationid,
                            customerorfirm, manualorderindicator, maxshow;

                quantmodel::TimeStamp::getUTCTime( ts );

                if( message.isSetField( 1 ) ) account = message.getField( 1 );
                if( message.isSetField( 6 ) ) avgpx = message.getField( 6 );
                if( message.isSetField( 11 ) ) clordid = message.getField( 11 );
                if( message.isSetField( 14 ) ) cumqty = message.getField( 14 );
                if( message.isSetField( 17 ) ) execid = message.getField( 17 );
                if( message.isSetField( 19 ) ) execrefid = message.getField( 19 );
                if( message.isSetField( 20 ) ) exectranstype = message.getField( 20 );
                if( message.isSetField( 31 ) ) lastpx = message.getField( 31 );
                if( message.isSetField( 32 ) ) lastshares = message.getField( 32 );
                if( message.getHeader().isSetField( 35 ) ) msgtype = message.getHeader().getField( 35 );
                if( message.isSetField( 37 ) ) orderid = message.getField( 37 );
                if( message.isSetField( 38 ) ) orderqty = message.getField( 38 );
                if( message.isSetField( 39 ) ) ordstatus = message.getField( 39 );
                if( message.isSetField( 40 ) ) ordtype = message.getField( 40 );
                if( message.isSetField( 41 ) ) origclordid = message.getField( 41 );
                if( message.isSetField( 44 ) ) price = message.getField( 44 );
                if( message.isSetField( 48 ) ) securityid = message.getField( 48 );
                if( message.getHeader().isSetField( 49 ) ) sendercompid = message.getHeader().getField( 49 );
                if( message.getHeader().isSetField( 50 ) ) sendersubid = message.getHeader().getField( 50 );
                if( message.getHeader().isSetField( 52 ) ) sendingtime = message.getHeader().getField( 52 );
                if( message.isSetField( 53 ) ) shares = message.getField( 53 );
                if( message.isSetField( 54 ) ) side = message.getField( 54 );
                if( message.isSetField( 55 ) ) symbol = message.getField( 55 );
                if( message.getHeader().isSetField( 56 ) ) targetcompid = message.getHeader().getField( 56 );
                if( message.getHeader().isSetField( 57 ) ) targetsubid = message.getHeader().getField( 57 );
                if( message.isSetField( 58 ) ) text = message.getField( 58 );
                if( message.isSetField( 59 ) ) timeinforce = message.getField( 59 );
                if( message.isSetField( 99 ) ) stoppx = message.getField( 99 );
                if( message.getHeader().isSetField( 142 ) ) senderlocationid = message.getHeader().getField( 142 );
                if( message.isSetField( 150 ) ) exectype = message.getField( 150 );
                if( message.isSetField( 151 ) ) leavesqty = message.getField( 151 );
                if( message.isSetField( 167 ) ) securitytype = message.getField( 167 );
                if( message.isSetField( 200 ) ) maturitymonthyear = message.getField( 200 );
                if( message.isSetField( 204 ) ) customerorfirm = message.getField( 204 );
                if( message.isSetField( 207 ) ) securityexchange = message.getField( 207 );
                if( message.isSetField( 210 ) ) maxshow = message.getField( 210 );
                if( message.isSetField( 1028 ) ) manualorderindicator = message.getField( 1028 );
                if( message.isSetField( 9702 ) ) cticode = message.getField( 9702 );
                if( message.isSetField( 9717 ) ) correlationclordid = message.getField( 9717 );

                if( text.size() > 0 )
                {
                    text.replace( text.begin(), text.end(), '\'', ' ' );
                }

                quantmodel::QuantModelProcedures::INSERT_FIX_AUDIT_EVENT(
                    database_request,
                    sessionID.toString(),
                    direction,
                    account,
                    avgpx,
                    clordid,
                    cumqty,
                    execid,
                    execrefid,
                    exectranstype,
                    lastpx,
                    lastshares,
                    msgtype,
                    orderid,
                    orderqty,
                    ordstatus,
                    ordtype,
                    origclordid,
                    price,
                    securityid,
                    sendercompid,
                    sendersubid,
                    sendingtime,
                    shares,
                    side,
                    symbol,
                    targetcompid,
                    targetsubid,
                    text,
                    timeinforce,
                    transacttime,
                    stoppx,
                    senderlocationid,
                    exectype,
                    leavesqty,
                    securitytype,
                    maturitymonthyear,
                    customerorfirm,
                    securityexchange,
                    maxshow,
                    manualorderindicator,
                    cticode,
                    correlationclordid,
                    ts,
                    "FIX_ENGINE" );

                if( direction == "OUTGOING" )
                {
                    quantmodel::ZMQMessage::sendRPC( &database_request, client_persistence );
                }
                else
                {
                    quantmodel::ZMQMessage::sendRPC( &database_request, persistence );
                }
            }

        private:


    };
}

#endif // QUANTMODEL_FIXAPPLICATION_H
