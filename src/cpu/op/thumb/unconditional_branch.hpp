#ifndef _THUMB_OPCODE_UNCONDITIONAL_BRANCH_ 
#define _THUMB_OPCODE_UNCONDITIONAL_BRANCH_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"
#include <vector>

namespace Thumb {

class UnconditionalBranch : public ThumbOpCode {
    public:
		UnconditionalBranch(uint16_t op, uint32_t pc, ARM7TDMI &cpu);
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t offset11;
        int32_t offsetVal;
        uint32_t oldPC;

        const static uint16_t OFFSET11_MASK = 0b0000011111111111; 
        const static uint16_t OFFSET11_SHIFT = 0;
};

}

#endif