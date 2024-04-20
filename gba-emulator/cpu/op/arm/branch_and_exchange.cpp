#include "branch_and_exchange.hpp"


BranchAndExchange::BranchAndExchange(uint32_t op): OpCode::OpCode(op) {
    Rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
}   


std::string BranchAndExchange::toString(){
    return "bx" + getCondFieldMnemonic() + " " + getRegMnemonic(Rn);
}

void BranchAndExchange::doExecute(ARM7TDMI &cpu){

}

bool BranchAndExchange::mustFlushPipeline() const{
    return true;
}

// A Branch and Exchange operation takes 3 cycles and is similar to a Branch
uint32_t BranchAndExchange::cyclesUsed() const {
    std::cerr << "TODO: BranchAndExchange::cyclesUsed" << std::endl;
    return 3;
}

