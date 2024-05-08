#include "unconditional_branch.hpp"

using namespace Thumb;

UnconditionalBranch::UnconditionalBranch(uint16_t op, uint32_t pc, ARM7TDMI &cpu):
 ThumbOpCode::ThumbOpCode(op, cpu) {
    offset11 = Utils::getRegBits(op, OFFSET11_MASK, OFFSET11_SHIFT);
    offsetVal = pc + Utils::twosComplementExtendSignTo(offset11 << 1, 9, 16) + 4;
}


std::string UnconditionalBranch::toString(){
    return "b #" + Utils::toHexString(offsetVal);
}

void UnconditionalBranch::doDecode(){

}

void UnconditionalBranch::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: UnconditionalBranch");
}

uint32_t UnconditionalBranch::cyclesUsed() const {
    return 1;
}