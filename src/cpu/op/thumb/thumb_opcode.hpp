#ifndef _THUMB_OPCODE_ 
#define _THUMB_OPCODE_ 

#include <iostream>
#include "../../../utils/utils.hpp"


class ThumbOpCode {
	private:
		uint16_t opcode = 0;
		
	

		// Software Interrupt
		const static uint16_t SOFTWARE_INTERRUPT_MASK = 0b1111111100000000;
		const static uint16_t SOFTWARE_INTERRUPT_FORMAT = 0b1101111100000000;

		// Add offset to stack pointer
		const static uint16_t ADD_OFFSET_TO_SP_MASK = 0b1111111100000000;
		const static uint16_t ADD_OFFSET_TO_SP_FORMAT = 0b1011000000000000;

		// Push/pop registers
		const static uint16_t PUSH_POP_REGISTERS_MASK = 0b1111011000000000;
		const static uint16_t PUSH_POP_REGISTERS_FORMAT = 0b1011010000000000;

		// ALU operations
		const static uint16_t ALU_OPERATIONS_MASK = 0b1111110000000000;
		const static uint16_t ALU_OPERATIONS_FORMAT = 0b0100000000000000;

		// Hi register operations / branch exchange
		const static uint16_t HI_REGISTER_BRANCH_EXCHANGE_MASK = 0b1111110000000000;
		const static uint16_t HI_REGISTER_BRANCH_EXCHANGE_FORMAT = 0b0100010000000000;

		// PC-relative load
		const static uint16_t PC_RELATIVE_LOAD_MASK = 0b1111100000000000;
		const static uint16_t PC_RELATIVE_LOAD_FORMAT = 0b0100100000000000;

		// Load/store with register offset
		const static uint16_t LOAD_STORE_REGISTER_OFFSET_MASK = 0b1111001000000000;
		const static uint16_t LOAD_STORE_REGISTER_OFFSET_FORMAT = 0b0101000000000000;

		// Load/store sign-extended byte/halfword
		const static uint16_t LOAD_STORE_SIGN_EXTENDED_MASK = 0b1111001000000000;
		const static uint16_t LOAD_STORE_SIGN_EXTENDED_FORMAT = 0b0101001000000000;

		// Unconditional branch
		const static uint16_t UNCONDITIONAL_BRANCH_MASK = 0b1111100000000000;
		const static uint16_t UNCONDITIONAL_BRANCH_FORMAT = 0b1110000000000000;

		// Add/subtract
		const static uint16_t ADD_SUBTRACT_MASK = 0b1111100000000000;
		const static uint16_t ADD_SUBTRACT_FORMAT = 0b0001100000000000;

		// Load/store halfword
		const static uint16_t LOAD_STORE_HALFWORD_MASK = 0b1111000000000000;
		const static uint16_t LOAD_STORE_HALFWORD_FORMAT = 0b1000000000000000;

		// SP-relative load/store
		const static uint16_t SP_LOAD_STORE_MASK = 0b1111000000000000;
		const static uint16_t SP_LOAD_STORE_FORMAT = 0b1001000000000000;

		// Load address
		const static uint16_t LOAD_ADDRESS_MASK = 0b1111000000000000;
		const static uint16_t LOAD_ADDRESS_FORMAT = 0b1010000000000000;

		// Multiple load/store
		const static uint16_t MULTIPLE_LOAD_STORE_MASK = 0b1111000000000000;
		const static uint16_t MULTIPLE_LOAD_STORE_FORMAT = 0b1100000000000000;

		// Conditional branch
		const static uint16_t CONDITIONAL_BRANCH_MASK = 0b1111000000000000;
		const static uint16_t CONDITIONAL_BRANCH_FORMAT = 0b1101000000000000;

		// Long branch with link
		const static uint16_t LONG_BRANCH_WITH_LINK_MASK = 0b1111000000000000;
		const static uint16_t LONG_BRANCH_WITH_LINK_FORMAT = 0b1111000000000000;

		// Move shifted register
		const static uint16_t MOVE_SHIFTED_REGISTER_MASK = 0b1110000000000000;
		const static uint16_t MOVE_SHIFTED_REGISTER_FORMAT = 0b0000000000000000;

		// Move/compare/add/subtract immediate
		const static uint16_t MOVE_COMP_ADD_SUB_IMM_MASK = 0b1110000000000000;
		const static uint16_t MOVE_COMP_ADD_SUB_IMM_FORMAT = 0b0010000000000000;

		// Load/store with immediate offset
		const static uint16_t LOAD_STORE_IMM_OFFSET_MASK = 0b1110000000000000;
		const static uint16_t LOAD_STORE_IMM_OFFSET_FORMAT = 0b0110000000000000;

	protected:

		ThumbOpCode(uint32_t op);
	public:
		virtual std::string toString() = 0;

		static bool isSoftwareInterrupt(uint16_t op);
		static bool isAddOffsetToSP(uint16_t op);
		static bool isPushPopRegisters(uint16_t op);
		static bool isALUOperations(uint16_t op);
		static bool isHiRegisterBranchExchange(uint16_t op);
		static bool isPCRelativeLoad(uint16_t op);
		static bool isLoadStoreRegisterOffset(uint16_t op);
		static bool isLoadStoreSignExtended(uint16_t op);
		static bool isUnconditionalBranch(uint16_t op);
		static bool isAddSubtract(uint16_t op);
		static bool isLoadStoreHalfword(uint16_t op);
		static bool isSPLoadStore(uint16_t op);
		static bool isLoadAddress(uint16_t op);
		static bool isMultipleLoadStore(uint16_t op);
		static bool isConditionalBranch(uint16_t op);
		static bool isLongBranchWithLink(uint16_t op);
		static bool isMoveShiftedRegister(uint16_t op);
		static bool isMoveCompAddSubImm(uint16_t op);
		static bool isLoadStoreImmOffset(uint16_t op);


		static std::string getRegMnemonic(uint16_t reg);

		enum OpCodeEnum {SOFTWARE_INTERRUPT, ADD_OFFSET_TO_SP, PUSH_POP_REGISTERS, ALU_OPERATIONS, HI_REGISTER_BRANCH_EXCHANGE,
							  PC_RELATIVE_LOAD, LOAD_STORE_REGISTER_OFFSET, LOAD_STORE_SIGN_EXTENDED, UNCONDITIONAL_BRANCH,
							  ADD_SUBTRACT, LOAD_STORE_HALFWORD, SP_LOAD_STORE, LOAD_ADDRESS, MULTIPLE_LOAD_STORE, CONDITIONAL_BRANCH,
							  LONG_BRANCH_WITH_LINK, MOVE_SHIFTED_REGISTER, MOVE_COMP_ADD_SUB_IMM, LOAD_STORE_IMM_OFFSET};
		OpCodeEnum _type;

};

#endif