#include "single_data_swap.hpp"

SingleDataSwap::SingleDataSwap(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    b = Utils::getRegBits(op, B_MASK, B_SHIFT);
    rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    rm = Utils::getRegBits(op, RM_MASK, RM_SHIFT);
}   


std::string SingleDataSwap::getBFlagMnemonic(){
    return bFlag2Mnemonic[b];
}

std::string SingleDataSwap::toString(){
    return "swp" + getCondFieldMnemonic() + getBFlagMnemonic() + " " + OpCode::getRegMnemonic(rd) + "," + \
            OpCode::getRegMnemonic(rm) + ",[" + OpCode::getRegMnemonic(rn) + "]";
}

void SingleDataSwap::doDecode(){

}

void SingleDataSwap::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: SingleDataSwap");
}

// SWP              1S+2N+1I
uint32_t SingleDataSwap::cyclesUsed() const {
    return 1 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE + 2 * ARM7TDMI::CPU_CYCLES_PER_N_CYCLE + 1 * ARM7TDMI::CPU_CYCLES_PER_I_CYCLE;
}