#ifndef _OPCODE_SOFTWARE_INTERRUPT_ 
#define _OPCODE_SOFTWARE_INTERRUPT_ 

#include <iostream>
#include "arm_opcode.hpp"
#include "../../../utils/utils.hpp"

namespace ARM {

class SoftwareInterrupt : public ArmOpcode {
	public:
		SoftwareInterrupt(uint32_t op, ARM7TDMI &cpu);
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;
        
    private:
        uint32_t comment;
        const static uint32_t COMMENT_FIELD_MASK = 0b00000000111111111111111111111111;
        const static uint32_t COMMENT_FIELD_SHIFT = 0;
};

}

#endif