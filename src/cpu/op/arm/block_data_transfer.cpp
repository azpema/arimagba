#include "block_data_transfer.hpp"

BlockDataTransfer::BlockDataTransfer(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    P = Utils::getRegBits(op, P_MASK, P_SHIFT);
    U = Utils::getRegBits(op, U_MASK, U_SHIFT);
    S = Utils::getRegBits(op, S_MASK, S_SHIFT);
    W = Utils::getRegBits(op, W_MASK, W_SHIFT);
    L = Utils::getRegBits(op, L_MASK, L_SHIFT);
    Rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
    registerList = Utils::getRegBits(op, REGISTER_LIST_MASK, REGISTER_LIST_SHIFT);

    for(size_t i = 0; i < 16; i++){
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
        regs += OpCode::getRegMnemonic(registerListVec[i]);
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
    return getOpAddressingModeMnemonic() + getCondFieldMnemonic() + " " + OpCode::getRegMnemonic(Rn) + getWFlagMnemonic() + "," + getRegisterListMnemonic() + getSFlagMnemonic();
}

void BlockDataTransfer::doDecode(){

}

// Optimizible!!
// TODO Flush pipeline if PC is written!!!
void BlockDataTransfer::doExecute(){
    if(L==0 && P==1 && U==0){
        uint32_t baseAddr = cpu.getReg(Rn) - registerListVec.size() * 4;
        uint32_t endAddr = baseAddr;
        for(size_t i=0; i < registerListVec.size(); i++){
            uint32_t regVal = cpu.getReg(registerListVec.at(i));
            cpu.getMemManager().store(baseAddr, regVal, 4);
            baseAddr += 4;
        }

        if(W==1){
            cpu.setReg(Rn, endAddr);
        }
    }else if(L==1 && P==0 && U==1){
        uint32_t baseAddr = cpu.getReg(Rn);
        uint32_t endAddr = baseAddr;
        for(size_t i=0; i < registerListVec.size(); i++){
            uint32_t val = cpu.getMemManager().readWord(endAddr);
            cpu.setReg(registerListVec.at(i), val);

            // If PC value is modified, flush pipeline
            if(registerListVec.at(i) == 15)
                cpu.setMustFlushPipeline(true);

            endAddr += 4;
        }

        if(W==1){
            cpu.setReg(Rn, endAddr);
        }

   /* }else if(L==0 && P==1 && U==1){
        uint32_t baseAddr = cpu.getReg(Rn) + registerListVec.size() * 4;
        uint32_t endAddr = baseAddr;
        for(size_t i=0; i < registerListVec.size(); i++){
            uint32_t regVal = cpu.getReg(registerListVec.at(i));
            cpu.getMemManager().store(endAddr, regVal, 4);
            endAddr -= 4;
        }

        if(W==1){
            cpu.setReg(Rn, baseAddr);
        }
    }*/
   }else if(L==0 && P==1 && U==1){
        uint32_t baseAddr = cpu.getReg(Rn) + 4;
        uint32_t endAddr = cpu.getReg(Rn) + registerListVec.size() * 4;
        for(size_t i=0; i < registerListVec.size(); i++){
            uint32_t regVal = cpu.getReg(registerListVec.at(i));
            cpu.getMemManager().store(baseAddr, regVal, 4);
            baseAddr += 4;
        }

        if(W==1){
            cpu.setReg(Rn, endAddr);
        }
    }
   else if(L==1 && P==0 && U==0){
        uint32_t endAddr = cpu.getReg(Rn) - registerListVec.size() * 4;
        uint32_t baseAddr = endAddr + 4;
        for(size_t i=0; i < registerListVec.size(); i++){
            uint32_t val = cpu.getMemManager().readWord(baseAddr);
            cpu.setReg(registerListVec.at(i), val);

            // If PC value is modified, flush pipeline
            if(registerListVec.at(i) == 15)
                cpu.setMustFlushPipeline(true);

            baseAddr += 4;
        }

        if(W==1){
            cpu.setReg(Rn, endAddr);
        }

    }else if(L==0 && P==0 && U==0){
        uint32_t baseAddr = cpu.getReg(Rn);
        uint32_t endAddr = baseAddr;
        for(size_t i=0; i < registerListVec.size(); i++){
            uint32_t regVal = cpu.getReg(registerListVec.at(i));
            cpu.getMemManager().store(endAddr, regVal, 4);

            // If PC value is modified, flush pipeline
            if(registerListVec.at(i) == 15)
                cpu.setMustFlushPipeline(true);

            endAddr -= 4;
        }

        if(W==1){
            cpu.setReg(Rn, endAddr);
        }

    }else if(L==0 && P==0 && U==1){
        uint32_t baseAddr = cpu.getReg(Rn);
        uint32_t endAddr = baseAddr;
        for(size_t i=0; i < registerListVec.size(); i++){
            uint32_t regVal = cpu.getReg(registerListVec.at(i));
            cpu.getMemManager().store(endAddr, regVal, 4);

            // If PC value is modified, flush pipeline
            if(registerListVec.at(i) == 15)
                cpu.setMustFlushPipeline(true);

            endAddr += 4;
        }

        if(W==1){
            cpu.setReg(Rn, endAddr);
        }

    }else if(L==1 && P==1 && U==0){
        uint32_t baseAddr = cpu.getReg(Rn) - registerListVec.size() * 4;
        uint32_t endAddr = baseAddr;
        for(size_t i=0; i < registerListVec.size(); i++){
            uint32_t val = cpu.getMemManager().readWord(endAddr);
            cpu.setReg(registerListVec.at(i), val);
            endAddr += 4;
        }

        if(W==1){
            cpu.setReg(Rn, baseAddr);
        }
           
    }else if(L==1 && P==1 && U==1){
        uint32_t baseAddr = cpu.getReg(Rn) + registerListVec.size() * 4;
        uint32_t endAddr = baseAddr;
        for(size_t i=0; i < registerListVec.size(); i++){
            uint32_t val = cpu.getMemManager().readWord(endAddr);
            cpu.setReg(registerListVec.at(i), val);
            endAddr -= 4;
        }

        if(W==1){
            cpu.setReg(Rn, baseAddr);
        }
           
    }else{
        throw std::runtime_error("TODO: BlockDataTransfer::doExecute; L=" + std::to_string(L) + " P=" + std::to_string(P) + " U=" + std::to_string(U));
    }
}

uint32_t BlockDataTransfer::cyclesUsed() const {
    std::cerr << "TODO: BlockDataTransfer::cyclesUsed" << std::endl;
    return 1;
}
