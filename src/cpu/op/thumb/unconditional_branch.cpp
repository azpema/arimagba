#include "unconditional_branch.hpp"
#include "../arm/branch.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

using namespace Thumb;

UnconditionalBranch::UnconditionalBranch(uint16_t op, ARM7TDMI &cpu):
 ThumbOpCode::ThumbOpCode(op, cpu) {
    init(op);
}

UnconditionalBranch::UnconditionalBranch(ARM7TDMI &cpu):
 ThumbOpCode::ThumbOpCode(cpu) {}

void UnconditionalBranch::init(uint32_t op){
    ThumbOpCode::init(op);
    oldPC = cpu.getPC() - 2;
    offset11 = Utils::getRegBits(op, OFFSET11_MASK, OFFSET11_SHIFT);
    int16_t signExtended = Utils::twosComplementExtendSignTo(offset11 << 1, 12, 16);
    offsetVal = oldPC + signExtended + 4;
}

std::string UnconditionalBranch::toString(){
    return "b #" + Utils::toHexString(offsetVal);
}

void UnconditionalBranch::doDecode(){

}

void UnconditionalBranch::doExecute(){
    //ARM::Branch opArm = ARM::Branch(offset11, offsetVal, 0, cpu);
    ARM::Branch *opArm = static_cast<ARM::Branch*>(cpu.getArmPool().getArmInstance(ArmOpcode::OpCodeEnum::BRANCH));
    opArm->init(offset11, offsetVal, 0);
    opArm->doExecute();
    //std::cout << "<< ARM >> " << opArm.toString() << '\n';
}

uint32_t UnconditionalBranch::cyclesUsed() const {
    return 1;
}