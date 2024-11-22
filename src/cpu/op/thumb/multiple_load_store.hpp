#ifndef _THUMB_OPCODE_MULTIPLE_LOAD_STORE_ 
#define _THUMB_OPCODE_MULTIPLE_LOAD_STORE_ 

#include <vector>
#include "thumb_opcode.hpp"

class MultipleLoadStore : public ThumbOpCode {
	public:
		MultipleLoadStore(uint16_t op, ARM7TDMI &cpu);
        MultipleLoadStore(ARM7TDMI &cpu);
        void init(uint32_t op) override;
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

    private:
        uint8_t l, rb, rList;
        std::vector<uint8_t> registerListVec;

        const static uint16_t L_MASK = 0b0000100000000000; 
        const static uint16_t L_SHIFT = 11;

        const static uint16_t RB_MASK = 0b0000011100000000; 
        const static uint16_t RB_SHIFT = 8;

        const static uint16_t RLIST_MASK = 0b0000000011111111; 
        const static uint16_t RLIST_SHIFT = 0;

        const static std::string op2Mnemonic[2];
        std::string getOpMnemonic();
};

#endif