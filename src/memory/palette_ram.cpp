#include "palette_ram.hpp"

PaletteRAM::PaletteRAM() {
    std::cerr << "TODO Set proper initial values at WRAM" << std::endl;
    for(size_t i=0; i<PALETTERAM_SIZE; i++){
        mem[i] = 0x00;
    }
}
