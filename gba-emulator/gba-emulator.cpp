// gba-emulator.cpp: define el punto de entrada de la aplicación.
//

#include "gba-emulator.h"
#include "cpu/arm7tdmi.hpp"
#include "utils/utils.hpp"
#include "graphics/ppu.hpp"
#include <bitset>

void decodeAllInstructionsThumb(ARM7TDMI &cpu);
void decodeAllInstructionsArm(ARM7TDMI &cpu);

int main(int argc, char** argv)
{
	/*
	GamePak gp = GamePak();
	gp.setFileStream("C:\\Users\\Markel\\Downloads\\shinchan.gba");
	gp.readHeader();
	gp.printHeader();
	*/

	/*uint32_t insCount = 8000;
	BIOS bios = BIOS("files/bios.bin");
	int64_t ins;
	int pc = 0;
	ins = bios.readWord(pc);
	while(ins != -1){
		op = cpu.decodeInstructionARM(ins, pc);
		std::cout << Utils::toHexString(pc, 8) << "     ";
		std::cout << Utils::toHexString(ins, 8) << "     ";
		if(op != nullptr){
			std::cout << op->toString() << std::endl;
			delete op;
		}else{
			std::cout << "Unrecognized" << std::endl;
		}

		pc += 4;
		ins = bios.readWord(pc);
		std::cout << ins << std::endl;
	}
	std::cout << ins << std::endl;*/

	/*ThumbOpCode* thumbOp;
	
	uint32_t insCount = 8000;
	BIOS bios = BIOS("files/bios.bin");
	uint16_t ins;
	int pc = 0;
	ins = bios.readHalfWord(pc);
	for(size_t i=0; i<insCount; i++){
		std::cout << Utils::toHexString(pc, 4) << "     ";
		std::cout << Utils::toHexString(ins, 4) << "     ";

		thumbOp = cpu.decodeInstructionThumb(ins, pc);

		if(thumbOp != nullptr){
				// Special case: Only THUMB instruction composed of 2 
			if(thumbOp->_type == ThumbOpCode::OpCodeEnum::LONG_BRANCH_WITH_LINK){

			}else{
				std::cout << thumbOp->toString() << std::endl;
			}
			
			delete thumbOp;
		}else{
			std::cout << "nola??" << std::endl;
		}

		pc += 2;
		ins = bios.readHalfWord(pc);

	}*/

	//decodeAllInstructionsThumb(cpu);
	
	//decodeAllInstructionsArm(cpu);

	/*uint32_t op1, op2;
	op1 = 0b11111111111111111111111111111111;
	op2 = 1;
	ALU alu;

	uint32_t res = alu.add(op1, op2);
	std::cout << " n" << alu.getN() << " z" << alu.getZ() << " c" << alu.getC() << " v" << alu.getV() << " " << std::endl;

	op1 = 0b01111111111111111111111111111111;
	op2 = 2;
	res = alu.add(op1, op2);
	std::cout << " n" << alu.getN() << " z" << alu.getZ() << " c" << alu.getC() << " v" << alu.getV() << " " << std::endl;


	op1 = 0b010000000000000000000000000000001;
	op2 = 0b010000000000000000000000000000001;
	res = alu.add(op1, op2);
	std::cout << " n" << alu.getN() << " z" << alu.getZ() << " c" << alu.getC() << " v" << alu.getV() << " " << std::endl;
	*/


	MemoryManager mem("files/bios.bin", "files/panda.gba");
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
		if(i % 1000 == 0)
			ppu.renderScanline();

	}

	SDL_Quit();

	return 0;
}



void decodeAllInstructionsArm(ARM7TDMI &cpu){
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
}

void decodeAllInstructionsThumb(ARM7TDMI &cpu){
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
