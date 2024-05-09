#include "generic_memory.hpp"

GenericMemory::GenericMemory(uint32_t size){
    mem = new uint16_t[size];
}

GenericMemory::~GenericMemory() {
    delete[] mem;
}

uint32_t GenericMemory::read(uint32_t addr, uint8_t bytes){
    uint32_t val=0;

    if(bytes == 2){
        if(addr % 2 != 0){
            std::cerr << "TODO: Unaligned memory access in GenericMemory::store" << std::endl;
        }else {
            val = mem[addr / 2];
        }
    }else if(bytes == 4){
        if(addr % 4 != 0){
            std::cerr << "TODO: Unaligned memory access in GenericMemory::store" << std::endl;
        }else{
            val = mem[addr/2];
            val |= mem[addr/2 + 1] << 16;
        }
    }

    return val;
}

void GenericMemory::store(uint32_t addr, uint32_t val, uint8_t bytes){ 
    if(bytes == 2){
        if(addr % 2 != 0){
            std::cerr << "TODO: Unaligned memory access in GenericMemory::store" << std::endl;
            return;
        }
        mem[addr / 2] = val;
    }else if(bytes == 4){
        if(addr % 4 != 0){
            std::cerr << "TODO: Unaligned memory access in GenericMemory::store" << std::endl;
            return;
        }
        mem[addr/2] = val & 0xFFFF;
        mem[addr/2 + 1] = (val & 0xFFFF0000) > 16;
    }
}

uint16_t* GenericMemory::getRawMemory(){
    return mem;
}
