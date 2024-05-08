#ifndef _PALETTE_RAM_ 
#define _PALETTE_RAM_ 

#include <iostream>

class PaletteRAM {
    public:
        PaletteRAM();
        uint32_t read(uint32_t addr, uint8_t bytes);
        void store(uint32_t addr, uint32_t val, uint8_t bytes);

        uint16_t* getRawMemory();
        
    private:
        // 16 bit access
        const static uint32_t PALETTERAM_SIZE_2WORDS = 0x400 / 2;
        uint16_t mem[PALETTERAM_SIZE_2WORDS];
        
};

#endif
