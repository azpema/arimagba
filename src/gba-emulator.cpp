#include "gba-emulator.h"
#include "cpu/arm7tdmi.hpp"
#include "utils/utils.hpp"
#include "graphics/ppu.hpp"
#include <bitset>
#include <filesystem>

void decodeAllInstructionsThumb(ARM7TDMI &cpu);
void decodeAllInstructionsArm(ARM7TDMI &cpu);

int main(int argc, char** argv)
{
    std::cout << "Current path is " << std::filesystem::current_path() << '\n';

	//std::string gamePath = "../files/FuzzARM.gba";
	std::string gamePath = "/home/markel/Descargas/libtonc-examples/ext/swi_demo/swi_demo.gba";
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
	ARM7TDMI cpu(&mem); 
	cpu.setPC(0x08000000);
	
	bool run = true;

	int i = 1;
	while(run){

		// Handle events
		if(i % 100 == 0){
			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) run = false;
			}
		}

		cpu.executeNextInstruction();
		i++;
		// Render scanline if necessary cycles have been consumed
		if(i % 500 == 0)
			ppu.renderScanline();

	}

	SDL_Quit();

	return 0;
}



/*void decodeAllInstructionsArm(ARM7TDMI &cpu){
	OpCode* op;
	int64_t ins;
	int pc = 0;

	while((ins = cpu.fetchInstructionArm(pc)) != -1){
		if(ins == -1){
			return;
		}
		std::cout << Utils::toHexString(pc, 4) << "     ";
		std::cout << Utils::toHexString(ins, 8) << "     ";

		if(pc == 0x31c){
			std::cout << "HEMEN" << std::endl;
		}

		op = cpu.decodeInstructionARM(ins, pc);

		if(op != nullptr){
			std::cout << op->toString() << std::endl;
			delete op;
		} else{
			std::cout << "Unknown" << std::endl;
		}

		pc += 4;
	}
}*/

/*void decodeAllInstructionsThumb(ARM7TDMI &cpu){
	ThumbOpCode* thumbOp;
	int64_t ins;
	int pc = 0;

	while((ins = cpu.fetchInstructionThumb(pc)) != -1){
		if(ins == -1){
			return;
		}
		std::cout << Utils::toHexString(pc, 4) << "     ";
		std::cout << Utils::toHexString(ins, 4) << "     ";

		if(pc == 0x2B4){
			std::cout << "HEMEN" << std::endl;
		}

		thumbOp = cpu.decodeInstructionThumb(ins, pc);

		if(thumbOp != nullptr){
			// Special case: Only THUMB instruction composed of 2 
			if(thumbOp->_type == ThumbOpCode::OpCodeEnum::LONG_BRANCH_WITH_LINK){
				std::cout << "NOSE" << std::endl;
			}else{
				std::cout << thumbOp->toString() << std::endl;
			}
			
			delete thumbOp;
		} else{
			std::cout << "Unknown" << std::endl;
		}

		pc += 2;
	}
}
*/