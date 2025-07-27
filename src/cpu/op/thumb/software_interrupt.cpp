#include "software_interrupt.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"
#include "../arm/software_interrupt.hpp"

using namespace Thumb;

SoftwareInterrupt::SoftwareInterrupt(uint16_t op, ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(op, cpu) { init(op); }

SoftwareInterrupt::SoftwareInterrupt(ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(cpu) {}

void SoftwareInterrupt::init(uint32_t op) {
    ThumbOpCode::init(op);
    value8 = Utils::getRegBits(op, VALUE8_MASK, VALUE8_SHIFT);
}

std::string SoftwareInterrupt::toString() { return "swi"; }

void SoftwareInterrupt::doDecode() {}

void SoftwareInterrupt::doExecute() {
    uint32_t opcode = 0b11101111000000000000000000000000 | value8;
    ARM::SoftwareInterrupt* opArm = static_cast<ARM::SoftwareInterrupt*>(cpu.getArmPool().getArmInstance(ArmOpcode::OpCodeEnum::SOFTWARE_INTERRUPT));

    opArm->init(opcode);
    opArm->doExecute();
    // std::cout << "<< ARM >> " << opArm.toString() << '\n';
}

uint32_t SoftwareInterrupt::cyclesUsed() const { return 1; }