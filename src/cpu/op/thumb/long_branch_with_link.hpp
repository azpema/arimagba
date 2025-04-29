#ifndef _THUMB_OPCODE_LONG_BRANCH_WITH_LINK_
#define _THUMB_OPCODE_LONG_BRANCH_WITH_LINK_

#include "thumb_opcode.hpp"

namespace Thumb {

class LongBranchWithLink : public ThumbOpCode {
  public:
    LongBranchWithLink(uint16_t op, ARM7TDMI& cpu);
    LongBranchWithLink(ARM7TDMI& cpu);
    void init(uint32_t op) override;
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    uint8_t h;
    uint16_t offset;
    uint32_t offsetVal;
    int32_t realOffsetVal;

    const static uint16_t H_MASK = 0b0000100000000000;
    const static uint16_t H_SHIFT = 11;

    const static uint16_t OFFSET_MASK = 0b0000011111111111;
    const static uint16_t OFFSET_SHIFT = 0;
};

} // namespace Thumb

#endif