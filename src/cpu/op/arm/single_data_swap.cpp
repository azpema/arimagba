#include "single_data_swap.hpp"

const std::string SingleDataSwap::bFlag2Mnemonic[2] = {"", "b"};

SingleDataSwap::SingleDataSwap(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    b = Utils::getRegBits(op, B_MASK, B_SHIFT);
    rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    rm = Utils::getRegBits(op, RM_MASK, RM_SHIFT);
}   

std::string SingleDataSwap::getBFlagMnemonic(){
    return bFlag2Mnemonic[b];
}

std::string SingleDataSwap::toString(){
    return "swp" + getCondFieldMnemonic() + getBFlagMnemonic() + " " + OpCode::getRegMnemonic(rd) + "," + \
            OpCode::getRegMnemonic(rm) + ",[" + OpCode::getRegMnemonic(rn) + "]";
}

void SingleDataSwap::doDecode(){

}

// TODO: Do not allow interruptions!
void SingleDataSwap::doExecute(){
    if(rd == 15 || rn == 15 || rm == 15)
        throw std::runtime_error("Error: SingleDataSwap PC register was used in Rd, Rn or Rs");

    /*
    
    // Word
            // LDR Force alignment
            loadVal = cpu.getMemManager().readWord(baseRegVal & 0xFFFFFFFC);
            if((baseRegVal & 0x3) != 0){
                loadVal = Utils::rotateRight(loadVal, (baseRegVal & 0x3)*8);
            }
    
    
    */

    uint8_t size = b ? 1 : 4;
    uint32_t origSwapAddr = cpu.getReg(rn);
    uint32_t alignedSwapAddr = origSwapAddr;
    // Force align if word swap
    if(b == 0){
        alignedSwapAddr &= 0xFFFFFFFC;
    }

    uint32_t rmVal = cpu.getReg(rm);

    uint32_t oldVal = cpu.getMemManager().read(alignedSwapAddr, size);
    if(b == 0 && (origSwapAddr & 0x3) != 0){
        oldVal = Utils::rotateRight(oldVal, (origSwapAddr & 0x3)*8);
    }
    cpu.getMemManager().store(alignedSwapAddr, rmVal, size);

    cpu.setReg(rd, oldVal);
}

// SWP              1S+2N+1I
uint32_t SingleDataSwap::cyclesUsed() const {
    return 1 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE + 2 * ARM7TDMI::CPU_CYCLES_PER_N_CYCLE + 1 * ARM7TDMI::CPU_CYCLES_PER_I_CYCLE;
}