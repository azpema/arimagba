#ifndef _OPCODE_PSR_TRANSFER_MSR_ 
#define _OPCODE_PSR_TRANSFER_MSR_ 

#include <iostream>
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class PSRTransferMSR : public OpCode {
    public:
        static bool isFullTransfer(uint32_t op);
        static bool isFlagBitsTransfer(uint32_t op);

    protected:
        PSRTransferMSR(uint32_t op, ARM7TDMI &cpu);
        std::string toString() override;
        virtual void doExecute() = 0;
        virtual void doDecode() = 0;
        uint32_t cyclesUsed() const override;

        uint16_t psr;
    private:
        const static uint32_t PSR_MASK = 0b00000000010000000000000000000000;
        const static uint32_t PSR_SHIFT = 22; 

        const static uint32_t RD_MASK = 0b00000000000000001111000000000000; 
        const static uint32_t RD_SHIFT = 12; 

        const static uint32_t FULL_TRANSFER_MASK = 0b00001111101111111111111111110000;
        const static uint32_t FULL_TRANSFER_FORMAT = 0b00000001001010011111000000000000;

        const static uint32_t FLAG_BITS_TRANSFER_MASK = 0b00001101101111111111000000000000;
        const static uint32_t FLAG_BITS_TRANSFER_FORMAT = 0b00000001001010001111000000000000;

        std::string PSR2Mnemonic[2] = {"cpsr", "spsr"};
        std::string getPSRMnemonic();
};

#endif