#ifndef _OPCODE_BRANCH_EXCHANGE_
#define _OPCODE_BRANCH_EXCHANGE_

#include "arm_opcode.hpp"
#include <iostream>

class BranchAndExchange : public ArmOpcode {
  public:
    BranchAndExchange(uint32_t op, ARM7TDMI& cpu);
    BranchAndExchange(ARM7TDMI& cpu);
    void init(uint32_t op) override;
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    uint32_t Rn;
    const static uint32_t RN_MASK = 0b00000000000000000000000000001111;
    const static uint32_t RN_SHIFT = 0;
};

#endif