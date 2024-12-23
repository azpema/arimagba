#include "rm.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

Rm::Rm(uint16_t val) : Operand::Operand(val, OperandType::RM), rmVal(Utils::getRegBits(val, RM_MASK, RM_SHIFT)){}

Rm::~Rm()
{
}

void Rm::init(uint16_t val){
    Operand::init(val, OperandType::RM);
    rmVal = Utils::getRegBits(val, RM_MASK, RM_SHIFT);
}

uint32_t Rm::getOperandVal(ARM7TDMI &cpu){
    return cpu.getReg(rmVal);
}

uint16_t Rm::getRmVal(){
    return value;
}

bool Rm::getC(){
    return c;
}
