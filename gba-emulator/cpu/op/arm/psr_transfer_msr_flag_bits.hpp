#ifndef _OPCODE_PSR_TRANSFER_MSR_FLAG_BITS_ 
#define _OPCODE_PSR_TRANSFER_MSR_FLAG_BITS_ 

#include <iostream>
#include "../opcode.hpp"
#include "psr_transfer_msr.hpp"
#include "../fields/operand.hpp"
#include "../../../utils/utils.hpp"

class PSRTransferMSRFlagBits : public PSRTransferMSR {
    public:
		PSRTransferMSRFlagBits(uint32_t op, ARM7TDMI &cpu);
        ~PSRTransferMSRFlagBits();

        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        bool mustFlushPipeline() const override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t I;
        Operand* sourceOperand;

        const static uint32_t I_MASK = 0b00000010000000000000000000000000; 
        const static uint32_t I_SHIFT = 25;

        const static uint32_t SOURCE_OPERAND_MASK = 0b00000000000000000000111111111111;
        const static uint32_t SOURCE_OPERAND_SHIFT = 0;
};

#endif