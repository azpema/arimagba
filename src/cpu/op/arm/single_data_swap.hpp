#ifndef _OPCODE_SINGLE_DATA_SWAP_
#define _OPCODE_SINGLE_DATA_SWAP_

#include <iostream>
#include "../opcode.hpp"

class SingleDataSwap : public OpCode {
    public:
		SingleDataSwap(uint32_t op, ARM7TDMI &cpu);
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t b, rn, rd, rm;

        const static uint32_t B_MASK = 0b00000000010000000000000000000000;
        const static uint32_t B_SHIFT = 22;
        std::string bFlag2Mnemonic[2] = {"", "b"};
        std::string getBFlagMnemonic(); 

        const static uint32_t RN_MASK = 0b00000000000011110000000000000000; 
        const static uint32_t RN_SHIFT = 16;

        const static uint32_t RD_MASK = 0b00000000000000001111000000000000; 
        const static uint32_t RD_SHIFT = 12;

        const static uint32_t RM_MASK = 0b00000000000000000000000000001111; 
        const static uint32_t RM_SHIFT = 0;
};

#endif