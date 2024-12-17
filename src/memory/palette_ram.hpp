#ifndef _PALETTE_RAM_ 
#define _PALETTE_RAM_ 

#include <iostream>
#include "generic_memory.hpp"

class PaletteRAM : public GenericMemory<PALETTERAM_SIZE>{
    public:
        PaletteRAM();
        const static uint32_t BG_OFFSET = 0;  
        const static uint32_t OBJ_OFFSET = 0x200;     
};

#endif
