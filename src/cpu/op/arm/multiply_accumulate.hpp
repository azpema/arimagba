#ifndef _OPCODE_MULTIPLY_ACCUMULATE_ 
#define _OPCODE_MULTIPLY_ACCUMULATE_ 

#include <iostream>
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"


class MultiplyAccumulate : public OpCode {
	public:
		MultiplyAccumulate(uint32_t op, ARM7TDMI &cpu);
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t a, s, rd, rn, rs, rm;
        const static uint32_t A_FLAG_MASK = 0b00000000001000000000000000000000; 
        const static uint32_t A_FLAG_SHIFT = 21; 

        const static uint32_t S_FLAG_MASK = 0b00000000000100000000000000000000; 
        const static uint32_t S_FLAG_SHIFT = 20;

        const static uint32_t RD_FLAG_MASK = 0b00000000000011110000000000000000; 
        const static uint32_t RD_FLAG_SHIFT = 16;

        const static uint32_t RN_FLAG_MASK = 0b00000000000000001111000000000000; 
        const static uint32_t RN_FLAG_SHIFT = 12;

        const static uint32_t RS_FLAG_MASK = 0b00000000000000000000111100000000; 
        const static uint32_t RS_FLAG_SHIFT = 8;

        const static uint32_t RM_FLAG_MASK = 0b00000000000000000000000000001111; 
        const static uint32_t RM_FLAG_SHIFT = 0;

        std::string sFlag2Mnemonic[2] = {"", "s"};
        std::string getSFlagMnemonic();

        std::string op2Mnemonic[2] = {"mul", "mla"};
        std::string getOpMnemonic();
};

#endif