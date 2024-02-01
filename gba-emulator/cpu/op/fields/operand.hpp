#ifndef _OPCODE_FIELD_OPERAND_
#define _OPCODE_FIELD_OPERAND_

#include <iostream>
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class Operand {
    protected:
        uint16_t val;
        enum OperandType {SHIFT_RM, ROTATE_IMM, IMM};
        OperandType _type;

	public:
		Operand(uint16_t val, OperandType _type);
};

#endif