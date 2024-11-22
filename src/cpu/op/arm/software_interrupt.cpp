#include "software_interrupt.hpp"

#include "../../components/exception_handler.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

using namespace ARM;

SoftwareInterrupt::SoftwareInterrupt(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    init(op);
}

SoftwareInterrupt::SoftwareInterrupt(ARM7TDMI &cpu): ArmOpcode::ArmOpcode(cpu) {

}

void SoftwareInterrupt::init(uint32_t op) {
    ArmOpcode::init(op);
    comment = Utils::getRegBits(op, COMMENT_FIELD_MASK, COMMENT_FIELD_SHIFT);
}

std::string SoftwareInterrupt::toString(){
    return "swi " + Utils::toHexString(comment);
}

void SoftwareInterrupt::doDecode(){

}

void SoftwareInterrupt::doExecute(){
    cpu.getExceptionHandler().raiseException(ExceptionHandler::Exception::SWI);
}

uint32_t SoftwareInterrupt::cyclesUsed() const {
    return 1;
}