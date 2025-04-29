#ifndef _THUMB_OPCODE_HI_REGISTER_BRANCH_EXCHANGE_
#define _THUMB_OPCODE_HI_REGISTER_BRANCH_EXCHANGE_

#include "thumb_opcode.hpp"

class HiRegisterBranchExchange : public ThumbOpCode {
  public:
    HiRegisterBranchExchange(uint16_t op, ARM7TDMI& cpu);
    HiRegisterBranchExchange(ARM7TDMI& cpu);
    void init(uint32_t op) override;
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    uint16_t opField, h1, h2, rsHs, rdHd, rs, rd;

    const static uint16_t OP_MASK = 0b0000001100000000;
    const static uint16_t OP_SHIFT = 8;

    const static uint16_t H1_MASK = 0b0000000010000000;
    const static uint16_t H1_SHIFT = 7;

    const static uint16_t H2_MASK = 0b0000000001000000;
    const static uint16_t H2_SHIFT = 6;

    const static uint16_t RS_HS_MASK = 0b0000000000111000;
    const static uint16_t RS_HS_SHIFT = 3;

    const static uint16_t RD_HD_MASK = 0b0000000000000111;
    const static uint16_t RD_HD_SHIFT = 0;

    enum OP { ADD = 0, CMP = 1, MOV = 2, BX = 3 };

    const static std::string op2Mnemonic[4];
    std::string getOpMnemonic();
};

#endif