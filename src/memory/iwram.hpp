#ifndef _IWRAM_
#define _IWRAM_

#include "generic_memory.hpp"
#include <iostream>

class IWRAM : public GenericMemory<IWRAM_SIZE> {
  public:
    IWRAM();
};

#endif
