#include "io_registers.hpp"
#include "dma.hpp"

IOregisters::IOregisters() : mustHaltCpu(false) {
    std::cerr << "TODO Set proper initial values at IOregisters" << std::endl;
    for(size_t i=0; i<IOREGISTERS_SIZE; i++){
        mem[i] = 0x00;
    }

    DMAxSAD[0] = reinterpret_cast<uint32_t*>(mem.get() + (REG_ADDR::DMAxSAD[0] - MemoryManager::IO_REGISTERS_OFFSET_START));
    DMAxSAD[1] = reinterpret_cast<uint32_t*>(mem.get() + (REG_ADDR::DMAxSAD[1] - MemoryManager::IO_REGISTERS_OFFSET_START));
    DMAxSAD[2] = reinterpret_cast<uint32_t*>(mem.get() + (REG_ADDR::DMAxSAD[2] - MemoryManager::IO_REGISTERS_OFFSET_START));
    DMAxSAD[3] = reinterpret_cast<uint32_t*>(mem.get() + (REG_ADDR::DMAxSAD[3] - MemoryManager::IO_REGISTERS_OFFSET_START));

    DMAxDAD[0] = reinterpret_cast<uint32_t*>(mem.get() + (REG_ADDR::DMAxDAD[0] - MemoryManager::IO_REGISTERS_OFFSET_START));
    DMAxDAD[1] = reinterpret_cast<uint32_t*>(mem.get() + (REG_ADDR::DMAxDAD[1] - MemoryManager::IO_REGISTERS_OFFSET_START));
    DMAxDAD[2] = reinterpret_cast<uint32_t*>(mem.get() + (REG_ADDR::DMAxDAD[2] - MemoryManager::IO_REGISTERS_OFFSET_START));
    DMAxDAD[3] = reinterpret_cast<uint32_t*>(mem.get() + (REG_ADDR::DMAxDAD[3] - MemoryManager::IO_REGISTERS_OFFSET_START));                

    DMAxCNT_L[0] = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::DMAxCNT_L[0] - MemoryManager::IO_REGISTERS_OFFSET_START));
    DMAxCNT_L[1] = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::DMAxCNT_L[1] - MemoryManager::IO_REGISTERS_OFFSET_START));
    DMAxCNT_L[2] = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::DMAxCNT_L[2] - MemoryManager::IO_REGISTERS_OFFSET_START));
    DMAxCNT_L[3] = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::DMAxCNT_L[3] - MemoryManager::IO_REGISTERS_OFFSET_START));

    DMAxCNT_H[0] = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::DMAxCNT_H[0] - MemoryManager::IO_REGISTERS_OFFSET_START));
    DMAxCNT_H[1] = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::DMAxCNT_H[0] - MemoryManager::IO_REGISTERS_OFFSET_START));
    DMAxCNT_H[2] = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::DMAxCNT_H[0] - MemoryManager::IO_REGISTERS_OFFSET_START));
    DMAxCNT_H[3] = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::DMAxCNT_H[0] - MemoryManager::IO_REGISTERS_OFFSET_START));

    BGxCNT[0] = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::BGxCNT[0] - MemoryManager::IO_REGISTERS_OFFSET_START));
    BGxCNT[1] = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::BGxCNT[1] - MemoryManager::IO_REGISTERS_OFFSET_START));
    BGxCNT[2] = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::BGxCNT[2] - MemoryManager::IO_REGISTERS_OFFSET_START));
    BGxCNT[3] = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::BGxCNT[3] - MemoryManager::IO_REGISTERS_OFFSET_START));

    DISPSTAT = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::DISPSTAT - MemoryManager::IO_REGISTERS_OFFSET_START));

    IE = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::IE - MemoryManager::IO_REGISTERS_OFFSET_START));
    IF = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::IF - MemoryManager::IO_REGISTERS_OFFSET_START));
    IME = reinterpret_cast<uint16_t*>(mem.get() + (REG_ADDR::IME - MemoryManager::IO_REGISTERS_OFFSET_START));
}
/*
    4000004h - DISPSTAT - General LCD Status (Read/Write)
    Bit   Expl.
    0     V-Blank flag   (Read only) (1=VBlank) (set in line 160..226; not 227)
    1     H-Blank flag   (Read only) (1=HBlank) (toggled in all lines, 0..227)
    2     V-Counter flag (Read only) (1=Match)  (set in selected line)     (R)
    3     V-Blank IRQ Enable         (1=Enable)                          (R/W)
    4     H-Blank IRQ Enable         (1=Enable)                          (R/W)
    5     V-Counter IRQ Enable       (1=Enable)                          (R/W)
    6     Not used (0) / DSi: LCD Initialization Ready (0=Busy, 1=Ready)   (R)
    7     Not used (0) / NDS: MSB of V-Vcount Setting (LYC.Bit8) (0..262)(R/W)
    8-15  V-Count Setting (LYC)      (0..227)                            (R/W)

    Bits 0-2 are read-only
*/
void IOregisters::writeDISPSTAT(const uint16_t writtenVal){
    uint16_t prevVal = read(REG_ADDR::DISPSTAT - MemoryManager::IO_REGISTERS_OFFSET_START, REG_SIZE::DISPSTAT);
    uint16_t newVal = (prevVal & 0b0000000000000111) | (writtenVal & 0b1111111111111000);
    store(REG_ADDR::DISPSTAT - MemoryManager::IO_REGISTERS_OFFSET_START, newVal, REG_SIZE::DISPSTAT);
}

/*
    4000008h - BGxCNT - BG3 Control (R/W) (BG Modes 0,2 only)
    Bit   Expl.
    0-1   BG Priority           (0-3, 0=Highest)
    2-3   Character Base Block  (0-3, in units of 16 KBytes) (=BG Tile Data)
    4-5   Not used (must be zero) (except in NDS mode: MSBs of char base)
    6     Mosaic                (0=Disable, 1=Enable)
    7     Colors/Palettes       (0=16/16, 1=256/1)
    8-12  Screen Base Block     (0-31, in units of 2 KBytes) (=BG Map Data)
    13    BG0/BG1: Not used (except in NDS mode: Ext Palette Slot for BG0/BG1)
    13    BG2/BG3: Display Area Overflow (0=Transparent, 1=Wraparound)
    14-15 Screen Size (0-3)

    Bit 13 IS read-only for BG0CNT and BG1CNT
*/
void IOregisters::writeBGxCNT(const int bg, const uint16_t writtenVal){
    uint16_t newVal;
    if(bg == 0 || bg == 1){
        newVal = writtenVal & 0b1101111111111111;
    }else{
        newVal = writtenVal;
    }
    
    store(REG_ADDR::BGxCNT[bg] - MemoryManager::IO_REGISTERS_OFFSET_START, newVal, REG_SIZE::BGxCNT);
}

void IOregisters::writeDMAxCNT_L(const uint8_t dma, const uint16_t writtenVal){
    uint16_t newVal = writtenVal;    
    if(writtenVal == 0xFFFF){
        newVal = 0;
    }
    store(REG_ADDR::DMAxCNT_L[dma] - MemoryManager::IO_REGISTERS_OFFSET_START, newVal, REG_SIZE::DMAxCNT_L);
}

void IOregisters::writeDMAxCNT_H(const uint8_t dma, const uint16_t writtenVal){
    uint16_t newVal;
    if(dma < 3){
		newVal = writtenVal & 0xF7E0;
	}else{
		newVal = writtenVal & 0xFFE0;
	}

    store(REG_ADDR::DMAxCNT_H[dma] - MemoryManager::IO_REGISTERS_OFFSET_START, newVal, REG_SIZE::DMAxCNT_H);
}

// DMA 0 has a 27 bit SAD, but 1 2 3 has a 28 bit SAD. 
void IOregisters::writeDMAxSAD(const uint8_t dma, const uint32_t writtenVal){
    uint32_t newVal;
    // Value 0 is irrelevant
    if(DMA<0>::isSourceAddrValid(dma, writtenVal)){
        if(dma == 0){
            newVal = writtenVal & 0x07FFFFFF;
        }else{
            newVal = writtenVal & 0x0FFFFFFF;
        }
        newVal = writtenVal & 0x0FFFFFFE;
    }else{
        std::cout << "Invalid DMA " << std::to_string(dma) << " source address " << Utils::toHexString(writtenVal) << "\n";
        newVal = 0;
    }

    store(REG_ADDR::DMAxSAD[dma] - MemoryManager::IO_REGISTERS_OFFSET_START, newVal, REG_SIZE::DMAxSAD);
}

// DMA 0 1 2 has a 27 bit DAD but DMA 3 has a 28 bit DAD
void IOregisters::writeDMAxDAD(const uint8_t dma, const uint32_t writtenVal){
    uint32_t newVal;
    // Value 0 is irrelevant
    if(DMA<0>::isDestAddrValid(dma, writtenVal)){
        if(dma == 3){
            newVal = writtenVal & 0x0FFFFFFF;
        }else{
            newVal = writtenVal & 0x07FFFFFF;
        }
        newVal = writtenVal & 0x0FFFFFFE;
    }else{
        std::cout << "Invalid DMA " << std::to_string(dma) << " dest address " << Utils::toHexString(writtenVal) << "\n";
        newVal = 0;
    }

    store(REG_ADDR::DMAxDAD[dma] - MemoryManager::IO_REGISTERS_OFFSET_START, newVal, REG_SIZE::DMAxDAD);
}

/*
    4000208h - IME - Interrupt Master Enable Register (R/W)
    Bit   Expl.
    0     Disable all interrupts         (0=Disable All, 1=See IE register)
    1-31  Not used
*/
void IOregisters::writeIME(const uint16_t writtenVal){
    uint16_t newVal = writtenVal & 0b0000000000000001;
    store(REG_ADDR::IME - MemoryManager::IO_REGISTERS_OFFSET_START, newVal, REG_SIZE::IME);
}

void IOregisters::writeIF(const uint16_t writtenVal){
    uint16_t prevVal = read(REG_ADDR::IF - MemoryManager::IO_REGISTERS_OFFSET_START, REG_SIZE::IF);
    uint16_t newVal = (~writtenVal) & prevVal;
    store(REG_ADDR::IF - MemoryManager::IO_REGISTERS_OFFSET_START, newVal, REG_SIZE::IF);
}

void IOregisters::writeIE(const uint16_t writtenVal){
    uint16_t newVal = writtenVal & 0b0000000011111111;
    store(REG_ADDR::IE - MemoryManager::IO_REGISTERS_OFFSET_START, newVal, REG_SIZE::IE);
}

void IOregisters::writeHALTCNT(const uint16_t writtenVal){
    // TODO
    //setMustHaltCpu();
}

void IOregisters::store8(uint32_t addr, uint32_t val){
    // Check 2 byte registers and 4 byte register's address and address + 2
    switch(0x04000000 + addr){
        case REG_ADDR::DMAxSAD[0]:
            writeDMAxSAD(0, val);
            break;
        case REG_ADDR::DMAxSAD[1]:
            writeDMAxSAD(1, val);
            break;
        case REG_ADDR::DMAxSAD[2]:
            writeDMAxSAD(2, val);
            break;
        case REG_ADDR::DMAxSAD[3]:
            writeDMAxSAD(3, val);
            break;
        case REG_ADDR::DMAxDAD[0]:
            writeDMAxDAD(0, val);
            break;
        case REG_ADDR::DMAxDAD[1]:
            writeDMAxDAD(1, val);
            break;
        case REG_ADDR::DMAxDAD[2]:
            writeDMAxDAD(2, val);
            break;
        case REG_ADDR::DMAxDAD[3]:
            writeDMAxDAD(3, val);
            break;

        case REG_ADDR::DMAxSAD[0] + 1:
        {
            auto prevVal = *DMAxSAD[0];
            Utils::setRegBits(prevVal, 0x0000FF00, val << 8);
            writeDMAxSAD(0, prevVal);
            break;
        }
        case REG_ADDR::DMAxSAD[1] + 1:
        {
            auto prevVal = *DMAxSAD[1];
            Utils::setRegBits(prevVal, 0x0000FF00, val << 8);
            writeDMAxSAD(1, prevVal);
            break;
        }
        case REG_ADDR::DMAxSAD[2] + 1:
        {
            auto prevVal = *DMAxSAD[2];
            Utils::setRegBits(prevVal, 0x0000FF00, val << 8);
            writeDMAxSAD(2, prevVal);
            break;
        }
        case REG_ADDR::DMAxSAD[3] + 1:
        {
            auto prevVal = *DMAxSAD[3];
            Utils::setRegBits(prevVal, 0x0000FF00, val << 8);
            writeDMAxSAD(3, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[0] + 1:
        {
            auto prevVal = *DMAxDAD[0];
            Utils::setRegBits(prevVal, 0x0000FF00, val << 8);
            writeDMAxDAD(0, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[1] + 1:
        {
            auto prevVal = *DMAxDAD[1];
            Utils::setRegBits(prevVal, 0x0000FF00, val << 8);
            writeDMAxDAD(1, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[2] + 1:
        {
            auto prevVal = *DMAxDAD[2];
            Utils::setRegBits(prevVal, 0x0000FF00, val << 8);
            writeDMAxDAD(2, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[3] + 1:
        {
            auto prevVal = *DMAxDAD[3];
            Utils::setRegBits(prevVal, 0x0000FF00, val << 8);
            writeDMAxDAD(3, prevVal);
            break;
        }

        case REG_ADDR::DMAxSAD[0] + 2:
        {
            auto prevVal = *DMAxSAD[0];
            Utils::setRegBits(prevVal, 0x00FF0000, val << 16);
            writeDMAxSAD(0, prevVal);
            break;
        }
        case REG_ADDR::DMAxSAD[1] + 2:
        {
            auto prevVal = *DMAxSAD[1];
            Utils::setRegBits(prevVal, 0x00FF0000, val << 16);
            writeDMAxSAD(1, prevVal);
            break;
        }
        case REG_ADDR::DMAxSAD[2] + 2:
        {
            auto prevVal = *DMAxSAD[2];
            Utils::setRegBits(prevVal, 0x00FF0000, val << 16);
            writeDMAxSAD(2, prevVal);
            break;
        }
        case REG_ADDR::DMAxSAD[3] + 2:
        {
            auto prevVal = *DMAxSAD[3];
            Utils::setRegBits(prevVal, 0x00FF0000, val << 16);
            writeDMAxSAD(3, prevVal);
            break;
        }

        case REG_ADDR::DMAxDAD[0] + 2:
        {
            auto prevVal = *DMAxDAD[0];
            Utils::setRegBits(prevVal, 0x00FF0000, val << 16);
            writeDMAxDAD(0, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[1] + 2:
        {
            auto prevVal = *DMAxDAD[1];
            Utils::setRegBits(prevVal, 0x00FF0000, val << 16);
            writeDMAxDAD(1, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[2] + 2:
        {
            auto prevVal = *DMAxDAD[2];
            Utils::setRegBits(prevVal, 0x00FF0000, val << 16);
            writeDMAxDAD(2, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[3] + 2:
        {
            auto prevVal = *DMAxDAD[3];
            Utils::setRegBits(prevVal, 0x00FF0000, val << 16);
            writeDMAxDAD(3, prevVal);
            break;
        }



        case REG_ADDR::DMAxSAD[0] + 3:
        {
            auto prevVal = *DMAxSAD[0];
            Utils::setRegBits(prevVal, 0xFF000000, val << 24);
            writeDMAxSAD(0, prevVal);
            break;
        }
        case REG_ADDR::DMAxSAD[1] + 3:
        {
            auto prevVal = *DMAxSAD[1];
            Utils::setRegBits(prevVal, 0xFF000000, val << 24);
            writeDMAxSAD(1, prevVal);
            break;
        }
        case REG_ADDR::DMAxSAD[2] + 3:
        {
            auto prevVal = *DMAxSAD[2];
            Utils::setRegBits(prevVal, 0xFF000000, val << 24);
            writeDMAxSAD(2, prevVal);
            break;
        }
        case REG_ADDR::DMAxSAD[3] + 3:
        {
            auto prevVal = *DMAxSAD[3];
            Utils::setRegBits(prevVal, 0xFF000000, val << 24);
            writeDMAxSAD(3, prevVal);
            break;
        }

        case REG_ADDR::DMAxDAD[0] + 3:
        {
            auto prevVal = *DMAxDAD[0];
            Utils::setRegBits(prevVal, 0xFF000000, val << 24);
            writeDMAxDAD(0, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[1] + 3:
        {
            auto prevVal = *DMAxDAD[1];
            Utils::setRegBits(prevVal, 0xFF000000, val << 24);
            writeDMAxDAD(1, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[2] + 3:
        {
            auto prevVal = *DMAxDAD[2];
            Utils::setRegBits(prevVal, 0xFF000000, val << 24);
            writeDMAxDAD(2, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[3] + 3:
        {
            auto prevVal = *DMAxDAD[3];
            Utils::setRegBits(prevVal, 0xFF000000, val << 24);
            writeDMAxDAD(3, prevVal);
            break;
        }

        // 2 byte registers
        case REG_ADDR::DISPSTAT:
            writeDISPSTAT(val);
            break;

        case REG_ADDR::DISPSTAT + 1:
        {
            uint32_t prevVal = *DISPSTAT;
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeDISPSTAT(prevVal);
            break;
        }
        case REG_ADDR::VCOUNT:
        case REG_ADDR::VCOUNT + 1:
            // Read-only register
            break;

        case REG_ADDR::BGxCNT[0]:
            writeBGxCNT(0, val);
            break;
        case REG_ADDR::BGxCNT[0] + 1:
        {
            uint32_t prevVal = *BGxCNT[0];
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeBGxCNT(0, prevVal);
            break;
        }
        case REG_ADDR::BGxCNT[1]:
            writeBGxCNT(1, val);
            break;
        case REG_ADDR::BGxCNT[1] + 1:
        {
            uint32_t prevVal = *BGxCNT[1];
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeBGxCNT(1, prevVal);
            break;
        }
        case REG_ADDR::BGxCNT[2]:
            writeBGxCNT(2, val);
            break;
        case REG_ADDR::BGxCNT[2] + 1:
        {
            uint32_t prevVal = *BGxCNT[2];
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeBGxCNT(2, prevVal);
            break;
        }
        case REG_ADDR::BGxCNT[3]:
            writeBGxCNT(3, val);
            break;
        case REG_ADDR::BGxCNT[3] + 1:
        {
            uint32_t prevVal = *BGxCNT[3];
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeBGxCNT(3, prevVal);
            break;
        }
        case REG_ADDR::DMAxCNT_L[0]:
            writeDMAxCNT_L(0, val);
            break;
        case REG_ADDR::DMAxCNT_L[0] + 1:
        {
            uint32_t prevVal = *DMAxCNT_L[0];
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeDMAxCNT_L(0, prevVal);
            break;
        }
        case REG_ADDR::DMAxCNT_L[1]:
            writeDMAxCNT_L(1, val);
            break;
        case REG_ADDR::DMAxCNT_L[1] + 1:
        {
            uint32_t prevVal = *DMAxCNT_L[1];
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeDMAxCNT_L(1, prevVal);
            break;
        }
        case REG_ADDR::DMAxCNT_L[2]:
            writeDMAxCNT_L(2, val);
            break;
        case REG_ADDR::DMAxCNT_L[2] + 1:
        {
            uint32_t prevVal = *DMAxCNT_L[2];
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeDMAxCNT_L(2, prevVal);
            break;
        }
        case REG_ADDR::DMAxCNT_L[3]:
            writeDMAxCNT_L(3, val);
            break;
        case REG_ADDR::DMAxCNT_L[3] + 1:
        {
            uint32_t prevVal = *DMAxCNT_L[3];
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeDMAxCNT_L(3, prevVal);
            break;
        }
        case REG_ADDR::DMAxCNT_H[0]:
            writeDMAxCNT_H(0, val);
            break;
        case REG_ADDR::DMAxCNT_H[0] + 1:
        {
            uint32_t prevVal = *DMAxCNT_H[0];
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeDMAxCNT_H(0, prevVal);
            break;
        }
        case REG_ADDR::DMAxCNT_H[1]:
            writeDMAxCNT_H(1, val);
            break;
        case REG_ADDR::DMAxCNT_H[1] + 1:
        {
            uint32_t prevVal = *DMAxCNT_H[1];
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeDMAxCNT_H(1, prevVal);
            break;
        }
        case REG_ADDR::DMAxCNT_H[2]:
            writeDMAxCNT_H(2, val);
            break;
        case REG_ADDR::DMAxCNT_H[2] + 1:
        {
            uint32_t prevVal = *DMAxCNT_H[2];
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeDMAxCNT_H(2, prevVal);
            break;
        }
        case REG_ADDR::DMAxCNT_H[3]:
            writeDMAxCNT_H(3, val);
            break;
        case REG_ADDR::DMAxCNT_H[3] + 1:
        {
            uint32_t prevVal = *DMAxCNT_H[3];
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeDMAxCNT_H(3, prevVal);
            break;
        }
        case REG_ADDR::IE:
            writeIE(val);
            break;
        case REG_ADDR::IE + 1:
        {
            uint32_t prevVal = *IE;
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeIE(prevVal);
            break;
        }
        case REG_ADDR::IF:
            writeIF(val);
            break;

        case REG_ADDR::IF + 1:
        {
            uint32_t prevVal = *IF;
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeIF(prevVal);
            break;
        }
        case REG_ADDR::IME:
            writeIME(val);
            break;
        case REG_ADDR::IME + 1:
        {
            uint32_t prevVal = *IME;
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeIME(prevVal);
            break;
        }
        // This should correspond to HALTCNT and POSTFLAG
        /*case REG_ADDR::HALTCNT:
            writeHALTCNT(val);
            break;
        case REG_ADDR::HALTCNT + 1:
        {
            auto prevVal = *HALTCNT;
            Utils::setRegBits(prevVal, 0xFF00, val << 8);
            writeHALTCNT(prevVal);
            break;
        }*/
        case REG_ADDR::KEYINPUT:
        case REG_ADDR::KEYINPUT + 1:
            // Read-only register
            break;
        default:
            store(addr, val, 1);
            break;
    }
}

void IOregisters::store16(uint32_t addr, uint32_t val){
    // Check 2 byte registers and 4 byte register's address and address + 2
    switch(0x04000000 + addr){
        case REG_ADDR::DMAxSAD[0]:
            writeDMAxSAD(0, val);
            break;
        case REG_ADDR::DMAxSAD[1]:
            writeDMAxSAD(1, val);
            break;
        case REG_ADDR::DMAxSAD[2]:
            writeDMAxSAD(2, val);
            break;
        case REG_ADDR::DMAxSAD[3]:
            writeDMAxSAD(3, val);
            break;
        case REG_ADDR::DMAxDAD[0]:
            writeDMAxDAD(0, val);
            break;
        case REG_ADDR::DMAxDAD[1]:
            writeDMAxDAD(1, val);
            break;
        case REG_ADDR::DMAxDAD[2]:
            writeDMAxDAD(2, val);
            break;
        case REG_ADDR::DMAxDAD[3]:
            writeDMAxDAD(3, val);
            break;

        case REG_ADDR::DMAxSAD[0] + 2:
        {
            auto prevVal = *DMAxSAD[0];
            Utils::setRegBits(prevVal, 0xFFFF0000, val << 16);
            writeDMAxSAD(0, prevVal);
            break;
        }
        case REG_ADDR::DMAxSAD[1] + 2:
        {
            auto prevVal = *DMAxSAD[1];
            Utils::setRegBits(prevVal, 0xFFFF0000, val << 16);
            writeDMAxSAD(1, prevVal);
            break;
        }
        case REG_ADDR::DMAxSAD[2] + 2:
        {
            auto prevVal = *DMAxSAD[2];
            Utils::setRegBits(prevVal, 0xFFFF0000, val << 16);
            writeDMAxSAD(2, prevVal);
            break;
        }
        case REG_ADDR::DMAxSAD[3] + 2:
        {
            auto prevVal = *DMAxSAD[3];
            Utils::setRegBits(prevVal, 0xFFFF0000, val << 16);
            writeDMAxSAD(3, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[0] + 2:
        {
            auto prevVal = *DMAxDAD[0];
            Utils::setRegBits(prevVal, 0xFFFF0000, val << 16);
            writeDMAxDAD(0, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[1] + 2:
        {
            auto prevVal = *DMAxDAD[1];
            Utils::setRegBits(prevVal, 0xFFFF0000, val << 16);
            writeDMAxDAD(1, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[2] + 2:
        {
            auto prevVal = *DMAxDAD[2];
            Utils::setRegBits(prevVal, 0xFFFF0000, val << 16);
            writeDMAxDAD(2, prevVal);
            break;
        }
        case REG_ADDR::DMAxDAD[3] + 2:
        {
            auto prevVal = *DMAxDAD[3];
            Utils::setRegBits(prevVal, 0xFFFF0000, val << 16);
            writeDMAxDAD(3, prevVal);
            break;
        }
        // 2 byte registers
        case REG_ADDR::DISPSTAT:
            writeDISPSTAT(val);
            break;

        case REG_ADDR::VCOUNT:
            // Read-only register
            break;

        case REG_ADDR::BGxCNT[0]:
            writeBGxCNT(0, val);
            break;
        case REG_ADDR::BGxCNT[1]:
            writeBGxCNT(1, val);
            break;
        case REG_ADDR::BGxCNT[2]:
            writeBGxCNT(2, val);
            break;
        case REG_ADDR::BGxCNT[3]:
            writeBGxCNT(3, val);
            break;
        case REG_ADDR::DMAxCNT_L[0]:
            writeDMAxCNT_L(0, val);
            break;
        case REG_ADDR::DMAxCNT_L[1]:
            writeDMAxCNT_L(1, val);
            break;
        case REG_ADDR::DMAxCNT_L[2]:
            writeDMAxCNT_L(2, val);
            break;
        case REG_ADDR::DMAxCNT_L[3]:
            writeDMAxCNT_L(3, val);
            break;
        case REG_ADDR::DMAxCNT_H[0]:
            writeDMAxCNT_H(0, val);
            break;
        case REG_ADDR::DMAxCNT_H[1]:
            writeDMAxCNT_H(1, val);
            break;
        case REG_ADDR::DMAxCNT_H[2]:
            writeDMAxCNT_H(2, val);
            break;
        case REG_ADDR::DMAxCNT_H[3]:
            writeDMAxCNT_H(3, val);
            break;
        case REG_ADDR::IE:
            writeIE(val);
            break;
        case REG_ADDR::IF:
            writeIF(val);
            break;
        case REG_ADDR::IME:
            writeIME(val);
            break;
        // This should correspond to HALTCNT and POSTFLAG
        case REG_ADDR::HALTCNT:
            writeHALTCNT(val);
            break;
        case REG_ADDR::KEYINPUT:
            // Read-only register
            break;
        default:
            store(addr, val, 2);
            break;
    }
}

void IOregisters::store32(uint32_t addr, uint32_t val){
    // 4 bytes registers
    switch(0x04000000 + addr){
        case REG_ADDR::DMAxSAD[0]:
            writeDMAxSAD(0, val);
            break;
        case REG_ADDR::DMAxSAD[1]:
            writeDMAxSAD(1, val);
            break;
        case REG_ADDR::DMAxSAD[2]:
            writeDMAxSAD(2, val);
            break;
        case REG_ADDR::DMAxSAD[3]:
            writeDMAxSAD(3, val);
            break;
        case REG_ADDR::DMAxDAD[0]:
            writeDMAxDAD(0, val);
            break;
        case REG_ADDR::DMAxDAD[1]:
            writeDMAxDAD(1, val);
            break;
        case REG_ADDR::DMAxDAD[2]:
            writeDMAxDAD(2, val);
            break;
        case REG_ADDR::DMAxDAD[3]:
            writeDMAxDAD(3, val);
            break;

        default:
            // It is not a 4 byte register, divide the store in two
            store16(addr, val & 0xFFFF);
            store16(addr + 2, val >> 16);
    }
}

void IOregisters::storeWrapper(uint32_t addr, uint32_t val, uint8_t bytes){
    if(bytes == 4){
        store32(addr, val);
    }else if(bytes == 2){
        store16(addr, val & 0xFFFF);
    }else{
        store8(addr, val & 0xFF);
    }
}

uint32_t IOregisters::getDISPCNT(){
    uint32_t dispcnt = read(REG_ADDR::DISPCNT - MemoryManager::IO_REGISTERS_OFFSET_START, 4);
    return dispcnt;
}

uint32_t IOregisters::getIE(){
    uint32_t ie = read(REG_ADDR::IE - MemoryManager::IO_REGISTERS_OFFSET_START, 2);
    return ie;
}

uint32_t IOregisters::getIF(){
    uint32_t regIf = read(REG_ADDR::IF - MemoryManager::IO_REGISTERS_OFFSET_START, 2);
    return regIf;
}

uint8_t IOregisters::getDCNT_MODE(){
    return Utils::getRegBits(getDISPCNT(), 0b0000000000000111, 0);
}

bool IOregisters::getMustHaltCpu(){
    return mustHaltCpu;
}

void IOregisters::setMustHaltCpu(){
    mustHaltCpu = true;
}

void IOregisters::clearMustHaltCpu(){
    mustHaltCpu = false;
}
