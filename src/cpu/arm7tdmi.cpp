#include "arm7tdmi.hpp"
#include "op/arm/arm_opcode.hpp"
#include "op/thumb/thumb_opcode.hpp"
#include "op/arm_pool.hpp"
#include "op/thumb_pool.hpp"
#include <iostream>

ARM7TDMI::ARM7TDMI(MemoryManager *memManager) : armPool(*this), thumbPool(*this){
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
	reg[0] = 0x00000CA5;
	reg[13] = 0x03007F00;
	reg[14] = 0x08000000;
	r13_irq[0] = 0x03007FA0;
	r13_svc[0] = 0x03007FE0;

	barrelShifter = std::make_unique<BarrelShifter>();
	exceptionHandler = std::make_unique<ExceptionHandler>(*this);

	spsr_fiq.setValue(0xF00000FF);

	generateThumbDecodingLookup();
}

ARM7TDMI::~ARM7TDMI() {
}

OpCode* ARM7TDMI::decodeInstruction(uint32_t op, uint32_t pc){
	if(cpsr.isThumbMode()){
		return decodeInstructionThumb(op);
	}else{
		return decodeInstructionARM(op, pc);
	}
}

OpCode* ARM7TDMI::decodeInstructionARM(uint32_t op, uint32_t pc) {
	OpCode * armOpcodeInstance = nullptr;

	if(ArmOpcode::isBranchAndExchange(op)){
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::BRANCH_AND_EXCHANGE);
	}else if (ArmOpcode::isBlockDataTransfer(op)) {
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::BRANCH_AND_EXCHANGE);
	}else if(ArmOpcode::isBranch(op)){
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::BRANCH);
	}else if(ArmOpcode::isSoftwareInterrupt(op)){
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::SOFTWARE_INTERRUPT);
	}else if(ArmOpcode::isUndefined(op)){
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::UNDEFINED);
	}else if(ArmOpcode::isSingleDataTransfer(op)){
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::SINGLE_DATA_TRANSFER);
	}else if(ArmOpcode::isSingleDataSwap(op)){
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::SINGLE_DATA_SWAP);
	}else if(ArmOpcode::isMultiply(op)){
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::MULTIPLY);
	}else if(ArmOpcode::isMultiplyLong(op)){
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::MULTIPLY_LONG);
	}else if(ArmOpcode::isHalfwordDataTransferRegister(op)){
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::HALFWORD_DATA_TRANSFER_REGISTER);
	}else if(ArmOpcode::isHalfwordDataTransferOffset(op)){
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::HALFWORD_DATA_TRANSFER_OFFSET);
	}else if(ArmOpcode::isPSRTransferMRS(op)){
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::PSR_TRANSFER_MRS);
	}else if(ArmOpcode::isPSRTransferMSR(op)){
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::PSR_TRANSFER_MSR);
	}else if(ArmOpcode::isDataProcessing(op)){
		armOpcodeInstance = armPool.getArmInstance(ArmOpcode::OpCodeEnum::DATA_PROCESSING);
	}else{
		throw std::runtime_error("ERROR: Unrecognized instruction in decodeInstructionARM");
	}

	armOpcodeInstance->init(op);

	return armOpcodeInstance;
}

/*OpCode* ARM7TDMI::decodeInstructionARM(uint32_t op, uint32_t pc) {
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
}*/

/*static std::unordered_map<uint16_t, std::function<std::unique_ptr<OpCode>(uint16_t)>> opCodeHandlers;

void ARM7TDMI::generateThumbDecodingLookup(){
	for(size_t i=0; i<256; i++){
		uint16_t op = i << 8;

		if(ThumbOpCode::isSoftwareInterrupt(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<Thumb::SoftwareInterrupt>(op, *this); } });
		}else if (ThumbOpCode::isAddOffsetToSP(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<AddOffsetSP>(op, *this); } });
		}else if(ThumbOpCode::isPushPopRegisters(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<PushPopRegisters>(op, *this); } });
		}else if(ThumbOpCode::isALUOperations(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<ALUOperations>(op, *this); } });
		}else if(ThumbOpCode::isHiRegisterBranchExchange(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<HiRegisterBranchExchange>(op, *this); } });
		}else if(ThumbOpCode::isPCRelativeLoad(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<PCRelativeLoad>(op, *this); } });
		}else if(ThumbOpCode::isLoadStoreRegisterOffset(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<LoadStoreRegisterOffset>(op, *this); } });
		}else if(ThumbOpCode::isLoadStoreSignExtended(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<LoadStoreSignExtended>(op, *this); } });
		}else if(ThumbOpCode::isUnconditionalBranch(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<Thumb::UnconditionalBranch>(op, *this); } });
		}else if(ThumbOpCode::isAddSubtract(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<AddSubtract>(op, *this); } });
		}else if(ThumbOpCode::isLoadStoreHalfword(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<LoadStoreHalfword>(op, *this); } });
		}else if(ThumbOpCode::isSPLoadStore(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<SPLoadStore>(op, *this); } });
		}else if(ThumbOpCode::isLoadAddress(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<LoadAddress>(op, *this); } });
		}else if(ThumbOpCode::isMultipleLoadStore(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<MultipleLoadStore>(op, *this); } });
		}else if(ThumbOpCode::isConditionalBranch(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<Thumb::ConditionalBranch>(op, *this); } });
		}else if(ThumbOpCode::isLongBranchWithLink(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<Thumb::LongBranchWithLink>(op, *this); } });
		}else if(ThumbOpCode::isMoveShiftedRegister(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<MoveShiftedRegister>(op, *this); } });
		}else if(ThumbOpCode::isMoveCompAddSubImm(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<MoveCompAddSubImm>(op, *this); } });
		}else if(ThumbOpCode::isLoadStoreImmOffset(op)){
			opCodeHandlers.insert({ op, [this](uint16_t op) { return std::make_unique<LoadStoreImmOffset>(op, *this); } });
		}

	}
}*/

static std::unordered_map<uint16_t, OpCode*> thumbOpcodeInstance;

void ARM7TDMI::generateThumbDecodingLookup(){
	for(size_t i=0; i<256; i++){
		uint16_t op = i << 8;

		ThumbOpCode::OpCodeEnum thumbEnum;

		if(ThumbOpCode::isSoftwareInterrupt(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::SOFTWARE_INTERRUPT;
		}else if (ThumbOpCode::isAddOffsetToSP(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::ADD_OFFSET_TO_SP;
		}else if(ThumbOpCode::isPushPopRegisters(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::PUSH_POP_REGISTERS;
		}else if(ThumbOpCode::isALUOperations(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::ALU_OPERATIONS;
		}else if(ThumbOpCode::isHiRegisterBranchExchange(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::HI_REGISTER_BRANCH_EXCHANGE;
		}else if(ThumbOpCode::isPCRelativeLoad(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::PC_RELATIVE_LOAD;
		}else if(ThumbOpCode::isLoadStoreRegisterOffset(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::LOAD_STORE_REGISTER_OFFSET;
		}else if(ThumbOpCode::isLoadStoreSignExtended(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::LOAD_STORE_SIGN_EXTENDED;
		}else if(ThumbOpCode::isUnconditionalBranch(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::UNCONDITIONAL_BRANCH;
		}else if(ThumbOpCode::isAddSubtract(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::ADD_SUBTRACT;
		}else if(ThumbOpCode::isLoadStoreHalfword(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::LOAD_STORE_HALFWORD;
		}else if(ThumbOpCode::isSPLoadStore(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::SP_LOAD_STORE;
		}else if(ThumbOpCode::isLoadAddress(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::LOAD_ADDRESS;
		}else if(ThumbOpCode::isMultipleLoadStore(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::MULTIPLE_LOAD_STORE;
		}else if(ThumbOpCode::isConditionalBranch(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::CONDITIONAL_BRANCH;
		}else if(ThumbOpCode::isLongBranchWithLink(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::LONG_BRANCH_WITH_LINK;
		}else if(ThumbOpCode::isMoveShiftedRegister(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::MOVE_SHIFTED_REGISTER;
		}else if(ThumbOpCode::isMoveCompAddSubImm(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::MOVE_COMP_ADD_SUB_IMM;
		}else if(ThumbOpCode::isLoadStoreImmOffset(op)){
			thumbEnum = ThumbOpCode::OpCodeEnum::LOAD_STORE_IMM_OFFSET;
		}

		thumbOpcodeInstance.insert({op, thumbPool.getThumbInstance(thumbEnum)});
	}
}

OpCode* ARM7TDMI::decodeInstructionThumb(uint16_t op) {

   	auto it = thumbOpcodeInstance.find(op & 0xFF00);
	if (it != thumbOpcodeInstance.end()) {
        // Call the handler to create the OpCode instance
		OpCode* poolOpcode = it->second;
		poolOpcode->init(op);

        return poolOpcode;
    }

    // Handle unknown opcode
    throw std::runtime_error("Unknown op in decodeInstructionThumb");
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

void ARM7TDMI::printRegisterValues(){
	for(int i=0; i<16; i++){
		std::cout << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << getReg(i) << " ";
	}
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

uint32_t ARM7TDMI::executeNextInstruction(){
	uint32_t cpuCycles = 0;
	static bool printDebug = true;
	// Flush for IRQ
		if(getMustFlushPipeline()){
			insFetchSet = false;
			insDecodeSet = false;
			setMustFlushPipeline(false);
		}
		// execute
		if(insDecodeSet){
			// print status
			std::string opString;
			if(printDebug){
				opString = opExecute->toString();
				//std::cout << opExecute->toString() <<  " - " << opExecute->toHexString() << std::endl;
			}
			insExecuteSet = opExecute->execute();
			cpuCycles = opExecute->cyclesUsed();
			if(printDebug){
				//printStatus();
				//std::cout << "<<<" << std::endl;
				printRegisterValues();
				std::cout << "cpsr: " << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << cpsr.getValue();
				std::cout << " | " << opString << std::endl;
			}

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

			opExecute = nullptr;
		}


		// decode
		if(insFetchSet){
			if(opExecute != nullptr)
				opExecute = nullptr;
			opExecute = decodeInstruction(insDecode, fetchPC);
			//opExecute->decode();
			insDecodeSet = true;
		}
		
		// fetch
		fetchPC = getPC();
		insFetch = fetchNextInstruction();

		// suite.gba
		/*if(fetchPC == 0x03003060){
			std::cout << "a";
		}*/

		insFetchSet = true;
		insDecode = insFetch;

		if(insDecodeSet && !cpsr.getIFlag()){
			exceptionHandler->handleException();
		}

		return cpuCycles;
}
