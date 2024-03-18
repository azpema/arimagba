#include "single_data_swap.hpp"

SingleDataSwap::SingleDataSwap(uint32_t op): OpCode::OpCode(op) {
    b = Utils::getRegBits(op, B_MASK, B_SHIFT);
    rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    rm = Utils::getRegBits(op, RM_MASK, RM_SHIFT);
}   


std::string SingleDataSwap::getBFlagMnemonic(){
    return bFlag2Mnemonic[b];
}

std::string SingleDataSwap::toString(){
    return "swp" + getCondFieldMnemonic() + getBFlagMnemonic() + " " + getRegMnemonic(rd) + "," + \
            getRegMnemonic(rm) + ",[" + getRegMnemonic(rn) + "]";
}

void SingleDataSwap::do_execute(ARM7TDMI &cpu){

}