#include "opcode.hpp"
#include "../../utils/utils.hpp"

const std::string OpCode::reg2Mnemonic[16] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
                                              "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc"};

OpCode::OpCode(uint32_t op, ARM7TDMI& cpu) : cpu(cpu) {
    opcode = op;
    condRaw = Utils::getRegBits(opcode, COND_FIELD_MASK, COND_FIELD_SHIFT);
    if(condRaw >= 0 and condRaw <= 14){
        cond = static_cast<Condition>(condRaw);
    }else{
        std::cerr << "ERROR: OpCode invalid condition" << std::endl;
    }
}

std::string OpCode::toHexString(){
    return Utils::toHexString(opcode);
}

bool OpCode::execute(){
    // Execute only if conditions are met
    bool execute = false;
    switch(cond){
        case EQ:
            execute = cpu.getCPSR().getZFlag();
            break;
        case NE:
            execute = !cpu.getCPSR().getZFlag();
            break;
        case CS:
            execute = cpu.getCPSR().getCFlag();
            break;
        case CC:
            execute = !cpu.getCPSR().getCFlag();
            break;
        case MI:
            execute = cpu.getCPSR().getNFlag();
            break;
        case PL:
            execute = !cpu.getCPSR().getNFlag();
            break;
        case VS:
            execute = cpu.getCPSR().getVFlag();
            break;
        case VC:
            execute = !cpu.getCPSR().getVFlag();
            break;
        case HI:
            execute = (cpu.getCPSR().getCFlag()) && (!cpu.getCPSR().getZFlag());
            break;
        case LS:
            execute = (!cpu.getCPSR().getCFlag()) || (cpu.getCPSR().getZFlag());
            break;
        case GE:
            execute = (cpu.getCPSR().getNFlag() == cpu.getCPSR().getVFlag());
            break;
        case LT:
            execute = (cpu.getCPSR().getNFlag() != cpu.getCPSR().getVFlag());
            break;
        case GT:
            execute = (!cpu.getCPSR().getZFlag()) && (cpu.getCPSR().getNFlag() == cpu.getCPSR().getVFlag());
            break;
        case LE:
            execute = (cpu.getCPSR().getZFlag()) || (cpu.getCPSR().getNFlag() != cpu.getCPSR().getVFlag());
            break;
        case AL:
            execute = true;
            break;
    }
        
    if(execute){
        doExecute();
        return true;
    }else {
        std::cout << "DEBUG: Instructions skipped; condition not met" << std::endl;
        return false;
    }

}

void OpCode::decode() {
    doDecode();
}

bool OpCode::checkOpCode(uint32_t op, uint32_t mask, uint32_t format){
    return (op & mask) == format;
}

// https://iitd-plos.github.io/col718/ref/arm-instructionset.pdf
bool OpCode::isBranchAndExchange(uint32_t op){
    return checkOpCode(op, BRANCH_EXCHANGE_MASK, BRANCH_EXCHANGE_FORMAT);
}

bool OpCode::isBlockDataTransfer(uint32_t op){
    return checkOpCode(op, BLOCK_DATA_TRANSFER_MASK, BLOCK_DATA_TRANSFER_FORMAT);
}

bool OpCode::isBranch(uint32_t op){
    return checkOpCode(op, BRANCH_MASK, BRANCH_FORMAT);
}

bool OpCode::isSoftwareInterrupt(uint32_t op){
    return checkOpCode(op, SOFTWARE_INTERRUPT_MASK, SOFTWARE_INTERRUPT_FORMAT);
}

bool OpCode::isUndefined(uint32_t op){
    return checkOpCode(op, UNDEFINED_MASK, UNDEFINED_FORMAT);
}

bool OpCode::isSingleDataTransfer(uint32_t op){
    return checkOpCode(op, SINGLE_DATA_TRANSFER_MASK, SINGLE_DATA_TRANSFER_FORMAT);
}

bool OpCode::isSingleDataSwap(uint32_t op){
    return checkOpCode(op, SINGLE_DATA_SWAP_MASK, SINGLE_DATA_SWAP_FORMAT);
}

bool OpCode::isMultiply(uint32_t op){
    return checkOpCode(op, MULTIPLY_MASK, MULTIPLY_FORMAT);
}

bool OpCode::isMultiplyLong(uint32_t op){
    return checkOpCode(op, MULTIPLY_LONG_MASK, MULTIPLY_LONG_FORMAT);
}

bool OpCode::isHalfwordDataTransferRegister(uint32_t op){
    return checkOpCode(op, HALFWORD_DATA_TRANSFER_REG_MASK, HALFWORD_DATA_TRANSFER_REG_FORMAT);
}

bool OpCode::isHalfwordDataTransferOffset(uint32_t op){
    return checkOpCode(op, HALFWORD_DATA_TRANSFER_IMM_MASK, HALFWORD_DATA_TRANSFER_IMM_FORMAT);
}

bool OpCode::isPSRTransferMRS(uint32_t op){
    return checkOpCode(op, PSR_TRANSFER_MRS_MASK, PSR_TRANSFER_MRS_FORMAT);
}

bool OpCode::isPSRTransferMSR(uint32_t op){
    return checkOpCode(op, PSR_TRANSFER_MSR_MASK, PSR_TRANSFER_MSR_FORMAT);
}

bool OpCode::isDataProcessing(uint32_t op){
    return checkOpCode(op, DATA_PROCESSING_MASK, DATA_PROCESSING_FORMAT);
}

OpCode::Condition OpCode::getCondField(){
    return cond;
}

std::string OpCode::getCondFieldMnemonic(){
	return condCode2Suffix[cond];
}

std::string OpCode::getRegMnemonic(uint16_t reg){
    return reg2Mnemonic[reg];
}