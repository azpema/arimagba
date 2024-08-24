#include "alu_operations.hpp"
#include "../arm/data_processing.hpp"

ALUOperations::ALUOperations(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
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
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_CMP_VAL, 1, rd, rs, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    }else if(opField == Opcode::MVN){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_MVN_VAL, 1, rd, rs, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    }else if(opField == Opcode::AND){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_AND_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    }else if(opField == Opcode::TST){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_TST_VAL, 1, rd, rs, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    }else if(opField == Opcode::BIC){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_BIC_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    }else if(opField == Opcode::ORR){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_ORR_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    }else if(opField == Opcode::EOR){
        ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_EOR_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    }else if(opField == Opcode::LSL){
        ShiftRm shiftRm = ShiftRm(rd, false, 0, ShiftRm::ShiftType::LSL, rs);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_MOV_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    }else if(opField == Opcode::LSR){
        ShiftRm shiftRm = ShiftRm(rd, false, 0, ShiftRm::ShiftType::LSR, rs);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_MOV_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    }else if(opField == Opcode::ASR){
        ShiftRm shiftRm = ShiftRm(rd, false, 0, ShiftRm::ShiftType::ASR, rs);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_MOV_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    }else if(opField == Opcode::ROR){
        ShiftRm shiftRm = ShiftRm(rd, false, 0, ShiftRm::ShiftType::ROR, rs);
        DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_MOV_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
        opArm.doExecute();
        std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    }else{
        throw std::runtime_error("ERROR todo alu operations thumb");
    }
    
   
       
}

uint32_t ALUOperations::cyclesUsed() const {
    return 1;
}