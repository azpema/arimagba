#ifndef _SRAM_ 
#define _SRAM_ 

#include <iostream>
#include "generic_memory.hpp"

class SRAM : public GenericMemory<0x10000 / 2>{
    public:
        SRAM();
        
    private:
        // 16 bit access
        const static uint32_t SRAM_SIZE_2WORDS = 0x10000 / 2;
                
};

#endif
