#include "multiply_accumulate.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

const std::string MultiplyAccumulate::sFlag2Mnemonic[2] = {"", "s"};
const std::string MultiplyAccumulate::op2Mnemonic[2] = {"mul", "mla"};

MultiplyAccumulate::MultiplyAccumulate(uint32_t op, ARM7TDMI& cpu) : ArmOpcode::ArmOpcode(op, cpu) { init(op); }

MultiplyAccumulate::MultiplyAccumulate(
    uint32_t a, uint32_t s, uint32_t rd, uint32_t rn, uint32_t rs, uint32_t rm, ARM7TDMI& cpu) :
    ArmOpcode::ArmOpcode(cpu) {
    init(a, s, rd, rn, rs, rm);
}

MultiplyAccumulate::MultiplyAccumulate(ARM7TDMI& cpu) : ArmOpcode::ArmOpcode(cpu) {}

void MultiplyAccumulate::init(uint32_t op) {
    ArmOpcode::init(op);
    a = Utils::getRegBits(op, A_FLAG_MASK, A_FLAG_SHIFT);
    s = Utils::getRegBits(op, S_FLAG_MASK, S_FLAG_SHIFT);
    rd = Utils::getRegBits(op, RD_FLAG_MASK, RD_FLAG_SHIFT);
    rn = Utils::getRegBits(op, RN_FLAG_MASK, RN_FLAG_SHIFT);
    rs = Utils::getRegBits(op, RS_FLAG_MASK, RS_FLAG_SHIFT);
    rm = Utils::getRegBits(op, RM_FLAG_MASK, RM_FLAG_SHIFT);
}

void MultiplyAccumulate::init(uint32_t a, uint32_t s, uint32_t rd, uint32_t rn, uint32_t rs, uint32_t rm) {
    this->a = a;
    this->s = s;
    this->rd = rd;
    this->rn = rn;
    this->rs = rs;
    this->rm = rm;
}

std::string MultiplyAccumulate::getSFlagMnemonic() { return sFlag2Mnemonic[s]; }

std::string MultiplyAccumulate::getOpMnemonic() { return op2Mnemonic[a]; }

std::string MultiplyAccumulate::toString() {
    std::string mnemonic = getOpMnemonic() + getCondFieldMnemonic() + getSFlagMnemonic() + " " +
                           OpCode::getRegMnemonic(rd) + "," + OpCode::getRegMnemonic(rm) + "," +
                           OpCode::getRegMnemonic(rs);
    if (a == 1)
        mnemonic += " " + OpCode::getRegMnemonic(rn);
    return mnemonic;
}

void MultiplyAccumulate::doDecode() {}

void MultiplyAccumulate::doExecute() {
    if (rd == rm) {
        // throw std::runtime_error("MultiplyAccumulate::doExecute: The destination register Rd must not be the same as
        // the operand register Rm");
        std::cout << "MultiplyAccumulate::doExecute: The destination register Rd must not be the same as the operand "
                     "register Rm"
                  << '\n';
    }
    if (rd == 15 || rn == 15 || rs == 15 || rm == 15) {
        throw std::runtime_error(
            "MultiplyAccumulate::doExecute: R15 must not be used as an operand or as the destination register");
    }

    uint32_t res = 0xDEAD;

    rsVal = cpu.getReg(rs);
    uint32_t rmVal = cpu.getReg(rm);
    uint32_t rnVal = cpu.getReg(rn);
    if (a == 0) {
        res = cpu.getALU().mul(rmVal, rsVal);
    } else if (a == 1) {
        res = cpu.getALU().mla(rmVal, rsVal, rnVal);
    }

    cpu.setReg(rd, res);

    if (s == 1) {
        cpu.getCPSR().setNFlag(cpu.getALU().getN());
        cpu.getCPSR().setZFlag(cpu.getALU().getZ());
    }
}

uint32_t MultiplyAccumulate::cyclesUsed() const {
    uint32_t cyclesUsed;
    uint8_t m;

    constexpr uint32_t mask1 = 0xFFFFFF00;
    constexpr uint32_t mask2 = 0xFFFF0000;
    constexpr uint32_t mask3 = 0xFF000000;

    // Unsigned
    // 1    if bits [31:8] of the multiplier operand are all zero or all one.
    // 2    if bits [31:16] of the multiplier operand are all zero or all one.
    // 3    if bits [31:24] of the multiplier operand are all zero or all one.
    // 4    in all other cases.
    if ((rsVal & mask1) == 0 || ((rsVal & mask1) == mask1)) {
        m = 1;
    } else if ((rsVal & mask2) == 0 || ((rsVal & mask2) == mask2)) {
        m = 2;
    } else if ((rsVal & mask3) == 0 || ((rsVal & mask3) == mask3)) {
        m = 3;
    } else {
        m = 4;
    }

    if (a == 0) {
        // MUL              1S+mI
        cyclesUsed = ARM7TDMI::CPU_CYCLES_PER_S_CYCLE + m * ARM7TDMI::CPU_CYCLES_PER_I_CYCLE;
    } else {
        // MLA              1S+(m+1)I
        cyclesUsed = ARM7TDMI::CPU_CYCLES_PER_S_CYCLE + (m + 1) * ARM7TDMI::CPU_CYCLES_PER_I_CYCLE;
    }

    return cyclesUsed;

}