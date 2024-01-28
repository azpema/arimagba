#ifndef _OPCODE_ 
#define _OPCODE_ 

#include <iostream>
#include <unordered_map>

class OpCode {
	private:
		uint32_t opcode = 0;
		uint16_t cond = 0;
	public:
		OpCode(uint32_t op);
		virtual std::string toString() = 0;
		
		// Branch and Exchange
		const static uint32_t BRANCH_EXCHANGE_MASK = 0b00001111111111111111111111110000;
		const static uint32_t BRANCH_EXCHANGE_FORMAT = 0b00000001001011111111111100010000;
		static bool isBranchAndExchange(uint32_t op);

		// Block Data Transfer
		const static uint32_t BLOCK_DATA_TRANSFER_MASK = 0b00001110000000000000000000000000;
		const static uint32_t BLOCK_DATA_TRANSFER_FORMAT = 0b00001000000000000000000000000000;
		static bool isBlockDataTransfer(uint32_t op);

		// Branch
		const static uint32_t BRANCH_MASK = 0b00001110000000000000000000000000;
		const static uint32_t BRANCH_FORMAT = 0b00001010000000000000000000000000;
		static bool isBranch(uint32_t op);

		// Software Interrupt
		const static uint32_t SOFTWARE_INTERRUPT_MASK = 0b00001111000000000000000000000000;
		const static uint32_t SOFTWARE_INTERRUPT_FORMAT = 0b00001111000000000000000000000000;
		static bool isSoftwareInterrupt(uint32_t op);

		// Undefined
		const static uint32_t UNDEFINED_MASK = 0b00001110000000000000000000010000;
		const static uint32_t UNDEFINED_FORMAT = 0b00000110000000000000000000010000;
		static bool isUndefined(uint32_t op);

		// Single Data Transfer
		const static uint32_t SINGLE_DATA_TRANSFER_MASK = 0b00001100000000000000000000000000;
		const static uint32_t SINGLE_DATA_TRANSFER_FORMAT = 0b00000100000000000000000000000000;
		static bool isSingleDataTransfer(uint32_t op);

		// Single Data Swap
		const static uint32_t SINGLE_DATA_SWAP_MASK = 0b00001111101100000000111111110000;
		const static uint32_t SINGLE_DATA_SWAP_FORMAT = 0b00000001000000000000000010010000;
		static bool isSingleDataSwap(uint32_t op);

		// Multiply
		const static uint32_t MULTIPLY_MASK = 0b00001111110000000000000011110000;
		const static uint32_t MULTIPLY_FORMAT = 0b00000000000000000000000010010000;
		static bool isMultiply(uint32_t op);

		// Multiply Long
		const static uint32_t MULTIPLY_LONG_MASK = 0b00001111100000000000000011110000;
		const static uint32_t MULTIPLY_LONG_FORMAT = 0b00000000100000000000000010010000;
		static bool isMultiplyLong(uint32_t op);

		// Halfword Data Transfer: register offset
		const static uint32_t HALFWORD_DATA_TRANSFER_REG_MASK = 0b00001110010000000000111110010000;
		const static uint32_t HALFWORD_DATA_TRANSFER_REG_FORMAT = 0b00000000000000000000000010010000;
		static bool isHalfwordDataTransferRegister(uint32_t op);

		// Halfword Data Transfer: immediate offset
		const static uint32_t HALFWORD_DATA_TRANSFER_IMM_MASK = 0b00001110010000000000000010010000;
		const static uint32_t HALFWORD_DATA_TRANSFER_IMM_FORMAT = 0b00000000010000000000000010010000;
		static bool isHalfwordDataTransferImmediate(uint32_t op);

		// PSR Transfer MRS
		const static uint32_t PSR_TRANSFER_MRS_MASK = 0b00001111101111110000000000000000;
		const static uint32_t PSR_TRANSFER_MRS_FORMAT = 0b00000001000011110000000000000000;
		static bool isPSRTransferMRS(uint32_t op);

		// PSR Transfer MSR
		const static uint32_t PSR_TRANSFER_MSR_MASK = 0b00001101101100001111000000000000;
		const static uint32_t PSR_TRANSFER_MSR_FORMAT = 0b00000001001000001111000000000000;
		static bool isPSRTransferMSR(uint32_t op);

		// Data Processing
		const static uint32_t DATA_PROCESSING_MASK = 0b00001100000000000000000000000000;
		const static uint32_t DATA_PROCESSING_FORMAT = 0b00000000000000000000000000000000;
		static bool isDataProcessing(uint32_t op);

		const static uint32_t COND_FIELD_MASK = 0b11110000000000000000000000000000;
		const static uint32_t COND_FIELD_SHIFT = 28;

		static bool checkOpCode(uint32_t op, uint32_t mask, uint32_t format);
		uint32_t getCondField();
		std::string getCondFieldMnemonic();
		std::unordered_map<uint32_t, std::string> condCode2Suffix = {
            {0b0000, "eq"},	// Z set, equal
            {0b0001, "ne"}, // Z clear, not equal
            {0b0010, "cs"}, // C set, unsigned higher or same
            {0b0011, "cc"}, // C clear, unsigned lower
            {0b0100, "mi"}, // N set, negative
            {0b0101, "pl"}, // N clear, positive or zero
            {0b0110, "vs"}, // V set, overflow
			{0b0111, "vc"}, // V clear, no overflow
			{0b1000, "hi"}, // C set and Z clear, unsigned higher
			{0b1001, "ls"}, // C clear or Z set, unsigned lower or same
			{0b1010, "ge"}, // N equals V, greater or equal
			{0b1011, "lt"}, // N not equal to V, less than
			{0b1100, "gt"}, // Z clear AND (N equals V), greater than
			{0b1101, "le"}, // Z set OR (N not equal to V), less than or equal
			{0b1110, ""}  // (ignored), always
        }; 

		

		const static uint32_t BRANCH = 0b00001010000000000000000000000000;
		const static uint32_t BRANCH_OFFSET = 0b00000000111111111111111111111111;
		const static uint32_t BRANCH_LINK =   0b00000001000000000000000000000000;
		const static uint32_t BRANCH_LINK_SHIFT = 24;


};

#endif