#ifndef _OPCODE_UNDEFINED_ 
#define _OPCODE_UNDEFINED_ 

#include <iostream>
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class Undefined : public OpCode {
	public:
		Undefined(uint32_t op, ARM7TDMI &cpu);
        std::string toString() override;
		void doExecute() override;
		void doDecode() override;
		uint32_t cyclesUsed() const override;
};

#endif