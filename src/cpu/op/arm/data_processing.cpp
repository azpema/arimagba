#include "data_processing.hpp"
#include "../../../utils/utils.hpp"
#include <string>

DataProcessing::DataProcessing(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    dataOpCode = Utils::getRegBits(op, OPCODE_MASK, OPCODE_SHIFT);
    i = Utils::getRegBits(op, IMMEDIATE_OPERAND_MASK, IMMEDIATE_OPERAND_SHIFT);
    s = Utils::getRegBits(op, SET_CONDITION_MASK, SET_CONDITION_SHIFT);
    rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);

    if(i == 0){
        operand2 = new ShiftRm(Utils::getRegBits(op, OPERAND2_MASK, OPERAND2_SHIFT));
    }else if (i == 1){
        operand2 = new RotateImm(Utils::getRegBits(op, OPERAND2_MASK, OPERAND2_SHIFT));
    }else{
        std::cout << "ERROR: Invalid I field value" << std::endl;
    }
}   

DataProcessing::~DataProcessing() {
    delete operand2;
}   

/*
Certain operations (TST, TEQ, CMP, CMN) do not write the result to Rd. They are used
only to perform tests and to set the condition codes on the result and always have the
S bit set
*/
std::string DataProcessing::toString(){
    std::string mnemonic = dataOpCode2Mnemonic[dataOpCode] + getCondFieldMnemonic();
    switch (dataOpCode)
    {
    // <opcode>{cond}{S} Rd,<Op2>
    case OPCODE_MOV_VAL:
    case OPCODE_MVN_VAL:
        mnemonic += SFlag2Mnemonic[s] + " " + OpCode::getRegMnemonic(rd) + "," + getOperand2Mnemonic();
        break;
    // <opcode>{cond} Rn,<Op2>
    case OPCODE_CMP_VAL:
    case OPCODE_CMN_VAL:
    case OPCODE_TEQ_VAL:
    case OPCODE_TST_VAL:
        mnemonic += " " + OpCode::getRegMnemonic(rn) + "," + getOperand2Mnemonic();
        break;
    // <opcode>{cond}{s} Rd,Rn,<Op2>
    case OPCODE_AND_VAL:
    case OPCODE_EOR_VAL:
    case OPCODE_SUB_VAL:
    case OPCODE_RSB_VAL:
    case OPCODE_ADD_VAL:
    case OPCODE_ADC_VAL:
    case OPCODE_SBC_VAL:
    case OPCODE_RSC_VAL:
    case OPCODE_ORR_VAL:
    case OPCODE_BIC_VAL:
        mnemonic += SFlag2Mnemonic[s] + " " + OpCode::getRegMnemonic(rd) + "," + OpCode::getRegMnemonic(rn) + "," + getOperand2Mnemonic();
        break;

    default:
        mnemonic = "ERROR OPCODE";
        break;
    }

    return mnemonic;
}

std::string DataProcessing::getOpCodeMnemonic(){
    return dataOpCode2Mnemonic[dataOpCode];
}

std::string DataProcessing::getRdMnemonic(){
    return OpCode::getRegMnemonic(rd);

}

std::string DataProcessing::getRnMnemonic(){
    return OpCode::getRegMnemonic(rn);
}

uint32_t DataProcessing::getOperand2Rm(){
    // TODO
    return -12345;
}

std::string DataProcessing::getOperand2Mnemonic(){
    if(i == 0){
        ShiftRm* shiftRm = static_cast<ShiftRm*>(operand2);
        return OpCode::getRegMnemonic(shiftRm->getRm()) + "," + shiftRm->getShiftTypeMnemonic() + " #" + Utils::toHexString(shiftRm->getShiftAmount());
    }else if(i == 1){
        RotateImm* rotateImm = static_cast<RotateImm*>(operand2);
        return "#" + Utils::toHexString(rotateImm->getMnemonicVal()); 
    }else{
        return "ERROR getOperand2Mnemonic Data_processing";
    }
}

//   ALU, 1S, +1S+1N if R15 loaded, +1I if SHIFT(Rs)
uint32_t DataProcessing::cyclesUsed() const {
    std::cerr << "TODO: DataProcessing::cyclesUsed" << std::endl;
    return 1 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE;
}

void DataProcessing::doExecuteCmp(ARM7TDMI &cpu){
    cpu.getALU().sub(op1, op2);
}

void DataProcessing::doExecuteMov(ARM7TDMI &cpu){
    cpu.setReg(rd, op2);
}

void DataProcessing::doExecuteAdd(ARM7TDMI &cpu){
    uint32_t addRes = cpu.getALU().add(op1, op2);
    cpu.setReg(rd, addRes);
}

void DataProcessing::doExecuteOrr(ARM7TDMI &cpu){
    uint32_t orrRes = op1 | op2;
    cpu.setReg(rd, orrRes);
}

void DataProcessing::doExecuteAnd(ARM7TDMI &cpu){
    uint32_t andRes = cpu.getALU().andOp(op1, op2);
    cpu.setReg(rd, andRes);
}

void DataProcessing::doExecuteTst(ARM7TDMI &cpu){
    cpu.getALU().andOp(op1, op2);
}

void DataProcessing::doExecuteMvn(ARM7TDMI &cpu){
    uint32_t mvnRes = cpu.getALU().mvn(op2);
    cpu.setReg(rd, mvnRes);
}

void DataProcessing::doDecode(){

}

void DataProcessing::doExecute(){
    // Assign values to op1 and op2
    bool rdIsPC = false;
    if(rd == 15){
        mustFlushPipeline = true;
        rdIsPC = true;
    }
        

    op1 = cpu.getReg(rn);
    op2 = operand2->getOperandVal(cpu);
    switch (dataOpCode)
    {
    case OPCODE_MOV_VAL:
        doExecuteMov(cpu);
        break;
    case OPCODE_MVN_VAL:
        doExecuteMvn(cpu);
        break;
    case OPCODE_CMP_VAL:
        doExecuteCmp(cpu);
        break;
    case OPCODE_CMN_VAL:
        throw std::runtime_error("Error: Unimplemented instruction: DataProcessing::OPCODE_CMN_VAL");
        break;
    case OPCODE_TEQ_VAL:
        throw std::runtime_error("Error: Unimplemented instruction: DataProcessing::OPCODE_TEQ_VAL");
        break;
    case OPCODE_TST_VAL:
        doExecuteTst(cpu);
        break;
    case OPCODE_AND_VAL:
        doExecuteAnd(cpu);
        break;
    case OPCODE_EOR_VAL:
        throw std::runtime_error("Error: Unimplemented instruction: DataProcessing::OPCODE_EOR_VAL");
        break;
    case OPCODE_SUB_VAL:
        throw std::runtime_error("Error: Unimplemented instruction: DataProcessing::OPCODE_SUB_VAL");
        break;
    case OPCODE_RSB_VAL:
        throw std::runtime_error("Error: Unimplemented instruction: DataProcessing::OPCODE_RSB_VAL");
        break;
    case OPCODE_ADD_VAL:
        doExecuteAdd(cpu);
        break;
    case OPCODE_ADC_VAL:
        throw std::runtime_error("Error: Unimplemented instruction: DataProcessing::OPCODE_ADC_VAL");
        break;
    case OPCODE_SBC_VAL:
        throw std::runtime_error("Error: Unimplemented instruction: DataProcessing::OPCODE_SBC_VAL");
        break;
    case OPCODE_RSC_VAL:
        throw std::runtime_error("Error: Unimplemented instruction: DataProcessing::OPCODE_RSC_VAL");
        break;
    case OPCODE_ORR_VAL:
        doExecuteOrr(cpu);
        break;
    case OPCODE_BIC_VAL:
        throw std::runtime_error("Error: Unimplemented instruction: DataProcessing::OPCODE_BIC_VAL");
        break;
    default:
        break;
    }

    switch (dataOpCode)
    {
    case OPCODE_AND_VAL:
    case OPCODE_EOR_VAL:
    case OPCODE_TST_VAL:
    case OPCODE_TEQ_VAL:
    case OPCODE_ORR_VAL:
    case OPCODE_MOV_VAL:
    case OPCODE_BIC_VAL:
    case OPCODE_MVN_VAL:
    //TODO: "If the S bit is set (and Rd is not R15, see below)"
        if(s == 1){
            cpu.getCPSR().setNFlag(cpu.getALU().getN());
            cpu.getCPSR().setZFlag(cpu.getALU().getZ());
            /*the C flag will be set to the carry out from the barrel shifter
             (or preserved when the shift operation is LSL #0)*/
            if(i == 0){
                ShiftRm* shiftRm = static_cast<ShiftRm*>(operand2);
                if(!((shiftRm->getShiftAmount() == 0 && shiftRm->getShiftType() == 0))){
                    cpu.getCPSR().setCFlag(cpu.getBarrelShifter().getC());
                }
                
            }
            /*
            When Rd is R15 and the S flag is set the result of the operation is placed in R15 and
            the SPSR corresponding to the current mode is moved to the CPSR
            */
            if(rdIsPC){
                throw std::runtime_error("ERROR DataProcessing::doExecute 88");
            }
        }
        break;
    case OPCODE_CMP_VAL:
    case OPCODE_CMN_VAL:
    case OPCODE_SUB_VAL:
    case OPCODE_RSB_VAL:
    case OPCODE_ADD_VAL:
    case OPCODE_ADC_VAL:
    case OPCODE_SBC_VAL:
    case OPCODE_RSC_VAL:
        if(s == 1){
            cpu.getCPSR().setNFlag(cpu.getALU().getN());
            cpu.getCPSR().setZFlag(cpu.getALU().getZ());
            cpu.getCPSR().setCFlag(cpu.getALU().getC());
            cpu.getCPSR().setVFlag(cpu.getALU().getV());
            if(rdIsPC){
                throw std::runtime_error("ERROR DataProcessing::doExecute 88");
            }
        }
        break;
    default:
        std::cerr << "ERROR: Invalid Opcode in DataProcessing::doExecute" << std::endl;
        break;
    }
}
