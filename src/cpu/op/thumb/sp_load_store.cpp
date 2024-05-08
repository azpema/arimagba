#include "sp_load_store.hpp"

SPLoadStore::SPLoadStore(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
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

void SPLoadStore::doDecode(){

}

void SPLoadStore::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: SPLoadStore");
}

uint32_t SPLoadStore::cyclesUsed() const {
    return 1;
}