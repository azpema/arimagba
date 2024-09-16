#include "psr_transfer_msr.hpp"
#include <bitset>

const std::string PSRTransferMSR::PSR2Mnemonic[2] = {"cpsr", "spsr"};

PSRTransferMSR::PSRTransferMSR(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    psr = Utils::getRegBits(op, PSR_MASK, PSR_SHIFT);
    I = Utils::getRegBits(op, I_MASK, I_SHIFT);
    c = Utils::getRegBits(op, C_MASK, C_SHIFT);

    if(I == 0){
        sourceOperand = std::make_unique<Rm>(Utils::getRegBits(op, SOURCE_OPERAND_MASK, SOURCE_OPERAND_SHIFT));
    }else if (I == 1){
        sourceOperand = std::make_unique<RotateImm>(Utils::getRegBits(op, SOURCE_OPERAND_MASK, SOURCE_OPERAND_SHIFT));
    }else{
        std::cerr << "ERROR: Invalid I field value" << std::endl;
    }
}

PSRTransferMSR::~PSRTransferMSR(){
}

std::string PSRTransferMSR::getPSRMnemonic(){
    return PSR2Mnemonic[psr];
}

/*bool PSRTransferMSR::isFullTransfer(uint32_t op){
    return OpCode::checkOpCode(op, FULL_TRANSFER_MASK, FULL_TRANSFER_FORMAT);
}

bool PSRTransferMSR::isFlagBitsTransfer(uint32_t op){
    return OpCode::checkOpCode(op, FLAG_BITS_TRANSFER_MASK, FLAG_BITS_TRANSFER_FORMAT);
}*/


uint32_t PSRTransferMSR::cyclesUsed() const {
    return 1 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE;
}


std::string PSRTransferMSR::toString() {
    std::string srcOper = "";
    if(sourceOperand->_type == Operand::RM){
        Rm* rm = static_cast<Rm*>(sourceOperand.get());
        srcOper = OpCode::getRegMnemonic(rm->getRmVal());
    }else if(sourceOperand->_type == Operand::ROTATE_IMM){
        RotateImm* rotImm = static_cast<RotateImm*>(sourceOperand.get());
        srcOper = "#" + Utils::toHexString(rotImm->getMnemonicVal());
    }else{
        std::cerr << "ERROR: Invald PSRTransferMSRFlagBits::toString sourceOperand type " << sourceOperand->_type << std::endl;
    }
    
    std::string cMnemonic = "";
    if(c == 1){
        cMnemonic = "c";
    }

    return "msr" + getCondFieldMnemonic() + " " + getPSRMnemonic() + "_f" + cMnemonic + "," + srcOper;
}

void PSRTransferMSR::doDecode(){

}

void PSRTransferMSR::doExecute(){
    uint32_t newVal = cpu.getCPSR().getValue();

    uint32_t newFlags = 0;
    if(sourceOperand->_type == Operand::RM){
        Rm* rm = static_cast<Rm*>(sourceOperand.get());
        newFlags = cpu.getReg(rm->getRmVal());
    }else if(sourceOperand->_type == Operand::ROTATE_IMM){
        RotateImm* rotImm = static_cast<RotateImm*>(sourceOperand.get());
        newFlags = rotImm->getMnemonicVal();
    }else{
        throw new std::runtime_error("ERROR: Invald PSRTransferMSRFlagBits::toString sourceOperand type " + std::to_string(sourceOperand->_type));
    }

    // Corresponds to flag bits (NZCV)
    Utils::setRegBits(newVal, 0xF0000000, newFlags);

    // Set control bits
    if(c == 1){
        Utils::setRegBits(newVal, 0x000000FF, newFlags);
    }

    if(psr == 0){
        cpu.getCPSR().setValue(newVal);
    }else if(psr == 1){
        cpu.setSPSR(newVal);
    }else{
        throw new std::runtime_error("ERROR: Invalid PSR value in PSRTransferMSRFlagBits::doExecute");
    }
    
}
