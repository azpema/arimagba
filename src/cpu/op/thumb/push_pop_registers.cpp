#include "push_pop_registers.hpp"
#include "../arm/block_data_transfer.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

const std::string PushPopRegisters::op2Mnemonic[2] = {"push", "pop"};

PushPopRegisters::PushPopRegisters(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    init(op);
}

PushPopRegisters::PushPopRegisters(ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(cpu) {}

void PushPopRegisters::init(uint32_t op){
    ThumbOpCode::init(op);
    l = Utils::getRegBits(op, L_MASK, L_SHIFT);
    r = Utils::getRegBits(op, R_MASK, R_SHIFT);
    rList = Utils::getRegBits(op, RLIST_MASK, RLIST_SHIFT);
    
    for(size_t i = 0; i < 8; i++){
        if(((rList >> i) & 0x1) == 0x1)
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
    if(l == 0 && r == 0){
        BlockDataTransfer opArm = BlockDataTransfer(1, 0, 0, 1, 0, 13, rList, cpu);
        //std::cout << "<< ARM >> " << opArm.toString() << std::endl;
        opArm.doExecute();  
    }else if (l == 1 && r == 0){
        BlockDataTransfer opArm = BlockDataTransfer(0, 1, 0, 1, 1, 13, rList, cpu);
        //std::cout << "<< ARM >> " << opArm.toString() << std::endl;
        opArm.doExecute();  
    }else if (l == 0 && r == 1){
        uint16_t rListWithLr = rList | (0x1 << 14);
        BlockDataTransfer opArm = BlockDataTransfer(1, 0, 0, 1, 0, 13, rListWithLr, cpu);
        //std::cout << "<< ARM >> " << opArm.toString() << std::endl;
        opArm.doExecute();  
    }else if (l == 1 && r == 1){
        uint16_t rListWithPc = rList | (0x1 << 15);
        BlockDataTransfer opArm = BlockDataTransfer(0, 1, 0, 1, 1, 13, rListWithPc, cpu);
        //std::cout << "<< ARM >> " << opArm.toString() << std::endl;
        opArm.doExecute(); 
    }else{
        throw std::runtime_error("Error: Unimplemented instruction: PushPopRegisters");
    }
}

uint32_t PushPopRegisters::cyclesUsed() const {
    return 1;
}