#include "move_shifted_register.hpp"

MoveShiftedRegister::MoveShiftedRegister(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    opField = Utils::getRegBits(op, OP_MASK, OP_SHIFT);
    offset5 = Utils::getRegBits(op, OFFSET5_MASK, OFFSET5_SHIFT);
    rs = Utils::getRegBits(op, RS_MASK, RS_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
}   

std::string MoveShiftedRegister::getOpMnemonic(){
    return op2Mnemonic[opField];
}


std::string MoveShiftedRegister::toString(){
    return getOpMnemonic() + " " + OpCode::getRegMnemonic(rd) + "," + OpCode::getRegMnemonic(rs) + ",#" + Utils::toHexString(offset5, 4);
}

void MoveShiftedRegister::doDecode(){

}

void MoveShiftedRegister::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: MoveShiftedRegister");
    //OpCode armOp = DataProcessing(op, cpu);
    
}

uint32_t MoveShiftedRegister::cyclesUsed() const {
    return 1;
}