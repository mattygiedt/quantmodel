#ifndef NOP_LOGIC_H
#define NOP_LOGIC_H

class NOPLogic : public TradeLogic
{
    public:

        NOPLogic( const System& system ) { init(); }

        void addExecutionInternal( ) { }

        void open( System& system ) { }

        void close( System& system, std::vector< TradeInstruction* >& trade_instruction_vec ) { }

        bool evaluate( System& system, std::vector< TradeInstruction* >& trade_instruction_vec ) { return false; }


    private:

};

#endif // NOP_LOGIC_H
