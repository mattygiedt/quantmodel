#ifndef INVESTMENT_SYSTEM_H
#define INVESTMENT_SYSTEM_H

#include "CLLogic.hpp"

class InvestmentSystem
{
    public:

        InvestmentSystem()
        {
            logic = NULL;
        }

        ~InvestmentSystem()
        {
            delete logic;
        }

        void init( const std::string& filename )
        {
            const XMLNode config = XMLNode::openFileHelper( filename.c_str() );
            const XMLNode params = config.getChildNode( "params" );

            if( strcmp( "CL", params.getAttribute( "strategy_id" ) ) == 0 )
            {
                logic = new CLLogic( );
            }
            else
            {
                std::cerr << "UNKNOWN STRATEGY: " << params.getAttribute( "strategy_id" ) << std::endl;
                exit( 1 );
            }

            logic->init( config );
        }

        void handleMarketDataBroadcast(
            const QMAPI::MarketDataBroadcast& msg,
            std::vector< QMAPI::CreateTradeRequest >& vec )
        {
            logic->evaluate( msg, vec );
        }

        void confirmTrade( const std::string& ext_order_id )
        {
            logic->confirmTrade( ext_order_id );
        }

    private:

        TradeLogic* logic;

};

#endif // INVESTMENT_SYSTEM_H