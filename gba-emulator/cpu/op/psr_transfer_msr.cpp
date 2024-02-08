#include "psr_transfer_msr.hpp"
#include <bitset>

PSRTransferMSR::PSRTransferMSR(uint32_t op): OpCode::OpCode(op) {
    psr = Utils::getRegBits(op, PSR_MASK, PSR_SHIFT);
    operandSource = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
}   

std::string PSRTransferMSR::getPSRMnemonic(){
    return PSR2Mnemonic[psr];
}

std::string PSRTransferMSR::toString(){
    return "msr" + getCondFieldMnemonic() + " r" + std::to_string(Rd) + "," + getPSRMnemonic();
}