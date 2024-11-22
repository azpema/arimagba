#include "move_comp_add_sub_imm.hpp"
#include "../arm/data_processing.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

const std::string MoveCompAddSubImm::op2Mnemonic[4] = {"mov", "cmp", "add", "sub"};

MoveCompAddSubImm::MoveCompAddSubImm(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    init(op);
}

MoveCompAddSubImm::MoveCompAddSubImm(ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(cpu) {}

void MoveCompAddSubImm::init(uint32_t op){
    ThumbOpCode::init(op);
    opField = Utils::getRegBits(op, OP_MASK, OP_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    offset8 = Utils::getRegBits(op, OFFSET8_MASK, OFFSET8_SHIFT);
}

std::string MoveCompAddSubImm::getOpMnemonic(){
    return op2Mnemonic[opField];
}


std::string MoveCompAddSubImm::toString(){
    return getOpMnemonic() + " " + OpCode::getRegMnemonic(rd) + ",#" + Utils::toHexString(offset8, 4);
}

void MoveCompAddSubImm::doDecode(){

}

void MoveCompAddSubImm::doExecute(){
    //RotateImm imm = RotateImm(0, rnOffset3);
    uint32_t opDataProcessing[4] = {DataProcessing::OPCODE_MOV_VAL, DataProcessing::OPCODE_CMP_VAL,
                                    DataProcessing::OPCODE_ADD_VAL, DataProcessing::OPCODE_SUB_VAL};

    DataProcessing opArm = DataProcessing(1, opDataProcessing[opField], 1, rd, rd, offset8, cpu);
    opArm.doExecute();
    //std::cout << "<< ARM >> " << opArm.toString() << std::endl;



    /*uint8_t op = static_cast<MoveCompAddSubImm::Op>(opField);
    uint32_t val = 0;
    switch (op)
    {
    case MOV:
        cpu.setReg(rd, offset8);
        break;
    case CMP:
        cpu.getALU().sub(cpu.getReg(rd), offset8);
        break;
    case ADD:
        val = cpu.getALU().add(cpu.getReg(rd), offset8);
        cpu.setReg(rd, val);
        break;
    case SUB:
        val = cpu.getALU().sub(cpu.getReg(rd), offset8);
        cpu.setReg(rd, val);
        break;
    default:
        throw std::runtime_error("Error: Unknown OpCode in: MoveCompAddSubImm");
        break;
    }

    // Set CPSR flags
    cpu.getCPSR().setNFlag(cpu.getALU().getN());
    cpu.getCPSR().setZFlag(cpu.getALU().getZ());
    if(op != MOV){
        cpu.getCPSR().setCFlag(cpu.getALU().getC());
        cpu.getCPSR().setVFlag(cpu.getALU().getV());
    }*/
}

uint32_t MoveCompAddSubImm::cyclesUsed() const {
    return 1;
}