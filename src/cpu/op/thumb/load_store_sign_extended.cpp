#include "load_store_sign_extended.hpp"
#include "../arm/halfword_data_transfer_register.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

const std::string LoadStoreSignExtended::op2Mnemonic[2][2] = {{"strh", "ldrh"},
                                                              {"ldsb", "ldsh"}};

LoadStoreSignExtended::LoadStoreSignExtended(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    h = Utils::getRegBits(op, H_MASK, H_SHIFT);
    s = Utils::getRegBits(op, S_MASK, S_SHIFT);
    ro = Utils::getRegBits(op, RO_MASK, RO_SHIFT);
    rb = Utils::getRegBits(op, RB_MASK, RB_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
}

LoadStoreSignExtended::LoadStoreSignExtended(ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(cpu) {}

void LoadStoreSignExtended::init(uint32_t op){
    ThumbOpCode::init(op);
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

void LoadStoreSignExtended::doDecode(){

}

void LoadStoreSignExtended::doExecute(){
    if(h == 0 && s == 0){
        HalfwordDataTransferRegister opArm = HalfwordDataTransferRegister(1, 1, 0, 0, rb, rd, 0, 1, ro, cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(h == 1 && s == 0){
        HalfwordDataTransferRegister opArm = HalfwordDataTransferRegister(1, 1, 0, 1, rb, rd, 0, 1, ro, cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(h == 0 && s == 1){
        HalfwordDataTransferRegister opArm = HalfwordDataTransferRegister(1, 1, 0, 1, rb, rd, s, h, ro, cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(s == 1 && h == 1){
        HalfwordDataTransferRegister opArm = HalfwordDataTransferRegister(1, 1, 0, 1, rb, rd, s, h, ro, cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else{
        throw std::runtime_error("TODO");
    }
}

uint32_t LoadStoreSignExtended::cyclesUsed() const {
    return 1;
}