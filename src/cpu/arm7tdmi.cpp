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

	// ???
	//reg[0] = 0x00000CA5;
	reg[13] = 0x03007F00;
	reg[14] = 0x08000000;
	r13_irq[0] = 0x03007FA0;
	r13_svc[0] = 0x03007FE0;

	barrelShifter = std::make_unique<BarrelShifter>();
	exceptionHandler = std::make_unique<ExceptionHandler>();

	spsr_fiq.setValue(0xF00000FF);
}

ARM7TDMI::~ARM7TDMI() {
}

std::unique_ptr<OpCode> ARM7TDMI::decodeInstruction(uint32_t op, uint32_t pc){
	if(cpsr.isThumbMode()){
		return decodeInstructionThumb(op, pc);
	}else{
		return decodeInstructionARM(op, pc);
	}
}

std::unique_ptr<OpCode> ARM7TDMI::decodeInstructionARM(uint32_t op, uint32_t pc) {
	if(ArmOpcode::isBranchAndExchange(op)){
		return std::make_unique<BranchAndExchange>(op, *this);
	}else if (ArmOpcode::isBlockDataTransfer(op)) {
		return std::make_unique<BlockDataTransfer>(op, *this);
	}else if(ArmOpcode::isBranch(op)){
		return std::make_unique<ARM::Branch>(op, pc, *this);
	}else if(ArmOpcode::isSoftwareInterrupt(op)){
		return std::make_unique<ARM::SoftwareInterrupt>(op, *this);
	}else if(ArmOpcode::isUndefined(op)){
		return std::make_unique<Undefined>(op, *this);
	}else if(ArmOpcode::isSingleDataTransfer(op)){
		return std::make_unique<SingleDataTransfer>(op, *this);
	}else if(ArmOpcode::isSingleDataSwap(op)){
		return std::make_unique<SingleDataSwap>(op, *this);
	}else if(ArmOpcode::isMultiply(op)){
		return std::make_unique<MultiplyAccumulate>(op, *this);
	}else if(ArmOpcode::isMultiplyLong(op)){
		return std::make_unique<MultiplyAccumulateLong>(op, *this);
	}else if(ArmOpcode::isHalfwordDataTransferRegister(op)){
		return std::make_unique<HalfwordDataTransferRegister>(op, *this);
	}else if(ArmOpcode::isHalfwordDataTransferOffset(op)){
		return std::make_unique<HalfwordDataTransferOffset>(op, *this);
	}else if(ArmOpcode::isPSRTransferMRS(op)){
		return std::make_unique<PSRTransferMRS>(op, *this);
	}else if(ArmOpcode::isPSRTransferMSR(op)){
		return std::make_unique<PSRTransferMSR>(op, *this);
	}else if(ArmOpcode::isDataProcessing(op)){
		return std::make_unique<DataProcessing>(op, *this);
	}

	throw std::runtime_error("ERROR: Unrecognized instruction in decodeInstructionARM");
	return nullptr;
}

std::unique_ptr<OpCode> ARM7TDMI::decodeInstructionThumb(uint16_t op, uint32_t pc) {
	if(ThumbOpCode::isSoftwareInterrupt(op)){
		return std::make_unique<Thumb::SoftwareInterrupt>(op, *this);
	}else if (ThumbOpCode::isAddOffsetToSP(op)) {
		return std::make_unique<AddOffsetSP>(op, *this);
	}else if(ThumbOpCode::isPushPopRegisters(op)) {
		return std::make_unique<PushPopRegisters>(op, *this);
	}else if(ThumbOpCode::isALUOperations(op)) {
		return std::make_unique<ALUOperations>(op, *this);
	}else if(ThumbOpCode::isHiRegisterBranchExchange(op)) {
		return std::make_unique<HiRegisterBranchExchange>(op, *this);
	}else if(ThumbOpCode::isPCRelativeLoad(op)) {
		return std::make_unique<PCRelativeLoad>(op, *this);
	}else if(ThumbOpCode::isLoadStoreRegisterOffset(op)) {
		return std::make_unique<LoadStoreRegisterOffset>(op, *this);
	}else if(ThumbOpCode::isLoadStoreSignExtended(op)) {
		return std::make_unique<LoadStoreSignExtended>(op, *this);
	}else if(ThumbOpCode::isUnconditionalBranch(op)) {
		return std::make_unique<Thumb::UnconditionalBranch>(op, pc, *this);
	}else if(ThumbOpCode::isAddSubtract(op)) {
		return std::make_unique<AddSubtract>(op, *this);
	}else if(ThumbOpCode::isLoadStoreHalfword(op)) {
		return std::make_unique<LoadStoreHalfword>(op, *this);
	}else if(ThumbOpCode::isSPLoadStore(op)) {
		return std::make_unique<SPLoadStore>(op, *this);
	}else if(ThumbOpCode::isLoadAddress(op)) {
		return std::make_unique<LoadAddress>(op, *this);
	}else if(ThumbOpCode::isMultipleLoadStore(op)) {
		return std::make_unique<MultipleLoadStore>(op, *this);
	}else if(ThumbOpCode::isConditionalBranch(op)) {
		return std::make_unique<Thumb::ConditionalBranch>(op, pc, *this);
	}else if(ThumbOpCode::isLongBranchWithLink(op)) {
		return std::make_unique<Thumb::LongBranchWithLink>(op, pc, *this);
	}else if(ThumbOpCode::isMoveShiftedRegister(op)) {
		return std::make_unique<MoveShiftedRegister>(op, *this);
	}else if(ThumbOpCode::isMoveCompAddSubImm(op)) {
		return std::make_unique<MoveCompAddSubImm>(op, *this);
	}else if(ThumbOpCode::isLoadStoreImmOffset(op)) {
		return std::make_unique<LoadStoreImmOffset>(op, *this);
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
	// ARM test 254
	// https://github.com/jsmolka/gba-tests/blob/master/arm/psr_transfer.asm
	case PSR::Mode::System:
	case PSR::Mode::User:
		return spsr_sys_usr;
	default:
		throw std::runtime_error("Error: Unrecognized CPSR mode in ARM7TDMI::getCPSR()");
		break;
	}
}

PSR& ARM7TDMI::getSPSR(){
	return getCorrespondingSPSR();
}

uint32_t ARM7TDMI::getSPSRval(){
	switch (cpsr.getMode())
	{
	case PSR::Mode::FIQ:
		return spsr_fiq.getValue();
	case PSR::Mode::Supervisor:
		return spsr_svc.getValue();
	case PSR::Mode::Abort:
		return spsr_abt.getValue();
	case PSR::Mode::IRQ:
		return spsr_irq.getValue();
	case PSR::Mode::Undefined:
		return spsr_und.getValue();
	case PSR::Mode::User:
	case PSR::Mode::System:
		return 0x00000000;
	default:
		throw std::runtime_error("Error: Unrecognized CPSR mode in ARM7TDMI::getCPSR()");
		break;
	}
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

ExceptionHandler& ARM7TDMI::getExceptionHandler(){
	return *exceptionHandler;
}

int64_t ARM7TDMI::fetchInstructionThumb(uint32_t offset){
	return mem->readHalfWord(offset);
}

int64_t ARM7TDMI::fetchInstructionArm(uint32_t offset){
	return mem->readWord(offset);
}

uint32_t ARM7TDMI::getReg(uint16_t n, bool userBank){
	if(n > 15){
		std::cerr << "ERROR: Invalid reg num: " << n << std::endl;
		return 0;
	}
	
	if(n == 15){
		return reg[15];
	}
		

	if(!userBank){
		switch (cpsr.getMode())
		{
		case PSR::Mode::System:
		case PSR::Mode::User:
			return reg[n];
			break;
		case PSR::Mode::FIQ:
			if(n<=7)
				return reg[n];
			else if(n>=8 && n<=14)
				return r8_fiq[n - 8];
			break;
		case PSR::Mode::Supervisor:
			if(n<=12)
				return reg[n];
			else if(n>=13 && n<=14)
				return r13_svc[n - 13];
			break;
		case PSR::Mode::Abort:
			if(n<=12)
				return reg[n];
			else if(n>=13 && n<=14)
				return r13_abt[n - 13];
			break;
		case PSR::Mode::IRQ:
			if(n<=12)
				return reg[n];
			else if(n>=13 && n<=14)
				return r13_irq[n - 13];
			break;
		case PSR::Mode::Undefined:
			if(n<=12)
				return reg[n];
			else if(n>=13 && n<=14)
				return r13_und[n - 13];
			break;
		default:
			std::cerr << "ERROR: Unknown CPSR Mode in CPU getReg" << std::endl;
			return 0;
			break;
		}
	}else{
		return reg[n];
	}

	
	return 0;
}

void ARM7TDMI::setReg(uint16_t n, uint32_t val, bool userBank){
	if(n > 15){
		std::cerr << "ERROR: Invalid reg num: " << n << std::endl;
		return;
	}

	if(!userBank){
		if(n == 15){
			reg[n] = val & 0xFFFFFFFE;
		}else{
			switch (cpsr.getMode())
			{
			case PSR::Mode::System:
			case PSR::Mode::User:
				reg[n] = val;
				break;
			case PSR::Mode::FIQ:
				if(n<=7)
					reg[n] = val;
				else if(n>=8 && n<=14)
					r8_fiq[n - 8] = val;
				break;
			case PSR::Mode::Supervisor:
				if(n<=12)
					reg[n] = val;
				else if(n>=13 && n<=14)
					r13_svc[n - 13] = val;
				break;
			case PSR::Mode::Abort:
				if(n<=12)
					reg[n] = val;
				else if(n>=13 && n<=14)
					r13_abt[n - 13] = val;
				break;
			case PSR::Mode::IRQ:
				if(n<=12)
					reg[n] = val;
				else if(n>=13 && n<=14)
					r13_irq[n - 13] = val;
				break;
			case PSR::Mode::Undefined:
				if(n<=12)
					reg[n] = val;
				else if(n>=13 && n<=14)
					r13_und[n - 13] = val;
				break;
			default:
				throw std::runtime_error("ERROR: Invalid CPSR Mode in ARM7TDMI::setReg");
				break;
			}
		}
	}else{
		reg[n] = val;
	}
	

}

bool ARM7TDMI::getMustFlushPipeline() const{
    return mustFlushPipeline;
}

void ARM7TDMI::setMustFlushPipeline(bool val){
    mustFlushPipeline = val;
}

uint32_t ARM7TDMI::getPC(){
	return reg[15];
}

void ARM7TDMI::setPC(uint32_t pc){
	setReg(15, pc);
}

void ARM7TDMI::setLR(uint32_t lr){
	setReg(14, lr);
}

void ARM7TDMI::printStatus(){
	std::string n = "-";
	if(cpsr.getNFlag()) n="N";  

	std::string z = "-";
	if(cpsr.getZFlag()) z="Z";

	std::string c = "-";
	if(cpsr.getCFlag()) c="C";

	std::string v = "-";
	if(cpsr.getVFlag()) v="V";

	std::string i = "-";
	if(cpsr.getIFlag()) i="I";

	std::string f = "-";
	if(cpsr.getFFlag()) f="F";

	std::string t = "-";
	if(cpsr.getTFlag()) t="T";

	// TODO: take thumb mode into account
	// PC is reduced by 8 to account for pipeline parallelism
	//std::cout << "pc:   " << Utils::toHexString(getPC(), 8) << std::endl;
	std::cout << "cpsr: " << Utils::toHexString(cpsr.getValue(), 8) << "\t" << "[" << n << z << c << v << i << f << t << "]" << "\t";
	if(cpsr.getTFlag() == 0)
		std::cout << "ARM" << "\t";
	else
		std::cout << "THUMB" << "\t";

	std::cout << cpsr.getModeString() << std::endl;
	
	std::cout << "spsr: " << Utils::toHexString(getSPSRval(), 8) << std::endl;

	for(int i=0; i<16; i++){
		std::cout << std::setw(3);
		std::cout << "r" + std::to_string(i) << ": " << Utils::toHexString(getReg(i), 8) << " ";
		if(i !=0 && (i+1) % 4 == 0)
			std::cout << std::endl;
	}
	std::cout << std::endl;
}

MemoryManager& ARM7TDMI::getMemManager(){
	return (*mem);
}


/*void ARM7TDMI::executeOpArm(OpCode *op){
	op->execute(); 
}*/

/*
Mis-aligned PC/R15 (branch opcodes, or MOV/ALU/LDR with Rd=R15)
For ARM code, the low bits of the target address should be usually zero, otherwise, R15 is forcibly aligned by clearing the lower two bits.

For THUMB code, the low bit of the target address may/should/must be set, the bit is (or is not) interpreted as thumb-bit (depending on the opcode),
 and R15 is then forcibly aligned by clearing the lower bit.

In short, R15 will be always forcibly aligned, so mis-aligned branches won’t have effect on subsequent opcodes that use R15, or [R15+disp] as operand.

*/


uint32_t ARM7TDMI::fetchNextInstruction(){
	uint32_t pc = getPC();
	uint32_t ins;
	if(cpsr.isThumbMode()){
		ins = mem->readHalfWord(pc);
		setPC(getPC() + 2);
	}else{
		ins = mem->readWord(pc);
		setPC(getPC() + 4);
	}
	
	return ins;
}

void ARM7TDMI::executeNextInstruction(){
		// execute
		if(insDecodeSet){
			// print status
			//std::cout << opExecute->toString() <<  " - " << opExecute->toHexString() << std::endl;
			insExecuteSet = opExecute->execute();
			//printStatus();
			//std::cout << "<<<" << std::endl;

			// flush pipeline if needed
			// dont flush if op is not executed
			if(insExecuteSet && getMustFlushPipeline()){
				insFetchSet = false;
				insDecodeSet = false;
				setMustFlushPipeline(false);
			}

			if(insExecuteSet){
				insExecuteSet = false;
			}

			opExecute.reset();
		}


		// decode
		if(insFetchSet){
			if(opExecute.get() != nullptr)
				opExecute.reset();
			opExecute = decodeInstruction(insDecode, fetchPC);
			//opExecute->decode();
			insDecodeSet = true;
		}
		
		// fetch
		fetchPC = getPC();
		insFetch = fetchNextInstruction();

		/*if(fetchPC == 0x080026FA){
			std::cout << "aa" << std::endl;
		}*/


		if(insFetch == 0x45d8){
			std::cout << "aa" << std::endl;
		}

		insFetchSet = true;
		insDecode = insFetch;
}
