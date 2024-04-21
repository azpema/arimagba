#include "software_interrupt.hpp"

using namespace ARM;

SoftwareInterrupt::SoftwareInterrupt(uint32_t op): OpCode::OpCode(op) {
    comment = Utils::getRegBits(op, COMMENT_FIELD_MASK, COMMENT_FIELD_SHIFT);
}   

std::string SoftwareInterrupt::toString(){
    return "swi";
}

void SoftwareInterrupt::doDecode(ARM7TDMI &cpu){

}

void SoftwareInterrupt::doExecute(ARM7TDMI &cpu){

}

bool SoftwareInterrupt::mustFlushPipeline() const {
    return false;
}

uint32_t SoftwareInterrupt::cyclesUsed() const {
    return 1;
}