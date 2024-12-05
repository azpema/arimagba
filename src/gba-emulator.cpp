#include "gba-emulator.h"
#include "cpu/arm7tdmi.hpp"
#include "utils/utils.hpp"
#include "graphics/ppu.hpp"
#include "memory/keys.hpp"
#include <bitset>
#include <filesystem>

void decodeAllInstructionsThumb(ARM7TDMI &cpu);
void decodeAllInstructionsArm(ARM7TDMI &cpu);

int main(int argc, char** argv)
{
    std::cout << "Current path is " << std::filesystem::current_path() << '\n';

	std::string gamePath = "../files/retAddr.gba";
	if(argc >= 2){
		gamePath = std::string(argv[1]);
	}

	BIOS bios("../files/bios.bin");
	GamePak gamepak(gamePath);
	VRAM vram;
	EWRAM ewram;
	IWRAM iwram;
	SRAM sram;
	OAM oam;
	PaletteRAM paletteram;
	IOregisters io;
	MemoryManager mem(bios, gamepak, vram, ewram, iwram, sram, oam, paletteram, io);

	Keys keys(&mem);
	ARM7TDMI cpu(&mem);
	PPU ppu("GBA", cpu, &mem);
	cpu.setPC(0x08000000);
	
	bool run = true;

	uint32_t cpuCycles = 0;
	uint64_t totalCpuCycles = 0;

	while(run){
		// Handle events
		if(cpuCycles % 100 == 0){
			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				switch(e.type){
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
		uint32_t lastCpuCycles;
		if(io.getMustHaltCpu()){
			if(io.getIE() & io.getIF()){
				io.clearMustHaltCpu();
			}
			// TODO how many cycles should I count on cpu halt?
			lastCpuCycles += 1;
		}else{
			lastCpuCycles = cpu.executeNextInstruction();
		}
		 
		cpuCycles += lastCpuCycles;
		totalCpuCycles += lastCpuCycles;
		// Render scanline if necessary cycles have been consumed
		if(cpuCycles >= 500){ // 1006?
			ppu.renderScanline();
			cpuCycles = 0;
		}

	}

	SDL_Quit();

	return 0;
}
