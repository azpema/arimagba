#ifndef _OPCODE_MULTIPLY_ACCUMULATE_
#define _OPCODE_MULTIPLY_ACCUMULATE_

#include "arm_opcode.hpp"
#include <string>

class MultiplyAccumulate : public ArmOpcode {
  public:
    MultiplyAccumulate(uint32_t op, ARM7TDMI& cpu);
    MultiplyAccumulate(ARM7TDMI& cpu);
    MultiplyAccumulate(uint32_t a, uint32_t s, uint32_t rd, uint32_t rn, uint32_t rs, uint32_t rm, ARM7TDMI& cpu);
    void init(uint32_t op) override;
    void init(uint32_t a, uint32_t s, uint32_t rd, uint32_t rn, uint32_t rs, uint32_t rm);
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    uint16_t a, s, rd, rn, rs, rm;
    const static uint32_t A_FLAG_MASK = 0b00000000001000000000000000000000;
    const static uint32_t A_FLAG_SHIFT = 21;

    const static uint32_t S_FLAG_MASK = 0b00000000000100000000000000000000;
    const static uint32_t S_FLAG_SHIFT = 20;

    const static uint32_t RD_FLAG_MASK = 0b00000000000011110000000000000000;
    const static uint32_t RD_FLAG_SHIFT = 16;

    const static uint32_t RN_FLAG_MASK = 0b00000000000000001111000000000000;
    const static uint32_t RN_FLAG_SHIFT = 12;

    const static uint32_t RS_FLAG_MASK = 0b00000000000000000000111100000000;
    const static uint32_t RS_FLAG_SHIFT = 8;

    const static uint32_t RM_FLAG_MASK = 0b00000000000000000000000000001111;
    const static uint32_t RM_FLAG_SHIFT = 0;

    const static std::string sFlag2Mnemonic[2];
    std::string getSFlagMnemonic();

    const static std::string op2Mnemonic[2];
    std::string getOpMnemonic();
};

#endif