#include "multiply_accumulate_long.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

const std::string MultiplyAccumulateLong::uFlag2Mnemonic[2] = {"u", "s"};
const std::string MultiplyAccumulateLong::sFlag2Mnemonic[2] = {"", "s"};
const std::string MultiplyAccumulateLong::op2Mnemonic[2] = {"mull", "mlal"};

MultiplyAccumulateLong::MultiplyAccumulateLong(uint32_t op, ARM7TDMI& cpu) : ArmOpcode::ArmOpcode(op, cpu) { init(op); }

MultiplyAccumulateLong::MultiplyAccumulateLong(ARM7TDMI& cpu) : ArmOpcode::ArmOpcode(cpu) {}

void MultiplyAccumulateLong::init(uint32_t op) {
    ArmOpcode::init(op);
    u = Utils::getRegBits(op, U_FLAG_MASK, U_FLAG_SHIFT);
    a = Utils::getRegBits(op, A_FLAG_MASK, A_FLAG_SHIFT);
    s = Utils::getRegBits(op, S_FLAG_MASK, S_FLAG_SHIFT);
    rdHi = Utils::getRegBits(op, RDHI_FLAG_MASK, RDHI_FLAG_SHIFT);
    rdLo = Utils::getRegBits(op, RDLO_FLAG_MASK, RDLO_FLAG_SHIFT);
    rs = Utils::getRegBits(op, RS_FLAG_MASK, RS_FLAG_SHIFT);
    rm = Utils::getRegBits(op, RM_FLAG_MASK, RM_FLAG_SHIFT);
}

std::string MultiplyAccumulateLong::getUFlagMnemonic() { return uFlag2Mnemonic[u]; }

std::string MultiplyAccumulateLong::getSFlagMnemonic() { return sFlag2Mnemonic[s]; }

std::string MultiplyAccumulateLong::getOpMnemonic() { return op2Mnemonic[a]; }

std::string MultiplyAccumulateLong::toString() {
    return getUFlagMnemonic() + getOpMnemonic() + getCondFieldMnemonic() + getSFlagMnemonic() + " " +
           OpCode::getRegMnemonic(rdLo) + "," + OpCode::getRegMnemonic(rdHi) + "," + OpCode::getRegMnemonic(rm) + "," +
           OpCode::getRegMnemonic(rs);
}

void MultiplyAccumulateLong::doDecode() {}

void MultiplyAccumulateLong::doExecute() {

    if (rs == 15 || rm == 15 || rdLo == 15 || rdHi == 15) {
        throw std::runtime_error(
            "Error: MultiplyAccumulateLong: R15 must not be used as an operand or as a destination register");
    }

    if (rdHi == rdLo || rdHi == rm || rdLo == rm) {
        throw std::runtime_error(
            "Error: MultiplyAccumulateLong:  RdHi, RdLo, and Rm must all specify different registers");
    }

    uint64_t uRes = 0;
    int64_t sRes = 0;
    rsVal = cpu.getReg(rs);
    uint32_t rmVal = cpu.getReg(rm);
    uint32_t rdLoVal, rdHiVal;
    if (a == 1) {
        rdLoVal = cpu.getReg(rdLo);
        rdHiVal = cpu.getReg(rdHi);
    }

    if (u == 0) {
        uRes = static_cast<uint64_t>(rsVal) * static_cast<uint64_t>(rmVal);
        if (a == 1) {
            uRes += (static_cast<uint64_t>(rdHiVal) << 32) | static_cast<uint64_t>(rdLoVal);
        }
        cpu.setReg(rdLo, uRes & 0xFFFFFFFF);
        cpu.setReg(rdHi, (uRes & 0xFFFFFFFF00000000) >> 32);
    } else if (u == 1) {
        sRes = static_cast<int64_t>(static_cast<int32_t>(rsVal)) * static_cast<int64_t>(static_cast<int32_t>(rmVal));
        if (a == 1) {
            sRes += static_cast<int64_t>((static_cast<uint64_t>(rdHiVal) << 32) | static_cast<uint64_t>(rdLoVal));
        }
        cpu.setReg(rdLo, sRes & 0xFFFFFFFF);
        cpu.setReg(rdHi, (sRes & 0xFFFFFFFF00000000) >> 32);
    } else {
        throw std::runtime_error("Error: Unknown U flag: MultiplyAccumulateLong");
    }

    if (s == 1) {
        if (u == 0) {
            cpu.getCPSR().setNFlag((uRes >> (64 - 1)) == 1);
            cpu.getCPSR().setZFlag(uRes == 0);
            // Both the C and V flags are set to meaningless values.
            // cpu.getCPSR().setCFlag();
            // cpu.getCPSR().setVFlag();
        } else {
            cpu.getCPSR().setNFlag((static_cast<uint64_t>(sRes) >> (64 - 1)) == 1);
            cpu.getCPSR().setZFlag(sRes == 0);
            // Both the C and V flags are set to meaningless values.
            // cpu.getCPSR().setCFlag();
            // cpu.getCPSR().setVFlag();
        }
    }
}

uint32_t MultiplyAccumulateLong::cyclesUsed() const {
    uint32_t cyclesUsed;
    uint8_t m;

    constexpr uint32_t mask1 = 0xFFFFFF00;
    constexpr uint32_t mask2 = 0xFFFF0000;
    constexpr uint32_t mask3 = 0xFF000000;

    if (u == 0) {
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
    } else {
        // Signed
        // 1    if bits [31:8] of the multiplier operand are all zero.
        // 2    if bits [31:16] of the multiplier operand are all zero.
        // 3    if bits [31:24] of the multiplier operand are all zero.
        // 4    in all other cases.
        if ((rsVal & mask1) == 0 ) {
            m = 1;
        } else if ((rsVal & mask2) == 0) {
            m = 2;
        } else if ((rsVal & mask3) == 0) {
            m = 3;
        } else {
            m = 4;
        }
    }

    if (a == 0) {
        // MULL             1S+(m+1)I
        cyclesUsed = ARM7TDMI::CPU_CYCLES_PER_S_CYCLE + (m + 1) * ARM7TDMI::CPU_CYCLES_PER_I_CYCLE;
    } else {
        // MLAL             1S+(m+2)I
        cyclesUsed = ARM7TDMI::CPU_CYCLES_PER_S_CYCLE + (m + 2) * ARM7TDMI::CPU_CYCLES_PER_I_CYCLE;
    }

    return cyclesUsed;
}
