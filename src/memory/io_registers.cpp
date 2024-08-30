#include "io_registers.hpp"

IOregisters::IOregisters() {
    std::cerr << "TODO Set proper initial values at IOregisters" << std::endl;
    for(size_t i=0; i<IOREGISTERS_SIZE; i++){
        mem[i] = 0x00;
    }
}
