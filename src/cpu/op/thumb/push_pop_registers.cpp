#include "push_pop_registers.hpp"

PushPopRegisters::PushPopRegisters(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    l = Utils::getRegBits(op, L_MASK, L_SHIFT);
    r = Utils::getRegBits(op, R_MASK, R_SHIFT);
    rList = Utils::getRegBits(op, RLIST_MASK, RLIST_SHIFT);
    
    for(size_t i = 0; i < 8; i++){
        if((rList >> i) & 0x1 == 0x1)
            registerListVec.push_back(i);    
    }
}   

std::string PushPopRegisters::getOpMnemonic(){
    return op2Mnemonic[l];
}

std::string PushPopRegisters::toString(){
    std::string mnemonic = getOpMnemonic() + " {";

    for(auto it = registerListVec.begin(); it != registerListVec.end(); it++){
        mnemonic += OpCode::getRegMnemonic(*it);
        if(std::next(it) != registerListVec.end())
            mnemonic += ",";
    }
    
    if(r == 1){
        if(l == 0){
            mnemonic += "," + OpCode::getRegMnemonic(14);
        }else if(l == 1){
            mnemonic += "," + OpCode::getRegMnemonic(15);
        }
    }

    mnemonic += "}";
    
    return mnemonic;
}

void PushPopRegisters::doDecode(){

}

void PushPopRegisters::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: PushPopRegisters");
}

uint32_t PushPopRegisters::cyclesUsed() const {
    return 1;
}