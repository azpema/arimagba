#include "shift_rm.hpp"
#include <iostream>

ShiftRm::ShiftRm(uint16_t val) : Operand::Operand(val, OperandType::SHIFT_RM){
    Shift = Utils::getRegBits(val, SHIFT_MASK, SHIFT_SHIFT);
    Rm = Utils::getRegBits(val, RM_MASK, RM_SHIFT);
    shiftType = Utils::getRegBits(Shift, SHIFT_TYPE_MASK, SHIFT_TYPE_SHIFT);

    if((Shift & 0b00000001) == 0b00000000){
        shiftAmount = Utils::getRegBits(Shift, SHIFT_AMOUNT_MASK, SHIFT_AMOUNT_SHIFT);
        _type = AMOUNT;
    }else if((Shift & 0b00001001) == 0b00000001){
        shiftReg = Utils::getRegBits(Shift, SHIFT_RS_MASK, SHIFT_RS_SHIFT);
        _type = REGISTER;
    }else{
        std::cerr << "ERROR: Unknown ShiftRm" << std::endl;
    }
}

std::string ShiftRm::getShiftTypeMnemonic(){
    return shiftType2Mnemonic[shiftType];
}

uint16_t ShiftRm::getRm(){
    return Rm;
}

uint16_t ShiftRm::getShiftAmount(){
    return shiftAmount;
}