#include "oam.hpp"

OAM::OAM() {
    std::cerr << "TODO Set proper initial values at OAM" << std::endl;
    for(size_t i=0; i<OAM_SIZE_2WORDS; i++){
        mem[i] = 0x0000;
    }
}
