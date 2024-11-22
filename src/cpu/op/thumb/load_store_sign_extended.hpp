#ifndef _THUMB_OPCODE_LOAD_STORE_SIGN_EXTENDED_ 
#define _THUMB_OPCODE_LOAD_STORE_SIGN_EXTENDED_ 

#include "thumb_opcode.hpp"

class LoadStoreSignExtended : public ThumbOpCode {
	public:
		LoadStoreSignExtended(uint16_t op, ARM7TDMI &cpu);
        LoadStoreSignExtended(ARM7TDMI &cpu);
        void init(uint32_t op) override;
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint8_t h, s, ro, rb, rd;

        const static uint16_t H_MASK = 0b0000100000000000; 
        const static uint16_t H_SHIFT = 11;

        const static uint16_t S_MASK = 0b0000010000000000; 
        const static uint16_t S_SHIFT = 10;

        const static uint16_t RO_MASK = 0b0000000111000000; 
        const static uint16_t RO_SHIFT = 6;

        const static uint16_t RB_MASK = 0b0000000000111000; 
        const static uint16_t RB_SHIFT = 3;

        const static uint16_t RD_MASK = 0b0000000000000111; 
        const static uint16_t RD_SHIFT = 0;

        const static std::string op2Mnemonic[2][2];
        std::string getOpMnemonic();
};

#endif