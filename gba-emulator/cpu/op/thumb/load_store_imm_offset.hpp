#ifndef _THUMB_OPCODE_LOAD_STORE_IMM_OFFSET_ 
#define _THUMB_OPCODE_LOAD_STORE_IMM_OFFSET_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class LoadStoreImmOffset : public ThumbOpCode {
    private:
        uint8_t b, l, offset5, rb, rd, offsetVal;

        const static uint16_t B_MASK = 0b0001000000000000; 
        const static uint16_t B_SHIFT = 12;

        std::string b2Mnemonic[2] = {"", "b"};
        std::string getBMnemonic();

        const static uint16_t L_MASK = 0b0000100000000000; 
        const static uint16_t L_SHIFT = 11;

        const static uint16_t OFFSET5_MASK = 0b0000011111000000; 
        const static uint16_t OFFSET5_SHIFT = 6;

        const static uint16_t RB_MASK = 0b0000000000111000; 
        const static uint16_t RB_SHIFT = 3;

        const static uint16_t RD_MASK = 0b0000000000000111; 
        const static uint16_t RD_SHIFT = 0;

        std::string op2Mnemonic[2] = {"str", "ldr"};
        std::string getOpMnemonic();

	public:
		LoadStoreImmOffset(uint16_t op);
        std::string toString();
};

#endif