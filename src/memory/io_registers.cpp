#include "io_registers.hpp"

IOregisters::IOregisters() {
    std::cerr << "TODO Set proper initial values at IOregisters" << std::endl;
    for(size_t i=0; i<IOREGISTERS_SIZE; i++){
        mem[i] = 0x00;
    }
}

uint16_t IOregisters::writeToIf(uint16_t val){
    uint16_t regIf = read(REG_ADDR::IF - MemoryManager::IO_REGISTERS_OFFSET_START, 2);
    uint16_t newVal = (~val) & regIf;
    return newVal;
}

void IOregisters::storeWrapper(uint32_t addr, uint32_t val, uint8_t bytes){
    uint32_t newVal = val;
    if(addr == (REG_ADDR::IME - MemoryManager::IO_REGISTERS_OFFSET_START)){
        /*
        4000208h - IME - Interrupt Master Enable Register (R/W)
        Bit   Expl.
        0     Disable all interrupts         (0=Disable All, 1=See IE register)
        1-31  Not used
        */
        newVal &= 0b00000000000000000000000000000001;
    }else if(addr == (REG_ADDR::IF - MemoryManager::IO_REGISTERS_OFFSET_START)){
        if(bytes != 2){
            throw new std::runtime_error("TODO storeWrapper");
        }
        newVal = writeToIf(val);
    }
    if(addr == (REG_ADDR::IE - MemoryManager::IO_REGISTERS_OFFSET_START)){
        if(bytes == 1){
            throw new std::runtime_error("TODO storeWrapper");
        }else if(bytes == 2){
            // Do nothing
        }else if(bytes == 4){
            uint16_t newRegIf = writeToIf(val >> 16);
            newVal = (newVal & 0b0000000011111111) | (newRegIf << 16);
        }
    }
    store(addr, newVal, bytes);
}

uint32_t IOregisters::getDISPCNT(){
    uint32_t dispcnt = read(REG_ADDR::DISPCNT - MemoryManager::IO_REGISTERS_OFFSET_START, 4);
    return dispcnt;
}

uint8_t IOregisters::getDCNT_MODE(){
    return Utils::getRegBits(getDISPCNT(), 0b0000000000000111, 0);
}