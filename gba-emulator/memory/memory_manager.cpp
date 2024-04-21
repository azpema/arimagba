#include "memory_manager.hpp"

MemoryManager::MemoryManager(std::string biosPath, std::string gamePath) : bios(biosPath), gamepak(gamePath) {}
/*{

    bios = BIOS(std::string("files/bios.bin"));
    //io = IOregisters();
    //vram = VRAM();
    //gamepak = GamePak(std::string("files/armwrestler-gba-fixed.gba"));
}*/

uint32_t MemoryManager::readWord(uint32_t addr) {
    return read(addr, 4);
}

uint16_t MemoryManager::readHalfWord(uint32_t addr) {
    return read(addr, 2);
}

uint32_t MemoryManager::read(uint32_t addr, uint8_t bytes) {
    uint32_t val = 0;
    if(addr >= BIOS_OFFSET_START && addr <= BIOS_OFFSET_END) {
        // BIOS

    }else if(addr >= GAMEPAK_WAIT_0_OFFSET_START && addr <= GAMEPAK_WAIT_0_OFFSET_END) {
        // GamePak Wait State 0
        val = gamepak.read(addr - GAMEPAK_WAIT_0_OFFSET_START, bytes);
    }else if(Utils::inRange(addr, LCD_REGISTERS_OFFSET_START, LCD_REGISTERS_OFFSET_END)){

    }else if(Utils::inRange(addr, VRAM_OFFSET_START, VRAM_OFFSET_END)){
        std::cerr << "TODO: VRAM Unimplemented memory region in MemoryManager::read" << std::endl;
    }else{
        throw std::runtime_error("Error: Unimplemented memory region in MemoryManager::read");
    }

    return val;
}

void MemoryManager::store(uint32_t addr, uint32_t val,  uint8_t bytes) {
    if(Utils::inRange(addr, LCD_REGISTERS_OFFSET_START, LCD_REGISTERS_OFFSET_END)){
        // IO registers
    }else if(Utils::inRange(addr, VRAM_OFFSET_START, VRAM_OFFSET_END)){
        vram.store(addr - VRAM_OFFSET_START, val, bytes);
    }

}

uint16_t* MemoryManager::getRawVRAM(){
    return vram.getRawMemory();
}

