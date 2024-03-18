#include "unconditional_branch.hpp"

using namespace Thumb;

UnconditionalBranch::UnconditionalBranch(uint16_t op, uint32_t pc): ThumbOpCode::ThumbOpCode(op) {
    offset11 = Utils::getRegBits(op, OFFSET11_MASK, OFFSET11_SHIFT);
    offsetVal = pc + Utils::twosComplementExtendSignTo(offset11 << 1, 9, 16) + 4;
}


std::string UnconditionalBranch::toString(){
    return "b #" + Utils::toHexString(offsetVal);
}