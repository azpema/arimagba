#include "load_address.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"
#include "../arm/data_processing.hpp"
#include "../fields/rotate_imm.hpp"

const uint8_t LoadAddress::sp2RegVal[2] = {15, 13};

LoadAddress::LoadAddress(uint16_t op, ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(op, cpu) { init(op); }

LoadAddress::LoadAddress(ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(cpu) {}

void LoadAddress::init(uint32_t op) {
    ThumbOpCode::init(op);
    sp = Utils::getRegBits(op, SP_MASK, SP_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    word8 = Utils::getRegBits(op, WORD8_MASK, WORD8_SHIFT);
    offsetVal = word8 << 2;
}

uint8_t LoadAddress::getSPRegVal() { return sp2RegVal[sp]; }

std::string LoadAddress::toString() {
    return "add " + OpCode::getRegMnemonic(rd) + "," + OpCode::getRegMnemonic(getSPRegVal()) + ",#" +
           Utils::toHexString(offsetVal);
}

void LoadAddress::doDecode() {}

void LoadAddress::doExecute() {
    /*uint32_t rsVal = cpu.getReg(getSPRegVal());
    // sp refers to PC
    if(sp == 0)
        rsVal &= 0xFFFFFFFE;

    cpu.setReg(rd, cpu.getALU().add(rsVal, offsetVal));*/

    // uint32_t rsVal = cpu.getReg(getSPRegVal());

    uint32_t pcSpVal = cpu.getReg(getSPRegVal());
    // Where the PC is used as the source register (SP = 0), bit 1 of the PC is always read as 0.
    if (sp == 0)
        pcSpVal &= 0xFFFFFFFD;

    RotateImm imm = RotateImm(offsetVal, RotateImm::ConstructorType::FINAL_IMM_VAL);
    // DataProcessing opArm = DataProcessing(1, DataProcessing::OPCODE_ADD_VAL, 0, getSPRegVal(), rd, imm.getRawVal(),
    // cpu, true, pcSpVal);
    DataProcessing* opArm =
        static_cast<DataProcessing*>(cpu.getArmPool().getArmInstance(ArmOpcode::OpCodeEnum::DATA_PROCESSING));
    opArm->init(1, DataProcessing::OPCODE_ADD_VAL, 0, getSPRegVal(), rd, imm.getRawVal(), true, pcSpVal);
    opArm->doExecute();
    // std::cout << "<< ARM >> " << opArm.toString() << '\n';
}

uint32_t LoadAddress::cyclesUsed() const { return 1; }