#include "software_interrupt.hpp"
#include "../arm/software_interrupt.hpp"

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
    uint32_t opcode = 0b11101111000000000000000000000000 | value8;
    ARM::SoftwareInterrupt opArm = ARM::SoftwareInterrupt(opcode, cpu);
    std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    opArm.doExecute();
}

uint32_t SoftwareInterrupt::cyclesUsed() const {
    return 1;
}