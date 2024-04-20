#include "psr_transfer_msr_full.hpp"

PSRTransferMSRFull::PSRTransferMSRFull(uint32_t op): PSRTransferMSR::PSRTransferMSR(op) {
    Rm = Utils::getRegBits(op, RM_MASK, RM_SHIFT);
}

std::string PSRTransferMSRFull::toString() {
    return PSRTransferMSR::toString() + "," + getRegMnemonic(Rm);
}

void PSRTransferMSRFull::doExecute(ARM7TDMI &cpu){

}

bool PSRTransferMSRFull::mustFlushPipeline() const {
    return false;
}

// MSR,MRS          1S
uint32_t PSRTransferMSRFull::cyclesUsed() const {
    return 1 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE;
}