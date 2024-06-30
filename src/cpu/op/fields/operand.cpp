#include "operand.hpp"

Operand::Operand(uint16_t val, OperandType _type) : val(val), _type(_type) {}

void Operand::setRawVal(uint16_t val) {
    this->val = val;
}

uint16_t Operand::getRawVal() {
    return val;
}