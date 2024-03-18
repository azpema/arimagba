#include "arm7tdmi.hpp"


#include "op/arm/data_processing.hpp"
#include "op/arm/branch.hpp"
#include "op/arm/branch_and_exchange.hpp"
#include "op/arm/single_data_transfer.hpp"
#include "op/arm/block_data_transfer.hpp"
#include "op/arm/psr_transfer_mrs.hpp"
#include "op/arm/psr_transfer_msr.hpp"
#include "op/arm/psr_transfer_msr_full.hpp"
#include "op/arm/psr_transfer_msr_flag_bits.hpp"
#include "op/arm/undefined.hpp"
#include "op/arm/software_interrupt.hpp"
#include "op/arm/multiply_accumulate.hpp"
#include "op/arm/multiply_accumulate_long.hpp"
#include "op/arm/single_data_swap.hpp"
#include "op/arm/halfword_data_transfer_register.hpp"
#include "op/arm/halfword_data_transfer_offset.hpp"

// Thumb
#include "op/thumb/move_comp_add_sub_imm.hpp"
#include "op/thumb/move_shifted_register.hpp"
#include "op/thumb/add_subtract.hpp"
#include "op/thumb/alu_operations.hpp"
#include "op/thumb/hi_register_branch_exchange.hpp"
#include "op/thumb/pc_relative_load.hpp"
#include "op/thumb/load_store_register_offset.hpp"
#include "op/thumb/push_pop_registers.hpp"
#include "op/thumb/add_offset_sp.hpp"
#include "op/thumb/sp_load_store.hpp"
#include "op/thumb/load_address.hpp"
#include "op/thumb/load_store_sign_extended.hpp"
#include "op/thumb/load_store_halfword.hpp"
#include "op/thumb/load_store_imm_offset.hpp"
#include "op/thumb/multiple_load_store.hpp"
#include "op/thumb/software_interrupt.hpp"
#include "op/thumb/unconditional_branch.hpp"
#include "op/thumb/conditional_branch.hpp"
#include "op/thumb/long_branch_with_link.hpp"

#include <iostream>

ARM7TDMI::ARM7TDMI() {
	spsr = 0;
	cpsr = CPSR();
	bios = BIOS(std::string("files/bios.bin"));
	for(size_t i=0; i<16; i++){
		reg[i] = 0;
	}
}

OpCode* ARM7TDMI::decodeInstructionARM(uint32_t op, uint32_t pc) {
	if(OpCode::isBranchAndExchange(op)){
		return new BranchAndExchange(op);
	}else if (OpCode::isBlockDataTransfer(op)) {
		return new BlockDataTransfer(op);
	}else if(OpCode::isBranch(op)){
		return new ARM::Branch(op, pc);
	}else if(OpCode::isSoftwareInterrupt(op)){
		return new ARM::SoftwareInterrupt(op);
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
		return new Thumb::SoftwareInterrupt(op);
	}else if (ThumbOpCode::isAddOffsetToSP(op)) {
		return new AddOffsetSP(op);
	}else if(ThumbOpCode::isPushPopRegisters(op)) {
		return new PushPopRegisters(op);
	}else if(ThumbOpCode::isALUOperations(op)) {
		return new ALUOperations(op);
	}else if(ThumbOpCode::isHiRegisterBranchExchange(op)) {
		return new HiRegisterBranchExchange(op);
	}else if(ThumbOpCode::isPCRelativeLoad(op)) {
		return new PCRelativeLoad(op);
	}else if(ThumbOpCode::isLoadStoreRegisterOffset(op)) {
		return new LoadStoreRegisterOffset(op);
	}else if(ThumbOpCode::isLoadStoreSignExtended(op)) {
		return new LoadStoreSignExtended(op);
	}else if(ThumbOpCode::isUnconditionalBranch(op)) {
		return new Thumb::UnconditionalBranch(op, pc);
	}else if(ThumbOpCode::isAddSubtract(op)) {
		return new AddSubtract(op);
	}else if(ThumbOpCode::isLoadStoreHalfword(op)) {
		return new LoadStoreHalfword(op);
	}else if(ThumbOpCode::isSPLoadStore(op)) {
		return new SPLoadStore(op);
	}else if(ThumbOpCode::isLoadAddress(op)) {
		return new LoadAddress(op);
	}else if(ThumbOpCode::isMultipleLoadStore(op)) {
		return new MultipleLoadStore(op);
	}else if(ThumbOpCode::isConditionalBranch(op)) {
		return new Thumb::ConditionalBranch(op, pc);
	}else if(ThumbOpCode::isLongBranchWithLink(op)) {
		return new Thumb::LongBranchWithLink(op, pc);
	}else if(ThumbOpCode::isMoveShiftedRegister(op)) {
		return new MoveShiftedRegister(op);
	}else if(ThumbOpCode::isMoveCompAddSubImm(op)) {
		return new MoveCompAddSubImm(op);
	}else if(ThumbOpCode::isLoadStoreImmOffset(op)) {
		return new LoadStoreImmOffset(op);
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

int64_t ARM7TDMI::fetchInstructionThumb(uint32_t offset){
	return bios.readHalfWord(offset);
}

int64_t ARM7TDMI::fetchInstructionArm(uint32_t offset){
	return bios.readWord(offset);
}

/*void ARM7TDMI::fetchNextInstruction(){
	uint32_t ins;
	if(cpsr.isThumbMode()){
		ins = bios.readHalfWord(reg[REG_PC]);
		reg[REG_PC] += 2;
	}else{
		ins = bios.readWord(reg[REG_PC]);
		reg[REG_PC] += 4;
	}
}*/

uint32_t ARM7TDMI::getPC(){
	return reg[15];
}

void ARM7TDMI::setPC(uint32_t pc){
	reg[15] = pc;
}

void ARM7TDMI::setLR(uint32_t lr){
	reg[14] = lr;
}

void ARM7TDMI::printStatus(){
	std::cout << "<<<" << std::endl;
	std::cout << "pc:   " << Utils::toHexString(getPC(), 8) << std::endl;
	std::cout << "cpsr: " << Utils::toHexString(cpsr.getValue(), 8) << std::endl;
	std::cout << "<<<" << std::endl;
}

/*void ARM7TDMI::executeOpArm(OpCode *op){
	op->execute(); 
}*/

void ARM7TDMI::executionLoop(){
	uint64_t ins;
	OpCode *op;
	int i=0;
	while(i<2){
		if(!cpsr.isThumbMode()){
			// fetch
			ins = bios.readWord(reg[15]);
			if(ins == -1) return;

			// decode
			op = decodeInstructionARM(ins, reg[15]);
			std::cout << op->toString() << std::endl;

			// execute
			printStatus();
			op->execute(*this);
		}else{

		}
		i++;
	}
	
}