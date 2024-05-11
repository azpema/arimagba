#ifndef _ALU_ 
#define _ALU_ 

#include <iostream>
#include "../../utils/utils.hpp"

// https://teaching.idallen.com/dat2343/10f/notes/040_overflow.txt
class ALU {
    public:
        ALU();
        uint32_t add(uint32_t op1, uint32_t op2);
        uint32_t sub(uint32_t op1, uint32_t op2);
        uint32_t andOp(uint32_t op1, uint32_t op2);
        uint32_t mvn(uint32_t op2);

        bool getN();
        bool getZ();
        bool getC();
        bool getV();
        
    private:
        // https://stackoverflow.com/questions/53712945/when-is-overflow-flag-set
        const static uint32_t MSB_MASK = 0x80000000;
        const static uint32_t MSB_SHIFT = 31;
        bool n, z, c, v;
};

#endif

