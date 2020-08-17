#ifndef TRADE_INSTRUCTION_H
#define TRADE_INSTRUCTION_H

enum TRADE_INSTRUCTION_SIDE
{
    TRADE_INSTRUCTION_SIDE_LONG,
    TRADE_INSTRUCTION_SIDE_SHORT
};

enum TRADE_INSTRUCTION_TYPE
{
    TRADE_INSTRUCTION_TYPE_MARKET,
    TRADE_INSTRUCTION_TYPE_LIMIT,
    TRADE_INSTRUCTION_TYPE_STOP,
    TRADE_INSTRUCTION_TYPE_STOP_LIMIT,
};

struct TradeInstruction
{
    TRADE_INSTRUCTION_TYPE type;
    TRADE_INSTRUCTION_SIDE side;
    unsigned int quantity;
    double price;

    TradeInstruction( TRADE_INSTRUCTION_SIDE instr_side, TRADE_INSTRUCTION_TYPE instr_type, unsigned int qty, double prc = 0 )
    {
        side = instr_side;
        type = instr_type;
        quantity = qty;
        price = prc;
    }
};

#endif // TRADE_INSTRUCTION_H
