#include "psr_transfer_msr_full.hpp"

PSRTransferMSRFull::PSRTransferMSRFull(uint32_t op, ARM7TDMI &cpu): PSRTransferMSR::PSRTransferMSR(op, cpu) {
    rm = Utils::getRegBits(op, RM_MASK, RM_SHIFT);
}

std::string PSRTransferMSRFull::toString() {
    return PSRTransferMSR::toString() + "," + getRegMnemonic(rm);
}

void PSRTransferMSRFull::doDecode(){
    rmVal = cpu.getReg(rm);
}

void PSRTransferMSRFull::doExecute(){

    switch(psr){
        // CPSR
        case 0:
            cpu.setCPSR(rmVal);
            break;

        // SPSR_mode
        case 1:
            cpu.setSPSR(rmVal);
            break;
    }
}

// MSR,MRS          1S
uint32_t PSRTransferMSRFull::cyclesUsed() const {
    return 1 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE;
}