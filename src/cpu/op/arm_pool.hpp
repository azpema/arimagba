#ifndef _ARM_POOL_
#define _ARM_POOL_

class ARM7TDMI;

#include "arm/arm_opcode.hpp"
#include <memory>
#include <unordered_map>

class ArmPool {
  public:
    ArmPool(ARM7TDMI& cpu);
    ArmOpcode* getArmInstance(ArmOpcode::OpCodeEnum key) const;

  private:
    std::unordered_map<ArmOpcode::OpCodeEnum, std::unique_ptr<ArmOpcode>> armPool{};
};

#endif