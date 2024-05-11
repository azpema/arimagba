#include "vram.hpp"

VRAM::VRAM() {
    std::cerr << "TODO Set proper initial values at VRAM" << std::endl;
    for(size_t i=0; i<VRAM_SIZE_2WORDS; i++){
        mem[i] = 0x0000;
    }
}
