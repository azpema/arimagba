#include "io_registers.hpp"

IOregisters::IOregisters(){
    std::cerr << "TODO Set proper initial values at IOregisters" << std::endl;
    for(size_t i=0; i<IO_REGISTERS_SIZE_2WORDS; i++){
        mem[i] = 0x0000;
    }
}

uint32_t IOregisters::read(uint32_t addr, uint8_t bytes){
    std::cerr << "TODO read IO reg" << std::endl;
    return 0;
}

void IOregisters::store(uint32_t addr, uint32_t val, uint8_t bytes){ 
    if(bytes == 2){
        if(addr % 2 != 0){
            std::cerr << "TODO: Unaligned memory access in IOregisters::store" << std::endl;
            return;
        }
        mem[addr / 2] = val;
    }else if(bytes == 4){
        if(addr % 4 != 0){
            std::cerr << "TODO: Unaligned memory access in IOregisters::store" << std::endl;
            return;
        }
        mem[addr / 2] = val & 0xFFFF;
        mem[addr / 2 + 1] = (val & 0xFFFF0000) > 16;
    }
}