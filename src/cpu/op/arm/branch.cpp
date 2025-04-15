#include "branch.hpp"

#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

using namespace ARM;

const std::string Branch::linkFlag2Mnemonic[2] = {"", "l"};

Branch::Branch(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    init(op);
}

Branch::Branch(ARM7TDMI &cpu): ArmOpcode::ArmOpcode(cpu) {
    
} 

Branch::Branch(uint32_t _offsetField, uint32_t _realOffset, uint32_t _cond, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(cpu){
    init(_offsetField, _realOffset, _cond);
}

void Branch::init(uint32_t op){
    ArmOpcode::init(op);
    l = Utils::getRegBits(op, LINK_MASK, LINK_SHIFT);
    offsetField = Utils::getRegBits(op, OFFSET_MASK, OFFSET_SHIFT);
    realOffset = static_cast<int32_t>(Utils::twosComplementExtendSignTo(offsetField, 24, 32) << 2) + cpu.getPC() + 4;
    oldPC = cpu.getPC() - 4;
}

void Branch::init(uint32_t _offsetField, uint32_t _realOffset, uint32_t _cond){
    l = 0;
    offsetField = _offsetField;
    realOffset = _realOffset;
    oldPC = cpu.getPC() - 4;

    condRaw = _cond;
    cond = static_cast<Condition>(_cond);
}

std::string Branch::getLinkFlagMnemonic(){
    return linkFlag2Mnemonic[l];
}

std::string Branch::toString(){
    return "b" + getLinkFlagMnemonic() + getCondFieldMnemonic() + " #" + Utils::toHexString(realOffset);
}

uint32_t Branch::cyclesUsed() const {
    // 2S + 1N
    return 2 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE + 1 * ARM7TDMI::CPU_CYCLES_PER_N_CYCLE;
}

void Branch::doDecode(){

}

void Branch::doExecute(){
    //uint32_t oldPC = cpu.getPC();
    cpu.setMustFlushPipeline(true);
    cpu.setPC(realOffset);

    if(l == 1){
        cpu.setLR(oldPC + 4);
    }
}