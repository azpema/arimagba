#ifndef _IOregisters_ 
#define _IOregisters_

#include <iostream>
#include "generic_memory.hpp"

class IOregisters : public GenericMemory<IOREGISTERS_SIZE_2WORDS>{
    public:
        IOregisters();
};

#endif
