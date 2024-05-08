#include "load_store_register_offset.hpp"

LoadStoreRegisterOffset::LoadStoreRegisterOffset(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    l = Utils::getRegBits(op, L_MASK, L_SHIFT);
    b = Utils::getRegBits(op, B_MASK, B_SHIFT);
    r0 = Utils::getRegBits(op, R0_MASK, R0_SHIFT);
    rb = Utils::getRegBits(op, RB_MASK, RB_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
}   

std::string LoadStoreRegisterOffset::getBFlagMnemonic(){
    return bFlag2Mnemonic[b];
}

std::string LoadStoreRegisterOffset::getOpMnemonic(){
    return op2Mnemonic[l];
}

std::string LoadStoreRegisterOffset::toString(){
    return getOpMnemonic() + getBFlagMnemonic() + " " + OpCode::getRegMnemonic(rd) + ",[" + \
            OpCode::getRegMnemonic(rb) + "," + OpCode::getRegMnemonic(r0) + "]";
}

void LoadStoreRegisterOffset::doDecode(){

}

void LoadStoreRegisterOffset::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: LoadStoreRegisterOffset");
}

uint32_t LoadStoreRegisterOffset::cyclesUsed() const {
    return 1;
}