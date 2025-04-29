#ifndef _THUMB_OPCODE_LOAD_STORE_HALFWORD_
#define _THUMB_OPCODE_LOAD_STORE_HALFWORD_

#include "thumb_opcode.hpp"

class LoadStoreHalfword : public ThumbOpCode {
  public:
    LoadStoreHalfword(uint16_t op, ARM7TDMI& cpu);
    LoadStoreHalfword(ARM7TDMI& cpu);
    void init(uint32_t op) override;
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    uint8_t l, offset5, rb, rd, offsetVal;

    const static uint16_t L_MASK = 0b0000100000000000;
    const static uint16_t L_SHIFT = 11;

    const static uint16_t OFFSET5_MASK = 0b0000011111000000;
    const static uint16_t OFFSET5_SHIFT = 6;

    const static uint16_t RB_MASK = 0b0000000000111000;
    const static uint16_t RB_SHIFT = 3;

    const static uint16_t RD_MASK = 0b0000000000000111;
    const static uint16_t RD_SHIFT = 0;

    const static std::string op2Mnemonic[2];
    std::string getOpMnemonic();
};

#endif