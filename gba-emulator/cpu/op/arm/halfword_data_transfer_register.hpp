#ifndef _OPCODE_HALFWORD_DATA_TRANSFER_REGISTER_ 
#define _OPCODE_HALFWORD_DATA_TRANSFER_REGISTER_ 

#include <iostream>
#include "halfword_data_transfer.hpp"
#include "../../../utils/utils.hpp"

class HalfwordDataTransferRegister : public HalfwordDataTransfer {
    public:
        HalfwordDataTransferRegister(uint32_t op);
        std::string toString() override;
        void doExecute(ARM7TDMI &cpu) override;
        void doDecode(ARM7TDMI &cpu) override;
        bool mustFlushPipeline() const override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t rm;

        const static uint32_t RM_FLAG_MASK = 0b00000000000000000000000000001111; 
        const static uint32_t RM_FLAG_SHIFT = 0;
};


/*class HalfwordDataTransferRegister : public OpCode {
    private:
        //uint16_t p, u, w, l, rn, rd, offsetHi, s, h, rm;

        const static uint32_t P_FLAG_MASK = 0b00000001000000000000000000000000; 
        const static uint32_t P_FLAG_SHIFT = 24;

        const static uint32_t U_FLAG_MASK = 0b00000000100000000000000000000000; 
        const static uint32_t U_FLAG_SHIFT = 23;
        std::string uFlag2Mnemonic[2] = {"-", ""};
        std::string getUFlagMnemonic();

        const static uint32_t W_FLAG_MASK = 0b00000000001000000000000000000000; 
        const static uint32_t W_FLAG_SHIFT = 21;
        std::string wFlag2Mnemonic[2] = {"", "!"};
        std::string getWFlagMnemonic();

        const static uint32_t L_FLAG_MASK = 0b00000000000100000000000000000000; 
        const static uint32_t L_FLAG_SHIFT = 20;

        const static uint32_t RN_FLAG_MASK = 0b00000000000011110000000000000000; 
        const static uint32_t RN_FLAG_SHIFT = 16;

        const static uint32_t RD_FLAG_MASK = 0b00000000000000001111000000000000; 
        const static uint32_t RD_FLAG_SHIFT = 12;

        const static uint32_t S_FLAG_MASK = 0b00000000000000000000000001000000; 
        const static uint32_t S_FLAG_SHIFT = 6;
        std::string sFlag2Mnemonic[2] = {"", "s"};
        std::string getSFlagMnemonic();

        const static uint32_t H_FLAG_MASK = 0b00000000000000000000000000100000; 
        const static uint32_t H_FLAG_SHIFT = 5;
        std::string hFlag2Mnemonic[2] = {"b", "h"};
        std::string getHFlagMnemonic();

        const static uint32_t RM_FLAG_MASK = 0b00000000000000000000000000001111; 
        const static uint32_t RM_FLAG_SHIFT = 0;

        std::string op2Mnemonic[2] = {"str", "ldr"};
        std::string getOpMnemonic();

	public:
		HalfwordDataTransferRegister(uint32_t op);
        std::string toString();

};*/

#endif