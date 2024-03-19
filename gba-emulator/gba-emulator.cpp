// gba-emulator.cpp: define el punto de entrada de la aplicación.
//

#include "gba-emulator.h"
#include "gamepak.hpp"
#include "cpu/arm7tdmi.hpp"
#include "utils/utils.hpp"
#include "bios.hpp"
#include <bitset>

void decodeAllInstructionsThumb(ARM7TDMI &cpu);
void decodeAllInstructionsArm(ARM7TDMI &cpu);

int main()
{
	/*
	GamePak gp = GamePak();
	gp.setFileStream("C:\\Users\\Markel\\Downloads\\shinchan.gba");
	gp.readHeader();
	gp.printHeader();
	*/

	ARM7TDMI cpu = ARM7TDMI();


	OpCode* op;

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

	cpu.executionLoop();

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
