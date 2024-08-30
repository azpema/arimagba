#ifndef _OAM_ 
#define _OAM_ 

#include <iostream>
#include "generic_memory.hpp"

class OAM : public GenericMemory<OAM_SIZE>{
    public:
        OAM();        
};

#endif
