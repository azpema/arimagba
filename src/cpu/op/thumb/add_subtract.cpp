#include "add_subtract.hpp"

AddSubtract::AddSubtract(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
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
        std::cerr << "ERROR: Thumb Add/subtract invalid opcode field" << std::endl;
    }

    return mnemonic;
}

void AddSubtract::doDecode(){

}

void AddSubtract::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: AddSubtract");
}

uint32_t AddSubtract::cyclesUsed() const {
    return 1;
}