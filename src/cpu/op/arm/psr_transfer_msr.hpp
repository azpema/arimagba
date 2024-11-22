#ifndef _OPCODE_PSR_TRANSFER_MSR_ 
#define _OPCODE_PSR_TRANSFER_MSR_ 

#include <iostream>
#include "arm_opcode.hpp"
#include "../fields/operand.hpp"

class PSRTransferMSR : public ArmOpcode {
    public:
        //static bool isFullTransfer(uint32_t op);
        //static bool isFlagBitsTransfer(uint32_t op);

    public:
        PSRTransferMSR(uint32_t op, ARM7TDMI &cpu);
        PSRTransferMSR(ARM7TDMI &cpu);
        ~PSRTransferMSR() override;
        void init(uint32_t op) override;
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;
        
    private:
        uint16_t psr;
        uint16_t I;
        uint8_t c;
        std::unique_ptr<Operand> sourceOperand;

        const static uint32_t PSR_MASK = 0b00000000010000000000000000000000;
        const static uint32_t PSR_SHIFT = 22; 

        const static uint32_t RD_MASK = 0b00000000000000001111000000000000; 
        const static uint32_t RD_SHIFT = 12; 

        const static uint32_t FULL_TRANSFER_MASK = 0b00001101101111111111000000000000;
        const static uint32_t FULL_TRANSFER_FORMAT = 0b00000001001010011111000000000000;

        const static uint32_t FLAG_BITS_TRANSFER_MASK = 0b00001101101111111111000000000000;
        const static uint32_t FLAG_BITS_TRANSFER_FORMAT = 0b00000001001010001111000000000000;

        const static uint32_t I_MASK = 0b00000010000000000000000000000000; 
        const static uint32_t I_SHIFT = 25;

        const static uint32_t C_MASK = 0b00000000000000010000000000000000; 
        const static uint32_t C_SHIFT = 16;

        const static uint32_t SOURCE_OPERAND_MASK = 0b00000000000000000000111111111111;
        const static uint32_t SOURCE_OPERAND_SHIFT = 0;

        const static std::string PSR2Mnemonic[2];
        std::string getPSRMnemonic();
};

#endif