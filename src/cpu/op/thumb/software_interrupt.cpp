#include "software_interrupt.hpp"

using namespace Thumb;

SoftwareInterrupt::SoftwareInterrupt(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    value8 = Utils::getRegBits(op, VALUE8_MASK, VALUE8_SHIFT);
}   

std::string SoftwareInterrupt::toString(){
    return "swi";
}


void SoftwareInterrupt::doDecode(){

}

void SoftwareInterrupt::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: SoftwareInterrupt");
}

uint32_t SoftwareInterrupt::cyclesUsed() const {
    return 1;
}