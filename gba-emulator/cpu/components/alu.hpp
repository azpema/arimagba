#ifndef _ALU_ 
#define _ALU_ 

#include <iostream>


class ALU {
    public:
        ALU();
        //uint32_t cmp(uint32_t op1, uint32_t op2);

        bool getN();
        bool getZ();
        bool getC();
        bool getV();
        
    private:
        bool n, z, c, v;
};

#endif

