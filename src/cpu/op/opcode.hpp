#ifndef _OPCODE_ 
#define _OPCODE_ 

class ARM7TDMI;

#include <iostream>
#include <unordered_map>
#include "../../utils/utils.hpp"
#include "fields/operand.hpp"
#include "../arm7tdmi.hpp"
#include "../../memory/memory_manager.hpp"

class OpCode {
	public:
		virtual std::string toString() = 0;
		std::string toHexString();
		bool execute();
		void decode();
		bool getMustFlushPipeline() const;
		virtual uint32_t cyclesUsed() const = 0;

		static bool isBranchAndExchange(uint32_t op);
		static bool isBlockDataTransfer(uint32_t op);
		static bool isBranch(uint32_t op);
		static bool isSoftwareInterrupt(uint32_t op);
		static bool isUndefined(uint32_t op);
		static bool isSingleDataTransfer(uint32_t op);
		static bool isSingleDataSwap(uint32_t op);
		static bool isMultiply(uint32_t op);
		static bool isMultiplyLong(uint32_t op);
		static bool isHalfwordDataTransferRegister(uint32_t op);
		static bool isHalfwordDataTransferOffset(uint32_t op);
		static bool isPSRTransferMRS(uint32_t op);
		static bool isPSRTransferMSR(uint32_t op);
		static bool isDataProcessing(uint32_t op);

		static bool checkOpCode(uint32_t op, uint32_t mask, uint32_t format);

		enum Condition { EQ = 0b0000, NE = 0b0001, CS = 0b0010, CC = 0b0011,
						 MI = 0b0100, PL = 0b0101, VS = 0b0110, VC = 0b0111,
						 HI = 0b1000, LS = 0b1001, GE = 0b1010, LT = 0b1011,
						 GT = 0b1100, LE = 0b1101, AL = 0b1110 
		};

		OpCode::Condition getCondField();
		std::string getCondFieldMnemonic();
		static std::string getRegMnemonic(uint16_t reg);

	protected:
		OpCode(uint32_t op, ARM7TDMI &cpu);
		ARM7TDMI &cpu;
		bool mustFlushPipeline = false;

	private:
		virtual void doExecute() = 0;
		virtual void doDecode() = 0;

		uint32_t opcode = 0;
		uint16_t condRaw = 0;

		Condition cond;

		std::unordered_map<Condition, std::string> condCode2Suffix = {
            {EQ, "eq"},	// Z set, equal
            {NE, "ne"}, // Z clear, not equal
            {CS, "cs"}, // C set, unsigned higher or same
            {CC, "cc"}, // C clear, unsigned lower
            {MI, "mi"}, // N set, negative
            {PL, "pl"}, // N clear, positive or zero
            {VS, "vs"}, // V set, overflow
			{VC, "vc"}, // V clear, no overflow
			{HI, "hi"}, // C set and Z clear, unsigned higher
			{LS, "ls"}, // C clear or Z set, unsigned lower or same
			{GE, "ge"}, // N equals V, greater or equal
			{LT, "lt"}, // N not equal to V, less than
			{GT, "gt"}, // Z clear AND (N equals V), greater than
			{LE, "le"}, // Z set OR (N not equal to V), less than or equal
			{AL, ""}  // (ignored), always
        }; 
		static const std::string reg2Mnemonic[16];

				
		// Branch and Exchange
		const static uint32_t BRANCH_EXCHANGE_MASK = 0b00001111111111111111111111110000;
		const static uint32_t BRANCH_EXCHANGE_FORMAT = 0b00000001001011111111111100010000;

		// Block Data Transfer
		const static uint32_t BLOCK_DATA_TRANSFER_MASK = 0b00001110000000000000000000000000;
		const static uint32_t BLOCK_DATA_TRANSFER_FORMAT = 0b00001000000000000000000000000000;

		// Branch
		const static uint32_t BRANCH_MASK = 0b00001110000000000000000000000000;
		const static uint32_t BRANCH_FORMAT = 0b00001010000000000000000000000000;

		// Software Interrupt
		const static uint32_t SOFTWARE_INTERRUPT_MASK = 0b00001111000000000000000000000000;
		const static uint32_t SOFTWARE_INTERRUPT_FORMAT = 0b00001111000000000000000000000000;

		// Undefined
		const static uint32_t UNDEFINED_MASK = 0b00001110000000000000000000010000;
		const static uint32_t UNDEFINED_FORMAT = 0b00000110000000000000000000010000;

		// Single Data Transfer
		const static uint32_t SINGLE_DATA_TRANSFER_MASK = 0b00001100000000000000000000000000;
		const static uint32_t SINGLE_DATA_TRANSFER_FORMAT = 0b00000100000000000000000000000000;

		// Single Data Swap
		const static uint32_t SINGLE_DATA_SWAP_MASK = 0b00001111101100000000111111110000;
		const static uint32_t SINGLE_DATA_SWAP_FORMAT = 0b00000001000000000000000010010000;

		// Multiply
		const static uint32_t MULTIPLY_MASK = 0b00001111110000000000000011110000;
		const static uint32_t MULTIPLY_FORMAT = 0b00000000000000000000000010010000;

		// Multiply Long
		const static uint32_t MULTIPLY_LONG_MASK = 0b00001111100000000000000011110000;
		const static uint32_t MULTIPLY_LONG_FORMAT = 0b00000000100000000000000010010000;

		// Halfword Data Transfer: register offset
		const static uint32_t HALFWORD_DATA_TRANSFER_REG_MASK = 0b00001110010000000000111110010000;
		const static uint32_t HALFWORD_DATA_TRANSFER_REG_FORMAT = 0b00000000000000000000000010010000;

		// Halfword Data Transfer: immediate offset
		const static uint32_t HALFWORD_DATA_TRANSFER_IMM_MASK = 0b00001110010000000000000010010000;
		const static uint32_t HALFWORD_DATA_TRANSFER_IMM_FORMAT = 0b00000000010000000000000010010000;

		// PSR Transfer MRS
		const static uint32_t PSR_TRANSFER_MRS_MASK = 0b00001111101111110000000000000000;
		const static uint32_t PSR_TRANSFER_MRS_FORMAT = 0b00000001000011110000000000000000;

		// PSR Transfer MSR
		const static uint32_t PSR_TRANSFER_MSR_MASK = 0b00001101101100001111000000000000;
		const static uint32_t PSR_TRANSFER_MSR_FORMAT = 0b00000001001000001111000000000000;

		// Data Processing
		const static uint32_t DATA_PROCESSING_MASK = 0b00001100000000000000000000000000;
		const static uint32_t DATA_PROCESSING_FORMAT = 0b00000000000000000000000000000000;

		const static uint32_t COND_FIELD_MASK = 0b11110000000000000000000000000000;
		const static uint32_t COND_FIELD_SHIFT = 28;

		const static uint32_t BRANCH = 0b00001010000000000000000000000000;
		const static uint32_t BRANCH_OFFSET = 0b00000000111111111111111111111111;
		const static uint32_t BRANCH_LINK =   0b00000001000000000000000000000000;
		const static uint32_t BRANCH_LINK_SHIFT = 24;

};

#endif