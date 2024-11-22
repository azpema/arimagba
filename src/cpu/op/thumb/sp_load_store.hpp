#ifndef _THUMB_OPCODE_SP_LOAD_STORE_ 
#define _THUMB_OPCODE_SP_LOAD_STORE_ 

#include "thumb_opcode.hpp"

class SPLoadStore : public ThumbOpCode {
    public:
		SPLoadStore(uint16_t op, ARM7TDMI &cpu);
        SPLoadStore(ARM7TDMI &cpu);
        void init(uint32_t op) override;
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint16_t l, rd, word8, offsetVal;
        
        const static uint16_t L_MASK = 0b0000100000000000; 
        const static uint16_t L_SHIFT = 11;

        const static uint16_t RD_MASK = 0b0000011100000000; 
        const static uint16_t RD_SHIFT = 8;

        const static uint16_t WORD8_MASK = 0b0000000011111111; 
        const static uint16_t WORD8_SHIFT = 0;

        const static std::string l2Mnemonic[2];
        std::string getLMnemonic();
};

#endif