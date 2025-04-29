#include "long_branch_with_link.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

using namespace Thumb;

LongBranchWithLink::LongBranchWithLink(uint16_t op, ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(op, cpu) { init(op); }

LongBranchWithLink::LongBranchWithLink(ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(cpu) {}

void LongBranchWithLink::init(uint32_t op) {
    ThumbOpCode::init(op);
    _type = ThumbOpCode::OpCodeEnum::LONG_BRANCH_WITH_LINK;
    h = Utils::getRegBits(op, H_MASK, H_SHIFT);
    offset = Utils::getRegBits(op, OFFSET_MASK, OFFSET_SHIFT);
    // TODO fix possible bug
    if (h == 0) {
        offsetVal = offset << 12;
    } else if (h == 1) {
        offsetVal = offset << 1;
        uint32_t a = cpu.getReg(14) - cpu.getReg(15);
        uint32_t b = a + offsetVal;
        realOffsetVal = Utils::twosComplementExtendSignTo(b, 23, 32);
    } else {
        throw std::runtime_error("ERROR: Invalid h value in LongBranchWithLink::LongBranchWithLink");
    }
}

std::string LongBranchWithLink::toString() {
    // TODO set proper decoded value
    if (h == 0)
        return "bl h=" + std::to_string(h) + " " + std::to_string(offsetVal);
    else
        return "bl h=" + std::to_string(h) + " " + Utils::toHexString(cpu.getPC() - 2 + realOffsetVal);
}

void LongBranchWithLink::doDecode() {}

void LongBranchWithLink::doExecute() {
    if (h == 0) {
        cpu.setLR(cpu.getPC() + offsetVal);
    } else if (h == 1) {
        // PC is 4 steps ahead (THUMB)
        uint32_t temp = cpu.getPC() - 2;
        cpu.setPC(temp + realOffsetVal);
        cpu.setLR(temp | 1);
        cpu.setMustFlushPipeline(true);
    } else {
        std::runtime_error("ERROR: Invalid h value in LongBranchWithLink::doExecute");
    }
}

uint32_t LongBranchWithLink::cyclesUsed() const { return 1; }