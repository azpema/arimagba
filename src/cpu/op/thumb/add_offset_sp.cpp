#include "add_offset_sp.hpp"
#include "../arm/data_processing.hpp"
#include "../../../utils/utils.hpp"
#include "../fields/rotate_imm.hpp"
#include "../../arm7tdmi.hpp"


const std::string AddOffsetSP::s2Mnemonic[2] = {"", "-"};

AddOffsetSP::AddOffsetSP(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    init(op);
}

AddOffsetSP::AddOffsetSP(ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(cpu) {}

void AddOffsetSP::init(uint32_t op){
    ThumbOpCode::init(op);
    s = Utils::getRegBits(op, S_MASK, S_SHIFT);
    sWord7 = Utils::getRegBits(op, SWORD7_MASK, SWORD7_SHIFT);
    offsetStr = (sWord7 << 2);
    if(s == 1)
        offsetVal = -offsetStr;
    else
        offsetVal = offsetStr;
}

std::string AddOffsetSP::getSMnemonic(){
    return s2Mnemonic[s];
}

std::string AddOffsetSP::toString(){
    return "add sp,#" + getSMnemonic() + Utils::toHexString(offsetStr);
}

void AddOffsetSP::doDecode(){

}

void AddOffsetSP::doExecute(){
    RotateImm imm = RotateImm(0, offsetStr);
    uint16_t operation = s == 0 ? DataProcessing::OPCODE_ADD_VAL : DataProcessing::OPCODE_SUB_VAL;
    //DataProcessing opArm = DataProcessing(1, operation, 0, 13, 13, imm.getRawVal(), cpu);
    DataProcessing *opArm = static_cast<DataProcessing*>(cpu.getArmPool().getArmInstance(ArmOpcode::OpCodeEnum::DATA_PROCESSING));
    opArm->init(1, operation, 0, 13, 13, imm.getRawVal());
    opArm->doExecute();
    //std::cout << "<< ARM >> " << opArm.toString() << std::endl;
}

uint32_t AddOffsetSP::cyclesUsed() const {
    return 1;
}