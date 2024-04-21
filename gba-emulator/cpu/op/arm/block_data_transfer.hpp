#ifndef _OPCODE_BLOCK_DATA_TRANSFER_
#define _OPCODE_BLOCK_DATA_TRANSFER_

#include <iostream>
#include <vector>
#include "../opcode.hpp"


class BlockDataTransfer : public OpCode {
    public:
		BlockDataTransfer(uint32_t op);
        std::string toString() override;
        void doExecute(ARM7TDMI &cpu) override;
        void doDecode(ARM7TDMI &cpu) override;
        bool mustFlushPipeline() const override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t P, U, S, W, L, Rn, registerList;
        std::vector<uint32_t> registerListVec;

        const static uint32_t P_MASK = 0b00000001000000000000000000000000; 
        const static uint32_t P_SHIFT = 24;

        const static uint32_t U_MASK = 0b00000000100000000000000000000000; 
        const static uint32_t U_SHIFT = 23;

        const static uint32_t S_MASK = 0b00000000010000000000000000000000; 
        const static uint32_t S_SHIFT = 22;
        std::string SFlag2Mnemonic[2] = {"", "^"};
        std::string getSFlagMnemonic();

        const static uint32_t W_MASK = 0b00000000001000000000000000000000; 
        const static uint32_t W_SHIFT = 21;
        std::string WFlag2Mnemonic[2] = {"", "!"};
        std::string getWFlagMnemonic();

        const static uint32_t L_MASK = 0b00000000000100000000000000000000; 
        const static uint32_t L_SHIFT = 20;

        const static uint32_t RN_MASK = 0b00000000000011110000000000000000; 
        const static uint32_t RN_SHIFT = 16;

        const static uint32_t REGISTER_LIST_MASK = 0b00000000000000001111111111111111; 
        const static uint32_t REGISTER_LIST_SHIFT = 0;
        std::string getRegisterListMnemonic();
        // L, P, U
        // TODO "Other" column: LDMIB, LDMIA, LDMDB...
        std::string opAddressingMode2Mnemonic[2][2][2] = {{{"stmed", "stmea"}, {"stmfd", "stmfa"}},
                                                          {{"ldmfa", "ldmfd"}, {"ldmea", "ldmed"}}};
        std::string getOpAddressingModeMnemonic();
};

#endif