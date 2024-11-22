#include "block_data_transfer.hpp"
#include <algorithm>
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

const std::string BlockDataTransfer::SFlag2Mnemonic[2] = {"", "^"};
const std::string BlockDataTransfer::WFlag2Mnemonic[2] = {"", "!"};
const std::string BlockDataTransfer::opAddressingMode2Mnemonic[2][2][2] = {{{"stmed", "stmea"}, {"stmfd", "stmfa"}},
                                                                           {{"ldmfa", "ldmfd"}, {"ldmea", "ldmed"}}};

BlockDataTransfer::BlockDataTransfer(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    // TODO ArmOpCode is initialized twice!! Look in all the rest of opcodes too!!
    init(op);
}

BlockDataTransfer::BlockDataTransfer(uint16_t P, uint16_t U, uint16_t S, uint16_t W, uint16_t L,
  uint16_t Rn, uint16_t registerList, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(cpu) {
    init(P, U, S, W, L, Rn, registerList);
}

BlockDataTransfer::BlockDataTransfer(ARM7TDMI &cpu): ArmOpcode::ArmOpcode(cpu){

}

void BlockDataTransfer::init(uint32_t op){
    ArmOpcode::init(op);
    P = Utils::getRegBits(op, P_MASK, P_SHIFT);
    U = Utils::getRegBits(op, U_MASK, U_SHIFT);
    S = Utils::getRegBits(op, S_MASK, S_SHIFT);
    W = Utils::getRegBits(op, W_MASK, W_SHIFT);
    L = Utils::getRegBits(op, L_MASK, L_SHIFT);
    Rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
    registerList = Utils::getRegBits(op, REGISTER_LIST_MASK, REGISTER_LIST_SHIFT);

    for(size_t i = 0; i < 16; i++){
        if(((registerList >> i) & 0x1) == 0x1)
            registerListVec.push_back(i);    
    }
}

void BlockDataTransfer::init(uint16_t P, uint16_t U, uint16_t S, uint16_t W, uint16_t L,
  uint16_t Rn, uint16_t registerList){
    this->P = P;
    this->U = U;
    this->S = S;
    this->W = W;
    this->L = L;
    this->Rn = Rn;
    this->registerList = registerList;

    for(size_t i = 0; i < 16; i++){
        if(((registerList >> i) & 0x1) == 0x1)
            registerListVec.push_back(i);    
    }
}

std::string BlockDataTransfer::getSFlagMnemonic(){
    return SFlag2Mnemonic[S];
}

std::string BlockDataTransfer::getWFlagMnemonic(){
    return WFlag2Mnemonic[W];
}

std::string BlockDataTransfer::getRegisterListMnemonic(){
    std::string regs = "{";
    for(size_t i = 0; i < registerListVec.size(); i++){
        regs += OpCode::getRegMnemonic(registerListVec[i]);
        if(i < registerListVec.size() - 1)
            regs += ",";
    }
    regs += "}";
    return regs;
}

std::string BlockDataTransfer::getOpAddressingModeMnemonic(){
    return opAddressingMode2Mnemonic[L][P][U];
}

std::string BlockDataTransfer::toString(){
    return getOpAddressingModeMnemonic() + getCondFieldMnemonic() + " " + OpCode::getRegMnemonic(Rn) + getWFlagMnemonic() + "," + getRegisterListMnemonic() + getSFlagMnemonic();
}

void BlockDataTransfer::doDecode(){

}

// Optimizible!!
// TODO Flush pipeline if PC is written!!!
void BlockDataTransfer::doExecute(){
    // Empty Rlist: R15 loaded/stored (ARMv4 only), and Rb=Rb+/-40h (ARMv4-v5).
    // TODO Rb=Rb+/-40h (ARMv4-v5).
    bool emptyList = false;
    if(registerListVec.empty()){
        emptyList = true;
        registerListVec.push_back(15);
    }

    bool regListHasBase = false;
    bool baseRegFirst = false;
    auto baseRegIterator = std::find(registerListVec.begin(), registerListVec.end(), Rn);
    if(baseRegIterator != registerListVec.end()){
        regListHasBase = true;
        if(baseRegIterator - registerListVec.begin() == 0)
            baseRegFirst = true;
    }

    std::vector<uint32_t> regVals;
    bool regListHasPc = false;
    if(std::find(registerListVec.begin(), registerListVec.end(), 15) != registerListVec.end()){
        regListHasPc = true;
    }

    if(L == 0){
        uint32_t endAddr = 0;
        uint32_t baseAddr = 0;
        int32_t emptyListOffset;
        // Pre calculate end address for possible writeback
        // STMDB / STMFD
        if(P==1 && U==0){
            emptyListOffset = emptyList ? -0x40 : -registerListVec.size() * 4;
            baseAddr = cpu.getReg(Rn) + emptyListOffset;
            endAddr = baseAddr;
        // STMIB / STMFA
        }else if(P==1 && U==1){
            baseAddr = cpu.getReg(Rn) + 4;
            endAddr = baseAddr + (registerListVec.size() - 1) * 4;
        // STMED / STMDA
        }else if(P==0 && U==0){
            emptyListOffset = emptyList ? -0x3c : 0;
            baseAddr = cpu.getReg(Rn) + emptyListOffset;
            endAddr = baseAddr - registerListVec.size() * 4;
            std::reverse(registerListVec.begin(), registerListVec.end());
        // STMEA / STMIA
        }else if(P==0 && U==1){
            baseAddr = cpu.getReg(Rn);
            endAddr = baseAddr + registerListVec.size() * 4;
        }

        for(const uint32_t regNum : registerListVec){
            uint32_t regVal = cpu.getReg(regNum, S == 1);
            /*
            * Whenever R15 is stored to memory the stored value is the address of the STM
            * instruction plus 12. PC is already 8 bytes ahead due to instruction pipelining
            * in ARM mode.
            */
            
            if(regListHasBase && !baseRegFirst){
                if(regNum == Rn){
                    regVal = endAddr;
                }
            }
            
            if(regNum == 15){
                regVal += cpu.getCPSR().isThumbMode() ? 2 : 4;
            }
                
            regVals.push_back(regVal);
        }
        
        int8_t baseAddrSign = 1;
        if(P==0 && U==0){
            baseAddrSign = -1;
        }

        for(const uint32_t regVal : regVals){
            cpu.getMemManager().store(baseAddr & 0xFFFFFFFC, regVal, 4);
            baseAddr += baseAddrSign * 4; 
        }

        /*
        Writeback with Rb included in Rlist: Store OLD base if Rb is FIRST entry in Rlist, otherwise store NEW base (STM/ARMv4), no writeback (LDM/ARMv4).
        */
        if(W==1 && S != 1 && !emptyList){
            cpu.setReg(Rn, endAddr);
        }

    }else if(L == 1){
        for(size_t i=0; i < registerListVec.size(); i++){
            // If PC value is modified, flush pipeline
            if(registerListVec.at(i) == 15){
                cpu.setMustFlushPipeline(true);
            }   
        }
        // LDMFD / LDMIA
        if(P==0 && U==1){
            uint32_t baseAddr = cpu.getReg(Rn);
            uint32_t endAddr = baseAddr;
            for(size_t i=0; i < registerListVec.size(); i++){
                uint32_t val = cpu.getMemManager().readWord(endAddr & 0xFFFFFFFC);
                cpu.setReg(registerListVec.at(i), val, !regListHasPc && (S == 1));
                endAddr += 4;
            }

            if(W == 1 && !(regListHasPc && (S == 1)) && !emptyList && !regListHasBase){
                cpu.setReg(Rn, endAddr);
            }
        // LDMFA / LDMDA
        }else if(P==0 && U==0){
            uint32_t endAddr = cpu.getReg(Rn) - registerListVec.size() * 4;
            uint32_t baseAddr = endAddr + 4;
            for(size_t i=0; i < registerListVec.size(); i++){
                uint32_t val = cpu.getMemManager().readWord(baseAddr & 0xFFFFFFFC);
                cpu.setReg(registerListVec.at(i), val, !regListHasPc && (S == 1));
                baseAddr += 4;
            }

            if(W == 1 && !(regListHasPc && (S == 1)) && !emptyList && !regListHasBase){
                cpu.setReg(Rn, endAddr);
            }
        // LDMEA / LDMDB
        }else if(P==1 && U==0){
            uint32_t baseAddr = cpu.getReg(Rn) - registerListVec.size() * 4;
            uint32_t endAddr = baseAddr;
            for(size_t i=0; i < registerListVec.size(); i++){
                uint32_t val = cpu.getMemManager().readWord(endAddr & 0xFFFFFFFC);
                cpu.setReg(registerListVec.at(i), val, !regListHasPc && (S == 1));
                endAddr += 4;
            }

            if(W == 1 && !(regListHasPc && (S == 1)) && !emptyList && !regListHasBase){
                cpu.setReg(Rn, baseAddr);
            }
            
        // LDMED / LDMIB 
        }else if(P==1 && U==1){
            uint32_t baseAddr = cpu.getReg(Rn) + registerListVec.size() * 4;
            uint32_t endAddr = baseAddr;

            for(int i=registerListVec.size()-1; i >= 0; i--){
                uint32_t val = cpu.getMemManager().readWord(endAddr & 0xFFFFFFFC);
                cpu.setReg(registerListVec.at(i), val, !regListHasPc && (S == 1));
                endAddr -= 4;
            }

            if(W == 1 && !(regListHasPc && (S == 1)) && !emptyList && !regListHasBase){
                cpu.setReg(Rn, baseAddr);
            }
        }

        /*
        * LDM with R15 in transfer list and S bit set (Mode changes)
        * If the instruction is a LDM then SPSR_<mode> is transferred to CPSR at the same
        * time as R15 is loaded.
        */
        if(regListHasPc && (S == 1)){
            cpu.setCPSR(cpu.getSPSR().getValue());
        }
    }else{
        throw std::runtime_error("ERROR: BlockDataTransfer::doExecute: Invalid L=" + std::to_string(L) + " value");
    }

    if(emptyList){
        if(U == 0){
            cpu.setReg(Rn, cpu.getReg(Rn) - 0x40);
        }else if(U == 1){
            cpu.setReg(Rn, cpu.getReg(Rn) + 0x40);
        }
    }
    
}

uint32_t BlockDataTransfer::cyclesUsed() const {
    //std::cerr << "TODO: BlockDataTransfer::cyclesUsed" << std::endl;
    return 1;
}
