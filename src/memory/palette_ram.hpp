#ifndef _PALETTE_RAM_ 
#define _PALETTE_RAM_ 

#include <iostream>
#include "generic_memory.hpp"

class PaletteRAM : public GenericMemory{
    public:
        PaletteRAM();
        
    private:
        // 16 bit access
        const static uint32_t PALETTERAM_SIZE_2WORDS = 0x400 / 2;
        
};

#endif
