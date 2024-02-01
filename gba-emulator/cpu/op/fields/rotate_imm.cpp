#include "rotate_imm.hpp"
#include <iostream>

RotateImm::RotateImm(uint16_t val) : Operand::Operand(val, OperandType::ROTATE_IMM){
    rotate = Utils::getRegBits(val, ROTATE_MASK, ROTATE_SHIFT);
    imm = Utils::getRegBits(val, IMM_MASK, IMM_SHIFT);
    operandVal = Utils::rotateRight(imm, 2*rotate);
}

uint16_t RotateImm::getOperandVal(){
    return operandVal;
}