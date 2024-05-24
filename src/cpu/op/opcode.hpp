#ifndef _OPCODE_ 
#define _OPCODE_ 

class ARM7TDMI;

#include <iostream>
#include "../../utils/utils.hpp"
#include "../arm7tdmi.hpp"
#include "../../memory/memory_manager.hpp"

class OpCode {
	public:
		virtual ~OpCode() {};
		virtual std::string toString() = 0;
		std::string toHexString();
		virtual bool execute() = 0;
		virtual void decode() = 0;
		virtual uint32_t cyclesUsed() const = 0;

		static bool checkOpCode(uint32_t op, uint32_t mask, uint32_t format);

		static std::string getRegMnemonic(uint16_t reg);

	protected:
		OpCode(uint32_t op, ARM7TDMI &cpu);
		ARM7TDMI &cpu;
		
		uint32_t opcode = 0;

	private:
		const static std::string reg2Mnemonic[16];
};

#endif