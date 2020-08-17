#ifndef ORDER_EXECUTION_H
#define ORDER_EXECUTION_H

struct OrderExecution;

enum SIDE_CODE
{
    SIDE_CODE_BUY,
    SIDE_CODE_SELL
};

struct OrderExecution
{
    time_t timestamp;
    time_t timebar;
    double price;
    unsigned int quantity;
    SIDE_CODE side_code;
    std::string order_id;

    OrderExecution( const time_t& ts,
                    const time_t& tb,
                    const double& p,
                    const unsigned int q,
                    const SIDE_CODE s,
                    const std::string o = "" )
    {
        timestamp = ts;
        timebar = tb;
        price = p;
        quantity = q;
        side_code = s;
        order_id = o;
    }
};

#endif // ORDER_EXECUTION_H