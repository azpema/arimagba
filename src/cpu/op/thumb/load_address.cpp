#include "load_address.hpp"
#include "../arm/data_processing.hpp"

LoadAddress::LoadAddress(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    sp = Utils::getRegBits(op, SP_MASK, SP_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    word8 = Utils::getRegBits(op, WORD8_MASK, WORD8_SHIFT);
    offsetVal = word8 << 2;
}

uint8_t LoadAddress::getSPRegVal(){
    return sp2RegVal[sp];
}

std::string LoadAddress::toString(){
    return "add " + OpCode::getRegMnemonic(rd) + "," + OpCode::getRegMnemonic(getSPRegVal()) + \
            ",#" + Utils::toHexString(offsetVal); 
}

void LoadAddress::doDecode(){

}

void LoadAddress::doExecute(){
    /*uint32_t rsVal = cpu.getReg(getSPRegVal());
    // sp refers to PC
    if(sp == 0)
        rsVal &= 0xFFFFFFFE;
    
    cpu.setReg(rd, cpu.getALU().add(rsVal, offsetVal));*/

    //uint32_t rsVal = cpu.getReg(getSPRegVal());

    uint32_t pcSpVal = cpu.getReg(getSPRegVal());
    // Where the PC is used as the source register (SP = 0), bit 1 of the PC is always read as 0.
    if(sp == 0)
        pcSpVal &= 0xFFFFFFFD;

    RotateImm imm = RotateImm(0, offsetVal);
    DataProcessing opArm = DataProcessing(1, DataProcessing::OPCODE_ADD_VAL, 0, getSPRegVal(), rd, imm.getRawVal(), cpu, true, pcSpVal);
    opArm.doExecute();
    std::cout << "<< ARM >> " << opArm.toString() << std::endl;
}

uint32_t LoadAddress::cyclesUsed() const {
    return 1;
}