#ifndef _THUMB_OPCODE_PC_RELATIVE_LOAD
#define _THUMB_OPCODE_PC_RELATIVE_LOAD

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class PCRelativeLoad : public ThumbOpCode {
    private:
        uint16_t rd, word8, offset;

        const static uint16_t RD_MASK = 0b0000011100000000;
        const static uint16_t RD_SHIFT = 8;

        const static uint16_t WORD8_MASK = 0b0000000011111111;
        const static uint16_t WORD8_SHIFT = 0;

	public:
		PCRelativeLoad(uint16_t op);
        std::string toString();

};

#endif