#ifndef _THUMB_OPCODE_SOFTWARE_INTERRUPT_ 
#define _THUMB_OPCODE_SOFTWARE_INTERRUPT_ 

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"
#include <vector>

namespace Thumb {

class SoftwareInterrupt : public ThumbOpCode {
    private:
        uint8_t value8;

        const static uint16_t VALUE8_MASK = 0b0000000011111111; 
        const static uint16_t VALUE8_SHIFT = 0;

	public:
		SoftwareInterrupt(uint16_t op);
        std::string toString();
};

}

#endif