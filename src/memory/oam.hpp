#ifndef _OAM_ 
#define _OAM_ 

#include <iostream>
#include "generic_memory.hpp"

class OAM : public GenericMemory<OAM_SIZE_2WORDS>{
    public:
        OAM();        
};

#endif
