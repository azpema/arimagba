#ifndef _THUMB_OPCODE_ADD_SUBTRACT_ 
#define _THUMB_OPCODE_ADD_SUBTRACT_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class AddSubtract : public ThumbOpCode {
    public:
		AddSubtract(uint16_t op, ARM7TDMI &cpu);
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t i, opField, rnOffset3, rs, rd;
        
        const static uint16_t I_MASK = 0b0000010000000000; 
        const static uint16_t I_SHIFT = 10;

        const static uint16_t OP_MASK = 0b0000001000000000;
        const static uint16_t OP_SHIFT = 9;

        const static uint16_t RN_OFFSET3_MASK = 0b0000000111000000;
        const static uint16_t RN_OFFSET3_SHIFT = 6;

        const static uint16_t RS_MASK = 0b0000000000111000;
        const static uint16_t RS_SHIFT = 3;

        const static uint16_t RD_MASK = 0b0000000000000111;
        const static uint16_t RD_SHIFT = 0;

        const static std::string op2Mnemonic[2];
        std::string getOpMnemonic();
};

#endif