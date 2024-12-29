#include "io_registers.hpp"

IOregisters::IOregisters() : mustHaltCpu(false) {
    std::cerr << "TODO Set proper initial values at IOregisters" << std::endl;
    for(size_t i=0; i<IOREGISTERS_SIZE; i++){
        mem[i] = 0x00;
    }
}

/*
    4000208h - IME - Interrupt Master Enable Register (R/W)
    Bit   Expl.
    0     Disable all interrupts         (0=Disable All, 1=See IE register)
    1-31  Not used
*/
void IOregisters::writeCallbackIME(){
    uint32_t newVal = read(REG_ADDR::IME - MemoryManager::IO_REGISTERS_OFFSET_START, REG_SIZE::IME);
    newVal &= 0b00000000000000000000000000000001;
    store(REG_ADDR::IME - MemoryManager::IO_REGISTERS_OFFSET_START, newVal, REG_SIZE::IME);
}

void IOregisters::writeCallbackIF(){
    static uint32_t oldVal = 0;

    uint16_t val = read(REG_ADDR::IF - MemoryManager::IO_REGISTERS_OFFSET_START, REG_SIZE::IF);
    uint16_t newVal = (~val) & oldVal;
    store(REG_ADDR::IF - MemoryManager::IO_REGISTERS_OFFSET_START, newVal, REG_SIZE::IF);

    // Update oldVal for next update
    oldVal = newVal;
}

void IOregisters::writeCallbackIE(){
    static uint32_t oldVal = 0;

    uint16_t val = read(REG_ADDR::IE - MemoryManager::IO_REGISTERS_OFFSET_START, REG_SIZE::IE);
    uint16_t newVal = (val & 0b0000000011111111);
    store(REG_ADDR::IE - MemoryManager::IO_REGISTERS_OFFSET_START, newVal, REG_SIZE::IE);

    // Update oldVal for next update
    oldVal = newVal;
}

void IOregisters::writeCallbackHALTCNT(){
    setMustHaltCpu();
}

// TODO refactor and check all possibilities
void IOregisters::storeWrapper(uint32_t addr, uint32_t val, uint8_t bytes){
    store(addr, val, bytes);

    bool writtenIE = false;
    bool writtenIF = false;
    bool writtenIME = false;
    bool writtenHALTCNT = false;

    // Determine, depending on addr and size, what mmio registers were written to
    for(uint32_t checkAddr = addr; checkAddr < addr + bytes; checkAddr++){
        if(!writtenIE && Utils::inRange<true, false>(checkAddr, REG_ADDR::IE - MemoryManager::IO_REGISTERS_OFFSET_START, REG_ADDR_END::IE - MemoryManager::IO_REGISTERS_OFFSET_START)){
            writtenIE = true;
        }else if(!writtenIF && Utils::inRange<true, false>(checkAddr, REG_ADDR::IF - MemoryManager::IO_REGISTERS_OFFSET_START, REG_ADDR_END::IF - MemoryManager::IO_REGISTERS_OFFSET_START)){
            writtenIF = true;
        }else if(!writtenIME && Utils::inRange<true, false>(checkAddr, REG_ADDR::IME - MemoryManager::IO_REGISTERS_OFFSET_START, REG_ADDR_END::IME - MemoryManager::IO_REGISTERS_OFFSET_START)){
            writtenIME = true;
        }else if(!writtenHALTCNT && Utils::inRange<true, false>(checkAddr, REG_ADDR::HALTCNT - MemoryManager::IO_REGISTERS_OFFSET_START, REG_ADDR_END::HALTCNT - MemoryManager::IO_REGISTERS_OFFSET_START)){
            writtenHALTCNT = true;
        }
    }

    // Call each mmio register's function to update accordingly
    if(writtenIE){
        writeCallbackIE();
    }
    if(writtenIF){
        writeCallbackIF();
    }
    if(writtenIME){
        writeCallbackIME();
    }
    if(writtenHALTCNT){
        writeCallbackHALTCNT();
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
