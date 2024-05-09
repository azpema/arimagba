#ifndef _GENERIC_MEMORY_ 
#define _GENERIC_MEMORY_ 

#include <iostream>

class GenericMemory {
    public:
        GenericMemory(uint32_t size);
        virtual ~GenericMemory();
        uint32_t read(uint32_t addr, uint8_t bytes);
        void store(uint32_t addr, uint32_t val, uint8_t bytes);

        uint16_t* getRawMemory();
        
    protected:
        // 16 bit access
        uint16_t* mem;
        
};

#endif
