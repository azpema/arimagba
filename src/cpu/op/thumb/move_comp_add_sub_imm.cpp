#include "move_comp_add_sub_imm.hpp"

MoveCompAddSubImm::MoveCompAddSubImm(uint16_t op): ThumbOpCode::ThumbOpCode(op) {
    opField = Utils::getRegBits(op, OP_MASK, OP_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    offset8 = Utils::getRegBits(op, OFFSET8_MASK, OFFSET8_SHIFT);
}   

std::string MoveCompAddSubImm::getOpMnemonic(){
    return op2Mnemonic[opField];
}


std::string MoveCompAddSubImm::toString(){
    return getOpMnemonic() + " " + OpCode::getRegMnemonic(rd) + ",#" + Utils::toHexString(offset8, 4);
}