#include "barrel_shifter.hpp"

BarrelShifter::BarrelShifter(){
    c = false;
}

// 32 BIT !!
// LSL #0 is a special case, where the shifter carry out is the old value of the CPSR C
// flag. The contents of Rm are used directly as the second operand.
uint32_t BarrelShifter::lsl(ARM7TDMI &cpu, uint32_t val, uint32_t n){
    if(n != 0){
        c = Utils::getRegSingleBit(val, 32 - n);
    }else {
        c = cpu.getCPSR().getCFlag();
    }
        
    return val << n;
}

// The form of the shift field which might be expected to correspond to LSR #0 is used to
// encode LSR #32, which has a zero result with bit 31 of Rm as the carry output. Logical
// shift right zero is redundant as it is the same as logical shift left zero, so the assembler
// will convert LSR #0 (and ASR #0 and ROR #0) into LSL #0, and allow LSR #32 to be
// specified.
uint32_t BarrelShifter::lsr(uint32_t val, uint32_t n){
    if(n == 0)
        n = 32;

    c = Utils::getRegSingleBit(val, n-1);
    return val >> n;
}

uint32_t BarrelShifter::asr(uint32_t val, uint32_t n){
    if(n == 0)
        n = 32;

    c = Utils::getRegSingleBit(val, n-1);

    const uint32_t highBit = Utils::getRegSingleBit(val, 31);
    uint32_t keepSign = Utils::getRegSingleBit(val, 31);
    for(size_t i = 1; i < n; i++){
        keepSign = (keepSign >> 1) | highBit;
    }
    
    return (val >> n) | keepSign;
}

uint32_t BarrelShifter::ror(ARM7TDMI &cpu, uint32_t val, uint32_t n){
    if(n == 0)
        return rrx(cpu, val);

    uint32_t res = val;
    for (size_t i = 0; i < n; i++) {
        uint32_t zerothBit = res & 0x1;
        res >>= 1;
        res |= (zerothBit << 31);
    }
    return res;

}

uint32_t BarrelShifter::rrx(ARM7TDMI &cpu, uint32_t val){
    c = Utils::getRegSingleBit(val, 0);
    return (val >> 1) | (cpu.getCPSR().getCFlag() << 31);
}


bool BarrelShifter::getC(){
    return c;
}
