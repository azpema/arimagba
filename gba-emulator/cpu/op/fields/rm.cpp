#include "rm.hpp"
#include <iostream>

Rm::Rm(uint16_t val) : Operand::Operand(val, OperandType::RM){
    rmVal = Utils::getRegBits(val, RM_MASK, RM_SHIFT);
}

uint32_t Rm::getOperandVal(ARM7TDMI &cpu){
    return cpu.getReg(rmVal);
}

uint16_t Rm::getRmVal(){
    return rmVal;
}

bool Rm::getC(){
    return c;
}
