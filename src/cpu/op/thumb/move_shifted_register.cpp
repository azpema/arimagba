#include "move_shifted_register.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"
#include "../arm/data_processing.hpp"
#include "../fields/shift_rm.hpp"
#include <vector>

const std::string MoveShiftedRegister::op2Mnemonic[3] = {"lsl", "lsr", "asr"};

MoveShiftedRegister::MoveShiftedRegister(uint16_t op, ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(op, cpu) { init(op); }

MoveShiftedRegister::MoveShiftedRegister(ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(cpu) {}

void MoveShiftedRegister::init(uint32_t op) {
    ThumbOpCode::init(op);
    opField = Utils::getRegBits(op, OP_MASK, OP_SHIFT);
    offset5 = Utils::getRegBits(op, OFFSET5_MASK, OFFSET5_SHIFT);
    rs = Utils::getRegBits(op, RS_MASK, RS_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
}

std::string MoveShiftedRegister::getOpMnemonic() { return op2Mnemonic[opField]; }

std::string MoveShiftedRegister::toString() {
    return getOpMnemonic() + " " + OpCode::getRegMnemonic(rd) + "," + OpCode::getRegMnemonic(rs) + ",#" +
           Utils::toHexString(offset5, 4);
}

void MoveShiftedRegister::doDecode() {}

void MoveShiftedRegister::doExecute() {
    /*
    "lsl" 0
    "lsr" 1
    "asr" 2

    */
    // ShiftRm(uint8_t rm, bool shiftAmount, uint8_t amount, uint8_t type);

    ShiftRm shiftRm = ShiftRm(rs, true, offset5, opField);
    // uint32_t operand2 = (((offset5 << 3) | (opField << 1)) << 3) | rs
    uint32_t rawVal = shiftRm.getRawVal();
    DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_MOV_VAL, 1, 0, rd, rawVal, cpu);
    opArm.doExecute();
    // std::cout << "<< ARM >> " << opArm.toString() << '\n';
}

uint32_t MoveShiftedRegister::cyclesUsed() const { return 1; }