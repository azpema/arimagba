#ifndef _OPCODE_HALFWORD_DATA_TRANSFER_REGISTER_ 
#define _OPCODE_HALFWORD_DATA_TRANSFER_REGISTER_ 

#include <iostream>
#include "halfword_data_transfer.hpp"

class HalfwordDataTransferRegister : public HalfwordDataTransfer {
    public:
        HalfwordDataTransferRegister(uint32_t op, ARM7TDMI &cpu);
        HalfwordDataTransferRegister(ARM7TDMI &cpu);
        HalfwordDataTransferRegister(uint16_t p, uint16_t u, uint16_t w, uint16_t l, uint16_t rn, 
            uint16_t rd, uint16_t s, uint16_t h, uint16_t rm, ARM7TDMI &cpu);
        void init(uint32_t op) override;
        void initRm(uint16_t rm);
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t rm;

        const static uint32_t RM_FLAG_MASK = 0b00000000000000000000000000001111; 
        const static uint32_t RM_FLAG_SHIFT = 0;
};

#endif