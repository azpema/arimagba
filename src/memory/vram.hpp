#ifndef _VRAM_ 
#define _VRAM_ 

#include <iostream>
#include "generic_memory.hpp"

class VRAM : public GenericMemory<VRAM_SIZE>{
    public:
        VRAM();
        const static uint32_t OBJECT_VRAM_OFFSET = 0x10000;
};

#endif
