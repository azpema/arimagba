#include "halfword_data_transfer.hpp"
#include "../opcode.hpp"

HalfwordDataTransfer::HalfwordDataTransfer(uint32_t op, ARM7TDMI &cpu): OpCode::OpCode(op, cpu) {
    p = Utils::getRegBits(op, P_FLAG_MASK, P_FLAG_SHIFT);
    u = Utils::getRegBits(op, U_FLAG_MASK, U_FLAG_SHIFT);
    w = Utils::getRegBits(op, W_FLAG_MASK, W_FLAG_SHIFT);
    l = Utils::getRegBits(op, L_FLAG_MASK, L_FLAG_SHIFT);
    rn = Utils::getRegBits(op, RN_FLAG_MASK, RN_FLAG_SHIFT);
    rd = Utils::getRegBits(op, RD_FLAG_MASK, RD_FLAG_SHIFT);
    s = Utils::getRegBits(op, S_FLAG_MASK, S_FLAG_SHIFT);
    h = Utils::getRegBits(op, H_FLAG_MASK, H_FLAG_SHIFT);
}   

std::string HalfwordDataTransfer::getUFlagMnemonic(){
    return uFlag2Mnemonic[u];
}

std::string HalfwordDataTransfer::getSFlagMnemonic(){
    return sFlag2Mnemonic[s];
}

std::string HalfwordDataTransfer::getHFlagMnemonic(){
    return hFlag2Mnemonic[h];
}

std::string HalfwordDataTransfer::getWFlagMnemonic(){
    return wFlag2Mnemonic[w];
}

std::string HalfwordDataTransfer::getOpMnemonic(){
    return op2Mnemonic[l];
}

/*std::string HalfwordDataTransfer::toString(){
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