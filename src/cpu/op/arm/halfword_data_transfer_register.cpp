#include "halfword_data_transfer_register.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

HalfwordDataTransferRegister::HalfwordDataTransferRegister(uint32_t op, ARM7TDMI &cpu): HalfwordDataTransfer::HalfwordDataTransfer(op, cpu) {
    init(op);
}

HalfwordDataTransferRegister::HalfwordDataTransferRegister(uint16_t p, uint16_t u, uint16_t w, uint16_t l, uint16_t rn,
 uint16_t rd, uint16_t s, uint16_t h, uint16_t rm, ARM7TDMI &cpu): HalfwordDataTransfer::HalfwordDataTransfer(p, u, w, l, rn, rd, s, h, cpu){
    initRm(rm);
}

HalfwordDataTransferRegister::HalfwordDataTransferRegister(ARM7TDMI &cpu): HalfwordDataTransfer::HalfwordDataTransfer(cpu) {}

void HalfwordDataTransferRegister::init(uint32_t op){
    HalfwordDataTransfer::init(op);
    rm = Utils::getRegBits(op, RM_FLAG_MASK, RM_FLAG_SHIFT);
}

void HalfwordDataTransferRegister::initRm(uint16_t rm){
    this->rm = rm;
}

std::string HalfwordDataTransferRegister::toString(){
    std::string base = getOpMnemonic() + getCondFieldMnemonic() + getSFlagMnemonic() + getHFlagMnemonic() +  + " " + \
                        OpCode::getRegMnemonic(rd) + ",";
    std::string address = "[" + OpCode::getRegMnemonic(rn);

    if(p == 1){
        address += "," + getUFlagMnemonic() + OpCode::getRegMnemonic(rm) + "]" + getWFlagMnemonic();
    }else if(p == 0){
        address += "]," + getUFlagMnemonic() + OpCode::getRegMnemonic(rm);
    }else{
        std::cerr << "ERROR: Invalid p flag value HalfwordDataTransferRegister" << std::endl;
    }

    return base + address;
}

// Depends on load or store
uint32_t HalfwordDataTransferRegister::cyclesUsed() const {
    //std::cerr << "TODO: HalfwordDataTransferRegister::cyclesUsed" << std::endl;
    return 1;
}

void HalfwordDataTransferRegister::doDecode(){

}

void HalfwordDataTransferRegister::doExecute(){
    uint32_t offsetRegVal = cpu.getReg(rm);
    _doExecute(offsetRegVal);
}
