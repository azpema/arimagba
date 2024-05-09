#include "palette_ram.hpp"

PaletteRAM::PaletteRAM() : GenericMemory(PALETTERAM_SIZE_2WORDS){
    std::cerr << "TODO Set proper initial values at WRAM" << std::endl;
    for(size_t i=0; i<PALETTERAM_SIZE_2WORDS; i++){
        mem[i] = 0x0000;
    }
}
