#ifndef _BIOS_ 
#define _BIOS_ 

#include <iostream>
#include "generic_memory.hpp"

class BIOS : public GenericMemory<BIOS_SIZE>{
    public:
        BIOS(const std::string &filePath);
        uint32_t readWrapper(uint32_t addr, uint8_t bytes, bool isPcInBios);

    private:
        uint32_t lastValidRead;
};

#endif
