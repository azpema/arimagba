#ifndef _PALETTE_RAM_ 
#define _PALETTE_RAM_ 

#include <iostream>
#include "generic_memory.hpp"

class PaletteRAM : public GenericMemory<PALETTERAM_SIZE_2WORDS>{
    public:
        PaletteRAM();     
};

#endif
