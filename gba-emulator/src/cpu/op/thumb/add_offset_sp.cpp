#include "add_offset_sp.hpp"

AddOffsetSP::AddOffsetSP(uint16_t op): ThumbOpCode::ThumbOpCode(op) {
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