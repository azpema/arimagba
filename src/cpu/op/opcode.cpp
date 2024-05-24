#include "opcode.hpp"
#include "../../utils/utils.hpp"

const std::string OpCode::reg2Mnemonic[16] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
                                              "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc"};

OpCode::OpCode(uint32_t op, ARM7TDMI& cpu) : cpu(cpu) {
    opcode = op;
}

std::string OpCode::toHexString(){
    return Utils::toHexString(opcode);
}

std::string OpCode::getRegMnemonic(uint16_t reg){
    return reg2Mnemonic[reg];
}

bool OpCode::checkOpCode(uint32_t op, uint32_t mask, uint32_t format){
    return (op & mask) == format;
}