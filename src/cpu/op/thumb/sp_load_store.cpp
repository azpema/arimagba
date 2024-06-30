#include "sp_load_store.hpp"
#include "../arm/single_data_transfer.hpp"
#include "../fields/imm.hpp"

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
    uint8_t i = 0;
    uint8_t p = 1;
    uint8_t u = 1;
    uint8_t b = 0;
    uint8_t w = 0;
    SingleDataTransfer opArm = SingleDataTransfer(i, p, u, b, w, l, 13, rd, offsetVal, cpu);
    std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    opArm.doExecute();  

}

uint32_t SPLoadStore::cyclesUsed() const {
    return 1;
}