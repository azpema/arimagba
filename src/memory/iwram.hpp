#ifndef _IWRAM_ 
#define _IWRAM_ 

#include <iostream>
#include "generic_memory.hpp"

class IWRAM : public GenericMemory<IWRAM_SIZE>{
    public:
        IWRAM();          
};

#endif
