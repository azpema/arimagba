#include "pc_relative_load.hpp"

PCRelativeLoad::PCRelativeLoad(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    word8 = Utils::getRegBits(op, WORD8_MASK, WORD8_SHIFT);
    offset = word8 << 2;
}   



std::string PCRelativeLoad::toString(){
    return "ldr " + OpCode::getRegMnemonic(rd) + ",[pc,#" + Utils::toHexString(offset) + "]";
}

void PCRelativeLoad::doDecode(){

}

void PCRelativeLoad::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: PCRelativeLoad");
}

uint32_t PCRelativeLoad::cyclesUsed() const {
    return 1;
}