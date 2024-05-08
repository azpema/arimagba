#ifndef _THUMB_OPCODE_CONDITIONAL_BRANCH_ 
#define _THUMB_OPCODE_CONDITIONAL_BRANCH_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"
#include <vector>

namespace Thumb {

class ConditionalBranch : public ThumbOpCode {
    public:
		ConditionalBranch(uint16_t op, uint32_t pc, ARM7TDMI &cpu);
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint8_t cond, sOffset8;
        int16_t offsetVal;

        const static uint16_t COND_MASK = 0b0000111100000000; 
        const static uint16_t COND_SHIFT = 8;
        std::string cond2Mnemonic[16] = {"eq", "ne", "cs", "cc",
                                         "mi", "pl", "vs", "vc",
                                         "hi", "ls", "ge", "lt",
                                         "gt", "le", "ERR", "ERR"};
        std::string getCondMnemonic();

        const static uint16_t SOFFSET8_MASK = 0b0000000011111111; 
        const static uint16_t SOFFSET8_SHIFT = 0;
};

}

#endif