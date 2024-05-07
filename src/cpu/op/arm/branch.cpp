#include "branch.hpp"
#include <bitset>

using namespace ARM;

Branch::Branch(uint32_t op, uint32_t pc, ARM7TDMI &cpu): OpCode::OpCode(op, cpu) {
    L = Utils::getRegBits(op, LINK_MASK, LINK_SHIFT);
    offsetField = Utils::getRegBits(op, OFFSET_MASK, OFFSET_SHIFT);
    realOffset = static_cast<int32_t>(Utils::twosComplementExtendSignTo(offsetField, 24, 32) << 2) + pc + 8;
    oldPC = pc;
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

void Branch::doDecode(){

}

void Branch::doExecute(){
    //uint32_t oldPC = cpu.getPC();
    cpu.setPC(realOffset);

    if(L == 1){
        cpu.setLR(oldPC + 4);
    }
}