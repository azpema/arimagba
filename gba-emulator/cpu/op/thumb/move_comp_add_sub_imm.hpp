#ifndef _THUMB_OPCODE_MOVE_COMP_ADD_SUB_IMM_ 
#define _THUMB_OPCODE_MOVE_COMP_ADD_SUB_IMM_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

/*
* Branch instructions contain a signed 2’s complement 24 bit offset. This is shifted left
* two bits, sign extended to 32 bits, and added to the PC. The instruction can therefore
* specify a branch of +/- 32Mbytes. The branch offset must take account of the prefetch
* operation, which causes the PC to be 2 words (8 bytes) ahead of the current instruction
*/

class MoveCompAddSubImm : public ThumbOpCode {
    public:
        uint16_t opField, rd, offset8;

    private:
        const static uint32_t OP_MASK = 0b0001100000000000; 
        const static uint32_t OP_SHIFT = 11; 

        const static uint32_t RD_MASK = 0b0000011100000000; 
        const static uint32_t RD_SHIFT = 8;

        const static uint32_t OFFSET8_MASK = 0b0000000011111111; 
        const static uint32_t OFFSET8_SHIFT = 0; 

        std::string op2Mnemonic[4] = {"mov", "cmp", "add", "sub"};
        std::string getOpMnemonic();

	public:
		MoveCompAddSubImm(uint16_t op);
        std::string toString();

};

#endif