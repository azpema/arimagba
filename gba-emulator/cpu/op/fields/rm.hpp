#ifndef _OPCODE_FIELD_RM_
#define _OPCODE_FIELD_RM_

#include <iostream>
#include "operand.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class Rm : public Operand {
    private:
        uint16_t rmVal;

        const uint16_t RM_MASK = 000000001111;
        const uint16_t RM_SHIFT = 0;

	public:
		Rm(uint16_t val);
        std::string toString();
        uint16_t getRmVal();
};

#endif