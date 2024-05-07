#include "software_interrupt.hpp"

using namespace ARM;

SoftwareInterrupt::SoftwareInterrupt(uint32_t op, ARM7TDMI &cpu): OpCode::OpCode(op, cpu) {
    comment = Utils::getRegBits(op, COMMENT_FIELD_MASK, COMMENT_FIELD_SHIFT);
}   

std::string SoftwareInterrupt::toString(){
    return "swi";
}

void SoftwareInterrupt::doDecode(){

}

void SoftwareInterrupt::doExecute(){

}

bool SoftwareInterrupt::mustFlushPipeline() const {
    return false;
}

uint32_t SoftwareInterrupt::cyclesUsed() const {
    return 1;
}