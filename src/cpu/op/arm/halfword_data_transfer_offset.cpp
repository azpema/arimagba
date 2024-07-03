#include "halfword_data_transfer_offset.hpp"

HalfwordDataTransferOffset::HalfwordDataTransferOffset(uint32_t op, ARM7TDMI &cpu): HalfwordDataTransfer::HalfwordDataTransfer(op, cpu) {
    offsetHi = Utils::getRegBits(op, OFFSETHI_FLAG_MASK, OFFSETHI_FLAG_SHIFT);
    offsetLo = Utils::getRegBits(op, OFFSETLO_FLAG_MASK, OFFSETLO_FLAG_SHIFT);

    offsetVal = (offsetHi << 4) | offsetLo;
}

uint16_t HalfwordDataTransferOffset::getOffsetVal() {
    return offsetVal;
}

std::string HalfwordDataTransferOffset::toString(){
    std::string base = getOpMnemonic() + getCondFieldMnemonic() + getSFlagMnemonic() + getHFlagMnemonic() +  + " " + \
                        OpCode::getRegMnemonic(rd) + ",";
    std::string address = "[" + OpCode::getRegMnemonic(rn);

    bool showOffset = getOffsetVal() !=0;

    if(p == 1){
        if(showOffset)
            address += "," + getUFlagMnemonic() + Utils::toHexString(getOffsetVal()) + "]" + getWFlagMnemonic();
        else
            address += "]" + getWFlagMnemonic();
    }else if(p == 0){
        address += "]";
        if(showOffset) 
            address += "," + getUFlagMnemonic() + Utils::toHexString(getOffsetVal());
    }else{
        std::cerr << "ERROR: Invalid p flag value HalfwordDataTransferOffset" << std::endl;
    }

    return base + address;
}

// Depends on load or store
uint32_t HalfwordDataTransferOffset::cyclesUsed() const {
    std::cerr << "TODO: HalfwordDataTransferOffset::cyclesUsed" << std::endl;
    return 1;
}

void HalfwordDataTransferOffset::doDecode(){

}

void HalfwordDataTransferOffset::doExecute(){
    uint32_t baseRegVal = cpu.getReg(rn);
    uint32_t loadVal = 0xDEADBEEF;

    // Pre-indexing
    if(p == 1){
        if(u == 0)
            baseRegVal -= offsetVal;
        else if(u == 1)
            baseRegVal += offsetVal;

        if(w == 1 && l == 1){
            cpu.setReg(rn, baseRegVal);
        }
    }

    if(l == 0){
        // Store to memory
        uint32_t storeVal = cpu.getReg(rd);
        if(rd == 15){
            // PC is already 8 bytes ahead, so add 4 to reach 12bytes
            storeVal += 4;
        }
        if(s == 0){
            if(h == 0){
                // SWP instruction
                throw std::runtime_error("Error: Unimplemented HalfwordDataTransferOffset l=0 s=0 h=0");
            }else if( h == 1){
                // Unsigned halfwords
                cpu.getMemManager().store(baseRegVal, storeVal, 2);
            }
        }else if(s == 1){
            if(h == 0){
                // Signed byte
                throw std::runtime_error("Error: Unimplemented HalfwordDataTransferOffset l=0 s=1 h=0");
            }else if( h == 1){
                // Signed halfwords
                throw std::runtime_error("Error: Unimplemented HalfwordDataTransferOffset l=0 s=0 h=1");
            }
        }

    }else if(l == 1){
        // Load from memory
        if(s == 0){
            if(h == 0){
                // SWP instruction
                throw std::runtime_error("Error: Unimplemented HalfwordDataTransferOffset l=1 s=0 h=0");
            }else if( h == 1){
                // Unsigned halfwords
                loadVal = cpu.getMemManager().readHalfWord(baseRegVal);
            }
        }else if(s == 1){
            if(h == 0){
                // Signed byte
                throw std::runtime_error("Error: Unimplemented HalfwordDataTransferOffset l=1 s=1 h=0");
            }else if( h == 1){
                // Signed halfwords
                throw std::runtime_error("Error: Unimplemented HalfwordDataTransferOffset l=1 s=1 h=1");
            }
        }
    }

    // Post Indexing, Writeback
    if(p == 0){
        if(u == 0)
            baseRegVal -= offsetVal;
        else if(u == 1)
            baseRegVal += offsetVal;

        cpu.setReg(rn, baseRegVal);
    }

    if(w == 1 && l == 0){
        cpu.setReg(rn, baseRegVal);
    }

    if(l == 1){
        cpu.setReg(rd, loadVal);
    }
}
