#ifndef _IOregisters_ 
#define _IOregisters_

#include <iostream>
#include "generic_memory.hpp"

namespace REG_ADDR{
    const static uint32_t DISPCNT       = 0x04000000;
    const static uint32_t GREEN_SWAP    = 0x04000002;
    const static uint32_t DISPSTAT      = 0x04000004;
    const static uint32_t VCOUNT        = 0x04000006;

    // Background control
    const static uint32_t BG0CNT        = 0x04000008;
    const static uint32_t BG1CNT        = 0x0400000A;
    const static uint32_t BG2CNT        = 0x0400000C;
    const static uint32_t BG3CNT        = 0x0400000E;

    const static uint32_t BG0HOFS       = 0x04000010;
    const static uint32_t BG0VOFS       = 0x04000012;

    const static uint32_t BG1HOFS       = 0x04000014;
    const static uint32_t BG1VOFS       = 0x04000016;

    const static uint32_t BG2HOFS       = 0x04000018;
    const static uint32_t BG2VOFS       = 0x0400001A;

    const static uint32_t BG3HOFS       = 0x0400001C;
    const static uint32_t BG3VOFS       = 0x0400001E;
}


class IOregisters : public GenericMemory<IOREGISTERS_SIZE>{
    public:
        IOregisters();
};

#endif
