#include "arm_opcode.hpp"

#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"
#include "../fields/operand.hpp"

const std::string ArmOpcode::condCode2Suffix[15] = {
    "eq", // Z set, equal
    "ne", // Z clear, not equal
    "cs", // C set, unsigned higher or same
    "cc", // C clear, unsigned lower
    "mi", // N set, negative
    "pl", // N clear, positive or zero
    "vs", // V set, overflow
    "vc", // V clear, no overflow
    "hi", // C set and Z clear, unsigned higher
    "ls", // C clear or Z set, unsigned lower or same
    "ge", // N equals V, greater or equal
    "lt", // N not equal to V, less than
    "gt", // Z clear AND (N equals V), greater than
    "le", // Z set OR (N not equal to V), less than or equal
    ""    // (ignored), always
};

ArmOpcode::ArmOpcode(uint32_t op, ARM7TDMI& cpu) : OpCode::OpCode(op, cpu) {
    // TODO OpCode is initialized twice!!!
    init(op);
}

ArmOpcode::ArmOpcode(ARM7TDMI& cpu) : OpCode::OpCode(cpu) {
    // Set some default values
    opcode = Condition::AL << COND_FIELD_SHIFT;
    condRaw = Utils::getRegBits(opcode, COND_FIELD_MASK, COND_FIELD_SHIFT);
    if (condRaw <= 14) {
        cond = static_cast<Condition>(condRaw);
    } else {
        throw std::runtime_error("ERROR: OpCode invalid condition in ArmOpcode");
    }
}

void ArmOpcode::init(uint32_t op) {
    OpCode::init(op);
    opcode = op;
    condRaw = Utils::getRegBits(opcode, COND_FIELD_MASK, COND_FIELD_SHIFT);
    if (condRaw <= 14) {
        cond = static_cast<Condition>(condRaw);
    } else {
        throw std::runtime_error("ERROR: OpCode invalid condition in ArmOpcode");
    }
}

std::string ArmOpcode::toHexString() { return Utils::toHexString(opcode); }

bool ArmOpcode::execute() {
    // Execute only if conditions are met
    bool execute = false;
    switch (cond) {
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

    if (execute) {
        doExecute();
        return true;
    } else {
        // std::cout << "DEBUG: Instructions skipped; condition not met" << '\n';
        return false;
    }
}

void ArmOpcode::decode() { doDecode(); }

// https://iitd-plos.github.io/col718/ref/arm-instructionset.pdf
bool ArmOpcode::isBranchAndExchange(uint32_t op) {
    return OpCode::checkOpCode(op, BRANCH_EXCHANGE_MASK, BRANCH_EXCHANGE_FORMAT);
}

bool ArmOpcode::isBlockDataTransfer(uint32_t op) {
    return OpCode::checkOpCode(op, BLOCK_DATA_TRANSFER_MASK, BLOCK_DATA_TRANSFER_FORMAT);
}

bool ArmOpcode::isBranch(uint32_t op) { return OpCode::checkOpCode(op, BRANCH_MASK, BRANCH_FORMAT); }

bool ArmOpcode::isSoftwareInterrupt(uint32_t op) {
    return OpCode::checkOpCode(op, SOFTWARE_INTERRUPT_MASK, SOFTWARE_INTERRUPT_FORMAT);
}

bool ArmOpcode::isUndefined(uint32_t op) { return OpCode::checkOpCode(op, UNDEFINED_MASK, UNDEFINED_FORMAT); }

bool ArmOpcode::isSingleDataTransfer(uint32_t op) {
    return OpCode::checkOpCode(op, SINGLE_DATA_TRANSFER_MASK, SINGLE_DATA_TRANSFER_FORMAT);
}

bool ArmOpcode::isSingleDataSwap(uint32_t op) {
    return OpCode::checkOpCode(op, SINGLE_DATA_SWAP_MASK, SINGLE_DATA_SWAP_FORMAT);
}

bool ArmOpcode::isMultiply(uint32_t op) { return OpCode::checkOpCode(op, MULTIPLY_MASK, MULTIPLY_FORMAT); }

bool ArmOpcode::isMultiplyLong(uint32_t op) {
    return OpCode::checkOpCode(op, MULTIPLY_LONG_MASK, MULTIPLY_LONG_FORMAT);
}

bool ArmOpcode::isHalfwordDataTransferRegister(uint32_t op) {
    return OpCode::checkOpCode(op, HALFWORD_DATA_TRANSFER_REG_MASK, HALFWORD_DATA_TRANSFER_REG_FORMAT);
}

bool ArmOpcode::isHalfwordDataTransferOffset(uint32_t op) {
    return OpCode::checkOpCode(op, HALFWORD_DATA_TRANSFER_IMM_MASK, HALFWORD_DATA_TRANSFER_IMM_FORMAT);
}

bool ArmOpcode::isPSRTransferMRS(uint32_t op) {
    return OpCode::checkOpCode(op, PSR_TRANSFER_MRS_MASK, PSR_TRANSFER_MRS_FORMAT);
}

bool ArmOpcode::isPSRTransferMSR(uint32_t op) {
    return OpCode::checkOpCode(op, PSR_TRANSFER_MSR_MASK, PSR_TRANSFER_MSR_FORMAT);
}

bool ArmOpcode::isDataProcessing(uint32_t op) {
    return OpCode::checkOpCode(op, DATA_PROCESSING_MASK, DATA_PROCESSING_FORMAT);
}

ArmOpcode::Condition ArmOpcode::getCondField() { return cond; }

std::string ArmOpcode::getCondFieldMnemonic() { return condCode2Suffix[cond]; }
