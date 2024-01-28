#include "single_data_transfer.hpp"

SingleDataTransfer::SingleDataTransfer(uint32_t op): OpCode::OpCode(op) {
    I = Utils::getRegBits(op, I_MASK, I_SHIFT);
    P = Utils::getRegBits(op, P_MASK, P_SHIFT);
    U = Utils::getRegBits(op, U_MASK, U_SHIFT);
    B = Utils::getRegBits(op, B_MASK, B_SHIFT);
    W = Utils::getRegBits(op, W_MASK, W_SHIFT);
    L = Utils::getRegBits(op, L_MASK, L_SHIFT);
    Rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
    Rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    offsetField = Utils::getRegBits(op, OFFSET_MASK, OFFSET_SHIFT);

    if(I == 0){
        Imm = offsetField;
    }else if(I == 1){
        Shift = Utils::getRegBits(offsetField, SHIFT_MASK, SHIFT_SHIFT);
        Rm = Utils::getRegBits(offsetField, RM_MASK, RM_SHIFT);
    }
}   

std::string SingleDataTransfer::getUFlagMnemonic(){
    return UFlag2Mnemonic[U];
}

std::string SingleDataTransfer::getBFlagMnemonic(){
    return BFlag2Mnemonic[B];
}

std::string SingleDataTransfer::getWFlagMnemonic(){
    return WFlag2Mnemonic[W];
}

std::string SingleDataTransfer::getLFlagMnemonic(){
    return LFlag2Mnemonic[L];
}

std::string SingleDataTransfer::toString(){
    std::string base = getLFlagMnemonic() + getBFlagMnemonic() + getCondFieldMnemonic() \
            + " r" + std::to_string(Rd) + ",";
    std::string writeBack = getWFlagMnemonic();
    std::string address;

    // Preindexing; add offset before transfer
    if(P == 1){
        // Offset is an immediate value
        if(I == 0){
            address = "[r" + std::to_string(Rn) + "," + Utils::toHexString(Imm) + "]" + writeBack;
        // Offset is a register
        }else if(I == 1){
            address = "TODO I=1";
        }
        
    // Postindexing; add offset after transfer
    }else if(P == 0){
        address = "TODO P=0";
    }

    return base + address;
}