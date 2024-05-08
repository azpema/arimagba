#include "load_store_imm_offset.hpp"

LoadStoreImmOffset::LoadStoreImmOffset(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    b = Utils::getRegBits(op, B_MASK, B_SHIFT);
    l = Utils::getRegBits(op, L_MASK, L_SHIFT);
    offset5 = Utils::getRegBits(op, OFFSET5_MASK, OFFSET5_SHIFT);
    rb = Utils::getRegBits(op, RB_MASK, RB_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);

    if(b == 0)
        offsetVal = offset5 << 2;
    else
        offsetVal = offset5;
}

std::string LoadStoreImmOffset::getOpMnemonic(){
    return op2Mnemonic[l];
}

std::string LoadStoreImmOffset::getBMnemonic(){
    return b2Mnemonic[b];
}

std::string LoadStoreImmOffset::toString(){
    return getOpMnemonic() + getBMnemonic() + " " + OpCode::getRegMnemonic(rd) + ",[" + OpCode::getRegMnemonic(rb) + \
            ",#" + Utils::toHexString(offsetVal) + "]";
}

void LoadStoreImmOffset::doDecode(){

}

void LoadStoreImmOffset::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: LoadStoreImmOffset");
}

uint32_t LoadStoreImmOffset::cyclesUsed() const {
    return 1;
}