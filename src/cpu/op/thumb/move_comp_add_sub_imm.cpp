#include "move_comp_add_sub_imm.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"
#include "../arm/data_processing.hpp"

const std::string MoveCompAddSubImm::op2Mnemonic[4] = {"mov", "cmp", "add", "sub"};

MoveCompAddSubImm::MoveCompAddSubImm(uint16_t op, ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(op, cpu) { init(op); }

MoveCompAddSubImm::MoveCompAddSubImm(ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(cpu) {}

void MoveCompAddSubImm::init(uint32_t op) {
    ThumbOpCode::init(op);
    opField = Utils::getRegBits(op, OP_MASK, OP_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    offset8 = Utils::getRegBits(op, OFFSET8_MASK, OFFSET8_SHIFT);
}

std::string MoveCompAddSubImm::getOpMnemonic() { return op2Mnemonic[opField]; }

std::string MoveCompAddSubImm::toString() {
    return getOpMnemonic() + " " + OpCode::getRegMnemonic(rd) + ",#" + Utils::toHexString(offset8, 4);
}

void MoveCompAddSubImm::doDecode() {}

void MoveCompAddSubImm::doExecute() {
    // RotateImm imm = RotateImm(rnOffset3, RotateImm::ConstructorType::FINAL_IMM_VAL);
    static const uint32_t opDataProcessing[4] = {DataProcessing::OPCODE_MOV_VAL,
                                                 DataProcessing::OPCODE_CMP_VAL,
                                                 DataProcessing::OPCODE_ADD_VAL,
                                                 DataProcessing::OPCODE_SUB_VAL};

    DataProcessing* opArm =
        static_cast<DataProcessing*>(cpu.getArmPool().getArmInstance(ArmOpcode::OpCodeEnum::DATA_PROCESSING));

    opArm->init(1, opDataProcessing[opField], 1, rd, rd, offset8);
    opArm->doExecute();
    // std::cout << "<< ARM >> " << opArm.toString() << '\n';
}

uint32_t MoveCompAddSubImm::cyclesUsed() const { return 1; }