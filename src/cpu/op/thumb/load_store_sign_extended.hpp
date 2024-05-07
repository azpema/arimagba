#ifndef _THUMB_OPCODE_LOAD_STORE_SIGN_EXTENDED_ 
#define _THUMB_OPCODE_LOAD_STORE_SIGN_EXTENDED_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class LoadStoreSignExtended : public ThumbOpCode {
    private:
        uint8_t h, s, ro, rb, rd;

        const static uint16_t H_MASK = 0b0000100000000000; 
        const static uint16_t H_SHIFT = 11;

        const static uint16_t S_MASK = 0b0000010000000000; 
        const static uint16_t S_SHIFT = 10;

        const static uint16_t RO_MASK = 0b0000000111000000; 
        const static uint16_t RO_SHIFT = 6;

        const static uint16_t RB_MASK = 0b0000000000111000; 
        const static uint16_t RB_SHIFT = 3;

        const static uint16_t RD_MASK = 0b0000000000000111; 
        const static uint16_t RD_SHIFT = 0;

        std::string op2Mnemonic[2][2] = {{"strh", "ldrh"},
                                         {"ldsb", "ldsh"}};
        std::string getOpMnemonic();

	public:
		LoadStoreSignExtended(uint16_t op);
        std::string toString();
};

#endif