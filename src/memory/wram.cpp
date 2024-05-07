#include "wram.hpp"

WRAM::WRAM(){
    std::cerr << "TODO Set proper initial values at WRAM" << std::endl;
    for(size_t i=0; i<WRAM_SIZE_2WORDS; i++){
        mem[i] = 0x0000;
    }
}

uint32_t WRAM::read(uint32_t addr, uint8_t bytes){
    uint32_t val=0;

    if(bytes == 2){
        if(addr % 2 != 0){
            std::cerr << "TODO: Unaligned memory access in VRAM::store" << std::endl;
        }else {
            val = mem[addr / 2];
        }
    }else if(bytes == 4){
        if(addr % 4 != 0){
            std::cerr << "TODO: Unaligned memory access in VRAM::store" << std::endl;
        }else{
            val = mem[addr/2];
            val |= mem[addr/2 + 1] << 16;
        }
    }

    return val;
}

void WRAM::store(uint32_t addr, uint32_t val, uint8_t bytes){ 
    if(bytes == 2){
        if(addr % 2 != 0){
            std::cerr << "TODO: Unaligned memory access in WRAM::store" << std::endl;
            return;
        }
        mem[addr / 2] = val;
    }else if(bytes == 4){
        if(addr % 4 != 0){
            std::cerr << "TODO: Unaligned memory access in WRAM::store" << std::endl;
            return;
        }
        mem[addr/2] = val & 0xFFFF;
        mem[addr/2 + 1] = (val & 0xFFFF0000) >> 16;
    }
}

uint16_t* WRAM::getRawMemory(){
    return mem;
}
