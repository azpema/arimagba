#ifndef _EWRAM_
#define _EWRAM_

#include "generic_memory.hpp"
#include <iostream>

class EWRAM : public GenericMemory<EWRAM_SIZE> {
  public:
    EWRAM();
};

#endif
