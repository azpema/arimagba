#ifndef _THUMB_OPCODE_PUSH_POP_REGISTERS_
#define _THUMB_OPCODE_PUSH_POP_REGISTERS_

#include "thumb_opcode.hpp"

class PushPopRegisters : public ThumbOpCode {
  public:
    PushPopRegisters(uint16_t op, ARM7TDMI& cpu);
    PushPopRegisters(ARM7TDMI& cpu);
    void init(uint32_t op) override;
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    uint16_t l, r, rList;
    uint16_t registerListVec[8];

    const static uint16_t L_MASK = 0b0000100000000000;
    const static uint16_t L_SHIFT = 11;

    const static uint16_t R_MASK = 0b0000000100000000;
    const static uint16_t R_SHIFT = 8;

    const static uint16_t RLIST_MASK = 0b0000000011111111;
    const static uint16_t RLIST_SHIFT = 0;

    const static std::string op2Mnemonic[2];
    std::string getOpMnemonic();
};

#endif