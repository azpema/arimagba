#ifndef _OPCODE_PSR_TRANSFER_MRS_ 
#define _OPCODE_PSR_TRANSFER_MRS_ 

#include <iostream>
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class PSRTransferMRS : public OpCode {
    private:
        uint16_t psr, Rd;

        const static uint32_t PSR_MASK = 0b00000000010000000000000000000000;
        const static uint32_t PSR_SHIFT = 22; 

        const static uint32_t RD_MASK = 0b00000000000000001111000000000000; 
        const static uint32_t RD_SHIFT = 12; 

        std::string PSR2Mnemonic[2] = {"cpsr", "spsr"};
        std::string getPSRMnemonic();

	public:
		PSRTransferMRS(uint32_t op);
        std::string toString();
};

#endif