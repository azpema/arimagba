#include "data_processing.hpp"
#include "../../utils/utils.hpp"
#include <string>

DataProcessing::DataProcessing(uint32_t op): OpCode::OpCode(op) {
    dataOpCode = Utils::getRegBits(op, OPCODE_MASK, OPCODE_SHIFT);
    I = Utils::getRegBits(op, IMMEDIATE_OPERAND_MASK, IMMEDIATE_OPERAND_SHIFT);
    S = Utils::getRegBits(op, SET_CONDITION_MASK, SET_CONDITION_SHIFT);
    Rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
    Rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);

    if(I == 0){
        operand2 = new ShiftRm(Utils::getRegBits(op, OPERAND2_MASK, OPERAND2_SHIFT));
    }else if (I == 1){
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
        mnemonic += SFlag2Mnemonic[S] + " r" + std::to_string(Rd) + "," + getOperand2Mnemonic();
        break;
    // <opcode>{cond} Rn,<Op2>
    case OPCODE_CMP_VAL:
    case OPCODE_CMN_VAL:
    case OPCODE_TEQ_VAL:
    case OPCODE_TST_VAL:
        mnemonic += " r" + std::to_string(Rn) + "," + getOperand2Mnemonic();
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
        mnemonic += SFlag2Mnemonic[S] + " r" + std::to_string(Rd) + ",r" + std::to_string(Rn) + "," + getOperand2Mnemonic();
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
    return "r" + std::to_string(Rd);
}

std::string DataProcessing::getRnMnemonic(){
    return "r" + std::to_string(Rn);
}

uint32_t DataProcessing::getOperand2Imm(){
    if(I == 1){
        //return Utils::rotateRight(Imm, 2*Rotate);
    }else
        return -1;
}

uint32_t DataProcessing::getOperand2Rm(){
    // TODO
    return -12345;
}

std::string DataProcessing::getOperand2Mnemonic(){
    if(I == 0){
        ShiftRm* shiftRm = static_cast<ShiftRm*>(operand2);
        return "r" + std::to_string(shiftRm->getRm()) + "," + shiftRm->getShiftTypeMnemonic() + " " + Utils::toHexString(shiftRm->getShiftAmount());
    }else if(I == 1){
        RotateImm* rotateImm = static_cast<RotateImm*>(operand2);
        return "#" + Utils::toHexString(rotateImm->getOperandVal()); 
    }else{
        return "ERROR getOperand2Mnemonic Data_processing";
    }
}

