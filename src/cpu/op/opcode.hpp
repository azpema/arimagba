#ifndef _OPCODE_ 
#define _OPCODE_ 

class ARM7TDMI;

#include <iostream>

class OpCode {
	public:
		virtual ~OpCode() {};
		virtual void init(uint32_t op);
		virtual std::string toString() = 0;
		std::string toHexString();
		virtual bool execute() = 0;
		virtual void decode() = 0;
		virtual uint32_t cyclesUsed() const = 0;

		static bool checkOpCode(uint32_t op, uint32_t mask, uint32_t format);

		static std::string getRegMnemonic(uint16_t reg);

		void setRawVal(uint32_t val);
		uint32_t getRawVal();

	protected:
		OpCode(uint32_t op, ARM7TDMI &cpu);
		OpCode(ARM7TDMI &cpu);

		ARM7TDMI &cpu;
		
		uint32_t opcode = 0;

	private:
		const static std::string reg2Mnemonic[16];
};

#endif