#ifndef _OPCODE_HALFWORD_DATA_TRANSFER_ 
#define _OPCODE_HALFWORD_DATA_TRANSFER_

#include <iostream>
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class HalfwordDataTransfer : public OpCode {
    public:
        virtual std::string toString() = 0;
        virtual void doExecute() = 0;
        virtual void doDecode() = 0;
    protected:
        uint16_t p, u, w, l, rn, rd, s, h;
        HalfwordDataTransfer(uint32_t op, ARM7TDMI &cpu);

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

        std::string op2Mnemonic[2] = {"str", "ldr"};
        std::string getOpMnemonic();
};


#endif