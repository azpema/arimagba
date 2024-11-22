#ifndef _THUMB_OPCODE_SOFTWARE_INTERRUPT_ 
#define _THUMB_OPCODE_SOFTWARE_INTERRUPT_ 

#include <vector>
#include "thumb_opcode.hpp"

namespace Thumb {

class SoftwareInterrupt : public ThumbOpCode {
    public:
		SoftwareInterrupt(uint16_t op, ARM7TDMI &cpu);
        SoftwareInterrupt(ARM7TDMI &cpu);
        void init(uint32_t op) override;
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;
    private:
        uint8_t value8;

        const static uint16_t VALUE8_MASK = 0b0000000011111111; 
        const static uint16_t VALUE8_SHIFT = 0;
};

}

#endif