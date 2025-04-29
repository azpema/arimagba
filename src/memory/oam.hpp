#ifndef _OAM_
#define _OAM_

#include "generic_memory.hpp"
#include <iostream>

class OAM : public GenericMemory<OAM_SIZE> {
  public:
    OAM();
};

#endif
