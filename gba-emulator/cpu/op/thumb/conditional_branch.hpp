#ifndef _THUMB_OPCODE_CONDITIONAL_BRANCH_ 
#define _THUMB_OPCODE_CONDITIONAL_BRANCH_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"
#include <vector>

namespace Thumb {

class ConditionalBranch : public ThumbOpCode {
    private:
        uint8_t cond, sOffset8;
        int16_t offsetVal;

        const static uint16_t COND_MASK = 0b0000111100000000; 
        const static uint16_t COND_SHIFT = 8;

        const static uint16_t SOFFSET8_MASK = 0b0000000011111111; 
        const static uint16_t SOFFSET8_SHIFT = 0;

	public:
		ConditionalBranch(uint16_t op, uint32_t pc);
        std::string toString();
};

}

#endif