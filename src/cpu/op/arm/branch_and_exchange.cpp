#include "branch_and_exchange.hpp"


BranchAndExchange::BranchAndExchange(uint32_t op, ARM7TDMI &cpu): OpCode::OpCode(op, cpu) {
    Rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
    mustFlushPipeline = true;
}   


std::string BranchAndExchange::toString(){
    return "bx" + getCondFieldMnemonic() + " " + getRegMnemonic(Rn);
}

void BranchAndExchange::doDecode(){

}

void BranchAndExchange::doExecute(){

}

// A Branch and Exchange operation takes 3 cycles and is similar to a Branch
uint32_t BranchAndExchange::cyclesUsed() const {
    std::cerr << "TODO: BranchAndExchange::cyclesUsed" << std::endl;
    return 3;
}

