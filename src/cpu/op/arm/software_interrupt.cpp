#include "software_interrupt.hpp"

#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"
#include "../../components/exception_handler.hpp"

using namespace ARM;

SoftwareInterrupt::SoftwareInterrupt(uint32_t op, ARM7TDMI& cpu) : ArmOpcode::ArmOpcode(op, cpu) { init(op); }

SoftwareInterrupt::SoftwareInterrupt(ARM7TDMI& cpu) : ArmOpcode::ArmOpcode(cpu) {}

void SoftwareInterrupt::init(uint32_t op) {
    ArmOpcode::init(op);
    comment = Utils::getRegBits(op, COMMENT_FIELD_MASK, COMMENT_FIELD_SHIFT);
}

std::string SoftwareInterrupt::toString() { return "swi " + Utils::toHexString(comment); }

void SoftwareInterrupt::doDecode() {}

void SoftwareInterrupt::doExecute() { cpu.getExceptionHandler().raiseException(ExceptionHandler::Exception::SWI); }

uint32_t SoftwareInterrupt::cyclesUsed() const { 
    // Software interrupt instructions take 2S + 1N incremental cycles 
    return 2 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE + ARM7TDMI::CPU_CYCLES_PER_N_CYCLE;
}