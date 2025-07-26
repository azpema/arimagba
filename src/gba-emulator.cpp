#include "cpu/arm7tdmi.hpp"
#include "graphics/ppu.hpp"
#include "memory/bios.hpp"
#include "memory/dma.hpp"
#include "memory/ewram.hpp"
#include "memory/gamepak.hpp"
#include "memory/io_registers.hpp"
#include "memory/iwram.hpp"
#include "memory/keys.hpp"
#include "memory/oam.hpp"
#include "memory/palette_ram.hpp"
#include "memory/sram.hpp"
#include "memory/vram.hpp"
#include "memory/persistent/eeprom.hpp"
#include "utils/utils.hpp"
#include <bitset>
#include <filesystem>

#include "imgui_impl_sdl2.h"

void decodeAllInstructionsThumb(ARM7TDMI& cpu);
void decodeAllInstructionsArm(ARM7TDMI& cpu);

int main(int argc, char** argv) {
    std::cout << "Current path is " << std::filesystem::current_path() << '\n';

    std::string gamePath = "../files/shinchan.gba";
    if (argc >= 2) {
        gamePath = std::string(argv[1]);
    }

    std::filesystem::path savePath = std::filesystem::absolute(gamePath);
    savePath.replace_extension(".sav");

    BIOS bios("../files/bios.bin");
    GamePak gamepak(gamePath);
    VRAM vram;
    EWRAM ewram;
    IWRAM iwram;
    SRAM sram;
    OAM oam;
    PaletteRAM paletteram;
    IOregisters io;
    // Gamepak may or may not have an EEPROM; we do not know its size either
    std::unique_ptr<EEPROM> eeprom = nullptr;
    MemoryManager mem(bios, gamepak, vram, ewram, iwram, sram, oam, paletteram, io, savePath,
        [&eeprom](std::unique_ptr<EEPROM> obj) {
        eeprom = std::move(obj);
    });

    Keys keys(&mem);
    ARM7TDMI cpu(&mem);
    mem.addCpu(&cpu);
    PPU ppu("ArimaGBA", cpu, &mem);
    DMA<0> dma0(cpu, mem);
    DMA<1> dma1(cpu, mem);
    DMA<2> dma2(cpu, mem);
    DMA<3> dma3(cpu, mem);

    cpu.setPC(0x08000000);

    bool run = true;

    uint32_t cpuCycles = 0;
    uint64_t totalCpuCycles = 0;

    bool vblankNow = false;
    bool hblankNow = false;

    gamepak.printInfo();

    while (run) {
        // Handle events
        if (cpuCycles % 500 == 0) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                ImGui_ImplSDL2_ProcessEvent(&e);
                switch (e.type) {
                case SDL_QUIT:
                    run = false;
                    break;

                case SDL_KEYDOWN:
                    keys.handleKey(e.key.keysym.sym, true);
                    break;

                case SDL_KEYUP:
                    keys.handleKey(e.key.keysym.sym, false);
                    break;
                }
            }
        }
        uint32_t lastCpuCycles = 0;
        if (io.getMustHaltCpu()) {
            if (io.getIE() & io.getIF()) {
                io.clearMustHaltCpu();
            }
            // TODO how many cycles should I count on cpu halt?
            lastCpuCycles += 1;
        } else {
            lastCpuCycles = cpu.executeNextInstruction();
        }

        cpuCycles += lastCpuCycles;
        totalCpuCycles += lastCpuCycles;
        // Render scanline if necessary cycles have been consumed
        if (cpuCycles >= 300) { // 1006?
            ppu.renderScanline(vblankNow, hblankNow);
            cpuCycles = 0;
        }

        // DMA
        dma0.runCycle(vblankNow, hblankNow);
        dma1.runCycle(vblankNow, hblankNow);
        dma2.runCycle(vblankNow, hblankNow);
        dma3.runCycle(vblankNow, hblankNow);
    }

    SDL_Quit();

    return 0;
}
