#include "multiple_load_store.hpp"
#include "../arm/block_data_transfer.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

const std::string MultipleLoadStore::op2Mnemonic[2] = {"stmia", "ldmia"};

MultipleLoadStore::MultipleLoadStore(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    init(op);
}

MultipleLoadStore::MultipleLoadStore(ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(cpu) {}

void MultipleLoadStore::init(uint32_t op){
    ThumbOpCode::init(op);
    l = Utils::getRegBits(op, L_MASK, L_SHIFT);
    rb = Utils::getRegBits(op, RB_MASK, RB_SHIFT);
    rList = Utils::getRegBits(op, RLIST_MASK, RLIST_SHIFT);

    registerListVec = {};
    for(size_t i = 0; i < 8; i++){
        if(((rList >> i) & 0x1) == 0x1)
            registerListVec.push_back(i);    
    }
}

std::string MultipleLoadStore::getOpMnemonic(){
    return op2Mnemonic[l];
}

std::string MultipleLoadStore::toString(){
    std::string mnemonic = getOpMnemonic() + " " + OpCode::getRegMnemonic(rb) + "!,{";

    for(auto it = registerListVec.begin(); it != registerListVec.end(); it++){
        mnemonic += OpCode::getRegMnemonic(*it);
        if(std::next(it) != registerListVec.end())
            mnemonic += ",";
    }
    mnemonic += "}";
    
    return mnemonic;
}

void MultipleLoadStore::doDecode(){

}

void MultipleLoadStore::doExecute(){
    uint8_t p = 0;
    uint8_t u = 1;
    //BlockDataTransfer opArm = BlockDataTransfer(p, u, 0, 1, l, rb, rList, cpu);
    BlockDataTransfer *opArm = static_cast<BlockDataTransfer*>(cpu.getArmPool().getArmInstance(ArmOpcode::OpCodeEnum::BLOCK_DATA_TRANSFER));
    opArm->init(p, u, 0, 1, l, rb, rList);
    opArm->doExecute();
    //std::cout << "<< ARM >> " << opArm.toString() << '\n';
}

uint32_t MultipleLoadStore::cyclesUsed() const {
    return 1;
}