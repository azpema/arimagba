#ifndef _BIOS_ 
#define _BIOS_ 

#include <iostream>
#include "generic_memory.hpp"

class BIOS : public GenericMemory<BIOS_SIZE>{
    public:
        BIOS(const std::string &filePath);      
};

#endif
