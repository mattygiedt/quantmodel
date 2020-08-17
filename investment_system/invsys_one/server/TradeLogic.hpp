#ifndef TRADE_LOGIC_H
#define TRADE_LOGIC_H

class TradeLogic
{
    public:

        virtual ~TradeLogic() {}

        virtual void open( System& ) = 0;
        virtual void close( System&, std::vector< TradeInstruction* >& ) = 0;
        virtual bool evaluate(  System&, std::vector< TradeInstruction* >& ) = 0;

        inline double getMultiplier()
        {
            return multiplier;
        }

        void addExecution( const unsigned int quantity, const double& price, const SIDE_CODE side_code )
        {
            if( open_buy_qty == 0 && open_sell_qty == 0 )
            {
                if( side_code == SIDE_CODE_BUY )
                {
                    open_buy_qty = quantity;
                    open_buy_val = quantity * price;
                    open_buy_avg = price;
                }
                else
                {
                    open_sell_qty = quantity;
                    open_sell_val = quantity * price;
                    open_sell_avg = price;
                }
            }
            else if( open_buy_qty > 0 )
            {
                if( side_code == SIDE_CODE_BUY )
                {
                    open_buy_qty += quantity;
                    open_buy_val += quantity * price;
                    open_buy_avg = open_buy_val / double( open_buy_qty );
                }
                else
                {
                    if( open_buy_qty == quantity )
                    {
                        closed_qty += quantity * 2;
                        closed_pl += ( ( quantity * price ) - ( open_buy_val ) ) * multiplier;
                        open_buy_qty = 0;
                        open_buy_val = open_buy_avg = 0.0;
                    }
                    else if( open_buy_qty > quantity )
                    {
                        closed_qty += quantity * 2;
                        closed_pl += ( ( quantity * price ) - ( quantity * open_buy_avg ) ) * multiplier;
                        open_buy_qty -= quantity;
                        open_buy_val = open_buy_avg * open_buy_qty;
                    }
                    else
                    {
                        open_sell_qty = quantity - open_buy_qty;
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
                    open_sell_qty += quantity;
                    open_sell_val += quantity * price;
                    open_sell_avg = open_sell_val / double( open_sell_qty );
                }
                else
                {
                    if( open_sell_qty == quantity )
                    {
                        closed_qty += quantity * 2;
                        closed_pl += ( ( open_sell_val ) - ( quantity * price ) ) * multiplier;
                        open_sell_qty = 0;
                        open_sell_val = open_sell_avg = 0.0;
                    }
                    else if( open_sell_qty > quantity )
                    {
                        closed_qty += quantity * 2;
                        closed_pl += ( ( quantity * open_sell_avg ) - ( quantity * price ) ) * multiplier;
                        open_sell_qty -= quantity;
                        open_sell_val = open_sell_avg * open_sell_qty;
                    }
                    else
                    {
                        open_buy_qty = quantity - open_sell_qty;
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

        inline unsigned int getClosedQty()
        {
            return closed_qty;
        }

        inline double getClosedPL()
        {
            return closed_pl;
        }

        double getOpenPL( const double& ask, const double& bid )
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

        //double roundUp( const double& price )
        //{
        //    double price_floor = floor( price );
        //    while( price_floor < price )
        //    {
        //        price_floor += tick_size;
        //    }
        //
        //    return price_floor;
        //}
        //
        //double roundDown( const double& price )
        //{
        //    double price_ceil = ceil( price );
        //
        //    while( price_ceil > price )
        //    {
        //        price_ceil -= tick_size;
        //    }
        //
        //    return price_ceil;
        //}

    protected:

        virtual void addExecutionInternal( ) = 0;

        void init()
        {
            is_open = false;
            open_buy_qty = open_sell_qty = closed_qty = 0;
            open_buy_val = open_sell_val =
            open_buy_avg = open_sell_avg =
            best_bid = best_ask = closed_pl = 0.0;
        }

        unsigned int open_buy_qty, open_sell_qty, closed_qty;
        double open_buy_val, open_sell_val, open_buy_avg, open_sell_avg, closed_pl;

        double multiplier, best_bid, best_ask;

        bool is_open;
};

#endif // TRADE_LOGIC_H
