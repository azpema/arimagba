#ifndef _OPCODE_SINGLE_DATA_TRANSFER_
#define _OPCODE_SINGLE_DATA_TRANSFER_

#include <iostream>
#include "arm_opcode.hpp"
#include "../fields/shift_rm.hpp"
#include "../fields/imm.hpp"

class SingleDataTransfer : public ArmOpcode {
	public:
		SingleDataTransfer(uint32_t op, ARM7TDMI &cpu);
        SingleDataTransfer(uint8_t i, uint8_t p, uint8_t u, uint8_t b, uint8_t w, uint8_t l, 
                           uint8_t rn, uint8_t rd, uint16_t offset, ARM7TDMI &cpu);
        ~SingleDataTransfer();
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint8_t I, P, U, B, W, L, Rn, Rd;
        Operand *offsetField;

        const static uint32_t I_MASK = 0b00000010000000000000000000000000; 
        const static uint32_t I_SHIFT = 25;

        const static uint32_t P_MASK = 0b00000001000000000000000000000000; 
        const static uint32_t P_SHIFT = 24;

        const static uint32_t U_MASK = 0b00000000100000000000000000000000; 
        const static uint32_t U_SHIFT = 23;
        std::string UFlag2Mnemonic[2] = {"-", ""};
        std::string getUFlagMnemonic();

        const static uint32_t B_MASK = 0b00000000010000000000000000000000; 
        const static uint32_t B_SHIFT = 22;
        std::string BFlag2Mnemonic[2] = {"", "b"};
        std::string getBFlagMnemonic();

        const static uint32_t W_MASK = 0b00000000001000000000000000000000; 
        const static uint32_t W_SHIFT = 21;
        std::string WFlag2Mnemonic[2] = {"", "!"};
        std::string getWFlagMnemonic();

        const static uint32_t L_MASK = 0b00000000000100000000000000000000; 
        const static uint32_t L_SHIFT = 20;

        std::string LFlag2Mnemonic[2] = {"str", "ldr"};
        std::string getLFlagMnemonic();

        const static uint32_t RN_MASK = 0b00000000000011110000000000000000; 
        const static uint32_t RN_SHIFT = 16;

        const static uint32_t RD_MASK = 0b00000000000000001111000000000000; 
        const static uint32_t RD_SHIFT = 12;

        const static uint32_t OFFSET_MASK = 0b00000000000000000000111111111111; 
        const static uint32_t OFFSET_SHIFT = 0;
};

#endif