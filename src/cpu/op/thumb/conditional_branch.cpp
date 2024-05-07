#include "conditional_branch.hpp"

using namespace Thumb;

ConditionalBranch::ConditionalBranch(uint16_t op, uint32_t pc): ThumbOpCode::ThumbOpCode(op) {
    cond = Utils::getRegBits(op, COND_MASK, COND_SHIFT);
    sOffset8 = Utils::getRegBits(op, SOFFSET8_MASK, SOFFSET8_SHIFT);

    offsetVal = pc + Utils::twosComplementExtendSignTo(sOffset8 << 1, 9, 16) + 4;
}

std::string ConditionalBranch::getCondMnemonic(){
    return cond2Mnemonic[cond];
}

std::string ConditionalBranch::toString(){
    return "b" + getCondMnemonic() +   " #" + Utils::toHexString(offsetVal);
}