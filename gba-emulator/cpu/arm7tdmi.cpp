#include "arm7tdmi.hpp"
#include "op/data_processing.hpp"
#include "op/branch.hpp"
#include "op/branch_and_exchange.hpp"
#include "op/single_data_transfer.hpp"
#include "op/block_data_transfer.hpp"
#include "op/psr_transfer_mrs.hpp"
#include "op/psr_transfer_msr.hpp"
#include <iostream>

ARM7TDMI::ARM7TDMI() {

}

OpCode* ARM7TDMI::decodeInstructionARM(uint32_t op) {
	if(OpCode::isBranchAndExchange(op)){
		return new BranchAndExchange(op);
	}else if (OpCode::isBlockDataTransfer(op)) {
		return new BlockDataTransfer(op);
	}else if(OpCode::isBranch(op)){
		return new Branch(op);
	}else if(OpCode::isSoftwareInterrupt(op)){
		std::cout << "swi" << std::endl;
	}else if(OpCode::isUndefined(op)){
		std::cout << "undef" << std::endl;
	}else if(OpCode::isSingleDataTransfer(op)){
		return new SingleDataTransfer(op);
	}else if(OpCode::isSingleDataSwap(op)){
		std::cout << "swp" << std::endl;
	}else if(OpCode::isMultiply(op)){
		std::cout << "mul/mla" << std::endl;
	}else if(OpCode::isMultiplyLong(op)){
		std::cout << "mull/mlal" << std::endl;
	}else if(OpCode::isHalfwordDataTransferRegister(op)){
		std::cout << "@nose1" << std::endl;
	}else if(OpCode::isHalfwordDataTransferImmediate(op)){
		std::cout << "@nose2" << std::endl;
	}else if(OpCode::isPSRTransferMRS(op)){
		return new PSRTransferMRS(op);
	}else if(OpCode::isPSRTransferMSR(op)){
		return new PSRTransferMSR(op);
	}else if(OpCode::isDataProcessing(op)){
		return new DataProcessing(op);
	}else{
		std::cout << "ERROR: Unrecognized instruction" << std::endl;
	}
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