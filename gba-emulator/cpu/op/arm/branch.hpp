#ifndef _OPCODE_BRANCH_ 
#define _OPCODE_BRANCH_ 

#include <iostream>
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

/*
* Branch instructions contain a signed 2’s complement 24 bit offset. This is shifted left
* two bits, sign extended to 32 bits, and added to the PC. The instruction can therefore
* specify a branch of +/- 32Mbytes. The branch offset must take account of the prefetch
* operation, which causes the PC to be 2 words (8 bytes) ahead of the current instruction
*/

namespace ARM{

class Branch : public OpCode {
    public:
        uint16_t L;
        uint32_t offsetField;
        int32_t realOffset;

    private:
        const static uint32_t LINK_MASK = 0b00000001000000000000000000000000; 
        const static uint32_t LINK_SHIFT = 24; 

        const static uint32_t OFFSET_MASK = 0b00000000111111111111111111111111; 
        const static uint32_t OFFSET_SHIFT = 0; 

        std::string linkFlag2Mnemonic[2] = {"", "l"};
        std::string getLinkFlagMnemonic();

	public:
		Branch(uint32_t op, uint32_t pc);
        std::string toString();

};

}

#endif