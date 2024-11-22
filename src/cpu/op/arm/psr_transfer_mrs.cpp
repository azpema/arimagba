#include "psr_transfer_mrs.hpp"
#include <bitset>
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

const std::string PSRTransferMRS::PSR2Mnemonic[2] = {"cpsr", "spsr"};

PSRTransferMRS::PSRTransferMRS(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    init(op);
}

PSRTransferMRS::PSRTransferMRS(ARM7TDMI &cpu): ArmOpcode::ArmOpcode(cpu) {}

void PSRTransferMRS::init(uint32_t op){
    ArmOpcode::init(op);
    psr = Utils::getRegBits(op, PSR_MASK, PSR_SHIFT);
    Rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
}   

std::string PSRTransferMRS::getPSRMnemonic(){
    return PSR2Mnemonic[psr];
}

std::string PSRTransferMRS::toString(){
    return "mrs" + getCondFieldMnemonic() + " " + OpCode::getRegMnemonic(Rd) + "," + getPSRMnemonic();
}

void PSRTransferMRS::doDecode(){

}

void PSRTransferMRS::doExecute(){
    if(psr == 0){
        cpu.setReg(Rd, cpu.getCPSR().getValue());
    }else if(psr == 1){
        cpu.setReg(Rd, cpu.getSPSR().getValue());
    }


}

// MSR,MRS          1S
uint32_t PSRTransferMRS::cyclesUsed() const {
    return 1 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE;
}