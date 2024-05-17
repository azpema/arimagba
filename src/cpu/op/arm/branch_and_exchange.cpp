#include "branch_and_exchange.hpp"

BranchAndExchange::BranchAndExchange(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    Rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
    mustFlushPipeline = true;
}   

std::string BranchAndExchange::toString(){
    return "bx" + getCondFieldMnemonic() + " " + OpCode::getRegMnemonic(Rn);
}

void BranchAndExchange::doDecode(){

}

void BranchAndExchange::doExecute(){
    uint32_t rnVal = cpu.getReg(Rn);
    cpu.setPC(rnVal);
    cpu.getCPSR().setTFlag(rnVal & 0x1 == 1);
    mustFlushPipeline = true;
}

// A Branch and Exchange operation takes 3 cycles and is similar to a Branch
uint32_t BranchAndExchange::cyclesUsed() const {
    std::cerr << "TODO: BranchAndExchange::cyclesUsed" << std::endl;
    return 3;
}
