#include "hi_register_branch_exchange.hpp"

HiRegisterBranchExchange::HiRegisterBranchExchange(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    opField = Utils::getRegBits(op, OP_MASK, OP_SHIFT);
    h1 = Utils::getRegBits(op, H1_MASK, H1_SHIFT);
    h2 = Utils::getRegBits(op, H2_MASK, H2_SHIFT);
    rsHs = Utils::getRegBits(op, RS_HS_MASK, RS_HS_SHIFT);
    rdHd = Utils::getRegBits(op, RD_HD_MASK, RD_HD_SHIFT);
    rs = rsHs + (h2 * 8);
    rd = rdHd + (h1 * 8);
}   

std::string HiRegisterBranchExchange::getOpMnemonic(){
    return op2Mnemonic[opField];
}

std::string HiRegisterBranchExchange::toString(){
    std::string mnemonic = getOpMnemonic() + " ";
    // bx
    if(opField != 0b11){
        mnemonic += OpCode::getRegMnemonic(rd) + ",";
    }
    mnemonic += OpCode::getRegMnemonic(rs);
    return mnemonic;
}

void HiRegisterBranchExchange::doDecode(){

}

void HiRegisterBranchExchange::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: HiRegisterBranchExchange");
}

uint32_t HiRegisterBranchExchange::cyclesUsed() const {
    return 1;
}