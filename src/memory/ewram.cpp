#include "ewram.hpp"

EWRAM::EWRAM() {
    std::cerr << "TODO Set proper initial values at EWRAM" << std::endl;
    for(size_t i=0; i<EWRAM_SIZE; i++){
        mem[i] = 0x00;
    }
}
