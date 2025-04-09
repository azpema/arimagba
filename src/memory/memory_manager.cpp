#include "memory_manager.hpp"
#include "../utils/utils.hpp"

MemoryManager::MemoryManager(BIOS &bios, GamePak &gamepak, VRAM &vram, EWRAM &ewram, IWRAM &iwram, SRAM &sram, OAM &oam, PaletteRAM &paletteRam, IOregisters &io)
 : bios(bios), gamepak(gamepak), vram(vram), ewram(ewram), iwram(iwram), sram(sram), oam(oam), paletteRam(paletteRam), io(io) {}

uint32_t MemoryManager::readWord(uint32_t addr, bool opPreFetch) {
    return read(addr, 4, opPreFetch);
}

uint16_t MemoryManager::readHalfWord(uint32_t addr, bool opPreFetch) {
    return read(addr, 2, opPreFetch);
}

uint8_t MemoryManager::readByte(uint32_t addr) {
    return read(addr, 1);
}

uint32_t MemoryManager::read(uint32_t addr, uint8_t bytes, bool opPreFetch) {
    uint32_t val = 0;
    auto region = static_cast<Region>(addr >> 24);
    bool validRead = true;
    switch(region){
        case Region::BIOS:
            if(addr <= 0x3FFF){
                val = bios.readWrapper(addr, bytes, cpu->isPcInBios());
            }else{
                validRead = false;
                std::cout << "BIOS OOB" << "\n";
                val = openBusVal;
                if((addr & 0x3) != 0){
                    val = Utils::rotateRight(val, (addr & 0x3)*8);
                }
            }            
            break;
        case Region::EWRAM:
            val = ewram.read((addr & EWRAM_OFFSET_END) - EWRAM_OFFSET_START, bytes);
            break;
        case Region::IWRAM:
            val = iwram.read((addr & IWRAM_OFFSET_END) - IWRAM_OFFSET_START, bytes);
            break;
        case Region::IO:
            val = io.read(addr - IO_REGISTERS_OFFSET_START, bytes);
            break;
        case Region::PALETTE_RAM:
            val = paletteRam.read((addr & PALETTE_RAM_OFFSET_END) - PALETTE_RAM_OFFSET_START, bytes);
            break;
        case Region::VRAM:
            if(Utils::inRange<>(addr, VRAM_OFFSET_START, VRAM_OFFSET_END)){
                val = vram.read(addr - VRAM_OFFSET_START, bytes);
            }else if(Utils::inRange<>(addr, VRAM_MIRROR_OFFSET_START, VRAM_MIRROR_OFFSET_END)){
                // In bitmap modes reads and writes to 0x06018000 - 0x0601BFFF do not work (writes are discarded; reads may always return 0?).
                // https://github.com/nba-emu/hw-test/tree/master/ppu/vram-mirror
                if(Utils::inRange<>(addr, 0x06018000, 0x0601BFFF)){
                    if(3 <= io.getDCNT_MODE() && io.getDCNT_MODE() <= 5){
                        return 0;
                    }
                }
                
                // Even though VRAM is sized 96K (64K+32K), it is repeated in steps of 128K (64K+32K+32K),
                // the two 32K blocks itself being mirrors of each other)
                uint32_t relativeAddr = (addr - VRAM_OFFSET_START) % VRAM_MIRROR_BLOCK_SIZE;
                if(relativeAddr >= 0x18000){
                    relativeAddr -= 0x8000;
                }

                if(relativeAddr >= 0x18000){
                    throw std::runtime_error("Invalid address calculated for VRAM mirror");
                }
                
                val = vram.read(relativeAddr, bytes);
            }
            break;
        case Region::OAM:
            val = oam.read((addr & OAM_OBJ_OFFSET_END) - OAM_OBJ_OFFSET_START, bytes);
            break;
        case Region::GAMEPAK_WAIT_0_REGION_1:
        case Region::GAMEPAK_WAIT_0_REGION_2:
            val = gamepak.read(addr - GAMEPAK_WAIT_0_OFFSET_START, bytes);
            break;
        case Region::GAMEPAK_WAIT_1_REGION_1:
        case Region::GAMEPAK_WAIT_1_REGION_2:
            val = gamepak.read(addr - GAMEPAK_WAIT_1_OFFSET_START, bytes);
            break;
        case Region::GAMEPAK_WAIT_2_REGION_1:
        case Region::GAMEPAK_WAIT_2_REGION_2:
            val = gamepak.read(addr - GAMEPAK_WAIT_2_OFFSET_START, bytes);
            break;
        case Region::SRAM:
        case Region::SRAM_MIRROR:
            val = sram.read((addr & GAMEPAK_SRAM_OFFSET_END) - GAMEPAK_SRAM_OFFSET_START, bytes);
            break;
        default:
            validRead = false;
            val = openBusVal;
            if((addr & 0x3) != 0){
                val = Utils::rotateRight(val, (addr & 0x3)*8);
            }
            break;
    }

    if(validRead && opPreFetch){
        openBusVal = val;
    }
    
    return val;
}

void MemoryManager::store(uint32_t addr, uint32_t val,  uint8_t bytes) {
    auto region = static_cast<Region>(addr >> 24);
    switch(region){
        case Region::BIOS:
            // Do nothing
            break;
        case Region::EWRAM:
            ewram.store((addr & EWRAM_OFFSET_END) - EWRAM_OFFSET_START, val, bytes);
            break;
        case Region::IWRAM:
            iwram.store((addr & IWRAM_OFFSET_END) - IWRAM_OFFSET_START, val, bytes);
            break;
        case Region::IO:
            io.storeWrapper(addr - IO_REGISTERS_OFFSET_START, val, bytes);
            break;
        case Region::PALETTE_RAM:
        {
            uint32_t newAddr = addr;
            uint32_t newVal = val;
            uint32_t newBytes = bytes;
            if(bytes == 1){
                newAddr &= 0xFFFFFFFE; 
                newVal = (val << 8) | val;
                newBytes = 2;
            }
            paletteRam.store((newAddr & PALETTE_RAM_OFFSET_END) - PALETTE_RAM_OFFSET_START, newVal, newBytes);
            break;
        }
        case Region::VRAM:
        {
            // In bitmap modes reads and writes to 0x06018000 - 0x0601BFFF do not work (writes are discarded; reads may always return 0?).
            // https://github.com/nba-emu/hw-test/tree/master/ppu/vram-mirror

            // Writing 8bit Data to Video Memory
            // Video Memory (BG, OBJ, OAM, Palette) can be written to in 16bit and 32bit units only. Attempts to write 8bit data (by STRB opcode) won’t work:
            // Writes to OBJ (6010000h-6017FFFh) (or 6014000h-6017FFFh in Bitmap mode) and to OAM (7000000h-70003FFh) are ignored, the memory content remains 
            // unchanged.
            // Writes to BG (6000000h-600FFFFh) (or 6000000h-6013FFFh in Bitmap mode) and to Palette (5000000h-50003FFh) are writing the new 8bit value to 
            // BOTH upper and lower 8bits of the addressed halfword, ie. “[addr AND NOT 1]=data*101h”.
            // https://rust-console.github.io/gbatek-gbaonly/#writing-8bit-data-to-video-memory
            
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
                // 
                if(Utils::inRange<>(addr, 0x06010000, 0x06017FFF)){
                    if(bytes == 1){
                        return;
                    }
                }
            }

            
            // Even though VRAM is sized 96K (64K+32K), it is repeated in steps of 128K (64K+32K+32K),
            // the two 32K blocks itself being mirrors of each other)
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
            break;
        }
        case Region::OAM:
            if(bytes == 1){
                return;
            }
            oam.store((addr & OAM_OBJ_OFFSET_END) - OAM_OBJ_OFFSET_START, val, bytes);
            break;
        case Region::GAMEPAK_WAIT_0_REGION_1:
        case Region::GAMEPAK_WAIT_0_REGION_2:
        case Region::GAMEPAK_WAIT_1_REGION_1:
        case Region::GAMEPAK_WAIT_1_REGION_2:
        case Region::GAMEPAK_WAIT_2_REGION_1:
        case Region::GAMEPAK_WAIT_2_REGION_2:
            // Do nothing
            break;
        case Region::SRAM:
        case Region::SRAM_MIRROR:
            sram.store((addr & GAMEPAK_SRAM_OFFSET_END) - GAMEPAK_SRAM_OFFSET_START, val, bytes);
            break;
        default:
            // Out of bounds, return OpenBus value?
            break;
    }
}

void MemoryManager::blockTransfer(uint32_t dst, uint32_t src, uint32_t bytes){
    // TODO Use something like memcpy, but you must deal with memory mirrors...
    for(size_t i = 0; i < bytes; i+= 2){
        uint16_t val = readHalfWord(src + i);
        store(dst + i, val, 2);
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

void MemoryManager::addCpu(ARM7TDMI *cpu){
    this->cpu = cpu;
}