#include "unconditional_branch.hpp"
#include "../arm/branch.hpp"

using namespace Thumb;

UnconditionalBranch::UnconditionalBranch(uint16_t op, uint32_t pc, ARM7TDMI &cpu):
 ThumbOpCode::ThumbOpCode(op, cpu) {
    oldPC = pc;
    offset11 = Utils::getRegBits(op, OFFSET11_MASK, OFFSET11_SHIFT);
    offsetVal = oldPC + Utils::twosComplementExtendSignTo(offset11 << 1, 9, 16) + 4;
}


std::string UnconditionalBranch::toString(){
    return "b #" + Utils::toHexString(offsetVal);
}

void UnconditionalBranch::doDecode(){

}

void UnconditionalBranch::doExecute(){
    ARM::Branch opArm = ARM::Branch(oldPC, offset11, offsetVal, 0, cpu);
    opArm.doExecute();
    std::cout << "<< ARM >> " << opArm.toString() << std::endl;
}

uint32_t UnconditionalBranch::cyclesUsed() const {
    return 1;
}