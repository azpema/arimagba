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
    constexpr static uint32_t BGxCNT[4]    = { 0x04000008, 0x0400000A, 0x0400000C, 0x0400000E };

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

    constexpr static uint32_t DMAxSAD[4]    = { 0x040000B0, 0x040000BC, 0x040000C8, 0x040000D4 };
    constexpr static uint32_t DMAxDAD[4]    = { 0x040000B4, 0x040000C0, 0x040000CC, 0x040000D8 };
    constexpr static uint32_t DMAxCNT_L[4]  = { 0x040000B8, 0x040000C4, 0x040000D0, 0x040000DC };
    constexpr static uint32_t DMAxCNT_H[4]  = { 0x040000BA, 0x040000C6, 0x040000D2, 0x040000DE };

    const static uint32_t KEYINPUT      = 0x04000130;
}

namespace REG_SIZE{
    const static uint8_t DISPSTAT      = 2;
    const static uint8_t VCOUNT        = 2;

    const static uint8_t BGxCNT        = 2;

    const static uint32_t DMAxSAD      = 4;
    const static uint32_t DMAxDAD      = 4;
    const static uint32_t DMAxCNT_L    = 2;
    const static uint32_t DMAxCNT_H    = 2;

    const static uint8_t IE            = 2;
    const static uint8_t IF            = 2;
    const static uint8_t WAITCNT       = 4;
    const static uint8_t IME           = 2;

    const static uint8_t HALTCNT       = 1;
    const static uint8_t KEYINPUT      = 2;
}

namespace REG_ADDR_END{
    constexpr static uint32_t DISPSTAT  = REG_ADDR::DISPSTAT + REG_SIZE::DISPSTAT;
    constexpr static uint32_t VCOUNT    = REG_ADDR::VCOUNT + REG_SIZE::VCOUNT;

    constexpr static uint32_t BG0CNT    = REG_ADDR::BGxCNT[0] + REG_SIZE::BGxCNT;
    constexpr static uint32_t BG1CNT    = REG_ADDR::BGxCNT[1] + REG_SIZE::BGxCNT;

    constexpr static uint32_t DMAxSAD[4]    = { REG_ADDR::DMAxSAD[0] + REG_SIZE::DMAxSAD,
                                                REG_ADDR::DMAxSAD[1] + REG_SIZE::DMAxSAD,
                                                REG_ADDR::DMAxSAD[2] + REG_SIZE::DMAxSAD,
                                                REG_ADDR::DMAxSAD[3] + REG_SIZE::DMAxSAD,
                                              };

    constexpr static uint32_t DMAxDAD[4]    = { REG_ADDR::DMAxDAD[0] + REG_SIZE::DMAxDAD,
                                                REG_ADDR::DMAxDAD[1] + REG_SIZE::DMAxDAD,
                                                REG_ADDR::DMAxDAD[2] + REG_SIZE::DMAxDAD,
                                                REG_ADDR::DMAxDAD[3] + REG_SIZE::DMAxDAD,
                                              };

    constexpr static uint32_t DMAxCNT_L[4]  = { REG_ADDR::DMAxCNT_L[0] + REG_SIZE::DMAxCNT_L,
                                                REG_ADDR::DMAxCNT_L[1] + REG_SIZE::DMAxCNT_L,
                                                REG_ADDR::DMAxCNT_L[2] + REG_SIZE::DMAxCNT_L,
                                                REG_ADDR::DMAxCNT_L[3] + REG_SIZE::DMAxCNT_L,
                                              };

    constexpr static uint32_t DMAxCNT_H[4]  = { REG_ADDR::DMAxCNT_H[0] + REG_SIZE::DMAxCNT_H,
                                                REG_ADDR::DMAxCNT_H[1] + REG_SIZE::DMAxCNT_H,
                                                REG_ADDR::DMAxCNT_H[2] + REG_SIZE::DMAxCNT_H,
                                                REG_ADDR::DMAxCNT_H[3] + REG_SIZE::DMAxCNT_H,
                                              };

    constexpr static uint32_t IE        = REG_ADDR::IE + REG_SIZE::IE;
    constexpr static uint32_t IF        = REG_ADDR::IF + REG_SIZE::IF;
    constexpr static uint32_t IME       = REG_ADDR::IME + REG_SIZE::IME;
    constexpr static uint32_t HALTCNT   = REG_ADDR::HALTCNT + REG_SIZE::HALTCNT;
    constexpr static uint32_t KEYINPUT  = REG_ADDR::KEYINPUT + REG_SIZE::KEYINPUT;
}

class IOregisters : public GenericMemory<IOREGISTERS_SIZE>{
    public:
        IOregisters();
        void storeWrapper(uint32_t addr, uint32_t val, uint8_t bytes);
        void store32(uint32_t addr, uint32_t val);
        void store16(uint32_t addr, uint32_t val);
        void store8(uint32_t addr, uint32_t val);

        uint32_t getDISPCNT();
        uint32_t getIE();
        uint32_t getIF();
        uint8_t getDCNT_MODE();
        bool getMustHaltCpu();
        void clearMustHaltCpu();

    private:
        void writeCallbackDISPSTAT(const uint16_t prevVal);
        void writeCallbackBG0CNT();
        void writeCallbackBG1CNT();

        void writeCallbackDMAxCNT_LO(const uint8_t dma);
        void writeCallbackDMAxCNT_HI(const uint8_t dma);
        void writeCallbackDMAxSAD(const uint8_t dma);
        void writeCallbackDMAxDAD(const uint8_t dma);

        void writeCallbackIME();
        void writeCallbackIE();
        void writeCallbackIF(const uint16_t prevVal);
        void writeCallbackHALTCNT();

        void writeDISPSTAT(const uint16_t writtenVal);
        void writeBGxCNT(const int bg, const uint16_t writtenVal);

        void writeDMAxCNT_L(const uint8_t dma, const uint16_t writtenVal);
        void writeDMAxCNT_H(const uint8_t dma, const uint16_t writtenVal);
        void writeDMAxSAD(const uint8_t dma, const uint32_t writtenVal);
        void writeDMAxDAD(const uint8_t dma, const uint32_t writtenVal);

        void writeIME(const uint16_t writtenVal);
        void writeIE(const uint16_t writtenVal);
        void writeIF(const uint16_t writtenVal);
        void writeHALTCNT(const uint16_t writtenVal);

        uint16_t writeToIf(uint16_t val);
        void setMustHaltCpu();

        bool mustHaltCpu;

        uint32_t* DMAxSAD[4];
        uint32_t* DMAxDAD[4];
        uint16_t* DMAxCNT_L[4];
        uint16_t* DMAxCNT_H[4];

        uint16_t* BGxCNT[4];

        uint16_t* DISPSTAT;

        uint16_t* IE;
        uint16_t* IF;
        uint16_t* IME;
};

#endif
