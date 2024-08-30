#ifndef _EWRAM_ 
#define _EWRAM_ 

#include <iostream>
#include "generic_memory.hpp"

class EWRAM : public GenericMemory<EWRAM_SIZE>{
    public:
        EWRAM();  
};

#endif
