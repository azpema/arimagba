#ifndef _OPCODE_UNDEFINED_ 
#define _OPCODE_UNDEFINED_ 

#include <iostream>
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class Undefined : public OpCode {
	public:
		Undefined(uint32_t op);
        std::string toString() override;
		void doExecute(ARM7TDMI &cpu);
		bool mustFlushPipeline() const override;
		uint32_t cyclesUsed() const override;
};

#endif