#ifndef _OPCODE_BRANCH_EXCHANGE_
#define _OPCODE_BRANCH_EXCHANGE_

#include <iostream>
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"



class BranchAndExchange : public OpCode {
    public:
        BranchAndExchange(uint32_t op);
        std::string toString();
        void doExecute(ARM7TDMI &cpu);

    private:
        uint32_t Rn;
        const static uint32_t RN_MASK = 0b00000000000000000000000000001111; 
        const static uint32_t RN_SHIFT = 0; 
};

#endif