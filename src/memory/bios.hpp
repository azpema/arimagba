#ifndef _BIOS_ 
#define _BIOS_ 

#include <iostream>
#include "generic_memory.hpp"

class BIOS : public GenericMemory<0x4000 / 2>{
    public:
        BIOS(std::string filePath);
        
    private:
        // 16 bit access
        const static uint32_t BIOS_SIZE_2WORDS = 0x4000 / 2;
                
};

#endif
