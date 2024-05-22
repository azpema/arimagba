#include "software_interrupt.hpp"

using namespace ARM;

SoftwareInterrupt::SoftwareInterrupt(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    comment = Utils::getRegBits(op, COMMENT_FIELD_MASK, COMMENT_FIELD_SHIFT);
}   

std::string SoftwareInterrupt::toString(){
    return "swi " + Utils::toHexString(comment);
}

void SoftwareInterrupt::doDecode(){

}

void SoftwareInterrupt::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: SoftwareInterrupt");
}

uint32_t SoftwareInterrupt::cyclesUsed() const {
    return 1;
}