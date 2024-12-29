#include "memory_manager.hpp"
#include "../utils/utils.hpp"

MemoryManager::MemoryManager(BIOS &bios, GamePak &gamepak, VRAM &vram, EWRAM &ewram, IWRAM &iwram, SRAM &sram, OAM &oam, PaletteRAM &paletteRam, IOregisters &io)
 : bios(bios), gamepak(gamepak), vram(vram), ewram(ewram), iwram(iwram), sram(sram), oam(oam), paletteRam(paletteRam), io(io) {}

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
    if(Utils::inRange<>(addr, BIOS_OFFSET_START, BIOS_OFFSET_END)){
        val = bios.read(addr, bytes);
    }else if(Utils::inRange<>(addr, GAMEPAK_WAIT_0_OFFSET_START, GAMEPAK_WAIT_0_OFFSET_END)) {
        // GamePak Wait State 0
        val = gamepak.read(addr - GAMEPAK_WAIT_0_OFFSET_START, bytes);
    }else if(Utils::inRange<>(addr, GAMEPAK_WAIT_1_OFFSET_START, GAMEPAK_WAIT_1_OFFSET_END)) {
        // GamePak Wait State 1
        val = gamepak.read(addr - GAMEPAK_WAIT_1_OFFSET_START, bytes);
    }else if(Utils::inRange<>(addr, GAMEPAK_WAIT_2_OFFSET_START, GAMEPAK_WAIT_2_OFFSET_END)) {
        // GamePak Wait State 2
        val = gamepak.read(addr - GAMEPAK_WAIT_2_OFFSET_START, bytes);
    }else if(Utils::inRange<>(addr, EWRAM_OFFSET_START, EWRAM_MIRROR_OFFSET_END)){
        val = ewram.read((addr & EWRAM_OFFSET_END) - EWRAM_OFFSET_START, bytes);
    }else if(Utils::inRange<>(addr, IWRAM_OFFSET_START, IWRAM_MIRROR_OFFSET_END)){
        val = iwram.read((addr & IWRAM_OFFSET_END) - IWRAM_OFFSET_START, bytes);
    }else if(Utils::inRange<>(addr, PALETTE_RAM_OFFSET_START, PALETTE_RAM_MIRROR_OFFSET_END)){
        val = paletteRam.read((addr & PALETTE_RAM_OFFSET_END) - PALETTE_RAM_OFFSET_START, bytes);
    }else if(Utils::inRange<>(addr, VRAM_OFFSET_START, VRAM_OFFSET_END)){
        //std::cerr << "TODO: VRAM Unimplemented memory region in MemoryManager::read" << std::endl;
        //std::cerr << "TODO: VRAM Mirroring" << std::endl;
        val = vram.read(addr - VRAM_OFFSET_START, bytes);
    }else if(Utils::inRange<>(addr, VRAM_MIRROR_OFFSET_START, VRAM_MIRROR_OFFSET_END)){
        /*
        In bitmap modes reads and writes to 0x06018000 - 0x0601BFFF do not work (writes are discarded; reads may always return 0?).
        https://github.com/nba-emu/hw-test/tree/master/ppu/vram-mirror
        */
        if(Utils::inRange<>(addr, 0x06018000, 0x0601BFFF)){
            if(3 <= io.getDCNT_MODE() && io.getDCNT_MODE() <= 5){
                return 0;
            }
        }
        /*
        Even though VRAM is sized 96K (64K+32K), it is repeated in steps of 128K (64K+32K+32K),
         the two 32K blocks itself being mirrors of each other)
        */
        uint32_t relativeAddr = (addr - VRAM_OFFSET_START) % VRAM_MIRROR_BLOCK_SIZE;
        if(relativeAddr >= 0x18000){
            relativeAddr -= 0x8000;
        }

        if(relativeAddr >= 0x18000){
            throw std::runtime_error("Invalid address calculated for VRAM mirror");
        }
        
        val = vram.read(relativeAddr, bytes);
    }else if(Utils::inRange<>(addr, IO_REGISTERS_OFFSET_START, IO_REGISTERS_OFFSET_END)){
        val = io.read(addr - IO_REGISTERS_OFFSET_START, bytes);
    }else if(Utils::inRange<>(addr, GAMEPAK_SRAM_OFFSET_START, GAMEPAK_SRAM_MIRROR_OFFSET_END)){
        val = sram.read((addr & GAMEPAK_SRAM_OFFSET_END) - GAMEPAK_SRAM_OFFSET_START, bytes);
    }else if(Utils::inRange<>(addr, UNUSED_MEMORY_1_OFFSET_START, UNUSED_MEMORY_1_OFFSET_END) ||
             Utils::inRange<>(addr, UNUSED_MEMORY_2_OFFSET_START, UNUSED_MEMORY_2_OFFSET_END)){
        //std::cout << "TODO: Openbus read" << std::endl;
    }else if(Utils::inRange<>(addr, OAM_OBJ_OFFSET_START, OAM_OBJ_MIRROR_OFFSET_END)){
        val = oam.read((addr & OAM_OBJ_OFFSET_END) - OAM_OBJ_OFFSET_START, bytes);
    }else{
        throw std::runtime_error("Error: Unimplemented memory region in MemoryManager::read");
    }

    return val;
}

void MemoryManager::store(uint32_t addr, uint32_t val,  uint8_t bytes) {
    if(Utils::inRange<>(addr, BIOS_OFFSET_START, BIOS_OFFSET_END)){
        std::cout << "TODO: BIOS write" << std::endl;
    }else if(Utils::inRange<>(addr, GAMEPAK_WAIT_0_OFFSET_START, GAMEPAK_WAIT_0_OFFSET_END)) {
        // Do nothing
    }else if(Utils::inRange<>(addr, VRAM_OFFSET_START, VRAM_MIRROR_OFFSET_END)){
        /*
        In bitmap modes reads and writes to 0x06018000 - 0x0601BFFF do not work (writes are discarded; reads may always return 0?).
        https://github.com/nba-emu/hw-test/tree/master/ppu/vram-mirror

        Writing 8bit Data to Video Memory
        Video Memory (BG, OBJ, OAM, Palette) can be written to in 16bit and 32bit units only. Attempts to write 8bit data (by STRB opcode) won’t work:

        Writes to OBJ (6010000h-6017FFFh) (or 6014000h-6017FFFh in Bitmap mode) and to OAM (7000000h-70003FFh) are ignored, the memory content remains 
        unchanged.

        Writes to BG (6000000h-600FFFFh) (or 6000000h-6013FFFh in Bitmap mode) and to Palette (5000000h-50003FFh) are writing the new 8bit value to 
        BOTH upper and lower 8bits of the addressed halfword, ie. “[addr AND NOT 1]=data*101h”.
        https://rust-console.github.io/gbatek-gbaonly/#writing-8bit-data-to-video-memory
        */
        // Bitmap modes
        if(3 <= io.getDCNT_MODE() && io.getDCNT_MODE() <= 5){
            if(Utils::inRange<>(addr, 0x06018000, 0x0601BFFF)){
                return;
            }

            if(Utils::inRange<>(addr, 0x06014000, 0x06017FFF)){
                if(bytes == 1){
                    return;
                }
            }
        }else{
            if(Utils::inRange<>(addr, 0x06010000, 0x06017FFF)){
                if(bytes == 1){
                    return;
                }
            }
        }

        /*
        Even though VRAM is sized 96K (64K+32K), it is repeated in steps of 128K (64K+32K+32K),
         the two 32K blocks itself being mirrors of each other)
        */
        uint32_t relativeAddr = (addr - VRAM_OFFSET_START) % VRAM_MIRROR_BLOCK_SIZE;
        if(relativeAddr >= 0x18000){
            relativeAddr -= 0x8000;
        }

        if(relativeAddr >= 0x18000){
            throw std::runtime_error("Invalid address calculated for VRAM mirror");
        }

        uint32_t newAddr = relativeAddr;
        uint32_t newVal = val;
        uint32_t newBytes = bytes;

        if(bytes == 1){
            if(3 <= io.getDCNT_MODE() && io.getDCNT_MODE() <= 5){
                if(Utils::inRange<>(addr, 0x06000000, 0x06013FFF)){
                    newAddr &= 0xFFFFFFFE;
                    newVal = (val << 8) | val;
                    newBytes = 2;
                }
            }else{
                if(Utils::inRange<>(addr, 0x06000000, 0x0600FFFF)){
                    if(bytes == 1){
                        newAddr &= 0xFFFFFFFE;
                        newVal = (val << 8) | val;
                        newBytes = 2;
                    }
                }
            }
        }
        
        vram.store(newAddr, newVal, newBytes);
    }else if(Utils::inRange<>(addr, EWRAM_OFFSET_START, EWRAM_MIRROR_OFFSET_END)){
        ewram.store((addr & EWRAM_OFFSET_END) - EWRAM_OFFSET_START, val, bytes);
    }else if(Utils::inRange<>(addr, IWRAM_OFFSET_START, IWRAM_MIRROR_OFFSET_END)){
        iwram.store((addr & IWRAM_OFFSET_END) - IWRAM_OFFSET_START, val, bytes);
    }else if(Utils::inRange<>(addr, PALETTE_RAM_OFFSET_START, PALETTE_RAM_MIRROR_OFFSET_END)){
        uint32_t newAddr = addr;
        uint32_t newVal = val;
        uint32_t newBytes = bytes;
        if(bytes == 1){
            newAddr &= 0xFFFFFFFE; 
            newVal = (val << 8) | val;
            newBytes = 2;
        }
        paletteRam.store((newAddr & PALETTE_RAM_OFFSET_END) - PALETTE_RAM_OFFSET_START, newVal, newBytes);
    }else if(Utils::inRange<>(addr, IO_REGISTERS_OFFSET_START, IO_REGISTERS_OFFSET_END)){
        io.storeWrapper(addr - IO_REGISTERS_OFFSET_START, val, bytes);
    }else if(Utils::inRange<>(addr, GAMEPAK_SRAM_OFFSET_START, GAMEPAK_SRAM_MIRROR_OFFSET_END)){
        sram.store((addr & GAMEPAK_SRAM_OFFSET_END) - GAMEPAK_SRAM_OFFSET_START, val, bytes);
    }else if(Utils::inRange<>(addr, OAM_OBJ_OFFSET_START, OAM_OBJ_MIRROR_OFFSET_END)){
        // TODO redundant
        if(Utils::inRange<>(addr, OAM_OBJ_OFFSET_START, OAM_OBJ_MIRROR_OFFSET_END)){
            if(bytes == 1){
                return;
            }
        }
        oam.store((addr & OAM_OBJ_OFFSET_END) - OAM_OBJ_OFFSET_START, val, bytes);
    }else if(Utils::inRange<>(addr, UNUSED_MEMORY_1_OFFSET_START, UNUSED_MEMORY_1_OFFSET_END) ||
             Utils::inRange<>(addr, UNUSED_MEMORY_2_OFFSET_START, UNUSED_MEMORY_2_OFFSET_END)){
        std::cout << "TODO: Openbus write" << std::endl;

    }else if(addr == INTERNAL_MEMORY_CONTROL_OFFSET_START){
        std::cout << "TODO: Implement internal memory control ' EWRAM wait state control'" << std::endl;
    }else{
        throw std::runtime_error("Error: Unimplemented memory region in MemoryManager::store");
    }
}

uint8_t* MemoryManager::getRawVRAM(){
    return vram.getRawMemory();
}

uint8_t* MemoryManager::getPaletteRAM(){
    return paletteRam.getRawMemory();
}

uint8_t* MemoryManager::getIOregisters(){
    return io.getRawMemory();
}

uint8_t* MemoryManager::getOAM(){
    return oam.getRawMemory();
}