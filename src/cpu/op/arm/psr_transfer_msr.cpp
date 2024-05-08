#include "psr_transfer_msr.hpp"
#include <bitset>

PSRTransferMSR::PSRTransferMSR(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    psr = Utils::getRegBits(op, PSR_MASK, PSR_SHIFT);
}

std::string PSRTransferMSR::getPSRMnemonic(){
    return PSR2Mnemonic[psr];
}

bool PSRTransferMSR::isFullTransfer(uint32_t op){
    return OpCode::checkOpCode(op, FULL_TRANSFER_MASK, FULL_TRANSFER_FORMAT);
}

bool PSRTransferMSR::isFlagBitsTransfer(uint32_t op){
    return OpCode::checkOpCode(op, FLAG_BITS_TRANSFER_MASK, FLAG_BITS_TRANSFER_FORMAT);
}

std::string PSRTransferMSR::toString(){
    return "msr" + getCondFieldMnemonic() + " " + getPSRMnemonic();
}

uint32_t PSRTransferMSR::cyclesUsed() const {
    return 1 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE;
}