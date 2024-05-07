#include "load_address.hpp"

LoadAddress::LoadAddress(uint16_t op): ThumbOpCode::ThumbOpCode(op) {
    sp = Utils::getRegBits(op, SP_MASK, SP_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    word8 = Utils::getRegBits(op, WORD8_MASK, WORD8_SHIFT);
    offsetVal = word8 << 2;
}

uint8_t LoadAddress::getSPRegVal(){
    return sp2RegVal[sp];
}

std::string LoadAddress::toString(){
    return "add " + OpCode::getRegMnemonic(rd) + "," + OpCode::getRegMnemonic(getSPRegVal()) + \
            ",#" + Utils::toHexString(offsetVal); 
}