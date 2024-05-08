#include "alu_operations.hpp"

ALUOperations::ALUOperations(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    opField = Utils::getRegBits(op, OP_MASK, OP_SHIFT);
    rs = Utils::getRegBits(op, RS_MASK, RS_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
}   

std::string ALUOperations::getOpMnemonic(){
    return op2Mnemonic[opField];
}

std::string ALUOperations::toString(){
    return getOpMnemonic() + " " + OpCode::getRegMnemonic(rd) + "," + OpCode::getRegMnemonic(rs);
}

void ALUOperations::doDecode(){

}

void ALUOperations::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: ALUOperations");
}

uint32_t ALUOperations::cyclesUsed() const {
    return 1;
}