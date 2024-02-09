#include "arm7tdmi.hpp"
#include "op/data_processing.hpp"
#include "op/branch.hpp"
#include "op/branch_and_exchange.hpp"
#include "op/single_data_transfer.hpp"
#include "op/block_data_transfer.hpp"
#include "op/psr_transfer_mrs.hpp"
#include "op/psr_transfer_msr.hpp"
#include "op/psr_transfer_msr_full.hpp"
// TODO
//#include "op/psr_transfer_flag_bits.hpp"
#include "op/undefined.hpp"
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
		return new Undefined(op);
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
		if(PSRTransferMSR::isFullTransfer(op)){
			return new PSRTransferMSRFull(op);
		}else if(PSRTransferMSR::isFlagBitsTransfer(op)){
			//return new PSRTransferMSRFlagBits(op);
		}else{
			std::cerr << "ERROR: Unrecognized PSR Transfer MSR format" << std::endl;
		}
	}else if(OpCode::isDataProcessing(op)){
		return new DataProcessing(op);
	}else{
		//std::cout << "ERROR: Unrecognized instruction" << std::endl;
		return nullptr;
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