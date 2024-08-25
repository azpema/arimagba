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
        throw std::runtime_error("ERROR: Invalid i value in DataProcessing::DataProcessing");
    }
}

DataProcessing::DataProcessing(uint8_t i, uint8_t opCode, uint8_t s, uint8_t rn, uint8_t rd, uint16_t operand2,
 ARM7TDMI &cpu, bool overrideOperands, uint32_t op1Val) : ArmOpcode::ArmOpcode(cpu){
    this->dataOpCode = opCode;
    this->i = i;
    this->s = s;
    this->rn = rn;
    this->rd = rd;

    this->overrideOperands = overrideOperands;
    this->op1Val = op1Val;

    if(i == 0){
        this->operand2 = new ShiftRm(operand2);
    }else if (i == 1){
        this->operand2 = new RotateImm(operand2);
    }else{
        throw std::runtime_error("ERROR: Invalid i value in DataProcessing::DataProcessing");
    }

    uint32_t raw = Condition::AL << COND_FIELD_SHIFT;
    Utils::setRegBits(raw, OPCODE_MASK, opCode << OPCODE_SHIFT);
    Utils::setRegBits(raw, IMMEDIATE_OPERAND_MASK, i << IMMEDIATE_OPERAND_SHIFT);
    Utils::setRegBits(raw, SET_CONDITION_MASK, s << SET_CONDITION_SHIFT);
    Utils::setRegBits(raw, RN_MASK, rn << RN_SHIFT);
    Utils::setRegBits(raw, RD_MASK, rd << RD_SHIFT);
    Utils::setRegBits(raw, OPERAND2_MASK, operand2 << OPERAND2_SHIFT);

    setRawVal(raw);
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
    throw std::runtime_error("ERROR: TODO DataProcessing::getOperand2Rm");
}

std::string DataProcessing::getOperand2Mnemonic(){
    if(i == 0){
        ShiftRm* shiftRm = static_cast<ShiftRm*>(operand2);
        std::string res = OpCode::getRegMnemonic(shiftRm->getRm()) + "," + shiftRm->getShiftTypeMnemonic();
        if(shiftRm->getType() == ShiftRm::Type::AMOUNT)
            res += " #" + Utils::toHexString(shiftRm->getShiftAmount());
        else if(shiftRm->getType() == ShiftRm::Type::REGISTER){
            res += " " + OpCode::getRegMnemonic(shiftRm->getShiftReg());
        }else{
            throw std::runtime_error("ERROR: Invalid shiftRm->getType() value in DataProcessing::getOperand2Mnemonic");
        }
        return res;
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
    //cpu.setReg(rd, cpu.getALU().mov(op2));
    uint32_t res = cpu.getALU().mov(op2);
    cpu.setReg(rd, res);
}

void DataProcessing::doExecuteAdd(ARM7TDMI &cpu){
    uint32_t addRes = cpu.getALU().add(op1, op2);
    cpu.setReg(rd, addRes);
}

void DataProcessing::doExecuteOrr(ARM7TDMI &cpu){
    uint32_t orrRes = cpu.getALU().orr(op1, op2);
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

void DataProcessing::doExecuteSub(ARM7TDMI &cpu){
    uint32_t subRes = cpu.getALU().sub(op1, op2);
    cpu.setReg(rd, subRes);
}

void DataProcessing::doExecuteAdc(ARM7TDMI &cpu){
    uint32_t adcRes = cpu.getALU().adc(op1, op2, cpu.getCPSR().getCFlag());
    cpu.setReg(rd, adcRes);
}

void DataProcessing::doExecuteSbc(ARM7TDMI &cpu){
    uint32_t carry = cpu.getCPSR().getCFlag();
    uint32_t sbcRes = cpu.getALU().sbc(op1, op2, carry);
    cpu.setReg(rd, sbcRes);
}

void DataProcessing::doExecuteEor(ARM7TDMI &cpu){
    uint32_t eorRes = cpu.getALU().eor(op1, op2);
    cpu.setReg(rd, eorRes); 
}

void DataProcessing::doExecuteBic(ARM7TDMI &cpu){
    uint32_t bicRes = cpu.getALU().andOp(op1, ~op2);
    cpu.setReg(rd, bicRes);
}

void DataProcessing::doExecuteRsb(ARM7TDMI &cpu){
    uint32_t rsbRes = cpu.getALU().sub(op2, op1);
    cpu.setReg(rd, rsbRes);
}

void DataProcessing::doExecuteRsc(ARM7TDMI &cpu){
    uint32_t carry = cpu.getCPSR().getCFlag();
    uint32_t sbcRes = cpu.getALU().sbc(op2, op1, carry);
    cpu.setReg(rd, sbcRes);
}

void DataProcessing::doExecuteCmn(ARM7TDMI &cpu){
    cpu.getALU().add(op1, op2);
}

void DataProcessing::doExecuteTeq(ARM7TDMI &cpu){
    cpu.getALU().eor(op1, op2);
}

void DataProcessing::doDecode(){

}

void DataProcessing::doExecute(){
    // Assign values to op1 and op2
    // Do you really need to flush when rd=15 in cmp r15, r0, for exmple?
    if(rd == 15 && dataOpCode != OPCODE_CMP_VAL){
        cpu.setMustFlushPipeline(true);
    }
        
    if(!overrideOperands){
        op1 = cpu.getReg(rn);
    }else {
        op1 = op1Val;
    }

    op2 = operand2->getOperandVal(cpu);

    if(i == 0){
        ShiftRm* shiftRm = static_cast<ShiftRm*>(operand2);
        if(shiftRm->getRm() == 15){
            if(shiftRm->getType() == ShiftRm::Type::AMOUNT){
                // op2 += 8;
                // PC is already 8 bytes ahead
            }else if(shiftRm->getType() == ShiftRm::Type::REGISTER){
                // op2 += 12;
                // PC is already 8 bytes ahead
                op2 += 4;
            }else{
                throw std::runtime_error("ERROR: Invalid shiftRm->getType() value in DataProcessing::getOperand2Mnemonic");
            }
        }
        if(rn == 15){
            if(shiftRm->getType() == ShiftRm::Type::AMOUNT){
                // op1 += 8;
                // PC is already 8 bytes ahead
            }else if(shiftRm->getType() == ShiftRm::Type::REGISTER){
                // op1 += 12;
                // PC is already 8 bytes ahead
                op1 += 4;
            }else{
                throw std::runtime_error("ERROR: Invalid shiftRm->getType() value in DataProcessing::getOperand2Mnemonic");
            }
        }
    }else if(i == 1){
        //throw std::runtime_error("ERROR: Unimplemented DataProcessing::doExecute rn==15 i==1");
        // TODO ?¿?¿?¿?
    }else{
        throw std::runtime_error("ERROR: Invalid I value DataProcessing::doExecute");
    }

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
        doExecuteCmn(cpu);
        break;
    case OPCODE_TEQ_VAL:
        doExecuteTeq(cpu);
        break;
    case OPCODE_TST_VAL:
        doExecuteTst(cpu);
        break;
    case OPCODE_AND_VAL:
        doExecuteAnd(cpu);
        break;
    case OPCODE_EOR_VAL:
        doExecuteEor(cpu);
        break;
    case OPCODE_SUB_VAL:
        doExecuteSub(cpu);
        break;
    case OPCODE_RSB_VAL:
        doExecuteRsb(cpu);
        break;
    case OPCODE_ADD_VAL:
        doExecuteAdd(cpu);
        break;
    case OPCODE_ADC_VAL:
        doExecuteAdc(cpu);
        break;
    case OPCODE_SBC_VAL:
        doExecuteSbc(cpu);
        break;
    case OPCODE_RSC_VAL:
        doExecuteRsc(cpu);
        break;
    case OPCODE_ORR_VAL:
        doExecuteOrr(cpu);
        break;
    case OPCODE_BIC_VAL:
        doExecuteBic(cpu);
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
                    //cpu.getCPSR().setCFlag(cpu.getBarrelShifter().getC());
                    cpu.getCPSR().setCFlag(shiftRm->getC());
                }
            }else if(i == 1){
                RotateImm* rotateImm = static_cast<RotateImm*>(operand2);
                if(rotateImm->getRorShiftAmount() != 0)
                    cpu.getCPSR().setCFlag(rotateImm->getC());
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
        }
        break;
    default:
        std::cerr << "ERROR: Invalid Opcode in DataProcessing::doExecute" << std::endl;
        break;
    }

    /*
        When Rd is R15 and the S flag is set the result of the operation is placed in R15 and
        the SPSR corresponding to the current mode is moved to the CPSR
    */
    if(rd == 15 && (s==1 || OPCODE_CMP_VAL)){
        PSR::Mode mode = cpu.getCPSR().getMode();
        if(mode != PSR::Mode::User && mode != PSR::Mode::System){
            cpu.setCPSR(cpu.getSPSR().getValue());
        }else{
            // User and System modes have no SPSR, so do nothing...
            std::cout << "eee" << std::endl;
        }
    }
}
