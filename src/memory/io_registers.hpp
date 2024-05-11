#ifndef _IOregisters_ 
#define _IOregisters_

#include <iostream>
#include "generic_memory.hpp"

class IOregisters : public GenericMemory<0x3FF / 2>{
    public:
        IOregisters();

    private:
        // 16 bit access
        const static uint32_t IOREGISTERS_SIZE_2WORDS = 0x3FF / 2;
        
};

#endif
