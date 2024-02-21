#include "arm7tdmi.hpp"
#include "op/data_processing.hpp"
#include "op/branch.hpp"
#include "op/branch_and_exchange.hpp"
#include "op/single_data_transfer.hpp"
#include "op/block_data_transfer.hpp"
#include "op/psr_transfer_mrs.hpp"
#include "op/psr_transfer_msr.hpp"
#include "op/psr_transfer_msr_full.hpp"
#include "op/psr_transfer_msr_flag_bits.hpp"
#include "op/undefined.hpp"
#include "op/software_interrupt.hpp"
#include "op/multiply_accumulate.hpp"
#include "op/multiply_accumulate_long.hpp"
#include "op/single_data_swap.hpp"
#include "op/halfword_data_transfer_register.hpp"
#include "op/halfword_data_transfer_offset.hpp"
// Thumb
#include "op/thumb/move_comp_add_sub_imm.hpp"

#include <iostream>

ARM7TDMI::ARM7TDMI() {

}

OpCode* ARM7TDMI::decodeInstructionARM(uint32_t op, uint32_t pc) {
	if(OpCode::isBranchAndExchange(op)){
		return new BranchAndExchange(op);
	}else if (OpCode::isBlockDataTransfer(op)) {
		return new BlockDataTransfer(op);
	}else if(OpCode::isBranch(op)){
		return new Branch(op, pc);
	}else if(OpCode::isSoftwareInterrupt(op)){
		return new SoftwareInterrupt(op);
	}else if(OpCode::isUndefined(op)){
		return new Undefined(op);
	}else if(OpCode::isSingleDataTransfer(op)){
		return new SingleDataTransfer(op);
	}else if(OpCode::isSingleDataSwap(op)){
		return new SingleDataSwap(op);
	}else if(OpCode::isMultiply(op)){
		return new MultiplyAccumulate(op);
	}else if(OpCode::isMultiplyLong(op)){
		return new MultiplyAccumulateLong(op);
	}else if(OpCode::isHalfwordDataTransferRegister(op)){
		return new HalfwordDataTransferRegister(op);
	}else if(OpCode::isHalfwordDataTransferOffset(op)){
		return new HalfwordDataTransferOffset(op);
	}else if(OpCode::isPSRTransferMRS(op)){
		return new PSRTransferMRS(op);
	}else if(OpCode::isPSRTransferMSR(op)){
		if(PSRTransferMSR::isFullTransfer(op)){
			return new PSRTransferMSRFull(op);
		}else if(PSRTransferMSR::isFlagBitsTransfer(op)){
			return new PSRTransferMSRFlagBits(op);
		}else{
			std::cerr << "ERROR: Unrecognized PSR Transfer MSR format" << std::endl;
		}
	}else if(OpCode::isDataProcessing(op)){
		return new DataProcessing(op);
	}else{
		//std::cout << "ERROR: Unrecognized instruction" << std::endl;
		return nullptr;
	}
	return nullptr;
}

ThumbOpCode* ARM7TDMI::decodeInstructionThumb(uint16_t op, uint32_t pc) {
	if(ThumbOpCode::isSoftwareInterrupt(op)){
		std::cout << "swi" << std::endl;
	}else if (ThumbOpCode::isAddOffsetToSP(op)) {
		std::cout << "add sp" << std::endl;
	}else if(ThumbOpCode::isPushPopRegisters(op)) {
		std::cout << "push/pop" << std::endl;
	}else if(ThumbOpCode::isALUOperations(op)) {
		std::cout << "alu op" << std::endl;
	}else if(ThumbOpCode::isHiRegisterBranchExchange(op)) {
		std::cout << "alu hi bx" << std::endl;
	}else if(ThumbOpCode::isPCRelativeLoad(op)) {
		std::cout << "ldr pc" << std::endl;
	}else if(ThumbOpCode::isLoadStoreRegisterOffset(op)) {
		std::cout << "ldr/str regi" << std::endl;
	}else if(ThumbOpCode::isLoadStoreSignExtended(op)) {
		std::cout << "ldr/str sign extended" << std::endl;
	}else if(ThumbOpCode::isUnconditionalBranch(op)) {
		std::cout << "b" << std::endl;
	}else if(ThumbOpCode::isAddSubtract(op)) {
		std::cout << "add/sub" << std::endl;
	}else if(ThumbOpCode::isLoadStoreHalfword(op)) {
		std::cout << "ldr/str halfword" << std::endl;
	}else if(ThumbOpCode::isSPLoadStore(op)) {
		std::cout << "ldr/str sp" << std::endl;
	}else if(ThumbOpCode::isLoadAddress(op)) {
		std::cout << "load address" << std::endl;
	}else if(ThumbOpCode::isMultipleLoadStore(op)) {
		std::cout << "ldmia/stmia" << std::endl;
	}else if(ThumbOpCode::isConditionalBranch(op)) {
		std::cout << "b cond" << std::endl;
	}else if(ThumbOpCode::isLongBranchWithLink(op)) {
		std::cout << "bl" << std::endl;
	}else if(ThumbOpCode::isMoveShiftedRegister(op)) {
		std::cout << "lsl/lsr/asr" << std::endl;
	}else if(ThumbOpCode::isMoveCompAddSubImm(op)) {
		return new MoveCompAddSubImm(op);
	}else if(ThumbOpCode::isLoadStoreImmOffset(op)) {
		std::cout << "ldr/str imm" << std::endl;
	}else{
		//std::cout << "ERROR: Unrecognized instruction" << std::endl;
		return nullptr;
	}
	return nullptr;
}

uint32_t ARM7TDMI::getCPSR(){
	return cpsr.getValue();
}

void ARM7TDMI::setNFlag(bool val){
	cpsr.setNFlag(val);
}

void ARM7TDMI::setZFlag(bool val){
	cpsr.setZFlag(val);
}

void ARM7TDMI::setCFlag(bool val){
	cpsr.setCFlag(val);
}

void ARM7TDMI::setVFlag(bool val){
	cpsr.setVFlag(val);
}

void ARM7TDMI::setMode(CPSR::Mode mode){
	cpsr.setMode(mode);
}

std::string ARM7TDMI::getModeString(){
	return cpsr.getModeString();
}