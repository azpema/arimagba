#include "block_data_transfer.hpp"

BlockDataTransfer::BlockDataTransfer(uint32_t op): OpCode::OpCode(op) {
    P = Utils::getRegBits(op, P_MASK, P_SHIFT);
    U = Utils::getRegBits(op, U_MASK, U_SHIFT);
    S = Utils::getRegBits(op, S_MASK, S_SHIFT);
    W = Utils::getRegBits(op, W_MASK, W_SHIFT);
    L = Utils::getRegBits(op, L_MASK, L_SHIFT);
    Rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
    registerList = Utils::getRegBits(op, REGISTER_LIST_MASK, REGISTER_LIST_SHIFT);

    for(size_t i = 0; i < 16-1; i++){
        if((registerList >> i) & 0x1 == 0x1)
            registerListVec.push_back(i);    
    }
}   

std::string BlockDataTransfer::getSFlagMnemonic(){
    return SFlag2Mnemonic[S];
}

std::string BlockDataTransfer::getWFlagMnemonic(){
    return WFlag2Mnemonic[W];
}

std::string BlockDataTransfer::getRegisterListMnemonic(){
    std::string regs = "{";
    bool addComma = false;
    for(size_t i = 0; i < registerListVec.size(); i++){
        regs += getRegMnemonic(registerListVec[i]);
        if(i < registerListVec.size() - 1)
            regs += ",";
    }
    regs += "}";
    return regs;
}

std::string BlockDataTransfer::getOpAddressingModeMnemonic(){
    return opAddressingMode2Mnemonic[L][P][U];
}

std::string BlockDataTransfer::toString(){
    return getOpAddressingModeMnemonic() + getCondFieldMnemonic() + " " + getRegMnemonic(Rn) + getWFlagMnemonic() + "," + getRegisterListMnemonic() + getSFlagMnemonic();
}

void BlockDataTransfer::doDecode(ARM7TDMI &cpu){

}

void BlockDataTransfer::doExecute(ARM7TDMI &cpu){
    uint32_t rnVal = cpu.getReg(Rn);
    if(L==0 && P==1 && U==0){
        
    }
}

bool BlockDataTransfer::mustFlushPipeline() const{
    return false;
}

uint32_t BlockDataTransfer::cyclesUsed() const {
    std::cerr << "TODO: BlockDataTransfer::cyclesUsed" << std::endl;
    return 1;
}
