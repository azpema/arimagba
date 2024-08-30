#include "sram.hpp"

SRAM::SRAM() {
    std::cerr << "TODO Set proper initial values at SRAM" << std::endl;
    for(size_t i=0; i<SRAM_SIZE; i++){
        mem[i] = 0x00;
    }
}
