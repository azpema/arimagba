#ifndef _WRAM_ 
#define _WRAM_ 

#include <iostream>

class WRAM {
    public:
        WRAM();
        uint32_t read(uint32_t addr, uint8_t bytes);
        void store(uint32_t addr, uint32_t val, uint8_t bytes);

        uint16_t* getRawMemory();
        
    private:
        // 16 bit access
        const static uint32_t WRAM_SIZE_2WORDS = 0x8000 / 2;
        uint16_t mem[WRAM_SIZE_2WORDS];
        
};

#endif
