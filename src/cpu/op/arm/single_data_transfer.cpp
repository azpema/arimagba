#include "single_data_transfer.hpp"

SingleDataTransfer::SingleDataTransfer(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
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
            + " " + OpCode::getRegMnemonic(Rd) + ",";
    std::string writeBack = getWFlagMnemonic();
    std::string address = "";

    // Preindexing; add offset before transfer
    if(P == 1){
        // Offset is an immediate value
        if(I == 0){
            Imm* imm = static_cast<Imm*>(offsetField);
            address = "[" + OpCode::getRegMnemonic(Rn) + "," + getUFlagMnemonic() + Utils::toHexString(imm->getMnemonicVal()) + "]" + writeBack;
        // Offset is a register
        }else if(I == 1){
            ShiftRm* shiftRm = static_cast<ShiftRm*>(offsetField);
            address += "[" + OpCode::getRegMnemonic(Rn) + "," + getUFlagMnemonic() + OpCode::getRegMnemonic(shiftRm->getRm()) + "," + \
                       shiftRm->getShiftTypeMnemonic() + " #" + Utils::toHexString(shiftRm->getShiftAmount()) + "]" + writeBack;
        }else{
            std::cerr << "ERROR: SingleDataTransfer P=1" << std::endl;
        }
        
        
    // Postindexing; add offset after transfer
    }else if(P == 0){
        address = "[" + OpCode::getRegMnemonic(Rn) + "],";
        if(I == 0){
            Imm* imm = static_cast<Imm*>(offsetField);
            address += getUFlagMnemonic() + Utils::toHexString(imm->getMnemonicVal());
        }else if(I == 1){
            ShiftRm* shiftRm = static_cast<ShiftRm*>(offsetField);
            address += getUFlagMnemonic() + OpCode::getRegMnemonic(shiftRm->getRm()) + "," + shiftRm->getShiftTypeMnemonic() + " #" + \
                       Utils::toHexString(shiftRm->getShiftAmount());
        }else{
            std::cerr << "ERROR: SingleDataTransfer P=0" << std::endl;
        }
    }

    return base + address;
}

void SingleDataTransfer::doDecode(){

}

void SingleDataTransfer::doExecute(){
    uint32_t baseRegVal = cpu.getReg(Rn);
    uint32_t offsetVal = offsetField->getOperandVal(cpu);

    // Pre-indexing
    if(P == 1){
        if(U == 0)
            baseRegVal -= offsetVal;
        else if(U == 1)
            baseRegVal += offsetVal;
    }
    if(L == 0){
        // Store to memory
        if(B == 0){
            // Word
            cpu.getMemManager().store(baseRegVal, cpu.getReg(Rd), 4);
        }else if(B == 1){
            // Byte
            throw std::runtime_error("Error: Unimplemented SingleDataTransfer L==0 B==1");
        }else{
            throw std::runtime_error("Error: Invalid B value in SingleDataTransfer::doExecute");
        }

    }else if(L == 1){
        // Load from memory
        throw std::runtime_error("Error: Unimplemented SingleDataTransfer L==1");
    }else {
        throw std::runtime_error("Error: Invalid L value in SingleDataTransfer::doExecute");
    }

    // Post Indexing, Writeback
    if(P == 0 || W == 1){
        if(U == 0)
            baseRegVal -= offsetVal;
        else if(U == 1)
            baseRegVal += offsetVal;

        cpu.setReg(Rn, baseRegVal);
    }

}

uint32_t SingleDataTransfer::cyclesUsed() const {
    return 1;
}