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
    uint32_t rsVal = cpu.getReg(rs);
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

// MULL             1S+(m+1)I
// MLAL             1S+(m+2)I
uint32_t MultiplyAccumulateLong::cyclesUsed() const {
    // std::cerr << "TODO: MultiplyAccumulateLong::cyclesUsed" << '\n';
    return 1 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE + (0 + 1) * ARM7TDMI::CPU_CYCLES_PER_I_CYCLE;
}
