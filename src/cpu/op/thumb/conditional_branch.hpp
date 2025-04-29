#ifndef _THUMB_OPCODE_CONDITIONAL_BRANCH_
#define _THUMB_OPCODE_CONDITIONAL_BRANCH_

#include "thumb_opcode.hpp"
#include <string>

namespace Thumb {

class ConditionalBranch : public ThumbOpCode {
  public:
    ConditionalBranch(uint16_t op, ARM7TDMI& cpu);
    ConditionalBranch(ARM7TDMI& cpu);
    void init(uint32_t op) override;
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    uint8_t cond, sOffset8;
    uint32_t offsetVal;
    uint32_t oldPC;

    const static uint16_t COND_MASK = 0b0000111100000000;
    const static uint16_t COND_SHIFT = 8;
    const static std::string cond2Mnemonic[16];
    std::string getCondMnemonic();

    const static uint16_t SOFFSET8_MASK = 0b0000000011111111;
    const static uint16_t SOFFSET8_SHIFT = 0;
};

} // namespace Thumb

#endif