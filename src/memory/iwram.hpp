#ifndef _IWRAM_ 
#define _IWRAM_ 

#include <iostream>
#include "generic_memory.hpp"

class IWRAM : public GenericMemory<0x8000 / 2>{
    public:
        IWRAM();
        
    private:
        // 16 bit access
        const static uint32_t IWRAM_SIZE_2WORDS = 0x8000 / 2;
                
};

#endif
