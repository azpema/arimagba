#include "software_interrupt.hpp"

using namespace Thumb;

SoftwareInterrupt::SoftwareInterrupt(uint16_t op): ThumbOpCode::ThumbOpCode(op) {
    value8 = Utils::getRegBits(op, VALUE8_MASK, VALUE8_SHIFT);
}   

std::string SoftwareInterrupt::toString(){
    return "swi";
}