#ifndef _OPCODE_FIELD_IMM_
#define _OPCODE_FIELD_IMM_

#include <iostream>
#include "operand.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class Imm : public Operand {

    private:
        uint16_t immVal;

	public:
		Imm(uint16_t val);
        std::string toString();
        uint16_t getVal();
        uint16_t getImmVal();
};

#endif