#include "alu_operations.hpp"
#include "../arm/data_processing.hpp"
#include "../arm/multiply_accumulate.hpp"
#include "../../../utils/utils.hpp"
#include "../fields/rotate_imm.hpp"
#include "../fields/shift_rm.hpp"
#include "../../arm7tdmi.hpp"

const std::string ALUOperations::op2Mnemonic[16] = {"and", "eor", "lsl", "lsr",
                                                    "asr", "adc", "sbc", "ror",
                                                    "tst", "neg", "cmp", "cmn",
                                                    "orr", "mul", "bic", "mvn"};

ALUOperations::ALUOperations(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    init(op);
}

ALUOperations::ALUOperations(ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(cpu) {}

void ALUOperations::init(uint32_t op){
    ThumbOpCode::init(op);
    opField = Utils::getRegBits(op, OP_MASK, OP_SHIFT);
    rs = Utils::getRegBits(op, RS_MASK, RS_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
}

std::string ALUOperations::getOpMnemonic(){
    return op2Mnemonic[opField];
}

std::string ALUOperations::toString(){
    return getOpMnemonic() + " " + OpCode::getRegMnemonic(rd) + "," + OpCode::getRegMnemonic(rs);
}

void ALUOperations::doDecode(){

}

void ALUOperations::doExecute(){
    if(opField == Opcode::CMP){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        //DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_CMP_VAL, 1, rd, rs, shiftRm.getRawVal(), cpu);
        DataProcessing *opArm = static_cast<DataProcessing*>(cpu.getArmPool().getArmInstance(ArmOpcode::OpCodeEnum::DATA_PROCESSING));
        opArm->init(0, DataProcessing::OPCODE_CMP_VAL, 1, rd, rs, shiftRm.getRawVal());
        opArm->doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::MVN){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        //DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_MVN_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        DataProcessing *opArm = static_cast<DataProcessing*>(cpu.getArmPool().getArmInstance(ArmOpcode::OpCodeEnum::DATA_PROCESSING));
        opArm->init(0, DataProcessing::OPCODE_MVN_VAL, 1, rd, rd, shiftRm.getRawVal());
        opArm->doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::AND){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        //DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_AND_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        DataProcessing *opArm = static_cast<DataProcessing*>(cpu.getArmPool().getArmInstance(ArmOpcode::OpCodeEnum::DATA_PROCESSING));
        opArm->init(0, DataProcessing::OPCODE_AND_VAL, 1, rd, rd, shiftRm.getRawVal());
        opArm->doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::TST){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_TST_VAL, 1, rd, rs, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::BIC){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_BIC_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::ORR){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_ORR_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::EOR){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_EOR_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::LSL){
        ShiftRm shiftRm = ShiftRm(rd, false, 0, ShiftRm::ShiftType::LSL, rs);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_MOV_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::LSR){
        ShiftRm shiftRm = ShiftRm(rd, false, 0, ShiftRm::ShiftType::LSR, rs);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_MOV_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::ASR){
        ShiftRm shiftRm = ShiftRm(rd, false, 0, ShiftRm::ShiftType::ASR, rs);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_MOV_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::ROR){
        ShiftRm shiftRm = ShiftRm(rd, false, 0, ShiftRm::ShiftType::ROR, rs);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_MOV_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::CMN){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_CMN_VAL, 1, rd, rs, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::ADC){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_ADC_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::SBC){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_SBC_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::NEG){
        RotateImm imm = RotateImm(0, 0);
        DataProcessing opArm = DataProcessing(1, DataProcessing::OPCODE_RSB_VAL, 1, rs, rd, imm.getRawVal(), cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else if(opField == Opcode::MUL){
        MultiplyAccumulate opArm = MultiplyAccumulate(0, 1, rd, 0, rd, rs, cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << '\n';
    }else{
        throw std::runtime_error("ERROR todo alu operations thumb");
    }
    
   
       
}

uint32_t ALUOperations::cyclesUsed() const {
    return 1;
}