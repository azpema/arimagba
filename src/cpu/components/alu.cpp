#include "alu.hpp"

ALU::ALU() : n(false), z(false), c(false), v(false) {}

// 32 BIT ALU!!!

uint32_t ALU::add(uint32_t op1, uint32_t op2) {
    uint32_t res = op1 + op2;

    bool op1Sign = Utils::getRegBits(op1, MSB_MASK, MSB_SHIFT) == 1;
    bool op2Sign = Utils::getRegBits(op2, MSB_MASK, MSB_SHIFT) == 1;
    bool resSign = Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1;

    n = (Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1);
    z = (res == 0);
    c = ((static_cast<uint64_t>(op1) + static_cast<uint64_t>(op2)) != res);
    // v = ( iOp1 + iOp2 != static_cast<int32_t>(res));
    v = (!op1Sign && !op2Sign && resSign) || (op1Sign && op2Sign && !resSign);

    return res;
}

uint32_t ALU::adc(uint32_t op1, uint32_t op2, uint8_t carry) {
    uint32_t res = op1 + op2 + carry;

    bool op1Sign = Utils::getRegBits(op1, MSB_MASK, MSB_SHIFT) == 1;
    bool op2Sign = Utils::getRegBits(op2, MSB_MASK, MSB_SHIFT) == 1;
    bool resSign = Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1;

    n = (Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1);
    z = (res == 0);
    c = ((static_cast<uint64_t>(op1) + static_cast<uint64_t>(op2) + static_cast<uint64_t>(carry)) != res);
    v = (!op1Sign && !op2Sign && resSign) || (op1Sign && op2Sign && !resSign);

    return res;
}

// https://wwww.dmulholl.com/notes/arm-condition-flags.html
uint32_t ALU::sub(uint32_t op1, uint32_t op2) {
    uint32_t res = op1 - op2;

    bool op1Sign = Utils::getRegBits(op1, MSB_MASK, MSB_SHIFT) == 1;
    bool op2Sign = Utils::getRegBits(op2, MSB_MASK, MSB_SHIFT) == 1;
    bool resSign = Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1;

    n = (Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1);
    z = (res == 0);
    c = op2 <= op1;
    // Before
    // c = op2 < op1;
    // c = op2 >= op1;
    v = (!op1Sign && op2Sign && resSign) || (op1Sign && !op2Sign && !resSign);

    return res;
}

uint32_t ALU::sbc(uint32_t op1, uint32_t op2, uint8_t carry) {
    uint32_t res = op1 + carry - 1 - op2;

    n = (Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1);
    z = (res == 0);
    // c = op2 <= interm;
    //  Before
    // c = op2 < op1;
    // c = op2 >= op1;
    // v = (!op1Sign && op2Sign && resSign) || (op1Sign && !op2Sign && !resSign);

    // for Op1 - Op2
    c = (static_cast<uint64_t>(op2) + 1 - carry <= op1 ? 1 : 0);
    v = (((op1 ^ op2) & (~op2 ^ res)) >> 31);

    return res;
}

uint32_t ALU::andOp(uint32_t op1, uint32_t op2) {
    uint32_t res = op1 & op2;

    n = (Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1);
    z = (res == 0);

    return res;
}

uint32_t ALU::mov(uint32_t op) {
    n = (Utils::getRegBits(op, MSB_MASK, MSB_SHIFT) == 1);
    z = (op == 0);
    return op;
}

uint32_t ALU::mvn(uint32_t op2) {
    uint32_t res = ~op2;

    n = (Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1);
    z = (res == 0);

    return res;
}

uint32_t ALU::orr(uint32_t op1, uint32_t op2) {
    uint32_t res = op1 | op2;

    n = (Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1);
    z = (res == 0);

    return res;
}

uint32_t ALU::eor(uint32_t op1, uint32_t op2) {
    uint32_t res = op1 ^ op2;

    n = (Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1);
    z = (res == 0);

    return res;
}

uint32_t ALU::mul(uint32_t op1, uint32_t op2) {
    uint64_t res = op1 * op2;
    res &= 0xFFFFFFFF;

    n = (Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1);
    z = (res == 0);
    /*The C (Carry) flag is set to a meaningless value and the V (oVerflow) flag is unaffected.*/

    return res;
}

uint32_t ALU::mla(uint32_t op1, uint32_t op2, uint32_t accum) {
    uint64_t res = op1 * op2 + accum;
    res &= 0xFFFFFFFF;

    n = (Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1);
    z = (res == 0);
    /*The C (Carry) flag is set to a meaningless value and the V (oVerflow) flag is unaffected.*/

    return res;
}

bool ALU::getN() { return n; }

bool ALU::getZ() { return z; }

bool ALU::getC() { return c; }

bool ALU::getV() { return v; }