#include "arm7tdmi.hpp"
#include "op/arm/arm_opcode.hpp"
#include "op/thumb/thumb_opcode.hpp"

// ARM
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

ARM7TDMI::ARM7TDMI(MemoryManager *memManager) {
	mem = memManager;
	//cpsr = PSR();
	for(size_t i=0; i<16; i++){
		reg[i] = 0;
	}

	for(size_t i=0; i<7; i++){
		r8_fiq[i] = 0;
	}

	for(size_t i=0; i<2; i++){
		r13_svc[i] = 0;
		r13_abt[i] = 0; 
		r13_irq[i] = 0; 
		r13_und[i] = 0; 
	}


	reg[0] = 0x00000CA5;
	reg[13] = 0x03007F00;
	reg[14] = 0x08000000;
	r13_irq[0] = 0x03007FA0;
	r13_svc[0] = 0x03007FE0;

	barrelShifter = new BarrelShifter();
}

ARM7TDMI::~ARM7TDMI() {
	delete barrelShifter;
}

OpCode* ARM7TDMI::decodeInstruction(uint32_t op, uint32_t pc){
	if(cpsr.isThumbMode()){
		return decodeInstructionThumb(op, pc);
	}else{
		return decodeInstructionARM(op, pc);
	}
}

OpCode* ARM7TDMI::decodeInstructionARM(uint32_t op, uint32_t pc) {
	if(ArmOpcode::isBranchAndExchange(op)){
		return new BranchAndExchange(op, *this);
	}else if (ArmOpcode::isBlockDataTransfer(op)) {
		return new BlockDataTransfer(op, *this);
	}else if(ArmOpcode::isBranch(op)){
		return new ARM::Branch(op, pc, *this);
	}else if(ArmOpcode::isSoftwareInterrupt(op)){
		return new ARM::SoftwareInterrupt(op, *this);
	}else if(ArmOpcode::isUndefined(op)){
		return new Undefined(op, *this);
	}else if(ArmOpcode::isSingleDataTransfer(op)){
		return new SingleDataTransfer(op, *this);
	}else if(ArmOpcode::isSingleDataSwap(op)){
		return new SingleDataSwap(op, *this);
	}else if(ArmOpcode::isMultiply(op)){
		return new MultiplyAccumulate(op, *this);
	}else if(ArmOpcode::isMultiplyLong(op)){
		return new MultiplyAccumulateLong(op, *this);
	}else if(ArmOpcode::isHalfwordDataTransferRegister(op)){
		return new HalfwordDataTransferRegister(op, *this);
	}else if(ArmOpcode::isHalfwordDataTransferOffset(op)){
		return new HalfwordDataTransferOffset(op, *this);
	}else if(ArmOpcode::isPSRTransferMRS(op)){
		return new PSRTransferMRS(op, *this);
	}else if(ArmOpcode::isPSRTransferMSR(op)){
		if(PSRTransferMSR::isFullTransfer(op)){
			return new PSRTransferMSRFull(op, *this);
		}else if(PSRTransferMSR::isFlagBitsTransfer(op)){
			return new PSRTransferMSRFlagBits(op, *this);
		}else{
			std::cerr << "ERROR: Unrecognized PSR Transfer MSR format" << std::endl;
		}
	}else if(ArmOpcode::isDataProcessing(op)){
		return new DataProcessing(op, *this);
	}

	std::runtime_error("ERROR: Unrecognized instruction in decodeInstructionARM");
	return nullptr;
}

OpCode* ARM7TDMI::decodeInstructionThumb(uint16_t op, uint32_t pc) {
	if(ThumbOpCode::isSoftwareInterrupt(op)){
		return new Thumb::SoftwareInterrupt(op, *this);
	}else if (ThumbOpCode::isAddOffsetToSP(op)) {
		return new AddOffsetSP(op, *this);
	}else if(ThumbOpCode::isPushPopRegisters(op)) {
		return new PushPopRegisters(op, *this);
	}else if(ThumbOpCode::isALUOperations(op)) {
		return new ALUOperations(op, *this);
	}else if(ThumbOpCode::isHiRegisterBranchExchange(op)) {
		return new HiRegisterBranchExchange(op, *this);
	}else if(ThumbOpCode::isPCRelativeLoad(op)) {
		return new PCRelativeLoad(op, *this);
	}else if(ThumbOpCode::isLoadStoreRegisterOffset(op)) {
		return new LoadStoreRegisterOffset(op, *this);
	}else if(ThumbOpCode::isLoadStoreSignExtended(op)) {
		return new LoadStoreSignExtended(op, *this);
	}else if(ThumbOpCode::isUnconditionalBranch(op)) {
		return new Thumb::UnconditionalBranch(op, pc, *this);
	}else if(ThumbOpCode::isAddSubtract(op)) {
		return new AddSubtract(op, *this);
	}else if(ThumbOpCode::isLoadStoreHalfword(op)) {
		return new LoadStoreHalfword(op, *this);
	}else if(ThumbOpCode::isSPLoadStore(op)) {
		return new SPLoadStore(op, *this);
	}else if(ThumbOpCode::isLoadAddress(op)) {
		return new LoadAddress(op, *this);
	}else if(ThumbOpCode::isMultipleLoadStore(op)) {
		return new MultipleLoadStore(op, *this);
	}else if(ThumbOpCode::isConditionalBranch(op)) {
		return new Thumb::ConditionalBranch(op, pc, *this);
	}else if(ThumbOpCode::isLongBranchWithLink(op)) {
		return new Thumb::LongBranchWithLink(op, pc, *this);
	}else if(ThumbOpCode::isMoveShiftedRegister(op)) {
		return new MoveShiftedRegister(op, *this);
	}else if(ThumbOpCode::isMoveCompAddSubImm(op)) {
		return new MoveCompAddSubImm(op, *this);
	}else if(ThumbOpCode::isLoadStoreImmOffset(op)) {
		return new LoadStoreImmOffset(op, *this);
	}

	std::runtime_error("ERROR: Unrecognized instruction in decodeInstructionARM");
	return nullptr;
}

PSR& ARM7TDMI::getCPSR(){
	return cpsr;
}

PSR& ARM7TDMI::getCorrespondingSPSR(){
	switch (cpsr.getMode())
	{
	case PSR::Mode::FIQ:
		return spsr_fiq;
	case PSR::Mode::Supervisor:
		return spsr_svc;
	case PSR::Mode::Abort:
		return spsr_abt;
	case PSR::Mode::IRQ:
		return spsr_irq;
	case PSR::Mode::Undefined:
		return spsr_und;
	default:
		throw std::runtime_error("Error: Unrecognized CPSR mode in ARM7TDMI::getCPSR()");
		break;
	}
}

PSR& ARM7TDMI::getSPSR(){
	return getCorrespondingSPSR();
}

void ARM7TDMI::setCPSR(uint32_t val){
	cpsr.setValue(val);
}

void ARM7TDMI::setSPSR(uint32_t val){
	getCorrespondingSPSR().setValue(val);
}

ALU& ARM7TDMI::getALU(){
	return alu;
}

BarrelShifter& ARM7TDMI::getBarrelShifter(){
	return *barrelShifter;
}

int64_t ARM7TDMI::fetchInstructionThumb(uint32_t offset){
	return mem->readHalfWord(offset);
}

int64_t ARM7TDMI::fetchInstructionArm(uint32_t offset){
	return mem->readWord(offset);
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

uint32_t ARM7TDMI::getReg(uint16_t n){
	if(n < 0 || n > 15){
		std::cerr << "ERROR: Invalid reg num: " << n << std::endl;
		return 0;
	}
	
	if(n == 15)
		return reg[15];

	switch (cpsr.getMode())
	{
	case PSR::Mode::System:
	case PSR::Mode::User:
		return reg[n];
		break;
	case PSR::Mode::FIQ:
		if(n>=0 && n<=7)
			return reg[n];
		else if(n>=8 && n<=14)
			return r8_fiq[n - 8];
		break;
	case PSR::Mode::Supervisor:
		if(n>=0 && n<=12)
			return reg[n];
		else if(n>=13 && n<=14)
			return r13_svc[n - 13];
		break;
	case PSR::Mode::Abort:
		if(n>=0 && n<=12)
			return reg[n];
		else if(n>=13 && n<=14)
			return r13_abt[n - 13];
		break;
	case PSR::Mode::IRQ:
		if(n>=0 && n<=12)
			return reg[n];
		else if(n>=13 && n<=14)
			return r13_irq[n - 13];
		break;
	case PSR::Mode::Undefined:
		if(n>=0 && n<=12)
			return reg[n];
		else if(n>=13 && n<=14)
			return r13_und[n - 13];
		break;
	default:
		std::cerr << "ERROR: Unknown CPSR Mode in CPU getReg" << std::endl;
		return 0;
		break;
	}
	
	return 0;
}

void ARM7TDMI::setReg(uint16_t n, uint32_t val){
	if(n < 0 || n > 15){
		std::cerr << "ERROR: Invalid reg num: " << n << std::endl;
		return;
	}
	switch (cpsr.getMode())
	{
	case PSR::Mode::System:
	case PSR::Mode::User:
		reg[n] = val;
		break;
	case PSR::Mode::FIQ:
		if(n>=0 && n<=7)
			reg[n] = val;
		else if(n>=8 && n<=14)
			r8_fiq[n - 8] = val;
		break;
	case PSR::Mode::Supervisor:
		if(n>=0 && n<=12)
			reg[n] = val;
		else if(n>=13 && n<=14)
			r13_svc[n - 13] = val;
		break;
	case PSR::Mode::Abort:
		if(n>=0 && n<=12)
			reg[n] = val;
		else if(n>=13 && n<=14)
			r13_abt[n - 13] = val;
		break;
	case PSR::Mode::IRQ:
		if(n>=0 && n<=12)
			reg[n] = val;
		else if(n>=13 && n<=14)
			r13_irq[n - 13] = val;
		break;
	case PSR::Mode::Undefined:
		if(n>=0 && n<=12)
			reg[n] = val;
		else if(n>=13 && n<=14)
			r13_und[n - 13] = val;
		break;
	default:
		std::cerr << "ERROR: Unknown CPSR Mode in CPU getReg" << std::endl;
		break;
	}
}


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
	// PC is reduced by 8 to account for pipeline parallelism
	std::cout << "pc:   " << Utils::toHexString(getPC(), 8) << std::endl;
	std::cout << "cpsr: " << Utils::toHexString(cpsr.getValue(), 8) << std::endl;
	std::cout << "n:" << cpsr.getNFlag() << " z:" << cpsr.getZFlag() << " c:" << cpsr.getCFlag() << " v:" << cpsr.getVFlag() <<std::endl;
	for(int i=0; i<16; i++){
		std::cout << "r" + std::to_string(i) + ": " + Utils::toHexString(getReg(i)) + " ";
	}
	std::cout << std::endl;
}

MemoryManager& ARM7TDMI::getMemManager(){
	return (*mem);
}


/*void ARM7TDMI::executeOpArm(OpCode *op){
	op->execute(); 
}*/

void ARM7TDMI::executeNextInstruction(){
		// execute
		if(insDecodeSet){
			insExecuteSet = opExecute->execute();

			// print status
			std::cout << opExecute->toString() <<  " - " << opExecute->toHexString() << std::endl;
			printStatus();
			std::cout << "<<<" << std::endl;
		}

		// flush pipeline if needed
		if(insExecuteSet && opExecute->getMustFlushPipeline()){
			insFetchSet = false;
			insDecodeSet = false;
		}

		if(insExecuteSet){
			insExecuteSet = false;
			delete opExecute;
		}

		// decode
		if(insFetchSet){
			opExecute = decodeInstruction(insDecode, fetchPC);
			opExecute->decode();
			insDecodeSet = true;
		}
		
		// fetch
		insFetch = mem->readWord(getPC());
		fetchPC = getPC();
		setPC(getPC() + 4);

		
		insFetchSet = true;
		insDecode = insFetch;
}


void ARM7TDMI::executionLoop(){
	uint32_t insFetch, insDecode;
	OpCode *opExecute = nullptr;
	bool insFetchSet = false;
	bool insDecodeSet = false;
	bool insExecuteSet = false;
	uint32_t fetchPC;
	while(true){
		if(!cpsr.isThumbMode()){
			// execute
			if(insDecodeSet){
				insExecuteSet = opExecute->execute();

				if(opExecute->toHexString() == "0xEAFFFFFE"){
					std::cout << "END" << std::endl;
					return;
				}

				// print status
				//std::cout << opExecute->toString() <<  " - " << opExecute->toHexString() << std::endl;
				//printStatus();
				//std::cout << "<<<" << std::endl;
			}

			// flush pipeline if needed
			if(insExecuteSet && opExecute->getMustFlushPipeline()){
				insFetchSet = false;
				insDecodeSet = false;
			}

			if(insExecuteSet){
				insExecuteSet = false;
				delete opExecute;
			}

			// decode
			if(insFetchSet){
				opExecute = decodeInstructionARM(insDecode, fetchPC);
				insDecodeSet = true;
			}
			
			// fetch
			insFetch = mem->readWord(getPC());
  			fetchPC = getPC();
			setPC(getPC() + 4);

			
			insFetchSet = true;
			insDecode = insFetch;
			
		}else{

		}
	}
	
}

