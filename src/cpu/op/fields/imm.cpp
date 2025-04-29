#include "imm.hpp"
#include <iostream>

Imm::Imm(uint16_t val) : Operand::Operand(val, OperandType::IMM), c(false), immVal(val) {}

Imm::~Imm() {}

void Imm::init(uint16_t val) {
    Operand::init(val, OperandType::IMM);
    c = false;
    immVal = val;
}

uint32_t Imm::getMnemonicVal() { return immVal; }

uint32_t Imm::getOperandVal(ARM7TDMI& /*cpu*/) { return getMnemonicVal(); }

bool Imm::getC() { return c; }