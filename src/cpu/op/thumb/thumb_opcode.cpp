#include "thumb_opcode.hpp"

#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"
#include "../fields/operand.hpp"

ThumbOpCode::ThumbOpCode(uint32_t op, ARM7TDMI& cpu) : OpCode::OpCode(op, cpu) { opcode = op; }

ThumbOpCode::ThumbOpCode(ARM7TDMI& cpu) : OpCode::OpCode(cpu) {}

void ThumbOpCode::init(uint32_t op) { OpCode::init(op); }

// Thumb instructions have no conditions; they are always executed
bool ThumbOpCode::execute() {
    doExecute();
    // std::cout << "<< ARM >> " << opArm.toString() << '\n';
    // opArm.doExecute();
    return true;
}

void ThumbOpCode::decode() { doDecode(); }

bool ThumbOpCode::isSoftwareInterrupt(uint16_t op) {
    return OpCode::checkOpCode(op, SOFTWARE_INTERRUPT_MASK, SOFTWARE_INTERRUPT_FORMAT);
}

bool ThumbOpCode::isAddOffsetToSP(uint16_t op) {
    return OpCode::checkOpCode(op, ADD_OFFSET_TO_SP_MASK, ADD_OFFSET_TO_SP_FORMAT);
}

bool ThumbOpCode::isPushPopRegisters(uint16_t op) {
    return OpCode::checkOpCode(op, PUSH_POP_REGISTERS_MASK, PUSH_POP_REGISTERS_FORMAT);
}

bool ThumbOpCode::isALUOperations(uint16_t op) {
    return OpCode::checkOpCode(op, ALU_OPERATIONS_MASK, ALU_OPERATIONS_FORMAT);
}

bool ThumbOpCode::isHiRegisterBranchExchange(uint16_t op) {
    return OpCode::checkOpCode(op, HI_REGISTER_BRANCH_EXCHANGE_MASK, HI_REGISTER_BRANCH_EXCHANGE_FORMAT);
}

bool ThumbOpCode::isPCRelativeLoad(uint16_t op) {
    return OpCode::checkOpCode(op, PC_RELATIVE_LOAD_MASK, PC_RELATIVE_LOAD_FORMAT);
}

bool ThumbOpCode::isLoadStoreRegisterOffset(uint16_t op) {
    return OpCode::checkOpCode(op, LOAD_STORE_REGISTER_OFFSET_MASK, LOAD_STORE_REGISTER_OFFSET_FORMAT);
}

bool ThumbOpCode::isLoadStoreSignExtended(uint16_t op) {
    return OpCode::checkOpCode(op, LOAD_STORE_SIGN_EXTENDED_MASK, LOAD_STORE_SIGN_EXTENDED_FORMAT);
}

bool ThumbOpCode::isUnconditionalBranch(uint16_t op) {
    return OpCode::checkOpCode(op, UNCONDITIONAL_BRANCH_MASK, UNCONDITIONAL_BRANCH_FORMAT);
}

bool ThumbOpCode::isAddSubtract(uint16_t op) { return OpCode::checkOpCode(op, ADD_SUBTRACT_MASK, ADD_SUBTRACT_FORMAT); }

bool ThumbOpCode::isLoadStoreHalfword(uint16_t op) {
    return OpCode::checkOpCode(op, LOAD_STORE_HALFWORD_MASK, LOAD_STORE_HALFWORD_FORMAT);
}

bool ThumbOpCode::isSPLoadStore(uint16_t op) {
    return OpCode::checkOpCode(op, SP_LOAD_STORE_MASK, SP_LOAD_STORE_FORMAT);
}

bool ThumbOpCode::isLoadAddress(uint16_t op) { return OpCode::checkOpCode(op, LOAD_ADDRESS_MASK, LOAD_ADDRESS_FORMAT); }

bool ThumbOpCode::isMultipleLoadStore(uint16_t op) {
    return OpCode::checkOpCode(op, MULTIPLE_LOAD_STORE_MASK, MULTIPLE_LOAD_STORE_FORMAT);
}

bool ThumbOpCode::isConditionalBranch(uint16_t op) {
    return OpCode::checkOpCode(op, CONDITIONAL_BRANCH_MASK, CONDITIONAL_BRANCH_FORMAT);
}

bool ThumbOpCode::isLongBranchWithLink(uint16_t op) {
    return OpCode::checkOpCode(op, LONG_BRANCH_WITH_LINK_MASK, LONG_BRANCH_WITH_LINK_FORMAT);
}

bool ThumbOpCode::isMoveShiftedRegister(uint16_t op) {
    return OpCode::checkOpCode(op, MOVE_SHIFTED_REGISTER_MASK, MOVE_SHIFTED_REGISTER_FORMAT);
}

bool ThumbOpCode::isMoveCompAddSubImm(uint16_t op) {
    return OpCode::checkOpCode(op, MOVE_COMP_ADD_SUB_IMM_MASK, MOVE_COMP_ADD_SUB_IMM_FORMAT);
}

bool ThumbOpCode::isLoadStoreImmOffset(uint16_t op) {
    return OpCode::checkOpCode(op, LOAD_STORE_IMM_OFFSET_MASK, LOAD_STORE_IMM_OFFSET_FORMAT);
}
