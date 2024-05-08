#ifndef _THUMB_OPCODE_LOAD_STORE_REGISTER_OFFSET_
#define _THUMB_OPCODE_LOAD_STORE_REGISTER_OFFSET_

#include <iostream>
#include "thumb_opcode.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class LoadStoreRegisterOffset : public ThumbOpCode { 
	public:
		LoadStoreRegisterOffset(uint16_t op, ARM7TDMI &cpu);
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t l, b, r0, rb, rd;

        const static uint16_t L_MASK = 0b0000100000000000;
        const static uint16_t L_SHIFT = 11;

        const static uint16_t B_MASK = 0b0000010000000000;
        const static uint16_t B_SHIFT = 10;
        std::string bFlag2Mnemonic[2] = {"", "b"};
        std::string getBFlagMnemonic();

        const static uint16_t R0_MASK = 0b0000000111000000;
        const static uint16_t R0_SHIFT = 6;

        const static uint16_t RB_MASK = 0b0000000000111000;
        const static uint16_t RB_SHIFT = 3;

        const static uint16_t RD_MASK = 0b0000000000000111;
        const static uint16_t RD_SHIFT = 0;

        std::string op2Mnemonic[2] = {"str", "ldr"};
        std::string getOpMnemonic();
};

#endif