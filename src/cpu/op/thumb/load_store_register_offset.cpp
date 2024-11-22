#include "load_store_register_offset.hpp"
#include "../arm/single_data_transfer.hpp"
#include "../fields/shift_rm.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

const std::string LoadStoreRegisterOffset::bFlag2Mnemonic[2] = {"", "b"};
const std::string LoadStoreRegisterOffset::op2Mnemonic[2] = {"str", "ldr"};

LoadStoreRegisterOffset::LoadStoreRegisterOffset(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    init(op);
}

LoadStoreRegisterOffset::LoadStoreRegisterOffset(ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(cpu) {}

void LoadStoreRegisterOffset::init(uint32_t op){
    ThumbOpCode::init(op);
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
    uint8_t i = 1;
    uint8_t p = 1;
    uint8_t u = 1;
    uint8_t w = 0;

    SingleDataTransfer opArm = SingleDataTransfer(i, p, u, b, w, l, rb, rd, r0, cpu);
    //std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    opArm.doExecute();  
}

uint32_t LoadStoreRegisterOffset::cyclesUsed() const {
    return 1;
}