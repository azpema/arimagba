#include "psr_transfer_msr_flag_bits.hpp"
#include "../fields/rotate_imm.hpp"
#include "../fields/rm.hpp"

PSRTransferMSRFlagBits::PSRTransferMSRFlagBits(uint32_t op): PSRTransferMSR::PSRTransferMSR(op) {
    I = Utils::getRegBits(op, I_MASK, I_SHIFT);

    if(I == 0){
        sourceOperand = new Rm(Utils::getRegBits(op, SOURCE_OPERAND_MASK, SOURCE_OPERAND_SHIFT));
    }else if (I == 1){
        sourceOperand = new RotateImm(Utils::getRegBits(op, SOURCE_OPERAND_MASK, SOURCE_OPERAND_SHIFT));
    }else{
        std::cerr << "ERROR: Invalid I field value" << std::endl;
    }
}

PSRTransferMSRFlagBits::~PSRTransferMSRFlagBits(){
    delete sourceOperand;
}

std::string PSRTransferMSRFlagBits::toString() {
    std::string srcOper = "";
    if(sourceOperand->_type == Operand::RM){
        Rm* rm = static_cast<Rm*>(sourceOperand);
        srcOper = getRegMnemonic(rm->getRmVal());
    }else if(sourceOperand->_type == Operand::ROTATE_IMM){
        RotateImm* rotImm = static_cast<RotateImm*>(sourceOperand);
        srcOper = "#" + Utils::toHexString(rotImm->getOperandVal());
    }else{
        std::cerr << "ERROR: Invald PSRTransferMSRFlagBits::toString sourceOperand type " << sourceOperand->_type << std::endl;
    }
    
    return PSRTransferMSR::toString() + "," + srcOper;
}

void PSRTransferMSRFlagBits::doExecute(ARM7TDMI &cpu){

}