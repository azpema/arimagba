#ifndef _THUMB_OPCODE_MOVE_COMP_ADD_SUB_IMM_
#define _THUMB_OPCODE_MOVE_COMP_ADD_SUB_IMM_

#include "thumb_opcode.hpp"

class MoveCompAddSubImm : public ThumbOpCode {
  public:
    MoveCompAddSubImm(uint16_t op, ARM7TDMI& cpu);
    MoveCompAddSubImm(ARM7TDMI& cpu);
    void init(uint32_t op) override;
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    uint16_t opField, rd, offset8;

    const static uint16_t OP_MASK = 0b0001100000000000;
    const static uint16_t OP_SHIFT = 11;

    const static uint16_t RD_MASK = 0b0000011100000000;
    const static uint16_t RD_SHIFT = 8;

    const static uint16_t OFFSET8_MASK = 0b0000000011111111;
    const static uint16_t OFFSET8_SHIFT = 0;

    enum Op { MOV = 0, CMP = 1, ADD = 2, SUB = 3 };

    const static std::string op2Mnemonic[4];
    std::string getOpMnemonic();
};

#endif