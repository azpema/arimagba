#ifndef _PALETTE_RAM_
#define _PALETTE_RAM_

#include "generic_memory.hpp"
#include <iostream>

class PaletteRAM : public GenericMemory {
  public:
    PaletteRAM();
    const static uint32_t BG_OFFSET = 0;
    const static uint32_t OBJ_OFFSET = 0x200;
};

#endif
