#ifndef _VRAM_
#define _VRAM_

#include "generic_memory.hpp"
#include <iostream>

class VRAM : public GenericMemory {
  public:
    VRAM();
    const static uint32_t OBJECT_VRAM_OFFSET = 0x10000;
};

#endif
