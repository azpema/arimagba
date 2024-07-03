#include "single_data_transfer.hpp"

SingleDataTransfer::SingleDataTransfer(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    I = Utils::getRegBits(op, I_MASK, I_SHIFT);
    P = Utils::getRegBits(op, P_MASK, P_SHIFT);
    U = Utils::getRegBits(op, U_MASK, U_SHIFT);
    B = Utils::getRegBits(op, B_MASK, B_SHIFT);
    W = Utils::getRegBits(op, W_MASK, W_SHIFT);
    L = Utils::getRegBits(op, L_MASK, L_SHIFT);
    Rn = Utils::getRegBits(op, RN_MASK, RN_SHIFT);
    Rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    
    uint16_t off = Utils::getRegBits(op, OFFSET_MASK, OFFSET_SHIFT);

    if(I == 0){
        offsetField = new Imm(off);
    }else if(I == 1){
        offsetField = new ShiftRm(off);
    }
}

SingleDataTransfer::SingleDataTransfer(uint8_t i, uint8_t p, uint8_t u, uint8_t b, uint8_t w, uint8_t l, 
    uint8_t rn, uint8_t rd, uint16_t offset, ARM7TDMI &cpu) : ArmOpcode::ArmOpcode(cpu){

    this->I = i;
    this->P = p;
    this->U = u;
    this->B = b;
    this->W = w;
    this->L = l;
    this->Rn = rn;
    this->Rd = rd;


    if(I == 0){
        this->offsetField = new Imm(offset);
    }else if(I == 1){
        this->offsetField = new ShiftRm(offset);
    }

    uint32_t raw = Condition::AL << COND_FIELD_SHIFT;
    Utils::setRegBits(raw, I_MASK, i << I_SHIFT);
    Utils::setRegBits(raw, P_MASK, p << P_SHIFT);
    Utils::setRegBits(raw, U_MASK, u << U_SHIFT);
    Utils::setRegBits(raw, B_MASK, b << B_SHIFT);
    Utils::setRegBits(raw, W_MASK, w << W_SHIFT);
    Utils::setRegBits(raw, L_MASK, l << L_SHIFT);
    Utils::setRegBits(raw, RN_MASK, rn << RN_SHIFT);
    Utils::setRegBits(raw, RD_MASK, rd << RD_SHIFT);
    Utils::setRegBits(raw, OFFSET_MASK, offset << OFFSET_SHIFT);

    setRawVal(raw);
}



SingleDataTransfer::~SingleDataTransfer(){
    delete offsetField;
}

std::string SingleDataTransfer::getUFlagMnemonic(){
    return UFlag2Mnemonic[U];
}

std::string SingleDataTransfer::getBFlagMnemonic(){
    return BFlag2Mnemonic[B];
}

std::string SingleDataTransfer::getWFlagMnemonic(){
    return WFlag2Mnemonic[W];
}

std::string SingleDataTransfer::getLFlagMnemonic(){
    return LFlag2Mnemonic[L];
}

std::string SingleDataTransfer::toString(){
    std::string base = getLFlagMnemonic() + getBFlagMnemonic() + getCondFieldMnemonic() \
            + " " + OpCode::getRegMnemonic(Rd) + ",";
    std::string writeBack = getWFlagMnemonic();
    std::string address = "";

    // Preindexing; add offset before transfer
    if(P == 1){
        // Offset is an immediate value
        if(I == 0){
            Imm* imm = static_cast<Imm*>(offsetField);
            address = "[" + OpCode::getRegMnemonic(Rn) + "," + getUFlagMnemonic() + Utils::toHexString(imm->getMnemonicVal()) + "]" + writeBack;
        // Offset is a register
        }else if(I == 1){
            ShiftRm* shiftRm = static_cast<ShiftRm*>(offsetField);
            address += "[" + OpCode::getRegMnemonic(Rn) + "," + getUFlagMnemonic() + OpCode::getRegMnemonic(shiftRm->getRm()) + "," + \
                       shiftRm->getShiftTypeMnemonic() + " #" + Utils::toHexString(shiftRm->getShiftAmount()) + "]" + writeBack;
        }else{
            std::cerr << "ERROR: SingleDataTransfer P=1" << std::endl;
        }
        
        
    // Postindexing; add offset after transfer
    }else if(P == 0){
        address = "[" + OpCode::getRegMnemonic(Rn) + "],";
        if(I == 0){
            Imm* imm = static_cast<Imm*>(offsetField);
            address += getUFlagMnemonic() + Utils::toHexString(imm->getMnemonicVal());
        }else if(I == 1){
            ShiftRm* shiftRm = static_cast<ShiftRm*>(offsetField);
            address += getUFlagMnemonic() + OpCode::getRegMnemonic(shiftRm->getRm()) + "," + shiftRm->getShiftTypeMnemonic() + " #" + \
                       Utils::toHexString(shiftRm->getShiftAmount());
        }else{
            std::cerr << "ERROR: SingleDataTransfer P=0" << std::endl;
        }
    }

    return base + address;
}

void SingleDataTransfer::doDecode(){

}
/*
TODO:
Mis-aligned STR,STRH,STM,LDM,LDRD,STRD,PUSH,POP (forced align)
The mis-aligned low bit(s) are ignored, the memory access goes to a forcibly aligned (rounded-down) memory address.

For LDRD/STRD, it isn’t clearly defined if the address must be aligned by 8 (on the NDS, align-4 seems to be okay)
 (align-8 may be required on other CPUs with 64bit databus).

Mis-aligned LDR,SWP (rotated read)
 Reads from forcibly aligned address “addr AND (NOT 3)”, and does then rotate the data as “ROR (addr AND 3)*8”.
 That effect is internally used by LDRB and LDRH opcodes (which do then mask-out the unused bits).
*/

void SingleDataTransfer::doExecute(){
    uint32_t baseRegVal = cpu.getReg(Rn);
    uint32_t offsetVal = offsetField->getOperandVal(cpu);

    // Pre-indexing
    if(P == 1){
        if(U == 0)
            baseRegVal -= offsetVal;
        else if(U == 1)
            baseRegVal += offsetVal;
    }
    if(L == 0){
        // Store to memory
        if(B == 0){
            // Word
            // STR Force alignment
            baseRegVal &= 0xFFFFFFFC;
            cpu.getMemManager().store(baseRegVal, cpu.getReg(Rd), 4);
        }else if(B == 1){
            // Byte
            // STH Force alignment
            baseRegVal &= 0xFFFFFFFE;
            cpu.getMemManager().store(baseRegVal, cpu.getReg(Rd) & 0xFF, 1);
        }else{
            throw std::runtime_error("Error: Invalid B value in SingleDataTransfer::doExecute");
        }


    }else if(L == 1){
        // Load from memory
                // Store to memory
        if(B == 0){
            // Word
            // LDR Force alignment
            uint32_t loadVal = cpu.getMemManager().readWord(baseRegVal & 0xFFFFFFFC);
            if((baseRegVal & 0x3) != 0){
                loadVal = Utils::rotateRight(loadVal, (baseRegVal & 0x3)*8);
            }
            cpu.setReg(Rd, loadVal);

        }else if(B == 1){
            uint16_t loadVal = cpu.getMemManager().readByte(baseRegVal & 0xFFFFFFFE);
            if((baseRegVal & 0x1) != 0){
                loadVal = Utils::rotateRight(loadVal, (baseRegVal & 0x1)*8);
            }
            cpu.setReg(Rd, loadVal);
        }else{
            throw std::runtime_error("Error: Invalid B value in SingleDataTransfer::doExecute");
        }
    }else {
        throw std::runtime_error("Error: Invalid L value in SingleDataTransfer::doExecute");
    }

    // Post Indexing, Writeback
    if(P == 0){
        if(U == 0)
            baseRegVal -= offsetVal;
        else if(U == 1)
            baseRegVal += offsetVal;
    }

    if(P == 0 || W == 1){
        cpu.setReg(Rn, baseRegVal);
    }

}

uint32_t SingleDataTransfer::cyclesUsed() const {
    return 1;
}