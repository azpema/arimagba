#include "block_data_transfer.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"
#include <algorithm>

const std::string BlockDataTransfer::SFlag2Mnemonic[2] = {"", "^"};
const std::string BlockDataTransfer::WFlag2Mnemonic[2] = {"", "!"};
const std::string BlockDataTransfer::opAddressingMode2Mnemonic[2][2][2] = {{{"stmed", "stmea"}, {"stmfd", "stmfa"}},
                                                                           {{"ldmfa", "ldmfd"}, {"ldmea", "ldmed"}}};

BlockDataTransfer::BlockDataTransfer(uint32_t op, ARM7TDMI& cpu) : ArmOpcode::ArmOpcode(op, cpu) {
    // TODO ArmOpCode is initialized twice!! Look in all the rest of opcodes too!!
    init(op);
}

BlockDataTransfer::BlockDataTransfer(uint16_t _p,
                                     uint16_t _u,
                                     uint16_t _s,
                                     uint16_t _w,
                                     uint16_t _l,
                                     uint16_t _rn,
                                     uint16_t _registerList,
                                     ARM7TDMI& cpu) :
    ArmOpcode::ArmOpcode(cpu) {
    init(_p, _u, _s, _w, _l, _rn, _registerList);
}

BlockDataTransfer::BlockDataTransfer(ARM7TDMI& cpu) : ArmOpcode::ArmOpcode(cpu) {}

void BlockDataTransfer::init(uint32_t op) {
    ArmOpcode::init(op);
    p = Utils::getRegBits(op, P_MASK, P_SHIFT);
    u = Utils::getRegBits(op, U_MASK, U_SHIFT);
    s = Utils::getRegBits(op, S_MASK, S_SHIFT);
    w = Utils::getRegBits(op, W_MASK, W_SHIFT);
    l = Utils::getRegBits(op, L_MASK, L_SHIFT);
    rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
    registerList = Utils::getRegBits(op, REGISTER_LIST_MASK, REGISTER_LIST_SHIFT);

    registerListVec = {};
    registerListVec.reserve(ARM7TDMI::REG_CNT);
    for (size_t i = 0; i < 16; i++) {
        if (((registerList >> i) & 0x1) == 0x1)
            registerListVec.push_back(i);
    }

    if (std::find(registerListVec.begin(), registerListVec.end(), 15) != registerListVec.end()) {
        regListHasPc = true;
    }
}

void BlockDataTransfer::init(
    uint16_t _p, uint16_t _u, uint16_t _s, uint16_t _w, uint16_t _l, uint16_t _rn, uint16_t _registerList) {
    p = _p;
    u = _u;
    s = _s;
    w = _w;
    l = _l;
    rn = _rn;
    registerList = _registerList;

    registerListVec = {};
    for (size_t i = 0; i < 16; i++) {
        if (((registerList >> i) & 0x1) == 0x1)
            registerListVec.push_back(i);
    }

    if (std::find(registerListVec.begin(), registerListVec.end(), 15) != registerListVec.end()) {
        regListHasPc = true;
    }
}

std::string BlockDataTransfer::getSFlagMnemonic() { return SFlag2Mnemonic[s]; }

std::string BlockDataTransfer::getWFlagMnemonic() { return WFlag2Mnemonic[w]; }

std::string BlockDataTransfer::getRegisterListMnemonic() {
    std::string regs = "{";
    for (size_t i = 0; i < registerListVec.size(); i++) {
        regs += OpCode::getRegMnemonic(registerListVec[i]);
        if (i < registerListVec.size() - 1)
            regs += ",";
    }
    regs += "}";
    return regs;
}

std::string BlockDataTransfer::getOpAddressingModeMnemonic() { return opAddressingMode2Mnemonic[l][p][u]; }

std::string BlockDataTransfer::toString() {
    return getOpAddressingModeMnemonic() + getCondFieldMnemonic() + " " + OpCode::getRegMnemonic(rn) +
           getWFlagMnemonic() + "," + getRegisterListMnemonic() + getSFlagMnemonic();
}

void BlockDataTransfer::doDecode() {}

// Optimizible!!
// TODO Flush pipeline if PC is written!!!
void BlockDataTransfer::doExecute() {
    // Empty Rlist: R15 loaded/stored (ARMv4 only), and Rb=Rb+/-40h (ARMv4-v5).
    // TODO Rb=Rb+/-40h (ARMv4-v5).
    bool emptyList = false;
    if (registerListVec.empty()) {
        emptyList = true;
        registerListVec.push_back(15);
    }

    bool regListHasBase = false;
    bool baseRegFirst = false;
    auto baseRegIterator = std::find(registerListVec.begin(), registerListVec.end(), rn);
    if (baseRegIterator != registerListVec.end()) {
        regListHasBase = true;
        if (baseRegIterator - registerListVec.begin() == 0)
            baseRegFirst = true;
    }

    std::vector<uint32_t> regVals;
    regVals.reserve(ARM7TDMI::REG_CNT);

    if (l == 0) {
        uint32_t endAddr = 0;
        uint32_t baseAddr = 0;
        int32_t emptyListOffset;
        // Pre calculate end address for possible writeback
        // STMDB / STMFD
        if (p == 1 && u == 0) {
            emptyListOffset = emptyList ? static_cast<int32_t>(-0x40) : -static_cast<int32_t>(registerListVec.size()) * 4;
            baseAddr = cpu.getReg(rn) + emptyListOffset;
            endAddr = baseAddr;
            // STMIB / STMFA
        } else if (p == 1 && u == 1) {
            baseAddr = cpu.getReg(rn) + 4;
            endAddr = baseAddr + (registerListVec.size() - 1) * 4;
            // STMED / STMDA
        } else if (p == 0 && u == 0) {
            emptyListOffset = emptyList ? -0x3c : 0;
            baseAddr = cpu.getReg(rn) + emptyListOffset;
            endAddr = baseAddr - registerListVec.size() * 4;
            std::reverse(registerListVec.begin(), registerListVec.end());
            // STMEA / STMIA
        } else if (p == 0 && u == 1) {
            baseAddr = cpu.getReg(rn);
            endAddr = baseAddr + registerListVec.size() * 4;
        }

        for (const uint32_t regNum : registerListVec) {
            uint32_t regVal = cpu.getReg(regNum, s == 1);
            /*
             * Whenever R15 is stored to memory the stored value is the address of the STM
             * instruction plus 12. PC is already 8 bytes ahead due to instruction pipelining
             * in ARM mode.
             */

            if (regListHasBase && !baseRegFirst) {
                if (regNum == rn) {
                    regVal = endAddr;
                }
            }

            if (regNum == 15) {
                regVal += cpu.getCPSR().isThumbMode() ? 2 : 4;
            }

            regVals.push_back(regVal);
        }

        int8_t baseAddrSign = 1;
        if (p == 0 && u == 0) {
            baseAddrSign = -1;
        }

        for (const uint32_t regVal : regVals) {
            cpu.getMemManager().store(baseAddr & 0xFFFFFFFC, regVal, 4);
            baseAddr += baseAddrSign * 4;
        }

        /*
        Writeback with Rb included in Rlist: Store OLD base if Rb is FIRST entry in Rlist, otherwise store NEW base
        (STM/ARMv4), no writeback (LDM/ARMv4).
        */
        if (w == 1 && s != 1 && !emptyList) {
            cpu.setReg(rn, endAddr);
        }

    } else if (l == 1) {
        for (size_t i = 0; i < registerListVec.size(); i++) {
            // If PC value is modified, flush pipeline
            if (registerListVec.at(i) == 15) {
                cpu.setMustFlushPipeline(true);
            }
        }
        // LDMFD / LDMIA
        if (p == 0 && u == 1) {
            uint32_t baseAddr = cpu.getReg(rn);
            uint32_t endAddr = baseAddr;
            for (size_t i = 0; i < registerListVec.size(); i++) {
                uint32_t val = cpu.getMemManager().readWord(endAddr & 0xFFFFFFFC);
                cpu.setReg(registerListVec.at(i), val, !regListHasPc && (s == 1));
                endAddr += 4;
            }

            if (w == 1 && !(regListHasPc && (s == 1)) && !emptyList && !regListHasBase) {
                cpu.setReg(rn, endAddr);
            }
            // LDMFA / LDMDA
        } else if (p == 0 && u == 0) {
            uint32_t endAddr = cpu.getReg(rn) - registerListVec.size() * 4;
            uint32_t baseAddr = endAddr + 4;
            for (size_t i = 0; i < registerListVec.size(); i++) {
                uint32_t val = cpu.getMemManager().readWord(baseAddr & 0xFFFFFFFC);
                cpu.setReg(registerListVec.at(i), val, !regListHasPc && (s == 1));
                baseAddr += 4;
            }

            if (w == 1 && !(regListHasPc && (s == 1)) && !emptyList && !regListHasBase) {
                cpu.setReg(rn, endAddr);
            }
            // LDMEA / LDMDB
        } else if (p == 1 && u == 0) {
            uint32_t baseAddr = cpu.getReg(rn) - registerListVec.size() * 4;
            uint32_t endAddr = baseAddr;
            for (size_t i = 0; i < registerListVec.size(); i++) {
                uint32_t val = cpu.getMemManager().readWord(endAddr & 0xFFFFFFFC);
                cpu.setReg(registerListVec.at(i), val, !regListHasPc && (s == 1));
                endAddr += 4;
            }

            if (w == 1 && !(regListHasPc && (s == 1)) && !emptyList && !regListHasBase) {
                cpu.setReg(rn, baseAddr);
            }

            // LDMED / LDMIB
        } else if (p == 1 && u == 1) {
            uint32_t baseAddr = cpu.getReg(rn) + registerListVec.size() * 4;
            uint32_t endAddr = baseAddr;

            for (int i = registerListVec.size() - 1; i >= 0; i--) {
                uint32_t val = cpu.getMemManager().readWord(endAddr & 0xFFFFFFFC);
                cpu.setReg(registerListVec.at(i), val, !regListHasPc && (s == 1));
                endAddr -= 4;
            }

            if (w == 1 && !(regListHasPc && (s == 1)) && !emptyList && !regListHasBase) {
                cpu.setReg(rn, baseAddr);
            }
        }

        /*
         * LDM with R15 in transfer list and S bit set (Mode changes)
         * If the instruction is a LDM then SPSR_<mode> is transferred to CPSR at the same
         * time as R15 is loaded.
         */
        if (regListHasPc && (s == 1)) {
            cpu.setCPSR(cpu.getSPSR().getValue());
        }
    } else {
        throw std::runtime_error("ERROR: BlockDataTransfer::doExecute: Invalid L=" + std::to_string(l) + " value");
    }

    if (emptyList) {
        if (u == 0) {
            cpu.setReg(rn, cpu.getReg(rn) - 0x40);
        } else if (u == 1) {
            cpu.setReg(rn, cpu.getReg(rn) + 0x40);
        }
    }
}

// 
// 
// STM instructions take (n-1)S + 2N incremental cycles to execute, 
uint32_t BlockDataTransfer::cyclesUsed() const {
    // STM
    uint32_t cyclesUsed;
    int n = registerListVec.size();
    if (n == 0)
    {
        throw std::runtime_error("Block data transfer register list empty");
    }

    if (l == 0){
        cyclesUsed = ((n - 1) * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE) + (2 * ARM7TDMI::CPU_CYCLES_PER_N_CYCLE);
    } else {
        if (regListHasPc) {
            // LDM PC takes (n+1)S + 2N + 1I
            cyclesUsed = ((n + 1) * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE) + (2 * ARM7TDMI::CPU_CYCLES_PER_N_CYCLE) + ARM7TDMI::CPU_CYCLES_PER_I_CYCLE;
        } else {
            // Normal LDM instructions take nS + 1N + 1I 
            cyclesUsed = (n * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE) + ARM7TDMI::CPU_CYCLES_PER_N_CYCLE + ARM7TDMI::CPU_CYCLES_PER_I_CYCLE;
        }   
    }

    return cyclesUsed;

}
