#include "branch.hpp"
#include <bitset>

using namespace ARM;

Branch::Branch(uint32_t op, uint32_t pc): OpCode::OpCode(op) {
    L = Utils::getRegBits(op, LINK_MASK, LINK_SHIFT);
    offsetField = Utils::getRegBits(op, OFFSET_MASK, OFFSET_SHIFT);
    realOffset = static_cast<int32_t>(Utils::twosComplementExtendSignTo(offsetField, 24, 32) << 2) + pc + 8;
}   

std::string Branch::getLinkFlagMnemonic(){
    return linkFlag2Mnemonic[L];
}

std::string Branch::toString(){
    return "b" + getLinkFlagMnemonic() + getCondFieldMnemonic() + " #" + Utils::toHexString(realOffset);
}

bool Branch::mustFlushPipeline() const {
    return true;
}

uint32_t Branch::cyclesUsed() const {
    // 2S + 1N
    return 2 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE + 1 * ARM7TDMI::CPU_CYCLES_PER_N_CYCLE;
}


void Branch::doExecute(ARM7TDMI &cpu){
    uint32_t oldPC = cpu.getPC();
    cpu.setPC(realOffset);

    if(L == 1){
        cpu.setLR(oldPC);
    }
}