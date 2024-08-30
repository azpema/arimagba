#ifndef _VRAM_ 
#define _VRAM_ 

#include <iostream>
#include "generic_memory.hpp"

class VRAM : public GenericMemory<VRAM_SIZE_2WORDS>{
    public:
        VRAM();
};

#endif
