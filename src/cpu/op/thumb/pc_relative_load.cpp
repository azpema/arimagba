#include "pc_relative_load.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"
#include "../arm/single_data_transfer.hpp"

PCRelativeLoad::PCRelativeLoad(uint16_t op, ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(op, cpu) { init(op); }

PCRelativeLoad::PCRelativeLoad(ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(cpu) {}

void PCRelativeLoad::init(uint32_t op) {
    ThumbOpCode::init(op);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    word8 = Utils::getRegBits(op, WORD8_MASK, WORD8_SHIFT);
    offset = word8 << 2;
}

std::string PCRelativeLoad::toString() {
    return "ldr " + OpCode::getRegMnemonic(rd) + ",[pc,#" + Utils::toHexString(offset) + "]";
}

void PCRelativeLoad::doDecode() {}

void PCRelativeLoad::doExecute() {
    uint8_t i = 0;
    uint8_t p = 1;
    uint8_t u = 1;
    uint8_t b = 0;
    uint8_t w = 0;
    uint8_t l = 1;
    bool forcePcBit1To0 = true;
    SingleDataTransfer opArm = SingleDataTransfer(i, p, u, b, w, l, 15, rd, offset, cpu, forcePcBit1To0);
    // std::cout << "<< ARM >> " << opArm.toString() << '\n';
    opArm.doExecute();
}

uint32_t PCRelativeLoad::cyclesUsed() const { return 1; }