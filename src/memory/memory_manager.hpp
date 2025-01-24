#ifndef _MEMORY_MANAGER_ 
#define _MEMORY_MANAGER_ 

#include <iostream>
#include <stdexcept>
#include "../utils/utils.hpp"
#include "bios.hpp"
#include "gamepak.hpp"
#include "vram.hpp"
#include "ewram.hpp"
#include "iwram.hpp"
#include "sram.hpp"
#include "oam.hpp"
#include "palette_ram.hpp"
#include "io_registers.hpp"

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

/*
Memory Mirrors
Most internal memory is mirrored across the whole 24bit/16MB address space in which it is located: 
  Slow On-board RAM at 2XXXXXXh, 
  Fast On-Chip RAM at 3XXXXXXh, 
  Palette RAM at 5XXXXXXh, 
  VRAM at 6XXXXXXh, 
  OAM at 7XXXXXXh. 
Even though VRAM is sized 96K (64K+32K), it is repeated in steps of 128K (64K+32K+32K, the two 32K blocks itself being mirrors of each other).

BIOS ROM, Normal ROM Cartridges, and I/O area are NOT mirrored, the only exception is the undocumented I/O port at 4000800h (repeated each 64K).

The 64K SRAM area is mirrored across the whole 32MB area at E000000h-FFFFFFFh, also, inside of the 64K SRAM field, 32K SRAM chips are repeated twice.

*/

class IOregisters;

class MemoryManager {
    public:
        MemoryManager(BIOS &bios, GamePak &gamepak, VRAM &vram, EWRAM &ewram, IWRAM &iwram, SRAM &sram, OAM &oam, PaletteRAM &paletteRam, IOregisters &io);
        uint32_t readWord(uint32_t addr);
        uint16_t readHalfWord(uint32_t addr);
        uint16_t readByte(uint32_t addr);
        uint32_t read(uint32_t addr, uint8_t bytes);
        void store(uint32_t addr, uint32_t val, uint8_t bytes);
        void blockTransfer(uint32_t dst, uint32_t src, uint32_t bytes);

        uint8_t* getRawVRAM();
        uint8_t* getPaletteRAM();
        uint8_t* getIOregisters();
        uint8_t* getOAM();

        // General Internal Memory
        const static uint32_t BIOS_OFFSET_START = 0x00000000;
        const static uint32_t BIOS_OFFSET_END = 0x00003FFF;

        const static uint32_t EWRAM_OFFSET_START = 0x02000000;
        const static uint32_t EWRAM_OFFSET_END = 0x0203FFFF;
        const static uint32_t EWRAM_MIRROR_OFFSET_END = 0x02FFFFFF;

        const static uint32_t IWRAM_OFFSET_START = 0x03000000;
        const static uint32_t IWRAM_OFFSET_END = 0x03007FFF;
        const static uint32_t IWRAM_MIRROR_OFFSET_END = 0x03FFFFFF;

        // I/O Registers
        const static uint32_t IO_REGISTERS_OFFSET_START = 0x04000000;
        const static uint32_t IO_REGISTERS_OFFSET_END = 0x040007FF;

        // Undocumented - Internal Memory Control (R/W)
        const static uint32_t INTERNAL_MEMORY_CONTROL_OFFSET_START = 0x04000800;

        // Internal Display Memory
        const static uint32_t PALETTE_RAM_OFFSET_START = 0x05000000;
        const static uint32_t PALETTE_RAM_OFFSET_END = 0x050003FF;
        const static uint32_t PALETTE_RAM_MIRROR_OFFSET_END = 0x05FFFFFF;

        const static uint32_t VRAM_OFFSET_START = 0x06000000;
        const static uint32_t VRAM_OFFSET_END = 0x06017FFF;
        const static uint32_t VRAM_MIRROR_OFFSET_START = 0x06018000;
        const static uint32_t VRAM_MIRROR_OFFSET_END = 0x06FFFFFF;
        const static uint32_t VRAM_MIRROR_BLOCK_SIZE = 0x00020000;

        const static uint32_t OAM_OBJ_OFFSET_START = 0x07000000;
        const static uint32_t OAM_OBJ_OFFSET_END = 0x070003FF;
        const static uint32_t OAM_OBJ_MIRROR_OFFSET_END = 0x07FFFFFF;

        // External Memory (Game Pak)
        const static uint32_t GAMEPAK_WAIT_0_OFFSET_START = 0x08000000;
        const static uint32_t GAMEPAK_WAIT_0_OFFSET_END = 0x09FFFFFF;

        const static uint32_t GAMEPAK_WAIT_1_OFFSET_START = 0x0A000000;
        const static uint32_t GAMEPAK_WAIT_1_OFFSET_END = 0x0BFFFFFF;

        const static uint32_t GAMEPAK_WAIT_2_OFFSET_START = 0x0C000000;
        const static uint32_t GAMEPAK_WAIT_2_OFFSET_END = 0x0DFFFFFF;

        const static uint32_t GAMEPAK_SRAM_OFFSET_START = 0x0E000000;
        const static uint32_t GAMEPAK_SRAM_OFFSET_END = 0x0E00FFFF;
        const static uint32_t GAMEPAK_SRAM_MIRROR_OFFSET_END = 0x0FFFFFFF;

        // Unused memory
        const static uint32_t UNUSED_MEMORY_1_OFFSET_START = 0x00004000;
        const static uint32_t UNUSED_MEMORY_1_OFFSET_END = 0x01FFFFFF;

        const static uint32_t UNUSED_MEMORY_2_OFFSET_START = 0x10000000;
        const static uint32_t UNUSED_MEMORY_2_OFFSET_END = 0xFFFFFFFF;

    private:
        BIOS &bios;
        GamePak &gamepak;
        VRAM &vram;
        EWRAM &ewram;
        IWRAM &iwram;
        SRAM &sram;
        OAM &oam;
        PaletteRAM &paletteRam;
        IOregisters &io;

        enum class Region {
            BIOS                    = 0x0,
            EWRAM                   = 0x2,
            IWRAM                   = 0x3,
            IO                      = 0x4,
            PALETTE_RAM             = 0x5,
            VRAM                    = 0x6,
            OAM                     = 0x7,
            GAMEPAK_WAIT_0_REGION_1 = 0x8,
            GAMEPAK_WAIT_0_REGION_2 = 0x9,
            GAMEPAK_WAIT_1_REGION_1 = 0xA,
            GAMEPAK_WAIT_1_REGION_2 = 0xB,
            GAMEPAK_WAIT_2_REGION_1 = 0xC,
            GAMEPAK_WAIT_2_REGION_2 = 0xD,
            SRAM                    = 0xE,
            SRAM_MIRROR             = 0xF
        };
};

#endif

