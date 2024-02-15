#include "rm.hpp"
#include <iostream>

Rm::Rm(uint16_t val) : Operand::Operand(val, OperandType::RM){
    rmVal = Utils::getRegBits(val, RM_MASK, RM_SHIFT);
}

uint16_t Rm::getRmVal(){
    return rmVal;
}
