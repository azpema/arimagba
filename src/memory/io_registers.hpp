#ifndef _IOREGISTERS_ 
#define _IOREGISTERS_

#include <iostream>
#include "generic_memory.hpp"
#include "../cpu/components/exception_handler.hpp"

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

    const static uint32_t IE            = 0x04000200;
    const static uint32_t IF            = 0x04000202;
    const static uint32_t WAITCNT       = 0x04000204;
    const static uint32_t IME           = 0x04000208;

    const static uint32_t HALTCNT       = 0x04000301;
}

namespace REG_SIZE{
    const static uint8_t IE            = 2;
    const static uint8_t IF            = 2;
    const static uint8_t WAITCNT       = 4;
    const static uint8_t IME           = 4;

    const static uint8_t HALTCNT       = 1;
}

namespace REG_ADDR_END{
    constexpr static uint32_t IE        = REG_ADDR::IE + REG_SIZE::IE;
    constexpr static uint32_t IF        = REG_ADDR::IF + REG_SIZE::IF;
    constexpr static uint32_t IME       = REG_ADDR::IME + REG_SIZE::IME;
    constexpr static uint32_t HALTCNT   = REG_ADDR::HALTCNT + REG_SIZE::HALTCNT;
}

class IOregisters : public GenericMemory<IOREGISTERS_SIZE>{
    public:
        IOregisters();
        void storeWrapper(uint32_t addr, uint32_t val, uint8_t bytes);

        uint32_t getDISPCNT();
        uint32_t getIE();
        uint32_t getIF();
        uint8_t getDCNT_MODE();
        bool getMustHaltCpu();
        void clearMustHaltCpu();

    private:
        void writeCallbackIME();
        void writeCallbackIE();
        void writeCallbackIF(const uint16_t prevVal);
        void writeCallbackHALTCNT();

        uint16_t writeToIf(uint16_t val);
        void setMustHaltCpu();

        bool mustHaltCpu;
};

#endif
