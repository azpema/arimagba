#ifndef _OPCODE_BRANCH_
#define _OPCODE_BRANCH_

#include "arm_opcode.hpp"
#include <cstdint>
#include <string>

/*
 * Branch instructions contain a signed 2’s complement 24 bit offset. This is shifted left
 * two bits, sign extended to 32 bits, and added to the PC. The instruction can therefore
 * specify a branch of +/- 32Mbytes. The branch offset must take account of the prefetch
 * operation, which causes the PC to be 2 words (8 bytes) ahead of the current instruction
 */

namespace ARM {

class Branch : public ArmOpcode {
  public:
    Branch(uint32_t op, ARM7TDMI& cpu);
    Branch(ARM7TDMI& cpu);
    Branch(uint32_t _offsetField, uint32_t _realOffset, uint32_t _cond, ARM7TDMI& cpu);
    void init(uint32_t op) override;
    void init(uint32_t _offsetField, uint32_t _realOffset, uint32_t _cond);
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    std::string getLinkFlagMnemonic();

    uint16_t l;
    uint32_t offsetField, oldPC;
    int32_t realOffset;

    const static uint32_t LINK_MASK = 0b00000001000000000000000000000000;
    const static uint32_t LINK_SHIFT = 24;

    const static uint32_t OFFSET_MASK = 0b00000000111111111111111111111111;
    const static uint32_t OFFSET_SHIFT = 0;

    const static std::string linkFlag2Mnemonic[2];
};

} // namespace ARM

#endif