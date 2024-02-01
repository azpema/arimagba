#include "imm.hpp"
#include <iostream>

Imm::Imm(uint16_t val) : Operand::Operand(val, OperandType::IMM){
    immVal = val;
}

uint16_t Imm::getImmVal(){
    return immVal;
}
