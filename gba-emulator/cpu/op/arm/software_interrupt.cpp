#include "software_interrupt.hpp"

using namespace ARM;

SoftwareInterrupt::SoftwareInterrupt(uint32_t op): OpCode::OpCode(op) {
    comment = Utils::getRegBits(op, COMMENT_FIELD_MASK, COMMENT_FIELD_SHIFT);
}   

std::string SoftwareInterrupt::toString(){
    return "swi";
}

void SoftwareInterrupt::doExecute(ARM7TDMI &cpu){

}