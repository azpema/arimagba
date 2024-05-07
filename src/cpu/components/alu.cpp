#include "alu.hpp"

ALU::ALU(){
    n = false;
    z = false;
    c = false;
    v = false;
}

// 32 BIT ALU!!!

uint32_t ALU::add(uint32_t op1, uint32_t op2){
    uint32_t res = op1 + op2;
    //int64_t iOp1 = static_cast<int32_t>(op1);
    //int64_t iOp2 = static_cast<int32_t>(op2);
    bool op1Sign = Utils::getRegBits(op1, MSB_MASK, MSB_SHIFT) == 1;
    bool op2Sign = Utils::getRegBits(op2, MSB_MASK, MSB_SHIFT) == 1;
    bool resSign = Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1;

    n = (Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1);
    z = (res == 0);
    c = ((static_cast<uint64_t>(op1) + static_cast<uint64_t>(op2)) != res);
    //v = ( iOp1 + iOp2 != static_cast<int32_t>(res));
    v = (!op1Sign && !op2Sign && resSign) || (op1Sign && op2Sign && !resSign);

    return res;
}

// https://wwww.dmulholl.com/notes/arm-condition-flags.html
uint32_t ALU::sub(uint32_t op1, uint32_t op2){
    uint32_t res = op1 - op2;

    bool op1Sign = Utils::getRegBits(op1, MSB_MASK, MSB_SHIFT) == 1;
    bool op2Sign = Utils::getRegBits(op2, MSB_MASK, MSB_SHIFT) == 1;
    bool resSign = Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1;

    n = (Utils::getRegBits(res, MSB_MASK, MSB_SHIFT) == 1);
    z = (res == 0);
    c = op2 <= op1;
    // Before
    //c = op2 < op1;
    //c = op2 >= op1;
    v = (!op1Sign && op2Sign && resSign) || (op1Sign && !op2Sign && !resSign);

    return res;
}


bool ALU::getN(){
    return n;
}

bool ALU::getZ(){
    return z;
}

bool ALU::getC(){
    return c;
}

bool ALU::getV(){
    return v;
}