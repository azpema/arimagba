#include "branch_and_exchange.hpp"


BranchAndExchange::BranchAndExchange(uint32_t op): OpCode::OpCode(op) {
    Rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
}   


std::string BranchAndExchange::toString(){
    return "bx" + getCondFieldMnemonic() + " r" + std::to_string(Rn);
}