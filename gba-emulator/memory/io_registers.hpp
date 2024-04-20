#ifndef _IO_REGISTERS_ 
#define _IO_REGISTERS_ 

#include <iostream>
#include <fstream>
#include <string>


class IOregisters {
    public:
        IOregisters();
        uint32_t read(uint32_t addr, uint8_t bytes);
        void store(uint32_t addr, uint32_t val, uint8_t bytes);
    private:
        // 16 bit access
        const static uint32_t IO_REGISTERS_SIZE_2WORDS = 0x3FF / 2;
        uint16_t mem[IO_REGISTERS_SIZE_2WORDS];
        
        
};

#endif