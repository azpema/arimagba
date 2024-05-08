#ifndef _THUMB_OPCODE_LONG_BRANCH_WITH_LINK_ 
#define _THUMB_OPCODE_LONG_BRANCH_WITH_LINK_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"
#include <vector>

namespace Thumb {

class LongBranchWithLink : public ThumbOpCode {
	public:
		LongBranchWithLink(uint16_t op, uint32_t pc, ARM7TDMI &cpu);
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;
        
    private:
        uint8_t h;
        uint16_t offset;
        uint32_t offsetVal;

        const static uint16_t H_MASK = 0b0000100000000000; 
        const static uint16_t H_SHIFT = 11;

        const static uint16_t OFFSET_MASK = 0b0000011111111111; 
        const static uint16_t OFFSET_SHIFT = 0;
};

}

#endif