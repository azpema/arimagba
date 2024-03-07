#include "load_store_sign_extended.hpp"

LoadStoreSignExtended::LoadStoreSignExtended(uint16_t op): ThumbOpCode::ThumbOpCode(op) {
    h = Utils::getRegBits(op, H_MASK, H_SHIFT);
    s = Utils::getRegBits(op, S_MASK, S_SHIFT);
    ro = Utils::getRegBits(op, RO_MASK, RO_SHIFT);
    rb = Utils::getRegBits(op, RB_MASK, RB_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
}

std::string LoadStoreSignExtended::getOpMnemonic(){
    return op2Mnemonic[s][h];
}

std::string LoadStoreSignExtended::toString(){
    return getOpMnemonic() + " " + OpCode::getRegMnemonic(rd) + ",[" + OpCode::getRegMnemonic(rb) + "," + \
            OpCode::getRegMnemonic(ro) + "]";
}