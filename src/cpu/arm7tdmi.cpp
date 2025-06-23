#include "arm7tdmi.hpp"
#include "op/arm/arm_opcode.hpp"
#include "op/arm_pool.hpp"
#include "op/thumb/thumb_opcode.hpp"
#include "op/thumb_pool.hpp"
#include "components/exception_handler.hpp"
#include <iostream>

ARM7TDMI::ARM7TDMI(MemoryManager* memoryManager) : armPool(*this), thumbPool(*this), mem(memoryManager) {
    // cpsr = PSR();
    for (size_t i = 0; i < REG_CNT; i++) {
        reg[i] = 0;
    }

    for (size_t i = 0; i < 7; i++) {
        r8_fiq[i] = 0;
    }

    for (size_t i = 0; i < 2; i++) {
        r13_svc[i] = 0;
        r13_abt[i] = 0;
        r13_irq[i] = 0;
        r13_und[i] = 0;
    }

    // ???
    reg[0] = 0x00000CA5;
    reg[REG_SP] = 0x03007F00;
    reg[REG_LR] = 0x08000000;
    r13_irq[0] = 0x03007FA0;
    r13_svc[0] = 0x03007FE0;

    barrelShifter = std::make_unique<BarrelShifter>();
    exceptionHandler = std::make_unique<ExceptionHandler>(*this);

    spsr_fiq.setValue(0xF00000FF);

    generateArmDecodingLookup();
    generateThumbDecodingLookup();
}

ARM7TDMI::~ARM7TDMI() {}

OpCode* ARM7TDMI::decodeInstruction(uint32_t op) {
    if (cpsr.isThumbMode()) {
        return decodeInstructionThumb(op);
    } else {
        return decodeInstructionARM(op);
    }
}

static OpCode* armOpcodeInstance[4096];

void ARM7TDMI::generateArmDecodingLookup() {
    for (size_t i = 0; i < 4096; i++) {
        // We look at bits 4-7 and 20-27 for decoding
        uint32_t op = ((i & 0b111111110000) << (20 - 4)) | ((i & 0b1111) << 4);

        ArmOpcode::OpCodeEnum armEnum;
        if (ArmOpcode::isBranchAndExchange(op)) {
            armEnum = ArmOpcode::OpCodeEnum::BRANCH_AND_EXCHANGE;
        } else if (ArmOpcode::isBlockDataTransfer(op)) {
            armEnum = ArmOpcode::OpCodeEnum::BLOCK_DATA_TRANSFER;
        } else if (ArmOpcode::isBranch(op)) {
            armEnum = ArmOpcode::OpCodeEnum::BRANCH;
        } else if (ArmOpcode::isSoftwareInterrupt(op)) {
            armEnum = ArmOpcode::OpCodeEnum::SOFTWARE_INTERRUPT;
        } else if (ArmOpcode::isUndefined(op)) {
            armEnum = ArmOpcode::OpCodeEnum::UNDEFINED;
        } else if (ArmOpcode::isSingleDataTransfer(op)) {
            armEnum = ArmOpcode::OpCodeEnum::SINGLE_DATA_TRANSFER;
        } else if (ArmOpcode::isSingleDataSwap(op)) {
            armEnum = ArmOpcode::OpCodeEnum::SINGLE_DATA_SWAP;
        } else if (ArmOpcode::isMultiply(op)) {
            armEnum = ArmOpcode::OpCodeEnum::MULTIPLY;
        } else if (ArmOpcode::isMultiplyLong(op)) {
            armEnum = ArmOpcode::OpCodeEnum::MULTIPLY_LONG;
        } else if (ArmOpcode::isHalfwordDataTransferRegister(op)) {
            armEnum = ArmOpcode::OpCodeEnum::HALFWORD_DATA_TRANSFER_REGISTER;
        } else if (ArmOpcode::isHalfwordDataTransferOffset(op)) {
            armEnum = ArmOpcode::OpCodeEnum::HALFWORD_DATA_TRANSFER_OFFSET;
        } else if (ArmOpcode::isPSRTransferMRS(op)) {
            armEnum = ArmOpcode::OpCodeEnum::PSR_TRANSFER_MRS;
        } else if (ArmOpcode::isPSRTransferMSR(op)) {
            armEnum = ArmOpcode::OpCodeEnum::PSR_TRANSFER_MSR;
        } else if (ArmOpcode::isDataProcessing(op)) {
            armEnum = ArmOpcode::OpCodeEnum::DATA_PROCESSING;
        } else {
            armEnum = ArmOpcode::OpCodeEnum::UNKNOWN;
        }

        armOpcodeInstance[i] = armPool.getArmInstance(armEnum);
    }
}

static OpCode* thumbOpcodeInstance[256];

void ARM7TDMI::generateThumbDecodingLookup() {
    for (size_t i = 0; i < 256; i++) {
        uint16_t op = i << 8;

        ThumbOpCode::OpCodeEnum thumbEnum;
        if (ThumbOpCode::isSoftwareInterrupt(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::SOFTWARE_INTERRUPT;
        } else if (ThumbOpCode::isAddOffsetToSP(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::ADD_OFFSET_TO_SP;
        } else if (ThumbOpCode::isPushPopRegisters(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::PUSH_POP_REGISTERS;
        } else if (ThumbOpCode::isALUOperations(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::ALU_OPERATIONS;
        } else if (ThumbOpCode::isHiRegisterBranchExchange(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::HI_REGISTER_BRANCH_EXCHANGE;
        } else if (ThumbOpCode::isPCRelativeLoad(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::PC_RELATIVE_LOAD;
        } else if (ThumbOpCode::isLoadStoreRegisterOffset(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::LOAD_STORE_REGISTER_OFFSET;
        } else if (ThumbOpCode::isLoadStoreSignExtended(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::LOAD_STORE_SIGN_EXTENDED;
        } else if (ThumbOpCode::isUnconditionalBranch(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::UNCONDITIONAL_BRANCH;
        } else if (ThumbOpCode::isAddSubtract(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::ADD_SUBTRACT;
        } else if (ThumbOpCode::isLoadStoreHalfword(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::LOAD_STORE_HALFWORD;
        } else if (ThumbOpCode::isSPLoadStore(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::SP_LOAD_STORE;
        } else if (ThumbOpCode::isLoadAddress(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::LOAD_ADDRESS;
        } else if (ThumbOpCode::isMultipleLoadStore(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::MULTIPLE_LOAD_STORE;
        } else if (ThumbOpCode::isConditionalBranch(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::CONDITIONAL_BRANCH;
        } else if (ThumbOpCode::isLongBranchWithLink(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::LONG_BRANCH_WITH_LINK;
        } else if (ThumbOpCode::isMoveShiftedRegister(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::MOVE_SHIFTED_REGISTER;
        } else if (ThumbOpCode::isMoveCompAddSubImm(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::MOVE_COMP_ADD_SUB_IMM;
        } else if (ThumbOpCode::isLoadStoreImmOffset(op)) {
            thumbEnum = ThumbOpCode::OpCodeEnum::LOAD_STORE_IMM_OFFSET;
        } else {
            thumbEnum = ThumbOpCode::OpCodeEnum::UNKNOWN;
        }

        thumbOpcodeInstance[i] = thumbPool.getThumbInstance(thumbEnum);
    }
}

OpCode* ARM7TDMI::decodeInstructionARM(uint32_t op) {

    uint32_t i = ((op & 0b1111111100000000000000000000) >> (20 - 4)) | ((op & 0b11110000) >> 4);

    OpCode* poolOpcode = armOpcodeInstance[i];
    poolOpcode->init(op);

    return poolOpcode;
}

OpCode* ARM7TDMI::decodeInstructionThumb(uint16_t op) {
    OpCode* poolOpcode = thumbOpcodeInstance[op >> 8];
    poolOpcode->init(op);

    return poolOpcode;
}

PSR& ARM7TDMI::getCPSR() { return cpsr; }

PSR& ARM7TDMI::getCorrespondingSPSR() {
    switch (cpsr.getMode()) {
    case PSR::Mode::FIQ:
        return spsr_fiq;
    case PSR::Mode::Supervisor:
        return spsr_svc;
    case PSR::Mode::Abort:
        return spsr_abt;
    case PSR::Mode::IRQ:
        return spsr_irq;
    case PSR::Mode::Undefined:
        return spsr_und;
    // ARM test 254
    // https://github.com/jsmolka/gba-tests/blob/master/arm/psr_transfer.asm
    case PSR::Mode::System:
    case PSR::Mode::User:
        return spsr_sys_usr;
    default:
        throw std::runtime_error("Error: Unrecognized CPSR mode in ARM7TDMI::getCPSR()");
        break;
    }
}

PSR& ARM7TDMI::getSPSR() { return getCorrespondingSPSR(); }

uint32_t ARM7TDMI::getSPSRval() {
    switch (cpsr.getMode()) {
    case PSR::Mode::FIQ:
        return spsr_fiq.getValue();
    case PSR::Mode::Supervisor:
        return spsr_svc.getValue();
    case PSR::Mode::Abort:
        return spsr_abt.getValue();
    case PSR::Mode::IRQ:
        return spsr_irq.getValue();
    case PSR::Mode::Undefined:
        return spsr_und.getValue();
    case PSR::Mode::User:
    case PSR::Mode::System:
        return 0x00000000;
    default:
        throw std::runtime_error("Error: Unrecognized CPSR mode in ARM7TDMI::getCPSR()");
        break;
    }
}

void ARM7TDMI::setCPSR(uint32_t val) { cpsr.setValue(val); }

void ARM7TDMI::setSPSR(uint32_t val) { getCorrespondingSPSR().setValue(val); }

ALU& ARM7TDMI::getALU() { return alu; }

BarrelShifter& ARM7TDMI::getBarrelShifter() { return *barrelShifter; }

ExceptionHandler& ARM7TDMI::getExceptionHandler() { return *exceptionHandler; }

int64_t ARM7TDMI::fetchInstructionThumb(uint32_t offset) { return mem->readHalfWord(offset); }

int64_t ARM7TDMI::fetchInstructionArm(uint32_t offset) { return mem->readWord(offset); }

uint32_t ARM7TDMI::getReg(uint16_t n, bool userBank) {
    if (n > REG_CNT - 1) {
        throw std::runtime_error("ERROR: Invalid reg number in getReg: " + std::to_string(n));
    }

    if (n == REG_PC) {
        return reg[REG_PC];
    }

    if (!userBank) {
        switch (cpsr.getMode()) {
        case PSR::Mode::System:
        case PSR::Mode::User:
            return reg[n];
            break;
        case PSR::Mode::FIQ:
            if (n <= 7)
                return reg[n];
            else if (n >= 8 && n <= 14)
                return r8_fiq[n - 8];
            break;
        case PSR::Mode::Supervisor:
            if (n <= 12)
                return reg[n];
            else if (n >= 13 && n <= 14)
                return r13_svc[n - 13];
            break;
        case PSR::Mode::Abort:
            if (n <= 12)
                return reg[n];
            else if (n >= 13 && n <= 14)
                return r13_abt[n - 13];
            break;
        case PSR::Mode::IRQ:
            if (n <= 12)
                return reg[n];
            else if (n >= 13 && n <= 14)
                return r13_irq[n - 13];
            break;
        case PSR::Mode::Undefined:
            if (n <= 12)
                return reg[n];
            else if (n >= 13 && n <= 14)
                return r13_und[n - 13];
            break;
        default:
            throw std::runtime_error("ERROR: Unknown CPSR Mode in CPU getReg");
            return 0;
            break;
        }
    } else {
        return reg[n];
    }

    return 0;
}

void ARM7TDMI::setReg(uint16_t n, uint32_t val, bool userBank) {
    if (n > REG_CNT - 1) {
        throw std::runtime_error("ERROR: Invalid reg number in setReg: " + std::to_string(n));
    }

    if (!userBank) {
        if (n == REG_PC) {
            reg[n] = val & 0xFFFFFFFE;
        } else {
            switch (cpsr.getMode()) {
            case PSR::Mode::System:
            case PSR::Mode::User:
                reg[n] = val;
                break;
            case PSR::Mode::FIQ:
                if (n <= 7)
                    reg[n] = val;
                else if (n >= 8 && n <= 14)
                    r8_fiq[n - 8] = val;
                break;
            case PSR::Mode::Supervisor:
                if (n <= 12)
                    reg[n] = val;
                else if (n >= 13 && n <= 14)
                    r13_svc[n - 13] = val;
                break;
            case PSR::Mode::Abort:
                if (n <= 12)
                    reg[n] = val;
                else if (n >= 13 && n <= 14)
                    r13_abt[n - 13] = val;
                break;
            case PSR::Mode::IRQ:
                if (n <= 12)
                    reg[n] = val;
                else if (n >= 13 && n <= 14)
                    r13_irq[n - 13] = val;
                break;
            case PSR::Mode::Undefined:
                if (n <= 12)
                    reg[n] = val;
                else if (n >= 13 && n <= 14)
                    r13_und[n - 13] = val;
                break;
            default:
                throw std::runtime_error("ERROR: Invalid CPSR Mode in ARM7TDMI::setReg");
                break;
            }
        }
    } else {
        reg[n] = val;
    }
}

bool ARM7TDMI::getMustFlushPipeline() const { return mustFlushPipeline; }

void ARM7TDMI::setMustFlushPipeline(bool val) { mustFlushPipeline = val; }

uint32_t ARM7TDMI::getPC() const { return reg[REG_PC]; }

void ARM7TDMI::setPC(uint32_t pc) { setReg(REG_PC, pc); }

void ARM7TDMI::setLR(uint32_t lr) { setReg(REG_LR, lr); }

void ARM7TDMI::printStatus() {
    const std::string offStr = "-";
    std::string n = cpsr.getNFlag() ? "N" : offStr;
    std::string z = cpsr.getZFlag() ? "Z" : offStr;
    std::string c = cpsr.getCFlag() ? "C" : offStr;
    std::string v = cpsr.getVFlag() ? "V" : offStr;
    std::string i = cpsr.getIFlag() ? "I" : offStr;
    std::string f = cpsr.getFFlag() ? "F" : offStr;
    std::string t = cpsr.getTFlag() ? "T" : offStr;
    std::string insMode = cpsr.getTFlag() ? "THUMB" : "ARM";

    // TODO: take thumb mode into account
    // PC is reduced by 8 to account for pipeline parallelism
    std::cout << "cpsr: " << Utils::toHexString(cpsr.getValue(), 8) << "\t"
              << "[" << n << z << c << v << i << f << t << "]"
              << "\t" << insMode << "\t";

    std::cout << cpsr.getModeString() << '\n';

    std::cout << "spsr: " << Utils::toHexString(getSPSRval(), 8) << '\n';

    for (size_t it = 0; it < REG_CNT; it++) {
        std::cout << std::setw(3);
        std::cout << "r" + std::to_string(it) << ": " << Utils::toHexString(getReg(it), 8) << " ";
        if (it != 0 && (it + 1) % 4 == 0)
            std::cout << '\n';
    }
    std::cout << '\n';
}

void ARM7TDMI::printRegisterValues() {
    for (size_t i = 0; i < REG_CNT; i++) {
        std::cout << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << getReg(i) << " " << std::dec;
    }
}

MemoryManager& ARM7TDMI::getMemManager() { return (*mem); }

/*void ARM7TDMI::executeOpArm(OpCode *op){
        op->execute();
}*/

/*
Mis-aligned PC/R15 (branch opcodes, or MOV/ALU/LDR with Rd=R15)
For ARM code, the low bits of the target address should be usually zero, otherwise, R15 is forcibly aligned by clearing
the lower two bits.

For THUMB code, the low bit of the target address may/should/must be set, the bit is (or is not) interpreted as
thumb-bit (depending on the opcode), and R15 is then forcibly aligned by clearing the lower bit.

In short, R15 will be always forcibly aligned, so mis-aligned branches won’t have effect on subsequent opcodes that use
R15, or [R15+disp] as operand.

*/

uint32_t ARM7TDMI::fetchNextInstruction() {
    uint32_t pc = getPC();
    uint32_t ins;
    if (cpsr.isThumbMode()) {
        ins = mem->readHalfWord(pc, true);
        setPC(getPC() + 2);
    } else {
        ins = mem->readWord(pc, true);
        setPC(getPC() + 4);
    }

    return ins;
}

uint32_t ARM7TDMI::executeNextInstruction() {
    uint32_t cpuCycles = 0;
    static bool printDebug = false;
    // Flush for IRQ
    if (getMustFlushPipeline()) {
        insFetchSet = false;
        insDecodeSet = false;
        setMustFlushPipeline(false);
    }
    // execute
    if (insDecodeSet) {
        // print status
        std::string opString;
        if (printDebug) {
            opString = opExecute->toString();
            // std::cout << opExecute->toString() <<  " - " << opExecute->toHexString() << '\n';
        }
        insExecuteSet = opExecute->execute();
        cpuCycles = opExecute->cyclesUsed();
        if (printDebug) {
            // printStatus();
            // std::cout << "<<<" << '\n';
            printRegisterValues();
            std::cout << "cpsr: " << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << cpsr.getValue() << std::dec;
            std::cout << " | " << opString << '\n';
        }

        // flush pipeline if needed
        // dont flush if op is not executed
        if (insExecuteSet && getMustFlushPipeline()) {
            insFetchSet = false;
            insDecodeSet = false;
            setMustFlushPipeline(false);
        }

        if (insExecuteSet) {
            insExecuteSet = false;
        }

        opExecute = nullptr;
    }

    // decode
    if (insFetchSet) {
        if (opExecute != nullptr)
            opExecute = nullptr;
        opExecute = decodeInstruction(insDecode);
        // opExecute->decode();
        insDecodeSet = true;
    }

    // fetch
    fetchPC = getPC();
    insFetch = fetchNextInstruction();

    // Force extra read for OpenBus values
    if (cpsr.isThumbMode()) {
        mem->readHalfWord(fetchPC + 4, true);
    } else {
        mem->readWord(fetchPC + 4, true);
    }

    insFetchSet = true;
    insDecode = insFetch;

    if (insDecodeSet && !cpsr.getIFlag()) {
        exceptionHandler->handleException();
    }

    return cpuCycles;
}

ArmPool& ARM7TDMI::getArmPool() { return armPool; }

bool ARM7TDMI::isPcInBios() const { return getPC() <= MemoryManager::BIOS_OFFSET_END; }