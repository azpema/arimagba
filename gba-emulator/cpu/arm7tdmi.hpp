#include <iostream>
#include "registers/cpsr.hpp"
#include "op/opcode.hpp"

class ARM7TDMI {
	public:
		ARM7TDMI();

	private:
		/*
		The ARM7TDMI processor has a total of 37 registers:
			- 31 general-purpose 32-bit registers:
				* r0-r15
				* r8_fiq-r14_fiq
				* r13_svc-r14_svc
				* r13_abt-r14_abt
				* r13_irq-r14_irq
				* r13_und-r14_und
			- 6 status registers:
				* CPSR
				* SPSR_fiq
				* SPSR_svc
				* SPSR_abt
				* SPSR_irq
				* SPSR_und

		Registers r0 to r13 are general-purpose registers used to hold either data or address values.
		Registers r14 and r15 have the following special functions:
		 
		Register 14 is used as the subroutine Link Register (LR)

		Register 15 holds the PC
		In ARM state, bits [1:0] of r15 are undefined and must be ignored.
		Bits [31:2] contain the PC.
		In Thumb state, bit [0] is undefined and must be ignored. Bits
		[31:1] contain the PC.

		By convention, r13 is used as the Stack Pointer (SP).
		*/
		uint32_t registers[16] = { 0 };



		uint32_t SPSR = 0;
		// Program Status Register
		CPSR cpsr = CPSR();

	public:
		OpCode* decodeInstructionARM(uint32_t op);

		uint32_t getCPSR();
		void setNFlag(bool val);
		void setZFlag(bool val);
		void setCFlag(bool val);
		void setVFlag(bool val);
		void setMode(CPSR::Mode mode);
		std::string getModeString();

};