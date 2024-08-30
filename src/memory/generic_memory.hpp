#ifndef _GENERIC_MEMORY_ 
#define _GENERIC_MEMORY_ 

#include <iostream>

const static uint32_t BIOS_SIZE_2WORDS = 0x4000 / 2;
const static uint32_t EWRAM_SIZE_2WORDS = 0x40000 / 2;
const static uint32_t IOREGISTERS_SIZE_2WORDS = 0x3FF / 2;
const static uint32_t IWRAM_SIZE_2WORDS = 0x8000 / 2;
const static uint32_t OAM_SIZE_2WORDS = 0x400 / 2;
const static uint32_t PALETTERAM_SIZE_2WORDS = 0x400 / 2;
const static uint32_t SRAM_SIZE_2WORDS = 0x10000 / 2;
const static uint32_t VRAM_SIZE_2WORDS = 0x18000 / 2;

template <std::uint32_t n>
class GenericMemory {
    public:
        //GenericMemory();
        //virtual ~GenericMemory();
        //uint32_t read(uint32_t addr, uint8_t bytes);
        //void store(uint32_t addr, uint32_t val, uint8_t bytes);

        //uint16_t* getRawMemory();
        
        
    protected:
        // 16 bit access
        uint16_t mem[n];

    public:
        uint32_t read(uint32_t addr, uint8_t bytes){
            uint32_t val=0;
            if(bytes == 1){
                // TODO What if address is unaligned?
                if(addr % 2 == 0){
                    //std::runtime_error("TODO: Unaligned memory access in GenericMemory::read bytes==1");
                    val = mem[addr/2] & 0x00FF;
                }else if(addr % 2 == 1){
                    val = (mem[addr/2] & 0xFF00) >> 8;
                }
                
            }else if(bytes == 2){
                if(addr % 2 != 0){
                    throw std::runtime_error("TODO: Unaligned memory access in GenericMemory::read bytes==2");
                }else {
                    val = mem[addr / 2];
                }
            }else if(bytes == 4){
                if(addr % 4 != 0){
                    throw std::runtime_error("TODO: Unaligned memory access in GenericMemory::read bytes==4");
                }else{
                    val = mem[addr/2];
                    val |= mem[addr/2 + 1] << 16;
                }
            }else{
                throw std::runtime_error("ERROR: Invalid bytes = " + std::to_string(bytes) + " value in GenericMemory::read");
            }

            return val;
        }

        void store(uint32_t addr, uint32_t val, uint8_t bytes){
            if(bytes == 1){
                if(addr % 2 == 0){
                    mem[addr / 2] = (mem[addr/2] & 0xFF00) | val;
                }else if(addr % 2 == 1){
                    mem[addr / 2] = (mem[addr/2] & 0x00FF) | (val << 8);
                }
                
            }else if(bytes == 2){
                if(addr % 2 != 0){
                    throw std::runtime_error("TODO: Unaligned memory access in GenericMemory::store bytes==2");
                }
                mem[addr / 2] = val;
            }else if(bytes == 4){
                if(addr % 4 != 0){
                    throw std::runtime_error("TODO: Unaligned memory access in GenericMemory::store bytes==4");
                }
                mem[addr/2] = val & 0xFFFF;
                mem[addr/2 + 1] = (val & 0xFFFF0000) >> 16;
            }else {
                throw std::runtime_error("ERROR: Invalid bytes = " + std::to_string(bytes) + " value in GenericMemory::store");
            }
        }

        uint16_t* getRawMemory(){
            return mem;
        }
        
};

#endif
