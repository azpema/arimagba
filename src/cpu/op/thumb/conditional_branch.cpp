#include "conditional_branch.hpp"

using namespace Thumb;

ConditionalBranch::ConditionalBranch(uint16_t op, uint32_t pc, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
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

void ConditionalBranch::doDecode(){

}

void ConditionalBranch::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: ConditionalBranch");
}

uint32_t ConditionalBranch::cyclesUsed() const {
    return 1;
}