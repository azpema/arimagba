#ifndef _SRAM_ 
#define _SRAM_ 

#include <iostream>
#include "generic_memory.hpp"

class SRAM : public GenericMemory<SRAM_SIZE_2WORDS>{
    public:
        SRAM();             
};

#endif
