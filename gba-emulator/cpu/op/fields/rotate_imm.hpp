#ifndef _OPCODE_FIELD_ROTATE_IMM_
#define _OPCODE_FIELD_ROTATE_IMM_

#include <iostream>
#include "operand.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class RotateImm : public Operand {

    private:
        uint16_t rotate, imm, operandVal;

        const static uint32_t ROTATE_MASK = 0b111100000000;
        const static uint32_t ROTATE_SHIFT = 8;

        const static uint32_t IMM_MASK = 0b000011111111;
        const static uint32_t IMM_SHIFT = 0;

	public:
		RotateImm(uint16_t val);
        uint16_t getOperandVal();
};

#endif