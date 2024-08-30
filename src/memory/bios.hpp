#ifndef _BIOS_ 
#define _BIOS_ 

#include <iostream>
#include "generic_memory.hpp"

class BIOS : public GenericMemory<BIOS_SIZE>{
    public:
        BIOS(std::string filePath);      
};

#endif
