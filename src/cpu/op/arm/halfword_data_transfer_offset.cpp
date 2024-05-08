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
    
    // Pre-indexing
    if(p == 1){
        if(u == 0)
            baseRegVal -= offsetVal;
        else if(u == 1)
            baseRegVal += offsetVal;
    }

    if(l == 0){
        // Store to memory
        uint32_t sourceRegVal = cpu.getReg(rd);
        if(s == 0){
            if(h == 0){
                // SWP instruction
            }else if( h == 1){
                // Unsigned halfwords
                cpu.getMemManager().store(baseRegVal, cpu.getReg(rd), 2);
            }
        }else if(s == 1){
            if(h == 0){
                // Signed byte

            }else if( h == 1){
                // Signed halfwords

            }
        }

    }else if(l == 1){
        // Load from memory
        if(s == 0){
            if(h == 0){
                // SWP instruction
            }else if( h == 1){
                // Unsigned halfwords
                uint16_t loadVal = cpu.getMemManager().readHalfWord(baseRegVal);
                cpu.setReg(rd, loadVal);
            }
        }else if(s == 1){
            if(h == 0){
                // Signed byte

            }else if( h == 1){
                // Signed halfwords

            }
        }
    }

    // Post Indexing, Writeback
    if(p == 0 || w == 1){
        if(u == 0)
            baseRegVal -= offsetVal;
        else if(u == 1)
            baseRegVal += offsetVal;

        cpu.setReg(rn, baseRegVal);
    }

}
