#ifndef _GENERIC_MEMORY_ 
#define _GENERIC_MEMORY_ 

#include <iostream>

const static uint32_t BIOS_SIZE = 0x4000;
const static uint32_t EWRAM_SIZE = 0x40000;
const static uint32_t IOREGISTERS_SIZE = 0x3FF;
const static uint32_t IWRAM_SIZE = 0x8000;
const static uint32_t OAM_SIZE = 0x400;
const static uint32_t PALETTERAM_SIZE = 0x400;
const static uint32_t SRAM_SIZE = 0x10000;
const static uint32_t VRAM_SIZE = 0x18000;

template <std::uint32_t n>
class GenericMemory {
    public:
        //GenericMemory();
        //virtual ~GenericMemory();
        //uint32_t read(uint32_t addr, uint8_t bytes);
        //void store(uint32_t addr, uint32_t val, uint8_t bytes);

        //uint16_t* getRawMemory();
        
        
    protected:
        // 8 bit access
        uint8_t mem[n];

    public:
        uint32_t read(uint32_t addr, uint8_t bytes){
            uint32_t val=0;
            if(bytes == 1){
                val = mem[addr];
            }else if(bytes == 2){
                if(addr % 2 != 0){
                    throw std::runtime_error("TODO: Unaligned memory access in GenericMemory::read bytes==2");
                }else {
                    val = mem[addr];
                    val |= mem[addr + 1] << 8;
                }
            }else if(bytes == 4){
                if(addr % 4 != 0){
                    throw std::runtime_error("TODO: Unaligned memory access in GenericMemory::read bytes==4");
                }else{
                    val = mem[addr];
                    val |= mem[addr + 1] << 8;
                    val |= mem[addr + 2] << 16;
                    val |= mem[addr + 3] << 24;
                }
            }else{
                throw std::runtime_error("ERROR: Invalid bytes = " + std::to_string(bytes) + " value in GenericMemory::read");
            }

            return val;
        }

        void store(uint32_t addr, uint32_t val, uint8_t bytes){
            if(bytes == 1){
                mem[addr] = val;
                /*if(addr % 2 == 0){
                    mem[addr / 2] = (mem[addr/2] & 0xFF00) | val;
                }else if(addr % 2 == 1){
                    mem[addr / 2] = (mem[addr/2] & 0x00FF) | (val << 8);
                }*/
                
            }else if(bytes == 2){
                if(addr % 2 != 0){
                    throw std::runtime_error("TODO: Unaligned memory access in GenericMemory::store bytes==2");
                }
                mem[addr] = val & 0xFF;
                mem[addr + 1] = (val & 0xFF00) >> 8;
            }else if(bytes == 4){
                if(addr % 4 != 0){
                    throw std::runtime_error("TODO: Unaligned memory access in GenericMemory::store bytes==4");
                }
                mem[addr] = val & 0xFF;
                mem[addr + 1] = (val & 0xFF00) >> 8;
                mem[addr + 2] = (val & 0xFF0000) >> 16;
                mem[addr + 3] = (val & 0xFF000000) >> 24;
            }else {
                throw std::runtime_error("ERROR: Invalid bytes = " + std::to_string(bytes) + " value in GenericMemory::store");
            }
        }

        uint8_t* getRawMemory(){
            return mem;
        }
        
};

#endif
