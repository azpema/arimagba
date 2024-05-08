#include "add_offset_sp.hpp"

AddOffsetSP::AddOffsetSP(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    s = Utils::getRegBits(op, S_MASK, S_SHIFT);
    sWord7 = Utils::getRegBits(op, SWORD7_MASK, SWORD7_SHIFT);
    offsetStr = (sWord7 << 2);
    if(s == 1)
        offsetVal = -offsetStr;
    else
        offsetVal = offsetStr;
}

std::string AddOffsetSP::getSMnemonic(){
    return s2Mnemonic[s];
}

std::string AddOffsetSP::toString(){
    return "add sp,#" + getSMnemonic() + Utils::toHexString(offsetStr);
}

void AddOffsetSP::doDecode(){

}

void AddOffsetSP::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: AddOffsetSP");
}

uint32_t AddOffsetSP::cyclesUsed() const {
    return 1;
}