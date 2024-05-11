#ifndef _VRAM_ 
#define _VRAM_ 

#include <iostream>
#include "generic_memory.hpp"

class VRAM : public GenericMemory<0x18000 / 2>{
    public:
        VRAM();

    private:
        // 16 bit access
        const static uint32_t VRAM_SIZE_2WORDS = 0x18000 / 2;
        
};

#endif
