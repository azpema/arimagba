#ifndef _OPCODE_SOFTWARE_INTERRUPT_ 
#define _OPCODE_SOFTWARE_INTERRUPT_ 

#include "arm_opcode.hpp"

namespace ARM {

class SoftwareInterrupt : public ArmOpcode {
	public:
		SoftwareInterrupt(uint32_t op, ARM7TDMI &cpu);
        SoftwareInterrupt(ARM7TDMI &cpu);
        void init(uint32_t op) override;
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