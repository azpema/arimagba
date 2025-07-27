#include "alu_operations.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"
#include "../arm/data_processing.hpp"
#include "../arm/multiply_accumulate.hpp"
#include "../fields/rotate_imm.hpp"
#include "../fields/shift_rm.hpp"

const std::string ALUOperations::op2Mnemonic[16] = {
    "and", "eor", "lsl", "lsr", "asr", "adc", "sbc", "ror", "tst", "neg", "cmp", "cmn", "orr", "mul", "bic", "mvn"};

ALUOperations::ALUOperations(uint16_t op, ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(op, cpu) { init(op); }

ALUOperations::ALUOperations(ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(cpu) {}

void ALUOperations::init(uint32_t op) {
    ThumbOpCode::init(op);
    opField = Utils::getRegBits(op, OP_MASK, OP_SHIFT);
    rs = Utils::getRegBits(op, RS_MASK, RS_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
}

std::string ALUOperations::getOpMnemonic() { return op2Mnemonic[opField]; }

std::string ALUOperations::toString() {
    return getOpMnemonic() + " " + OpCode::getRegMnemonic(rd) + "," + OpCode::getRegMnemonic(rs);
}

void ALUOperations::doDecode() {}

void ALUOperations::doExecute() {
    DataProcessing* opArm =
                static_cast<DataProcessing*>(cpu.getArmPool().getArmInstance(ArmOpcode::OpCodeEnum::DATA_PROCESSING));
    switch (opField) {
        case Opcode::CMP:
        {
            ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);            
            opArm->init(0, DataProcessing::OPCODE_CMP_VAL, 1, rd, rs, shiftRm.getRawVal());
            break;
        } 
        case Opcode::MVN:
        {
            ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
            opArm->init(0, DataProcessing::OPCODE_MVN_VAL, 1, rd, rd, shiftRm.getRawVal());
            break;
        } 
        case Opcode::AND:
        {
            ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
            opArm->init(0, DataProcessing::OPCODE_AND_VAL, 1, rd, rd, shiftRm.getRawVal());
            break;
        } 
        case Opcode::TST:
        {
            ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
            opArm->init(0, DataProcessing::OPCODE_TST_VAL, 1, rd, rs, shiftRm.getRawVal());
            break;
        } 
        case Opcode::BIC:
        {
            ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
            opArm->init(0, DataProcessing::OPCODE_BIC_VAL, 1, rd, rd, shiftRm.getRawVal());
            break;
        } 
        case Opcode::ORR:
        {
            ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
            opArm->init(0, DataProcessing::OPCODE_ORR_VAL, 1, rd, rd, shiftRm.getRawVal());
            break;
        } 
        case Opcode::EOR:
        {
            ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
            opArm->init(0, DataProcessing::OPCODE_EOR_VAL, 1, rd, rd, shiftRm.getRawVal());
            break;
        } 
        case Opcode::LSL:
        {
            ShiftRm shiftRm = ShiftRm(rd, false, 0, ShiftRm::ShiftType::LSL, rs);
            opArm->init(0, DataProcessing::OPCODE_MOV_VAL, 1, rd, rd, shiftRm.getRawVal());
            break;
        } 
        case Opcode::LSR:
        {
            ShiftRm shiftRm = ShiftRm(rd, false, 0, ShiftRm::ShiftType::LSR, rs);
            opArm->init(0, DataProcessing::OPCODE_MOV_VAL, 1, rd, rd, shiftRm.getRawVal());
            break;
        } 
        case Opcode::ASR:
        {
            ShiftRm shiftRm = ShiftRm(rd, false, 0, ShiftRm::ShiftType::ASR, rs);
            opArm->init(0, DataProcessing::OPCODE_MOV_VAL, 1, rd, rd, shiftRm.getRawVal());
            break;
        } 
        case Opcode::ROR:
        {
            ShiftRm shiftRm = ShiftRm(rd, false, 0, ShiftRm::ShiftType::ROR, rs);
            opArm->init(0, DataProcessing::OPCODE_MOV_VAL, 1, rd, rd, shiftRm.getRawVal());
            break;
        } 
        case Opcode::CMN:
        {
            ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
            opArm->init(0, DataProcessing::OPCODE_CMN_VAL, 1, rd, rs, shiftRm.getRawVal());
            break;
        } 
        case Opcode::ADC:
        {
            ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
            opArm->init(0, DataProcessing::OPCODE_ADC_VAL, 1, rd, rd, shiftRm.getRawVal());
            break;
        }
        case Opcode::SBC:
        {
            ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
            opArm->init(0, DataProcessing::OPCODE_SBC_VAL, 1, rd, rd, shiftRm.getRawVal());
            break;
        } 
        case Opcode::NEG:
        {
            RotateImm imm = RotateImm(0, 0);
            opArm->init(1, DataProcessing::OPCODE_RSB_VAL, 1, rs, rd, imm.getRawVal());
            break;
        } 
        case Opcode::MUL:
        {
            MultiplyAccumulate mulOpArm = MultiplyAccumulate(0, 1, rd, 0, rd, rs, cpu);
            mulOpArm.execute();
            return;
        } 
        default:
            throw std::runtime_error("ERROR Unknown THUMB ALUOperations op");

    }
    opArm->doExecute();
    // std::cout << "<< ARM >> " << opArm.toString() << '\n';
}

uint32_t ALUOperations::cyclesUsed() const { return 1; }