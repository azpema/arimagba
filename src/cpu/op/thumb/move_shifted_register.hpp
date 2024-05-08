#ifndef _THUMB_OPCODE_MOVE_SHIFTED_REGISTER_ 
#define _THUMB_OPCODE_MOVE_SHIFTED_REGISTER_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class MoveShiftedRegister : public ThumbOpCode {
	public:
		MoveShiftedRegister(uint16_t op, ARM7TDMI &cpu);
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t opField, offset5, rs, rd;

        const static uint16_t OP_MASK = 0b0001100000000000; 
        const static uint16_t OP_SHIFT = 11;

        const static uint16_t OFFSET5_MASK = 0b0000011111000000; 
        const static uint16_t OFFSET5_SHIFT = 6;

        const static uint16_t RS_MASK = 0b0000000000111000; 
        const static uint16_t RS_SHIFT = 3;

        const static uint16_t RD_MASK = 0b0000000000000111; 
        const static uint16_t RD_SHIFT = 0; 

        std::string op2Mnemonic[3] = {"lsl", "lsr", "asr"};
        std::string getOpMnemonic();
};

#endif