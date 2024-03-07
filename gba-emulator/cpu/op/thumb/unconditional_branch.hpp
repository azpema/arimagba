#ifndef _THUMB_OPCODE_UNCONDITIONAL_BRANCH_ 
#define _THUMB_OPCODE_UNCONDITIONAL_BRANCH_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"
#include <vector>

namespace Thumb {

class UnconditionalBranch : public ThumbOpCode {
    private:
        uint16_t offset11;
        int16_t offsetVal;

        const static uint16_t OFFSET11_MASK = 0b0000011111111111; 
        const static uint16_t OFFSET11_SHIFT = 0;

	public:
		UnconditionalBranch(uint16_t op, uint32_t pc);
        std::string toString();
};

}

#endif