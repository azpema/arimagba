#include "move_comp_add_sub_imm.hpp"

MoveCompAddSubImm::MoveCompAddSubImm(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
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

void MoveCompAddSubImm::doDecode(){

}

void MoveCompAddSubImm::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: MoveCompAddSubImm");
}

uint32_t MoveCompAddSubImm::cyclesUsed() const {
    return 1;
}