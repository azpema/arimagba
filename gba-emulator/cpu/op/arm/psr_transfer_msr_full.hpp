#ifndef _OPCODE_PSR_TRANSFER_MSR_FULL_ 
#define _OPCODE_PSR_TRANSFER_MSR_FULL_ 

#include <iostream>
#include "../opcode.hpp"
#include "psr_transfer_msr.hpp"
#include "../../../utils/utils.hpp"

class PSRTransferMSRFull : public PSRTransferMSR {
    public:
		PSRTransferMSRFull(uint32_t op);
        std::string toString();
        void doExecute(ARM7TDMI &cpu);

    private:
        uint16_t Rm;

        const static uint32_t RM_MASK = 0b00000000000000000000000000001111; 
        const static uint32_t RM_SHIFT = 0; 
};

#endif