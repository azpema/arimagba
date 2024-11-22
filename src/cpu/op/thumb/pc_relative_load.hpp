#ifndef _THUMB_OPCODE_PC_RELATIVE_LOAD
#define _THUMB_OPCODE_PC_RELATIVE_LOAD

#include "thumb_opcode.hpp"

class PCRelativeLoad : public ThumbOpCode {
	public:
		PCRelativeLoad(uint16_t op, ARM7TDMI &cpu);
        PCRelativeLoad(ARM7TDMI &cpu);
        void init(uint32_t op) override;
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t rd, word8, offset;

        const static uint16_t RD_MASK = 0b0000011100000000;
        const static uint16_t RD_SHIFT = 8;

        const static uint16_t WORD8_MASK = 0b0000000011111111;
        const static uint16_t WORD8_SHIFT = 0;
};

#endif