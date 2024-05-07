#ifndef _THUMB_OPCODE_LOAD_ADDRESS_ 
#define _THUMB_OPCODE_LOAD_ADDRESS_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class LoadAddress : public ThumbOpCode {
    private:
        uint8_t sp, rd, word8;
        uint16_t offsetVal;

        const static uint16_t SP_MASK = 0b0000100000000000; 
        const static uint16_t SP_SHIFT = 11;

        const static uint16_t RD_MASK = 0b0000011100000000; 
        const static uint16_t RD_SHIFT = 8;

        const static uint16_t WORD8_MASK = 0b0000000011111111; 
        const static uint16_t WORD8_SHIFT = 0;

        uint8_t sp2RegVal[2] = {15, 13};
        uint8_t getSPRegVal();
	public:
		LoadAddress(uint16_t op);
        std::string toString();
};

#endif