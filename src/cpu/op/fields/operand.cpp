#include "operand.hpp"

Operand::Operand(uint16_t val, OperandType _type) : value(val), _type(_type) {}

void Operand::setRawVal(uint16_t val) {
    this->value = val;
}

uint16_t Operand::getRawVal() {
    return value;
}