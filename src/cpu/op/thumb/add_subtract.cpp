#include "add_subtract.hpp"
#include "../arm/data_processing.hpp"
#include "../fields/rotate_imm.hpp"
#include "../fields/shift_rm.hpp"

const std::string AddSubtract::op2Mnemonic[2] = {"add", "sub"};

AddSubtract::AddSubtract(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    init(op);
}

AddSubtract::AddSubtract(ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(cpu) {}

void AddSubtract::init(uint32_t op){
    ThumbOpCode::init(op);
    i = Utils::getRegBits(op, I_MASK, I_SHIFT);
    opField = Utils::getRegBits(op, OP_MASK, OP_SHIFT);
    rnOffset3 = Utils::getRegBits(op, RN_OFFSET3_MASK, RN_OFFSET3_SHIFT);
    rs = Utils::getRegBits(op, RS_MASK, RS_SHIFT);
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
}

std::string AddSubtract::getOpMnemonic(){
    return op2Mnemonic[opField];
}

std::string AddSubtract::toString(){
    std::string mnemonic = getOpMnemonic() + " " + OpCode::getRegMnemonic(rd) + "," + OpCode::getRegMnemonic(rs) + ",";
    if(i == 0){
        mnemonic += OpCode::getRegMnemonic(rnOffset3);
    }else if(i == 1){
        // https://github.com/rust-console/gbatek-gbaonly/blob/gh-pages/index.md#thumb2-addsubtract
        // Pseudo/alias opcode with Imm=0:
        if(opField == 0 && rnOffset3 == 0){
            mnemonic = "mov " + OpCode::getRegMnemonic(rd) + "," + OpCode::getRegMnemonic(rs);
        }else {
            mnemonic += "#" + Utils::toHexString(rnOffset3, 4);

        }
    }else{
        throw std::runtime_error("ERROR: Thumb Add/subtract invalid opcode field");
    }

    return mnemonic;
}

void AddSubtract::doDecode(){

}

void AddSubtract::doExecute(){

    if(opField == 0){
        if(i == 1){
            RotateImm imm = RotateImm(rnOffset3, RotateImm::ConstructorType::FINAL_IMM_VAL);
            DataProcessing opArm = DataProcessing(i, DataProcessing::OPCODE_ADD_VAL, 1, rs, rd, imm.getRawVal(), cpu);
            opArm.doExecute();
            //std::cout << "<< ARM >> " << opArm.toString() << '\n';
        }else{
            ShiftRm shiftRm = ShiftRm(rnOffset3, true, 0, 0);
            DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_ADD_VAL, 1, rs, rd, shiftRm.getRawVal(), cpu);
            opArm.doExecute();
            //std::cout << "<< ARM >> " << opArm.toString() << '\n';
        }
    }else if(opField == 1){
        if(i == 1){
            RotateImm imm = RotateImm(rnOffset3, RotateImm::ConstructorType::FINAL_IMM_VAL);
            DataProcessing opArm = DataProcessing(i, DataProcessing::OPCODE_SUB_VAL, 1, rs, rd, imm.getRawVal(), cpu);
            opArm.doExecute();
            //std::cout << "<< ARM >> " << opArm.toString() << '\n';
        }else{
            ShiftRm shiftRm = ShiftRm(rnOffset3, true, 0, 0);
            DataProcessing opArm = DataProcessing(i, DataProcessing::OPCODE_SUB_VAL, 1, rs, rd, shiftRm.getRawVal(), cpu);
            opArm.doExecute();
            //std::cout << "<< ARM >> " << opArm.toString() << '\n';
        }
    }else{
        throw std::runtime_error("ERROR: Invalid AddSubtract::doExecute opField value");
    }
}

uint32_t AddSubtract::cyclesUsed() const {
    return 1;
}