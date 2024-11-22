#include "rotate_imm.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

//ROR by 32 has result equal to Rm, carry out equal to bit 31 of Rm.
RotateImm::RotateImm(uint16_t val) : Operand::Operand(val, OperandType::ROTATE_IMM){
    rotate = Utils::getRegBits(val, ROTATE_MASK, ROTATE_SHIFT);
    imm = Utils::getRegBits(val, IMM_MASK, IMM_SHIFT);
    // TODO What if ROR#0 is specified
    operandVal = Utils::rotateRight(imm, 2*rotate);
    c = ((operandVal & 0x80000000) >> 31) == 1;
}

RotateImm::RotateImm(uint8_t rot, uint8_t imm) : Operand::Operand(0xDEAD, OperandType::ROTATE_IMM){
    this->rotate = rot;
    this->imm = imm;

    uint32_t raw = 0;
    Utils::setRegBits(raw, ROTATE_MASK, rot);
    Utils::setRegBits(raw, IMM_MASK, imm);
    setRawVal(raw);
}

RotateImm::~RotateImm()
{
}

uint32_t RotateImm::getMnemonicVal(){
    return operandVal;
}

uint32_t RotateImm::getOperandVal(ARM7TDMI &cpu){
    return getMnemonicVal();
}

uint32_t RotateImm::getRorShiftAmount(){
    return rotate;
}

bool RotateImm::getC(){
    return c;
}