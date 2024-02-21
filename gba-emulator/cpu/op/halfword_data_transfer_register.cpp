#include "halfword_data_transfer_register.hpp"


HalfwordDataTransferRegister::HalfwordDataTransferRegister(uint32_t op): HalfwordDataTransfer::HalfwordDataTransfer(op) {
    rm = Utils::getRegBits(op, RM_FLAG_MASK, RM_FLAG_SHIFT);
}   

std::string HalfwordDataTransferRegister::toString(){
    std::string base = getOpMnemonic() + getCondFieldMnemonic() + getSFlagMnemonic() + getHFlagMnemonic() +  + " " + \
                        getRegMnemonic(rd) + ",";
    std::string address = "[" + getRegMnemonic(rn);

    if(p == 1){
        address += "," + getUFlagMnemonic() + getRegMnemonic(rm) + "]" + getWFlagMnemonic();
    }else if(p == 0){
        address += "]," + getUFlagMnemonic() + getRegMnemonic(rm);
    }else{
        std::cerr << "ERROR: Invalid p flag value HalfwordDataTransferRegister" << std::endl;
    }

    return base + address;
}


/*#include "halfword_data_transfer_register.hpp"
#include "halfword_data_transfer.hpp"

HalfwordDataTransferRegister::HalfwordDataTransferRegister(uint32_t op): OpCode::OpCode(op) {
    p = Utils::getRegBits(op, P_FLAG_MASK, P_FLAG_SHIFT);
    u = Utils::getRegBits(op, U_FLAG_MASK, U_FLAG_SHIFT);
    w = Utils::getRegBits(op, W_FLAG_MASK, W_FLAG_SHIFT);
    l = Utils::getRegBits(op, L_FLAG_MASK, L_FLAG_SHIFT);
    rn = Utils::getRegBits(op, RN_FLAG_MASK, RN_FLAG_SHIFT);
    rd = Utils::getRegBits(op, RD_FLAG_MASK, RD_FLAG_SHIFT);
    s = Utils::getRegBits(op, S_FLAG_MASK, S_FLAG_SHIFT);
    h = Utils::getRegBits(op, H_FLAG_MASK, H_FLAG_SHIFT);
    rm = Utils::getRegBits(op, RM_FLAG_MASK, RM_FLAG_SHIFT);
}   

std::string HalfwordDataTransferRegister::getUFlagMnemonic(){
    return uFlag2Mnemonic[u];
}

std::string HalfwordDataTransferRegister::getSFlagMnemonic(){
    return sFlag2Mnemonic[s];
}

std::string HalfwordDataTransferRegister::getHFlagMnemonic(){
    return hFlag2Mnemonic[h];
}

std::string HalfwordDataTransferRegister::getWFlagMnemonic(){
    return wFlag2Mnemonic[w];
}

std::string HalfwordDataTransferRegister::getOpMnemonic(){
    return op2Mnemonic[l];
}

std::string HalfwordDataTransferRegister::toString(){
    std::string base = getOpMnemonic() + getCondFieldMnemonic() + getSFlagMnemonic() + getHFlagMnemonic() +  + " " + \
                        getRegMnemonic(rd) + ",";
    std::string address = "[" + getRegMnemonic(rn);

    if(p == 1){
        address += "," + getUFlagMnemonic() + getRegMnemonic(rm) + "]" + getWFlagMnemonic();
    }else if(p == 0){
        address += "]," + getUFlagMnemonic() + getRegMnemonic(rm);
    }else{
        std::cerr << "ERROR: Invalid p flag value HalfwordDataTransferRegister" << std::endl;
    }

    return base + address;
}*/