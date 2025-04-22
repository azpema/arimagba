#include "load_store_imm_offset.hpp"

#include "../arm/single_data_transfer.hpp"
#include "../fields/imm.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

const std::string LoadStoreImmOffset::b2Mnemonic[2] = {"", "b"};
const std::string LoadStoreImmOffset::op2Mnemonic[2] = {"str", "ldr"};

LoadStoreImmOffset::LoadStoreImmOffset(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    init(op);
}

LoadStoreImmOffset::LoadStoreImmOffset(ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(cpu) {}

void LoadStoreImmOffset::init(uint32_t op){
    ThumbOpCode::init(op);
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
    uint8_t i = 0;
    uint8_t p = 1;
    uint8_t u = 1;
    uint8_t w = 0;
    SingleDataTransfer opArm = SingleDataTransfer(i, p, u, b, w, l, rb, rd, offsetVal, cpu);
    //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    opArm.doExecute();  
}

uint32_t LoadStoreImmOffset::cyclesUsed() const {
    return 1;
}