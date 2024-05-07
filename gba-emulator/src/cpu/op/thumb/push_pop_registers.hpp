#ifndef _THUMB_OPCODE_PUSH_POP_REGISTERS_ 
#define _THUMB_OPCODE_PUSH_POP_REGISTERS_ 

#include <iostream>
#include <vector>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class PushPopRegisters : public ThumbOpCode {
    private:
        uint16_t l, r, rList;
        std::vector<uint16_t> registerListVec;
        
        const static uint16_t L_MASK = 0b0000100000000000; 
        const static uint16_t L_SHIFT = 11;

        const static uint16_t R_MASK = 0b0000000100000000;
        const static uint16_t R_SHIFT = 8;

        const static uint16_t RLIST_MASK = 0b0000000011111111;
        const static uint16_t RLIST_SHIFT = 0;

        std::string op2Mnemonic[2] = {"push", "pop"};
        std::string getOpMnemonic();

	public:
		PushPopRegisters(uint16_t op);
        std::string toString();

};

#endif