#ifndef _OPCODE_FIELD_OPERAND_
#define _OPCODE_FIELD_OPERAND_

#include <iostream>
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class Operand { 
	public:
        enum OperandType {SHIFT_RM, ROTATE_IMM, IMM, RM};
        OperandType _type;
		Operand(uint16_t val, OperandType _type);
        virtual uint32_t getOperandVal(ARM7TDMI &cpu) = 0;
        virtual bool getC() = 0;

        uint16_t getRawVal();
        void setRawVal(uint16_t val);
    protected:
        uint16_t val;
};

#endif