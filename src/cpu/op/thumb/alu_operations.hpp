#ifndef _THUMB_OPCODE_ALU_OPERATIONS_
#define _THUMB_OPCODE_ALU_OPERATIONS_

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class ALUOperations : public ThumbOpCode {
	public:
        ALUOperations(uint16_t op, ARM7TDMI &cpu);
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t opField, rs, rd;

        const static uint16_t OP_MASK = 0b0000001111000000;
        const static uint16_t OP_SHIFT = 6;

        const static uint16_t RS_MASK = 0b0000000000111000;
        const static uint16_t RS_SHIFT = 3;

        const static uint16_t RD_MASK = 0b0000000000000111;
        const static uint16_t RD_SHIFT = 0;

        std::string op2Mnemonic[16] = {"and", "eor", "lsl", "lsr",
                                       "asr", "adc", "sbc", "ror",
                                       "tst", "neg", "cmp", "cmn",
                                       "orr", "mul", "bic", "mvn"};
        std::string getOpMnemonic();

};

#endif