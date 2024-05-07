#include "psr_transfer_mrs.hpp"
#include <bitset>

PSRTransferMRS::PSRTransferMRS(uint32_t op, ARM7TDMI &cpu): OpCode::OpCode(op, cpu) {
    psr = Utils::getRegBits(op, PSR_MASK, PSR_SHIFT);
    Rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
}   

std::string PSRTransferMRS::getPSRMnemonic(){
    return PSR2Mnemonic[psr];
}

std::string PSRTransferMRS::toString(){
    return "mrs" + getCondFieldMnemonic() + " " + getRegMnemonic(Rd) + "," + getPSRMnemonic();
}

void PSRTransferMRS::doDecode(){

}

void PSRTransferMRS::doExecute(){

}

// MSR,MRS          1S
uint32_t PSRTransferMRS::cyclesUsed() const {
    return 1 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE;
}