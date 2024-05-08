#ifndef _BARREL_SHIFTER_ 
#define _BARREL_SHIFTER_ 

class ARM7TDMI;

#include <iostream>
#include "../../utils/utils.hpp"
#include "../arm7tdmi.hpp"

class BarrelShifter {
    public:
        BarrelShifter();
        uint32_t lsl(ARM7TDMI &cpu, uint32_t val, uint32_t n);
        uint32_t lsr(uint32_t val, uint32_t n);
        uint32_t asr(uint32_t val, uint32_t n);
        uint32_t ror(ARM7TDMI &cpu, uint32_t val, uint32_t n);
        bool getC();

    private:
        uint32_t rrx(ARM7TDMI &cpu, uint32_t val);
        bool c;
};

#endif

