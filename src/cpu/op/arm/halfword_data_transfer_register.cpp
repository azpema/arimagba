#include "halfword_data_transfer_register.hpp"


HalfwordDataTransferRegister::HalfwordDataTransferRegister(uint32_t op, ARM7TDMI &cpu): HalfwordDataTransfer::HalfwordDataTransfer(op, cpu) {
    rm = Utils::getRegBits(op, RM_FLAG_MASK, RM_FLAG_SHIFT);
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
    std::cerr << "TODO: HalfwordDataTransferRegister::cyclesUsed" << std::endl;
    return 1;
}

void HalfwordDataTransferRegister::doDecode(){

}

void HalfwordDataTransferRegister::doExecute(){
    uint32_t offsetRegVal = cpu.getReg(rm);
    _doExecute(offsetRegVal);
}
