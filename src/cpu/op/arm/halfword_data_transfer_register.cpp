#include "halfword_data_transfer_register.hpp"


HalfwordDataTransferRegister::HalfwordDataTransferRegister(uint32_t op, ARM7TDMI &cpu): HalfwordDataTransfer::HalfwordDataTransfer(op, cpu) {
    rm = Utils::getRegBits(op, RM_FLAG_MASK, RM_FLAG_SHIFT);
}   

std::string HalfwordDataTransferRegister::toString(){
    std::string base = getOpMnemonic() + getCondFieldMnemonic() + getSFlagMnemonic() + getHFlagMnemonic() +  + " " + \
                        OpCode::getRegMnemonic(rd) + ",";
    std::string address = "[" + OpCode::getRegMnemonic(rn);

    if(p == 1){
        address += "," + getUFlagMnemonic() + OpCode::getRegMnemonic(rm) + "]" + getWFlagMnemonic();
    }else if(p == 0){
        address += "]," + getUFlagMnemonic() + OpCode::getRegMnemonic(rm);
    }else{
        std::cerr << "ERROR: Invalid p flag value HalfwordDataTransferRegister" << std::endl;
    }

    return base + address;
}

// Depends on load or store
uint32_t HalfwordDataTransferRegister::cyclesUsed() const {
    std::cerr << "TODO: HalfwordDataTransferRegister::cyclesUsed" << std::endl;
    return 1;
}

void HalfwordDataTransferRegister::doDecode(){

}

void HalfwordDataTransferRegister::doExecute(){
    uint32_t baseRegVal = cpu.getReg(rn);
    uint32_t sourceRegVal = cpu.getReg(rd);
    uint32_t offsetRegVal = cpu.getReg(rm);
    uint32_t loadVal = 0xDEADBEEF;

    // Pre-indexing
    if(p == 1){
        if(u == 0)
            baseRegVal -= offsetRegVal;
        else if(u == 1)
            baseRegVal += offsetRegVal;

        if(w == 1 && l == 1){
            cpu.setReg(rn, baseRegVal);
        }
    }

    if(l == 0){
        // Store to memory
        if(s == 0){
            uint32_t storeVal = cpu.getReg(rd);
            if(rd == 15){
                // PC is already 8 bytes ahead, so add 4 to reach 12bytes
                storeVal += 4;
            }
                
            if(h == 0){
                // SWP instruction
                throw std::runtime_error("Error: Unimplemented HalfwordDataTransferRegister l=0 s=0 h=0");
            }else if( h == 1){
                // Unsigned halfwords
                cpu.getMemManager().store(baseRegVal, storeVal, 2);
            }
        }else if(s == 1){
            if(h == 0){
                // Signed byte
                throw std::runtime_error("Error: Unimplemented HalfwordDataTransferRegister l=0 s=1 h=0");
            }else if( h == 1){
                // Signed halfwords
                throw std::runtime_error("Error: Unimplemented HalfwordDataTransferRegister l=0 s=1 h=1");
            }
        }

    }else if(l == 1){
        // Load from memory
        if(s == 0){
            if(h == 0){
                // SWP instruction
                throw std::runtime_error("Error: Unimplemented HalfwordDataTransferRegister l=1 s=0 h=0");
            }else if( h == 1){
                // Unsigned halfwords
                loadVal = cpu.getMemManager().readHalfWord(baseRegVal);
            }
        }else if(s == 1){
            if(h == 0){
                // Signed byte
                throw std::runtime_error("Error: Unimplemented HalfwordDataTransferRegister l=1 s=1 h=0");
            }else if( h == 1){
                // Signed halfwords
                throw std::runtime_error("Error: Unimplemented HalfwordDataTransferRegister l=1 s=1 h=1");
            }
        }
    }

    // Post Indexing, Writeback
    if(p == 0){
        if(u == 0)
            baseRegVal -= offsetRegVal;
        else if(u == 1)
            baseRegVal += offsetRegVal;

        cpu.setReg(rn, baseRegVal);
    }

    if(w == 1 && l == 0){
        cpu.setReg(rn, baseRegVal);
    }

    if(l == 1){
        cpu.setReg(rd, loadVal);
    }
}
