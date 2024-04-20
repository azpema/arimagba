#ifndef _OPCODE_MULTIPLY_ACCUMULATE_LONG_ 
#define _OPCODE_MULTIPLY_ACCUMULATE_LONG_ 

#include <iostream>
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"


class MultiplyAccumulateLong : public OpCode {
	public:
		MultiplyAccumulateLong(uint32_t op);
        std::string toString() override;
        void doExecute(ARM7TDMI &cpu);
        bool mustFlushPipeline() const override;
        uint32_t cyclesUsed() const override;
        
    private:
        uint16_t u, a, s, rdHi, rdLo, rs, rm;

        const static uint32_t U_FLAG_MASK = 0b00000000010000000000000000000000;
        const static uint32_t U_FLAG_SHIFT = 22;
        std::string uFlag2Mnemonic[2] = {"u", "s"};
        std::string getUFlagMnemonic();

        const static uint32_t A_FLAG_MASK = 0b00000000001000000000000000000000; 
        const static uint32_t A_FLAG_SHIFT = 21; 

        const static uint32_t S_FLAG_MASK = 0b00000000000100000000000000000000; 
        const static uint32_t S_FLAG_SHIFT = 20;
        std::string sFlag2Mnemonic[2] = {"", "s"};
        std::string getSFlagMnemonic();

        const static uint32_t RDHI_FLAG_MASK = 0b00000000000011110000000000000000; 
        const static uint32_t RDHI_FLAG_SHIFT = 16;

        const static uint32_t RDLO_FLAG_MASK = 0b00000000000000001111000000000000; 
        const static uint32_t RDLO_FLAG_SHIFT = 12;

        const static uint32_t RS_FLAG_MASK = 0b00000000000000000000111100000000; 
        const static uint32_t RS_FLAG_SHIFT = 8;

        const static uint32_t RM_FLAG_MASK = 0b00000000000000000000000000001111; 
        const static uint32_t RM_FLAG_SHIFT = 0;

        std::string op2Mnemonic[2] = {"mull", "mlal"};
        std::string getOpMnemonic();
};

#endif