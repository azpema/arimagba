#include "multiply_accumulate.hpp"

MultiplyAccumulate::MultiplyAccumulate(uint32_t op, ARM7TDMI &cpu): OpCode::OpCode(op, cpu) {
    a = Utils::getRegBits(op, A_FLAG_MASK, A_FLAG_SHIFT);
    s = Utils::getRegBits(op, S_FLAG_MASK, S_FLAG_SHIFT);
    rd = Utils::getRegBits(op, RD_FLAG_MASK, RD_FLAG_SHIFT);
    rn = Utils::getRegBits(op, RN_FLAG_MASK, RN_FLAG_SHIFT);
    rs = Utils::getRegBits(op, RS_FLAG_MASK, RS_FLAG_SHIFT);
    rm = Utils::getRegBits(op, RM_FLAG_MASK, RM_FLAG_SHIFT);
}   

std::string MultiplyAccumulate::getSFlagMnemonic(){
    return sFlag2Mnemonic[s];
}

std::string MultiplyAccumulate::getOpMnemonic(){
    return op2Mnemonic[a];
}

std::string MultiplyAccumulate::toString(){
    std::string mnemonic = getOpMnemonic() + getCondFieldMnemonic() + getSFlagMnemonic() + " " + \
                           getRegMnemonic(rd) + "," + getRegMnemonic(rm) + "," + getRegMnemonic(rs);
    if(a == 1)
        mnemonic += " " + getRegMnemonic(rn);
    return mnemonic;
}

void MultiplyAccumulate::doDecode(){

}

void MultiplyAccumulate::doExecute(){

}

// MUL              1S+ml
// MLA              1S+(m+1)I
uint32_t MultiplyAccumulate::cyclesUsed() const {
    std::cerr << "TODO: MultiplyAccumulate::cyclesUsed" << std::endl;
    return 1 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE + (0 + 1) * ARM7TDMI::CPU_CYCLES_PER_I_CYCLE;
}