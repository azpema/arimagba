#include "iwram.hpp"

IWRAM::IWRAM() {
    std::cerr << "TODO Set proper initial values at IWRAM" << std::endl;
    for(size_t i=0; i<IWRAM_SIZE; i++){
        mem[i] = 0x00;
    }
}
