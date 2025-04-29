#ifndef _THUMB_OPCODE_UNCONDITIONAL_BRANCH_
#define _THUMB_OPCODE_UNCONDITIONAL_BRANCH_

#include "thumb_opcode.hpp"
#include <vector>

namespace Thumb {

class UnconditionalBranch : public ThumbOpCode {
  public:
    UnconditionalBranch(uint16_t op, ARM7TDMI& cpu);
    UnconditionalBranch(ARM7TDMI& cpu);
    void init(uint32_t op) override;
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    uint16_t offset11;
    int32_t offsetVal;
    uint32_t oldPC;

    const static uint16_t OFFSET11_MASK = 0b0000011111111111;
    const static uint16_t OFFSET11_SHIFT = 0;
};

} // namespace Thumb

#endif