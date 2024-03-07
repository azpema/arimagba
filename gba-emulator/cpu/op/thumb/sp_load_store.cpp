#include "sp_load_store.hpp"

SPLoadStore::SPLoadStore(uint16_t op): ThumbOpCode::ThumbOpCode(op) {
    l = Utils::getRegBits(op, L_MASK, L_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    word8 = Utils::getRegBits(op, WORD8_MASK, WORD8_SHIFT);
    offsetVal = word8 << 2;
}

std::string SPLoadStore::getLMnemonic(){
    return l2Mnemonic[l];
}

std::string SPLoadStore::toString(){
    return getLMnemonic() + " " + OpCode::getRegMnemonic(rd) + ",[" + OpCode::getRegMnemonic(13) + \
            ",#" + Utils::toHexString(offsetVal) + "]";
}