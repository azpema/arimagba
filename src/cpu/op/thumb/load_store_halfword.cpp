#include "load_store_halfword.hpp"
#include "../arm/halfword_data_transfer_offset.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

const std::string LoadStoreHalfword::op2Mnemonic[2] = {"strh", "ldrh"};

LoadStoreHalfword::LoadStoreHalfword(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    init(op);
}

LoadStoreHalfword::LoadStoreHalfword(ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(cpu) {}

void LoadStoreHalfword::init(uint32_t op){
    ThumbOpCode::init(op);
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
    HalfwordDataTransferOffset opArm = HalfwordDataTransferOffset(1, 1, 0, l, rb, rd, 0, 1, (offsetVal & 0xF0) >> 4 , offsetVal & 0xF, cpu);
    opArm.doExecute();
    //std::cout << "<< ARM >> " << opArm.toString() << std::endl;
}

uint32_t LoadStoreHalfword::cyclesUsed() const {
    return 1;
}