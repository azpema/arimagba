#ifndef _GENERIC_MEMORY_ 
#define _GENERIC_MEMORY_ 

#include <iostream>
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
            }else if(bytes == 1){
                // TODO What if address is unaligned?
                val = mem[addr/2] & 0x00FF;
            }else{
                throw std::runtime_error("ERROR: Unsupported read byte amount in GenericMemory::read");
            }

            return val;
        }

        void store(uint32_t addr, uint32_t val, uint8_t bytes){ 
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
                mem[addr/2 + 1] = (val & 0xFFFF0000) >> 16;
            }
        }

        uint16_t* getRawMemory(){
            return mem;
        }
        
};

#endif
