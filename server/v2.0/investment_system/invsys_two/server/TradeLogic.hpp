#ifndef TRADE_LOGIC_H
#define TRADE_LOGIC_H

class TradeLogic
{
    public:

        virtual ~TradeLogic() {}
        virtual void confirmTrade( const std::string& ) = 0;
        virtual void init( const XMLNode& ) = 0;
        virtual void evaluate( const QMAPI::MarketDataBroadcast& ,
                               std::vector< QMAPI::CreateTradeRequest >& ) = 0;

    protected:

        time_t last_market_tm;
        double multiplier;
        LimitOrderBook order_book;

        unsigned int open_buy_qty, open_sell_qty, closed_qty;
        double open_buy_val, open_sell_val, open_buy_avg, open_sell_avg, closed_pl;

        std::string opening_tm;
        std::string closing_tm;
        std::string shutdown_tm;
        std::string strategy_id;
        std::string investment_system_id;

        void addExecution( QMAPI::CreateTradeRequest& trade, const long long int& price )
        {
            if( open_buy_qty == 0 && open_sell_qty == 0 )
            {
                if( trade.side_code() == QMAPI::CreateTradeRequest::BUY )
                {
                    open_buy_qty = trade.signal_size();
                    open_buy_val = trade.signal_size() * price;
                    open_buy_avg = price;
                }
                else
                {
                    open_sell_qty = trade.signal_size();
                    open_sell_val = trade.signal_size() * price;
                    open_sell_avg = price;
                }
            }
            else if( open_buy_qty > 0 )
            {
                if( trade.side_code() == QMAPI::CreateTradeRequest::BUY )
                {
                    open_buy_qty += trade.signal_size();
                    open_buy_val += trade.signal_size() * price;
                    open_buy_avg = open_buy_val / double( open_buy_qty );
                }
                else
                {
                    if( open_buy_qty == trade.signal_size() )
                    {
                        closed_qty += trade.signal_size() * 2;
                        closed_pl += ( ( trade.signal_size() * price ) - ( open_buy_val ) ) * multiplier;
                        open_buy_qty = 0;
                        open_buy_val = open_buy_avg = 0.0;
                    }
                    else if( open_buy_qty > trade.signal_size() )
                    {
                        closed_qty += trade.signal_size() * 2;
                        closed_pl += ( ( trade.signal_size() * price ) - ( trade.signal_size() * open_buy_avg ) ) * multiplier;
                        open_buy_qty -= trade.signal_size();
                        open_buy_val = open_buy_avg * open_buy_qty;
                    }
                    else
                    {
                        open_sell_qty = trade.signal_size() - open_buy_qty;
                        open_sell_val = open_sell_qty * price;
                        open_sell_avg = price;

                        closed_qty += open_buy_qty * 2;
                        closed_pl += ( ( open_buy_qty * price ) - ( open_buy_val ) ) * multiplier;
                        open_buy_qty = 0;
                        open_buy_val = open_buy_avg = 0.0;
                    }
                }
            }
            else if( open_sell_qty > 0 )
            {
                if( trade.side_code() == QMAPI::CreateTradeRequest::SELL )
                {
                    open_sell_qty += trade.signal_size();
                    open_sell_val += trade.signal_size() * price;
                    open_sell_avg = open_sell_val / double( open_sell_qty );
                }
                else
                {
                    if( open_sell_qty == trade.signal_size() )
                    {
                        closed_qty += trade.signal_size() * 2;
                        closed_pl += ( ( open_sell_val ) - ( trade.signal_size() * price ) ) * multiplier;
                        open_sell_qty = 0;
                        open_sell_val = open_sell_avg = 0.0;
                    }
                    else if( open_sell_qty > trade.signal_size() )
                    {
                        closed_qty += trade.signal_size() * 2;
                        closed_pl += ( ( trade.signal_size() * open_sell_avg ) - ( trade.signal_size() * price ) ) * multiplier;
                        open_sell_qty -= trade.signal_size();
                        open_sell_val = open_sell_avg * open_sell_qty;
                    }
                    else
                    {
                        open_buy_qty = trade.signal_size() - open_sell_qty;
                        open_buy_val = open_buy_qty * price;
                        open_buy_avg = price;

                        closed_qty += open_sell_qty * 2;
                        closed_pl += ( ( open_sell_val ) - ( open_sell_qty * price ) ) * multiplier;
                        open_sell_qty = 0;
                        open_sell_val = open_sell_avg = 0.0;
                    }
                }
            }
        }

        double getOpenPL( const long long int& ask, const long long int& bid ) const
        {
            if( open_buy_qty > 0 )
            {
                return ( ( open_buy_qty * bid ) - ( open_buy_val ) ) * multiplier;
            }
            else if( open_sell_qty > 0 )
            {
                return ( ( open_sell_val ) - ( open_sell_qty * ask ) ) * multiplier;
            }
            else
            {
                return 0.0;
            }
        }

    private:

/*
        void addExecution( const unsigned int trade.signal_size(), const double& price, const SIDE_CODE side_code )
        {
            if( open_buy_qty == 0 && open_sell_qty == 0 )
            {
                if( side_code == SIDE_CODE_BUY )
                {
                    open_buy_qty = trade.signal_size();
                    open_buy_val = trade.signal_size() * price;
                    open_buy_avg = price;
                }
                else
                {
                    open_sell_qty = trade.signal_size();
                    open_sell_val = trade.signal_size() * price;
                    open_sell_avg = price;
                }
            }
            else if( open_buy_qty > 0 )
            {
                if( side_code == SIDE_CODE_BUY )
                {
                    open_buy_qty += trade.signal_size();
                    open_buy_val += trade.signal_size() * price;
                    open_buy_avg = open_buy_val / double( open_buy_qty );
                }
                else
                {
                    if( open_buy_qty == trade.signal_size() )
                    {
                        closed_qty += trade.signal_size() * 2;
                        closed_pl += ( ( trade.signal_size() * price ) - ( open_buy_val ) ) * multiplier;
                        open_buy_qty = 0;
                        open_buy_val = open_buy_avg = 0.0;
                    }
                    else if( open_buy_qty > trade.signal_size() )
                    {
                        closed_qty += trade.signal_size() * 2;
                        closed_pl += ( ( trade.signal_size() * price ) - ( trade.signal_size() * open_buy_avg ) ) * multiplier;
                        open_buy_qty -= trade.signal_size();
                        open_buy_val = open_buy_avg * open_buy_qty;
                    }
                    else
                    {
                        open_sell_qty = trade.signal_size() - open_buy_qty;
                        open_sell_val = open_sell_qty * price;
                        open_sell_avg = price;

                        closed_qty += open_buy_qty * 2;
                        closed_pl += ( ( open_buy_qty * price ) - ( open_buy_val ) ) * multiplier;
                        open_buy_qty = 0;
                        open_buy_val = open_buy_avg = 0.0;
                    }
                }
            }
            else if( open_sell_qty > 0 )
            {
                if( side_code == SIDE_CODE_SELL )
                {
                    open_sell_qty += trade.signal_size();
                    open_sell_val += trade.signal_size() * price;
                    open_sell_avg = open_sell_val / double( open_sell_qty );
                }
                else
                {
                    if( open_sell_qty == trade.signal_size() )
                    {
                        closed_qty += trade.signal_size() * 2;
                        closed_pl += ( ( open_sell_val ) - ( trade.signal_size() * price ) ) * multiplier;
                        open_sell_qty = 0;
                        open_sell_val = open_sell_avg = 0.0;
                    }
                    else if( open_sell_qty > trade.signal_size() )
                    {
                        closed_qty += trade.signal_size() * 2;
                        closed_pl += ( ( trade.signal_size() * open_sell_avg ) - ( trade.signal_size() * price ) ) * multiplier;
                        open_sell_qty -= trade.signal_size();
                        open_sell_val = open_sell_avg * open_sell_qty;
                    }
                    else
                    {
                        open_buy_qty = trade.signal_size() - open_sell_qty;
                        open_buy_val = open_buy_qty * price;
                        open_buy_avg = price;

                        closed_qty += open_sell_qty * 2;
                        closed_pl += ( ( open_sell_val ) - ( open_sell_qty * price ) ) * multiplier;
                        open_sell_qty = 0;
                        open_sell_val = open_sell_avg = 0.0;
                    }
                }
            }

            addExecutionInternal(  );
        }

        inline unsigned int getClosedQty() const
        {
            return closed_qty;
        }

        inline unsigned int getOpenQty() const
        {
            return open_buy_qty + open_sell_qty;
        }

        inline double getMultiplier() const
        {
            return multiplier;
        }

        inline double getClosedPL() const
        {
            return closed_pl;
        }

        double getOpenPL( const double& ask, const double& bid ) const
        {
            if( open_buy_qty == 0 && open_sell_qty == 0 )
            {
                return 0.0;
            }
            else if( open_buy_qty > 0 )
            {
                return ( ( open_buy_qty * bid ) - ( open_buy_val ) ) * multiplier;
            }
            else if( open_sell_qty > 0 )
            {
                return ( ( open_sell_val ) - ( open_sell_qty * ask ) ) * multiplier;
            }
            else
            {
                //
                //  ERROR!!!
                //

                return 0.0;
            }
        }

        double getSystemPL( const double& ask, const double& bid )
        {
            return getOpenPL( ask, bid ) + closed_pl;
        }

    protected:

        virtual void addExecutionInternal( ) = 0;

        void init()
        {
            is_open = false;
            open_buy_qty = open_sell_qty = closed_qty = 0;
            open_buy_val = open_sell_val =
            open_buy_avg = open_sell_avg =
            best_bid = best_ask = closed_pl =
            vwap_1 = vwap_2 = vwap_3 = vwap_7 = vwap_13 = 0.0;
        }

        unsigned int open_buy_qty, open_sell_qty, closed_qty;
        double open_buy_val, open_sell_val, open_buy_avg, open_sell_avg, closed_pl;

        double multiplier, best_bid, best_ask, vwap_1, vwap_2, vwap_3, vwap_7, vwap_13;

        bool is_open;
    */
};

#endif // TRADE_LOGIC_H
