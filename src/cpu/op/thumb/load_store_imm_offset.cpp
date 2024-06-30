#include "load_store_imm_offset.hpp"

#include "../arm/single_data_transfer.hpp"
#include "../fields/imm.hpp"

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

// SingleDataTransfer::SingleDataTransfer(uint8_t i, uint8_t p, uint8_t u, uint8_t b, uint8_t w, uint8_t l, 
 //   uint8_t rn, uint8_t rd, uint16_t offset, ARM7TDMI &cpu) : ArmOpcode::ArmOpcode(cpu){

void LoadStoreImmOffset::doExecute(){
    if(l == 0 && b == 1){
        uint8_t i = 0;
        uint8_t p = 1;
        uint8_t u = 1;
        uint8_t b = 1;
        uint8_t w = 0;
        uint8_t l = 0;
        SingleDataTransfer opArm = SingleDataTransfer(i, p, u, b, w, l, rb, rd, offsetVal, cpu);
        std::cout << "<< ARM >> " << opArm.toString() << std::endl;
        opArm.doExecute();  
    }else{
        throw std::runtime_error("TODO: LoadStoreImmOffset::doExecute");
    }

}

uint32_t LoadStoreImmOffset::cyclesUsed() const {
    return 1;
}