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

	//std::string gamePath = "/home/markel/Descargas/libtonc-examples/basic/key_demo/key_demo.gba";
    //std::string gamePath = "/home/markel/Descargas/libtonc-examples/basic/m3_demo/m3_demo.gba";
	//std::string gamePath = "../files/beeg.gba";
	std::string gamePath = "../files/armwrestler-gba-fixed.gba";
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

	PPU ppu("GBA", &mem);
	Keys keys(&mem);
	ARM7TDMI cpu(&mem); 
	cpu.setPC(0x08000000);
	
	bool run = true;

	int i = 1;

	while(run){
		// Handle events
		if(i % 100 == 0){
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

		cpu.executeNextInstruction();
		i++;
		// Render scanline if necessary cycles have been consumed
		if(i % 1000 == 0)
			ppu.renderScanline();

	}

	SDL_Quit();

	return 0;
}
