#include "multiply_accumulate_long.hpp"

MultiplyAccumulateLong::MultiplyAccumulateLong(uint32_t op, ARM7TDMI &cpu): OpCode::OpCode(op, cpu) {
    u = Utils::getRegBits(op, U_FLAG_MASK, U_FLAG_SHIFT);
    a = Utils::getRegBits(op, A_FLAG_MASK, A_FLAG_SHIFT);
    s = Utils::getRegBits(op, S_FLAG_MASK, S_FLAG_SHIFT);
    rdHi = Utils::getRegBits(op, RDHI_FLAG_MASK, RDHI_FLAG_SHIFT);
    rdLo = Utils::getRegBits(op, RDLO_FLAG_MASK, RDLO_FLAG_SHIFT);
    rs = Utils::getRegBits(op, RS_FLAG_MASK, RS_FLAG_SHIFT);
    rm = Utils::getRegBits(op, RM_FLAG_MASK, RM_FLAG_SHIFT);
}   

std::string MultiplyAccumulateLong::getUFlagMnemonic(){
    return uFlag2Mnemonic[u];
}

std::string MultiplyAccumulateLong::getSFlagMnemonic(){
    return sFlag2Mnemonic[s];
}

std::string MultiplyAccumulateLong::getOpMnemonic(){
    return op2Mnemonic[a];
}

std::string MultiplyAccumulateLong::toString(){
    return getUFlagMnemonic() + getOpMnemonic() + getCondFieldMnemonic() + getSFlagMnemonic() + " " + \
            getRegMnemonic(rdLo) + "," + getRegMnemonic(rdHi) + "," + getRegMnemonic(rm) + "," + \
            getRegMnemonic(rs);
}

void MultiplyAccumulateLong::doDecode(){

}

void MultiplyAccumulateLong::doExecute(){

}

// MULL             1S+(m+1)I
// MLAL             1S+(m+2)I
uint32_t MultiplyAccumulateLong::cyclesUsed() const {
    std::cerr << "TODO: MultiplyAccumulateLong::cyclesUsed" << std::endl;
    return 1 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE + (0 + 1) * ARM7TDMI::CPU_CYCLES_PER_I_CYCLE;
}
