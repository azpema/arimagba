#ifndef _OPCODE_FIELD_ROTATE_IMM_
#define _OPCODE_FIELD_ROTATE_IMM_

#include <iostream>
#include "operand.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class RotateImm : public Operand {
	public:
		RotateImm(uint16_t val);
        RotateImm(uint8_t rot, uint8_t imm);
        ~RotateImm() override;
        uint32_t getMnemonicVal();

        uint32_t getOperandVal(ARM7TDMI &cpu) override;
        uint32_t getRorShiftAmount();
        bool getC() override;

    private:
        bool c;
        uint16_t rotate, imm;
        uint32_t operandVal;

        const static uint32_t ROTATE_MASK = 0b111100000000;
        const static uint32_t ROTATE_SHIFT = 8;

        const static uint32_t IMM_MASK = 0b000011111111;
        const static uint32_t IMM_SHIFT = 0;
};

#endif