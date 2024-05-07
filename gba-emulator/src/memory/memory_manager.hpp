#ifndef _MEMORY_MANAGER_ 
#define _MEMORY_MANAGER_ 

#include <iostream>
#include <stdexcept>
#include "../utils/utils.hpp"
#include "bios.hpp"
#include "gamepak.hpp"
#include "vram.hpp"
#include "wram.hpp"
/*
  General Internal Memory
    00000000-00003FFF   BIOS - System ROM         (16 KBytes)
    00004000-01FFFFFF   Not used
    02000000-0203FFFF   WRAM - On-board Work RAM  (256 KBytes) 2 Wait
    02040000-02FFFFFF   Not used
    03000000-03007FFF   WRAM - On-chip Work RAM   (32 KBytes)
    03008000-03FFFFFF   Not used
    04000000-040003FE   I/O Registers
    04000400-04FFFFFF   Not used

  Internal Display Memory
    05000000-050003FF   BG/OBJ Palette RAM        (1 Kbyte)
    05000400-05FFFFFF   Not used
    06000000-06017FFF   VRAM - Video RAM          (96 KBytes)
    06018000-06FFFFFF   Not used
    07000000-070003FF   OAM - OBJ Attributes      (1 Kbyte)
    07000400-07FFFFFF   Not used

  External Memory (Game Pak)
    08000000-09FFFFFF   Game Pak ROM/FlashROM (max 32MB) - Wait State 0
    0A000000-0BFFFFFF   Game Pak ROM/FlashROM (max 32MB) - Wait State 1
    0C000000-0DFFFFFF   Game Pak ROM/FlashROM (max 32MB) - Wait State 2
    0E000000-0E00FFFF   Game Pak SRAM    (max 64 KBytes) - 8bit Bus width
    0E010000-0FFFFFFF   Not used

  Unused Memory Area
    10000000-FFFFFFFF   Not used (upper 4bits of address bus unused)
*/

class MemoryManager {
    public:
        MemoryManager(BIOS &bios, GamePak &gamepak, VRAM &vram, WRAM &wram);
        uint32_t readWord(uint32_t addr);
        uint16_t readHalfWord(uint32_t addr);
        void store(uint32_t addr, uint32_t val, uint8_t bytes);

        uint16_t* getRawVRAM();
        
    private:
        uint32_t read(uint32_t addr, uint8_t bytes);
        
        BIOS &bios;
        GamePak &gamepak;
        VRAM &vram;
        WRAM &wram;
        // General Internal Memory
        const static uint32_t BIOS_OFFSET_START = 0x00000000;
        const static uint32_t BIOS_OFFSET_END = 0x00003FFF;

        const static uint32_t WRAM_OFFSET_START = 0x03000000;
        const static uint32_t WRAM_OFFSET_END = 0x03007FFF;

        // LCD I/O Registers
        const static uint32_t LCD_REGISTERS_OFFSET_START = 0x04000000;
        const static uint32_t LCD_REGISTERS_OFFSET_END = 0x04000056;

        // Internal Display Memory
        const static uint32_t BG_OBJ_PALETTE_RAM_OFFSET_START = 0x05000000;
        const static uint32_t BG_OBJ_PALETTE_RAM_OFFSET_END = 0x050003FF;

        const static uint32_t VRAM_OFFSET_START = 0x06000000;
        const static uint32_t VRAM_OFFSET_END = 0x06017FFF;

        const static uint32_t OAM_OBJ_OFFSET_START = 0x07000000;
        const static uint32_t OAM_OBJ_OFFSET_END = 0x070003FF;

        // External Memory (Game Pak)
        const static uint32_t GAMEPAK_WAIT_0_OFFSET_START = 0x08000000;
        const static uint32_t GAMEPAK_WAIT_0_OFFSET_END = 0x09FFFFFF;
};

#endif

