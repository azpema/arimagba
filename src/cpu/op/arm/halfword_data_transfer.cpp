#include "halfword_data_transfer.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

const std::string HalfwordDataTransfer::uFlag2Mnemonic[2] = {"-", ""};
const std::string HalfwordDataTransfer::wFlag2Mnemonic[2] = {"", "!"};
const std::string HalfwordDataTransfer::sFlag2Mnemonic[2] = {"", "s"};
const std::string HalfwordDataTransfer::hFlag2Mnemonic[2] = {"b", "h"};
const std::string HalfwordDataTransfer::op2Mnemonic[2] = {"str", "ldr"};

HalfwordDataTransfer::HalfwordDataTransfer(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    init(op);
}

HalfwordDataTransfer::HalfwordDataTransfer(uint16_t p, uint16_t u, uint16_t w, uint16_t l, uint16_t rn,
 uint16_t rd, uint16_t s, uint16_t h, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(cpu) {
    init(p, u, w, l, rn, rd, s, h);
}

HalfwordDataTransfer::HalfwordDataTransfer(ARM7TDMI &cpu): ArmOpcode::ArmOpcode(cpu) {}

void HalfwordDataTransfer::init(uint32_t op){
    ArmOpcode::init(op);
    p = Utils::getRegBits(op, P_FLAG_MASK, P_FLAG_SHIFT);
    u = Utils::getRegBits(op, U_FLAG_MASK, U_FLAG_SHIFT);
    w = Utils::getRegBits(op, W_FLAG_MASK, W_FLAG_SHIFT);
    l = Utils::getRegBits(op, L_FLAG_MASK, L_FLAG_SHIFT);
    rn = Utils::getRegBits(op, RN_FLAG_MASK, RN_FLAG_SHIFT);
    rd = Utils::getRegBits(op, RD_FLAG_MASK, RD_FLAG_SHIFT);
    s = Utils::getRegBits(op, S_FLAG_MASK, S_FLAG_SHIFT);
    h = Utils::getRegBits(op, H_FLAG_MASK, H_FLAG_SHIFT);
}

void HalfwordDataTransfer::init(uint16_t p, uint16_t u, uint16_t w, uint16_t l, uint16_t rn, uint16_t rd, uint16_t s, uint16_t h){
    this->p = p;
    this->u = u;
    this->w = w;
    this->l = l;
    this->rn = rn;
    this->rd = rd;
    this->s = s;
    this->h = h;
}

std::string HalfwordDataTransfer::getUFlagMnemonic(){
    return uFlag2Mnemonic[u];
}

std::string HalfwordDataTransfer::getSFlagMnemonic(){
    return sFlag2Mnemonic[s];
}

std::string HalfwordDataTransfer::getHFlagMnemonic(){
    return hFlag2Mnemonic[h];
}

std::string HalfwordDataTransfer::getWFlagMnemonic(){
    return wFlag2Mnemonic[w];
}

std::string HalfwordDataTransfer::getOpMnemonic(){
    return op2Mnemonic[l];
}

void HalfwordDataTransfer::_doExecute(uint32_t offsetVal){
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
                cpu.getMemManager().store(baseRegVal & 0xFFFFFFFE, storeVal, 2);
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
        if(rd == 15){
            cpu.setMustFlushPipeline(true);
        }

        if(s == 0){
            if(h == 0){
                // SWP instruction
                throw std::runtime_error("Error: Unimplemented HalfwordDataTransferOffset l=1 s=0 h=0");
            }else if( h == 1){
                // Unsigned halfwords
                loadVal = cpu.getMemManager().readHalfWord(baseRegVal & 0xFFFFFFFE);
                if((baseRegVal & 0x1) != 0){
                    loadVal = Utils::rotateRight(loadVal, (baseRegVal & 0x1)*8);
                }
            }
        }else if(s == 1){
            if(h == 0){
                /*
                  Signed byte

                  The LDRSB instruction loads the selected Byte into bits 7 to 0 of the destination
                  register and bits 31 to 8 of the destination register are set to the value of bit 7, the sign
                  bit
                */ 
                loadVal = cpu.getMemManager().readByte(baseRegVal);
                uint8_t signBit = Utils::getRegSingleBit(loadVal, 7);

                Utils::setRegBits(loadVal, 0xFFFFFF00, signBit ? 0xFFFFFF00 : 0);
                
            }else if( h == 1){
                /*
                  Signed halfwords

                  The LDRSH instruction loads the selected Half-word into bits 15 to 0 of the destination
                  register and bits 31 to 16 of the destination register are set to the value of bit 15, the
                  sign bit
                */
               // Misaligned
               if((baseRegVal & 0x1) != 0){
                    loadVal = cpu.getMemManager().readByte(baseRegVal);
                    uint8_t signBit = Utils::getRegSingleBit(loadVal, 7);

                    Utils::setRegBits(loadVal, 0xFFFFFF00, signBit ? 0xFFFFFF00 : 0);
                }else{
                    loadVal = cpu.getMemManager().readHalfWord(baseRegVal);
                    uint8_t signBit = Utils::getRegSingleBit(loadVal, 15);

                    Utils::setRegBits(loadVal, 0xFFFF0000, signBit ? 0xFFFF0000 : 0);
                }
                
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