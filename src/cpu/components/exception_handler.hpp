#ifndef _EXCEPTION_HANDLER_ 
#define _EXCEPTION_HANDLER_ 

class ARM7TDMI;

#include <iostream>
#include <unordered_map>
#include "../../utils/utils.hpp"
#include "../arm7tdmi.hpp"

class ExceptionHandler {
    public:
        ExceptionHandler(ARM7TDMI &cpu);
        enum class Exception {RESET, UNDEF, SWI, PREFETCH_ABORT, DATA_ABORT, ADDRESS_EXCEED, FIQ, IRQ};
        enum class Interrupt {VBLANK, HBLANK, VCOUNT, TIMER0, TIMER1, TIMER2, TIMER3, COM, DMA0, DMA1, DMA2, DMA3, KEYPAD, GAMEPAK, DUMMY};

        void raiseException(Exception ex, Interrupt inter=Interrupt::DUMMY);
        void handleException();

    private:
        void doHandleException();
        bool isMasterInterruptEnabled();
        bool isSpecificInterruptEnabled(Interrupt inter);
        void setInterruptIF(Interrupt inter, bool raise);

        //Interrupt inter;
        Exception ex;
        //bool mustHandleException;
        
        ARM7TDMI &cpu;

        std::unordered_map<Exception, PSR::Mode> except2Mode = {
            {Exception::RESET,              PSR::Supervisor},
            {Exception::UNDEF,              PSR::Undefined},
            {Exception::SWI,                PSR::Supervisor},
            {Exception::PREFETCH_ABORT,     PSR::Abort},
            {Exception::DATA_ABORT,         PSR::Abort},
            {Exception::ADDRESS_EXCEED,     PSR::Supervisor},
            {Exception::FIQ,                PSR::FIQ},
            {Exception::IRQ,                PSR::IRQ}
        };

        std::unordered_map<Exception, uint32_t> exceptionVector = {
            {Exception::RESET,             0x00000000},
            {Exception::UNDEF,             0x00000004},
            {Exception::SWI,               0x00000008},
            {Exception::PREFETCH_ABORT,    0x0000000C},
            {Exception::DATA_ABORT,        0x00000010},
            {Exception::ADDRESS_EXCEED,    0x00000014},
            {Exception::IRQ,               0x00000018},    
            {Exception::FIQ,               0x0000001C}  // GBA does not use this
        };

        uint8_t* io;
        uint16_t* IE;
        uint16_t* IF;
        uint16_t* IME;

        struct REG_IF {
            constexpr static uint32_t VBLANK_MASK = 0b0000000000000001;
            constexpr static uint32_t VBLANK_SHIFT = 0x0;

            constexpr static uint32_t HBLANK_MASK = 0b0000000000000010;
            constexpr static uint32_t HBLANK_SHIFT = 0x1;

            constexpr static uint32_t VCOUNT_MASK = 0b0000000000000100;
            constexpr static uint32_t VCOUNT_SHIFT = 0x2;

            constexpr static uint32_t DMA0_MASK = 0b0000000100000000;
            constexpr static uint32_t DMA0_SHIFT = 0x8;

            constexpr static uint32_t DMA1_MASK = 0b0000001000000000;
            constexpr static uint32_t DMA1_SHIFT = 0x9;

            constexpr static uint32_t DMA2_MASK = 0b0000010000000000;
            constexpr static uint32_t DMA2_SHIFT = 0xA;

            constexpr static uint32_t DMA3_MASK = 0b0000100000000000;
            constexpr static uint32_t DMA3_SHIFT = 0xB;
        };

        std::unordered_map<Interrupt, uint32_t> interrupt2IfMask = {
            {Interrupt::VBLANK,     REG_IF::VBLANK_MASK},
            {Interrupt::HBLANK,     REG_IF::HBLANK_MASK},
            {Interrupt::VCOUNT,     REG_IF::VCOUNT_MASK},
            {Interrupt::DMA0,       REG_IF::DMA0_MASK},
            {Interrupt::DMA1,       REG_IF::DMA1_MASK},
            {Interrupt::DMA2,       REG_IF::DMA2_MASK},
            {Interrupt::DMA3,       REG_IF::DMA3_MASK}
        };

        std::unordered_map<Interrupt, uint32_t> interrupt2IfShift = {
            {Interrupt::VBLANK,     REG_IF::VBLANK_SHIFT},
            {Interrupt::HBLANK,     REG_IF::HBLANK_SHIFT},
            {Interrupt::VCOUNT,     REG_IF::VCOUNT_SHIFT},
            {Interrupt::DMA0,       REG_IF::DMA0_SHIFT},
            {Interrupt::DMA1,       REG_IF::DMA1_SHIFT},
            {Interrupt::DMA2,       REG_IF::DMA2_SHIFT},
            {Interrupt::DMA3,       REG_IF::DMA3_SHIFT}
        };
};

#endif


