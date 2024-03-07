#ifndef _THUMB_OPCODE_ADD_OFFSET_SP_ 
#define _THUMB_OPCODE_ADD_OFFSET_SP_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class AddOffsetSP : public ThumbOpCode {
    private:
        uint16_t s, sWord7, offsetStr;
        int16_t offsetVal;
        
        const static uint16_t S_MASK = 0b0000000010000000; 
        const static uint16_t S_SHIFT = 7;

        const static uint16_t SWORD7_MASK = 0b0000000001111111; 
        const static uint16_t SWORD7_SHIFT = 0;

        std::string s2Mnemonic[2] = {"", "-"};
        std::string getSMnemonic();
	public:
		AddOffsetSP(uint16_t op);
        std::string toString();

};

#endif