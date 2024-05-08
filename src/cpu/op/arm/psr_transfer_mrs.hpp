#ifndef _OPCODE_PSR_TRANSFER_MRS_ 
#define _OPCODE_PSR_TRANSFER_MRS_ 

#include <iostream>
#include "arm_opcode.hpp"
#include "../../../utils/utils.hpp"

class PSRTransferMRS : public ArmOpcode {
    public:
		PSRTransferMRS(uint32_t op, ARM7TDMI &cpu);
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t psr, Rd;

        const static uint32_t PSR_MASK = 0b00000000010000000000000000000000;
        const static uint32_t PSR_SHIFT = 22; 

        const static uint32_t RD_MASK = 0b00000000000000001111000000000000; 
        const static uint32_t RD_SHIFT = 12; 

        std::string PSR2Mnemonic[2] = {"cpsr", "spsr"};
        std::string getPSRMnemonic();
};

#endif