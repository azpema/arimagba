#ifndef _THUMB_OPCODE_SP_LOAD_STORE_ 
#define _THUMB_OPCODE_SP_LOAD_STORE_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class SPLoadStore : public ThumbOpCode {
    private:
        uint16_t l, rd, word8, offsetVal;
        
        const static uint16_t L_MASK = 0b0000100000000000; 
        const static uint16_t L_SHIFT = 11;

        const static uint16_t RD_MASK = 0b0000011100000000; 
        const static uint16_t RD_SHIFT = 8;

        const static uint16_t WORD8_MASK = 0b0000000011111111; 
        const static uint16_t WORD8_SHIFT = 0;

        std::string l2Mnemonic[2] = {"str", "ldr"};
        std::string getLMnemonic();
	public:
		SPLoadStore(uint16_t op);
        std::string toString();
};

#endif