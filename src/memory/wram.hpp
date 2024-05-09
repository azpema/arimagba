#ifndef _WRAM_ 
#define _WRAM_ 

#include <iostream>
#include "generic_memory.hpp"

class WRAM : public GenericMemory{
    public:
        WRAM();
        
    private:
        // 16 bit access
        const static uint32_t WRAM_SIZE_2WORDS = 0x8000 / 2;
                
};

#endif
