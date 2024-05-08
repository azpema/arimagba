#include "load_store_halfword.hpp"

LoadStoreHalfword::LoadStoreHalfword(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    l = Utils::getRegBits(op, L_MASK, L_SHIFT);
    offset5 = Utils::getRegBits(op, OFFSET5_MASK, OFFSET5_SHIFT);
    rb = Utils::getRegBits(op, RB_MASK, RB_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    offsetVal = offset5 << 1;
}

std::string LoadStoreHalfword::getOpMnemonic(){
    return op2Mnemonic[l];
}

std::string LoadStoreHalfword::toString(){
    return getOpMnemonic() + " " + OpCode::getRegMnemonic(rd) + ",[" + OpCode::getRegMnemonic(rb) + \
            ",#" + Utils::toHexString(offsetVal) + "]";
}

void LoadStoreHalfword::doDecode(){

}

void LoadStoreHalfword::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: LoadStoreHalfword");
}

uint32_t LoadStoreHalfword::cyclesUsed() const {
    return 1;
}