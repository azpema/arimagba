#include "memory_manager.hpp"

MemoryManager::MemoryManager(BIOS &bios, GamePak &gamepak, VRAM &vram, EWRAM &ewram, IWRAM &iwram, SRAM &sram, PaletteRAM &paletteRam, IOregisters &io)
 : bios(bios), gamepak(gamepak), vram(vram), ewram(ewram), iwram(iwram), sram(sram), paletteRam(paletteRam), io(io) {}

uint32_t MemoryManager::readWord(uint32_t addr) {
    return read(addr, 4);
}

uint16_t MemoryManager::readHalfWord(uint32_t addr) {
    return read(addr, 2);
}

uint16_t MemoryManager::readByte(uint32_t addr) {
    return read(addr, 1);
}

uint32_t MemoryManager::read(uint32_t addr, uint8_t bytes) {
    uint32_t val = 0;
    if(Utils::inRange(addr, BIOS_OFFSET_START, BIOS_OFFSET_END)){
        val = bios.read(addr, bytes);
    }else if(Utils::inRange(addr, GAMEPAK_WAIT_0_OFFSET_START, GAMEPAK_WAIT_0_OFFSET_END)) {
        // GamePak Wait State 0
        val = gamepak.read(addr - GAMEPAK_WAIT_0_OFFSET_START, bytes);
    }else if(Utils::inRange(addr, EWRAM_OFFSET_START, EWRAM_MIRROR_OFFSET_END)){
        val = ewram.read((addr & EWRAM_OFFSET_END) - EWRAM_OFFSET_START, bytes);
    }else if(Utils::inRange(addr, IWRAM_OFFSET_START, IWRAM_MIRROR_OFFSET_END)){
        val = iwram.read((addr & IWRAM_OFFSET_END) - IWRAM_OFFSET_START, bytes);
    }else if(Utils::inRange(addr, PALETTE_RAM_OFFSET_START, PALETTE_RAM_MIRROR_OFFSET_END)){
        val = paletteRam.read((addr & PALETTE_RAM_OFFSET_END) - PALETTE_RAM_OFFSET_START, bytes);
    }else if(Utils::inRange(addr, VRAM_OFFSET_START, VRAM_OFFSET_END)){
        //std::cerr << "TODO: VRAM Unimplemented memory region in MemoryManager::read" << std::endl;
        //std::cerr << "TODO: VRAM Mirroring" << std::endl;
        throw std::runtime_error("Error: Unimplemented memory region in MemoryManager::load");
    }else if(Utils::inRange(addr, IO_REGISTERS_OFFSET_START, IO_REGISTERS_OFFSET_END)){
        val = io.read(addr - IO_REGISTERS_OFFSET_START, bytes);
    }else if(Utils::inRange(addr, GAMEPAK_SRAM_OFFSET_START, GAMEPAK_SRAM_MIRROR_OFFSET_END)){
        val = sram.read((addr & GAMEPAK_SRAM_OFFSET_END) - GAMEPAK_SRAM_OFFSET_START, bytes);
    }else if(Utils::inRange(addr, UNUSED_MEMORY_1_OFFSET_START, UNUSED_MEMORY_1_OFFSET_END) ||
             Utils::inRange(addr, UNUSED_MEMORY_2_OFFSET_START, UNUSED_MEMORY_2_OFFSET_END)){
        std::cout << "TODO: Openbus read" << std::endl;
    }else{
        throw std::runtime_error("Error: Unimplemented memory region in MemoryManager::read");
    }

    return val;
}

void MemoryManager::store(uint32_t addr, uint32_t val,  uint8_t bytes) {
    if(Utils::inRange(addr, BIOS_OFFSET_START, BIOS_OFFSET_END)){
        std::cout << "TODO: BIOS write" << std::endl;
    } else if(Utils::inRange(addr, VRAM_OFFSET_START, VRAM_OFFSET_END)){
        vram.store(addr - VRAM_OFFSET_START, val, bytes);
        //std::cerr << "TODO: VRAM Mirroring" << std::endl;
    }else if(Utils::inRange(addr, EWRAM_OFFSET_START, EWRAM_MIRROR_OFFSET_END)){
        ewram.store((addr & EWRAM_OFFSET_END) - EWRAM_OFFSET_START, val, bytes);
    }else if(Utils::inRange(addr, IWRAM_OFFSET_START, IWRAM_MIRROR_OFFSET_END)){
        iwram.store((addr & IWRAM_OFFSET_END) - IWRAM_OFFSET_START, val, bytes);
    }else if(Utils::inRange(addr, PALETTE_RAM_OFFSET_START, PALETTE_RAM_MIRROR_OFFSET_END)){
        paletteRam.store((addr & PALETTE_RAM_OFFSET_END) - PALETTE_RAM_OFFSET_START, val, bytes);
    }else if(Utils::inRange(addr, IO_REGISTERS_OFFSET_START, IO_REGISTERS_OFFSET_END)){
        io.store(addr - IO_REGISTERS_OFFSET_START, val, bytes);
    }else if(Utils::inRange(addr, GAMEPAK_SRAM_OFFSET_START, GAMEPAK_SRAM_MIRROR_OFFSET_END)){
        sram.store((addr & GAMEPAK_SRAM_OFFSET_END) - GAMEPAK_SRAM_OFFSET_START, val, bytes);
    }else if(Utils::inRange(addr, UNUSED_MEMORY_1_OFFSET_START, UNUSED_MEMORY_1_OFFSET_END) ||
             Utils::inRange(addr, UNUSED_MEMORY_2_OFFSET_START, UNUSED_MEMORY_2_OFFSET_END)){
        std::cout << "TODO: Openbus write" << std::endl;

    }else{
        throw std::runtime_error("Error: Unimplemented memory region in MemoryManager::store");
    }
}

uint16_t* MemoryManager::getRawVRAM(){
    return vram.getRawMemory();
}

uint16_t* MemoryManager::getPaletteRAM(){
    return paletteRam.getRawMemory();
}

uint16_t* MemoryManager::getIOregisters(){
    return io.getRawMemory();
}

