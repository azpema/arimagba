#ifndef _OPCODE_UNDEFINED_
#define _OPCODE_UNDEFINED_

#include "../../../utils/utils.hpp"
#include "arm_opcode.hpp"
#include <iostream>

class Undefined : public ArmOpcode {
  public:
    Undefined(uint32_t op, ARM7TDMI& cpu);
    Undefined(ARM7TDMI& cpu);
    void init(uint32_t op) override;
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;
};

#endif