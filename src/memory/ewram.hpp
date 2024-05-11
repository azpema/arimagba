#ifndef _EWRAM_ 
#define _EWRAM_ 

#include <iostream>
#include "generic_memory.hpp"

class EWRAM : public GenericMemory<0x40000 / 2>{
    public:
        EWRAM();
        
    private:
        // 16 bit access
        const static uint32_t EWRAM_SIZE_2WORDS = 0x40000 / 2;
                
};

#endif
