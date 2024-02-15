#include "psr_transfer_mrs.hpp"
#include <bitset>

PSRTransferMRS::PSRTransferMRS(uint32_t op): OpCode::OpCode(op) {
    psr = Utils::getRegBits(op, PSR_MASK, PSR_SHIFT);
    Rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
}   

std::string PSRTransferMRS::getPSRMnemonic(){
    return PSR2Mnemonic[psr];
}

std::string PSRTransferMRS::toString(){
    return "mrs" + getCondFieldMnemonic() + " " + getRegMnemonic(Rd) + "," + getPSRMnemonic();
}