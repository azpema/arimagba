#include "alu_operations.hpp"

ALUOperations::ALUOperations(uint16_t op): ThumbOpCode::ThumbOpCode(op) {
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