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
    
    uint16_t off = Utils::getRegBits(op, OFFSET_MASK, OFFSET_SHIFT);

    if(I == 0){
        offsetField = new Imm(off);
    }else if(I == 1){
        offsetField = new ShiftRm(off);
    }
}   

SingleDataTransfer::~SingleDataTransfer(){
    delete offsetField;
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
            + " " + getRegMnemonic(Rd) + ",";
    std::string writeBack = getWFlagMnemonic();
    std::string address = "";

    // Preindexing; add offset before transfer
    if(P == 1){
        // Offset is an immediate value
        if(I == 0){
            Imm* imm = static_cast<Imm*>(offsetField);
            address = "[" + getRegMnemonic(Rn) + "," + getUFlagMnemonic() + Utils::toHexString(imm->getMnemonicVal()) + "]" + writeBack;
        // Offset is a register
        }else if(I == 1){
            ShiftRm* shiftRm = static_cast<ShiftRm*>(offsetField);
            address += "[" + getRegMnemonic(Rn) + "," + getUFlagMnemonic() + getRegMnemonic(shiftRm->getRm()) + "," + \
                       shiftRm->getShiftTypeMnemonic() + " #" + Utils::toHexString(shiftRm->getShiftAmount()) + "]" + writeBack;
        }else{
            std::cerr << "ERROR: SingleDataTransfer P=1" << std::endl;
        }
        
        
    // Postindexing; add offset after transfer
    }else if(P == 0){
        address = "[" + getRegMnemonic(Rn) + "],";
        if(I == 0){
            Imm* imm = static_cast<Imm*>(offsetField);
            address += getUFlagMnemonic() + Utils::toHexString(imm->getMnemonicVal());
        }else if(I == 1){
            ShiftRm* shiftRm = static_cast<ShiftRm*>(offsetField);
            address += getUFlagMnemonic() + getRegMnemonic(shiftRm->getRm()) + "," + shiftRm->getShiftTypeMnemonic() + " #" + \
                       Utils::toHexString(shiftRm->getShiftAmount());
        }else{
            std::cerr << "ERROR: SingleDataTransfer P=0" << std::endl;
        }
    }

    return base + address;
}

void SingleDataTransfer::doExecute(ARM7TDMI &cpu){

}

bool SingleDataTransfer::mustFlushPipeline() const {
    return false;
}

uint32_t SingleDataTransfer::cyclesUsed() const {
    return 1;
}