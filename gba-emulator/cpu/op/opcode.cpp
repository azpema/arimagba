#include "opcode.hpp"
#include "../../utils/utils.hpp"

const std::string OpCode::reg2Mnemonic[16] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
                                              "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc"};

OpCode::OpCode(uint32_t op) {
    opcode = op;
}

void OpCode::execute(ARM7TDMI &cpu) {
    // Execute only if conditions are met
    do_execute(cpu);
}

 /*std::string OpCode::toString(){
    return "";
 }*/

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

uint32_t OpCode::getCondField(){
    return Utils::getRegBits(opcode, COND_FIELD_MASK, COND_FIELD_SHIFT);
}

std::string OpCode::getCondFieldMnemonic(){
	return condCode2Suffix[getCondField()];
}

std::string OpCode::getRegMnemonic(uint16_t reg){
    return reg2Mnemonic[reg];
}