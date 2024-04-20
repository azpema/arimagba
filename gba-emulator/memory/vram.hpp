#ifndef _VRAM_ 
#define _VRAM_ 

#include <iostream>

class VRAM {
    public:
        VRAM();
        uint32_t read(uint32_t addr, uint8_t bytes);
        void store(uint32_t addr, uint32_t val, uint8_t bytes);

        uint16_t* getRawMemory();
        
    private:
        // 16 bit access
        const static uint32_t VRAM_SIZE_2WORDS = 0x18000 / 2;
        uint16_t mem[VRAM_SIZE_2WORDS];
        
};

#endif
