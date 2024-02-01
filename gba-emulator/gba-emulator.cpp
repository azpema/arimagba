// gba-emulator.cpp: define el punto de entrada de la aplicación.
//

#include "gba-emulator.h"
#include "gamepak.hpp"
#include "cpu/arm7tdmi.hpp"
#include "cpu/op/data_processing.hpp"
#include "utils/utils.hpp"
#include "bios.hpp"
#include <bitset>

int main()
{
	/*
	GamePak gp = GamePak();
	gp.setFileStream("C:\\Users\\Markel\\Downloads\\shinchan.gba");
	gp.readHeader();
	gp.printHeader();
	*/

	ARM7TDMI cpu = ARM7TDMI();
	/*
	std::cout << "CPSR: " << std::bitset<32>(cpu.getCPSR()) << std::endl;

	cpu.setNFlag(true);
	std::cout << "CPSR: " << std::bitset<32>(cpu.getCPSR()) << std::endl;

	cpu.setZFlag(true);
	std::cout << "CPSR: " << std::bitset<32>(cpu.getCPSR()) << std::endl;

	cpu.setCFlag(true);
	std::cout << "CPSR: " << std::bitset<32>(cpu.getCPSR()) << std::endl;

	cpu.setVFlag(true);
	std::cout << "CPSR: " << std::bitset<32>(cpu.getCPSR()) << std::endl;

	cpu.setZFlag(false);
	std::cout << "CPSR: " << std::bitset<32>(cpu.getCPSR()) << std::endl;

	cpu.setCFlag(false);
	std::cout << "CPSR: " << std::bitset<32>(cpu.getCPSR()) << std::endl;
	*/
	/*
	std::cout << "Mode: " << cpu.getModeString() << std::endl;

	cpu.setMode(CPSR::Mode::User);
	std::cout << "Mode: " << cpu.getModeString() << std::endl;

	OpCode op = OpCode(0xea000018);
	std::cout << "OP: " << op.getCondFieldMnemonic() << std::endl; // al
	
	op = OpCode(0x11a020a2);
	std::cout << "OP: " << op.getCondFieldMnemonic() << std::endl; // ne

	op = OpCode(0x0e32402b);
	std::cout << "OP: " << op.getCondFieldMnemonic() << std::endl; // eq
	*/

	/*
	cpu.decodeInstructionARM(0xea000018);
	cpu.decodeInstructionARM(0xe59fd1a0);
	cpu.decodeInstructionARM(0xe92d5000);
	cpu.decodeInstructionARM(0xe14fc000);
	cpu.decodeInstructionARM(0xe10fe000);
	cpu.decodeInstructionARM(0xe92d5000);
	cpu.decodeInstructionARM(0xe3a0c302);
	cpu.decodeInstructionARM(0xe5dce09c);
	cpu.decodeInstructionARM(0xe35e00a5);
	cpu.decodeInstructionARM(0x1a000004);
	std::cout << "WAIT" << std::endl;
	cpu.decodeInstructionARM(0x05dce0b4);
	cpu.decodeInstructionARM(0x021ee080);
	cpu.decodeInstructionARM(0xe28fe004);
	cpu.decodeInstructionARM(0x159ff220);
	cpu.decodeInstructionARM(0x059ff220);
	cpu.decodeInstructionARM(0xe59fd164);
	cpu.decodeInstructionARM(0xe8bd5000);
	cpu.decodeInstructionARM(0xe169f00c);
	cpu.decodeInstructionARM(0xe8bd5000);
	cpu.decodeInstructionARM(0xe25ef004);

	DataProcessing otro = DataProcessing(0xa303);

	std::cout << otro.getOpCodeMnemonic() << " " << otro.getRdMnemonic() << " #" << std::hex << otro.getOperand2Imm() << std::dec << std::endl; // add

	otro = DataProcessing(0xe35e00a5);
	std::cout << otro.getOpCodeMnemonic() << std::endl; // cmp

	otro = DataProcessing(0xe28fe004);
	std::cout << otro.getOpCodeMnemonic() << std::endl; // add

	otro = DataProcessing(0xe25ef004);
	std::cout << otro.getOpCodeMnemonic() << std::endl; // sub
	*/

	/*std::cout << "First BIOS instructions" << std::endl;
	BIOS bios = BIOS("files/bios.bin");
	uint32_t op;
	for(size_t i = 0; i < 10*4; i +=4 ){
		op = bios.readWord(i);
		std::cout << "0x" << std::hex << std::uppercase << op << std::dec << std::endl;
		cpu.decodeInstructionARM(op);
	}
	*/

	OpCode* op;

	//op = cpu.decodeInstructionARM(0xF00FF32);
	//std::cout << op->toString() << std::endl;	// cmp r14,#0xA5
	//delete op;

	/*DataProcessing otro = DataProcessing(0xe35e00a5);
	std::cout << otro.toString() << std::endl;	// cmp r14,#0xA5

	DataProcessing otro2 = DataProcessing(0x021ee080);
	std::cout << otro2.toString() << std::endl;	// andeqs r14,r14,#0x80

	DataProcessing otro3 = DataProcessing(0xe28f0001); // @0x0000014 no$gba -> add r0,=#0x11D 	mGBA -> add r0, pc, #1
	std::cout << otro3.toString() << std::endl;	// 

	DataProcessing otro4 = DataProcessing(0xe28fb078); // @0x00000148 no$gba -> add r11,=#0x1C8 == add r11, r15, #0x78
													   // #0x1C8 equals content of r15(pc)=0x0148 + 0x78 + 0x8
	std::cout << otro4.toString() << std::endl;	// */

	// op = cpu.decodeInstructionARM(0xeaefe6f6); // PC = 0x08367650 OP= 0xEAEFE6F b #0x7f61230
	// std::cout << op->toString() << std::endl;	
	// delete op;

	// op = cpu.decodeInstructionARM(0xea000018); // PC = 0x00000000 OP= 0xea000018 b #0x68 (104)
	// std::cout << op->toString() << std::endl;	
	// delete op;

	// op = cpu.decodeInstructionARM(0xe12fff10); // PC = 0x000000B0 OP= 0xe12fff10 bx r0
	// std::cout << op->toString() << std::endl;
	// delete op;

	// op = cpu.decodeInstructionARM(0xe12FFF1E); // PC = 0x000000DC OP= 0xe12FFF1E bx r14
	// std::cout << op->toString() << std::endl;	
	// delete op;

	// op = cpu.decodeInstructionARM(0xE59FD1A0); // 0000001C  E59FD1A0  ldr       sp,[pc,0x1A0]
	// std::cout << op->toString() << std::endl;	
	// delete op;

	// op = cpu.decodeInstructionARM(0xE5DCE09C); // 00000034  E5DCE09C  ldrb      lr,[r12,0x9C]
	// std::cout << op->toString() << std::endl;	
	// delete op;
	
	// op = cpu.decodeInstructionARM(0xE79BC10C); // 0000014C  E79BC10C  ldr       r12,[r11,r12,lsl 0x2]
	// std::cout << op->toString() << std::endl;	
	// delete op;

	// op = cpu.decodeInstructionARM(0x07F8D421); // 0000009C  07F8D421  ldrbeq    sp,[r8,r1,lsr 0x8]!
	// std::cout << op->toString() << std::endl;	
	// delete op;

	// op = cpu.decodeInstructionARM(0x4552554B); // 000000A0  4552554B  ldrbmi    r5,[r2,-0x54B]
	// std::cout << op->toString() << std::endl;	
	// delete op;

	op = cpu.decodeInstructionARM(0x51AEFF24); // 00000004  51AEFF24  movpl     pc,r4,lsr 0x1E
	std::cout << op->toString() << std::endl;	
	delete op;

	op = cpu.decodeInstructionARM(0x21A29A69); // 00000008  21A29A69  movcs     r9,r9,ror 0x14
	std::cout << op->toString() << std::endl;	
	delete op;

	std::unique_ptr<OpCode> op2{cpu.decodeInstructionARM(0x021ee080)};
	std::cout << op2->toString() << std::endl;	// andeqs r14,r14,#0x80

	

	return 0;
}
