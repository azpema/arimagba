#ifndef _THUMB_OPCODE_MOVE_COMP_ADD_SUB_IMM_ 
#define _THUMB_OPCODE_MOVE_COMP_ADD_SUB_IMM_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class MoveCompAddSubImm : public ThumbOpCode {
	public:
		MoveCompAddSubImm(uint16_t op, ARM7TDMI &cpu);
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t opField, rd, offset8;

        const static uint16_t OP_MASK = 0b0001100000000000; 
        const static uint16_t OP_SHIFT = 11; 

        const static uint16_t RD_MASK = 0b0000011100000000; 
        const static uint16_t RD_SHIFT = 8;

        const static uint16_t OFFSET8_MASK = 0b0000000011111111; 
        const static uint16_t OFFSET8_SHIFT = 0; 

        std::string op2Mnemonic[4] = {"mov", "cmp", "add", "sub"};
        std::string getOpMnemonic();
};

#endif