#include "io_registers.hpp"

IOregisters::IOregisters() : GenericMemory(IOREGISTERS_SIZE_2WORDS){
    std::cerr << "TODO Set proper initial values at IOregisters" << std::endl;
    for(size_t i=0; i<IOREGISTERS_SIZE_2WORDS; i++){
        mem[i] = 0x0003;
    }
}
