#ifndef _EXCEPTION_HANDLER_ 
#define _EXCEPTION_HANDLER_ 

class ARM7TDMI;

#include <iostream>
#include <unordered_map>
#include "../../utils/utils.hpp"
#include "../arm7tdmi.hpp"

namespace REG_IF {
    const static uint32_t VBLANK_MASK = 0b0000000000000000;
    const static uint32_t VBLANK_SHIFT = 0;
}

class ExceptionHandler {
    public:
        ExceptionHandler(ARM7TDMI &cpu);
        enum class Exception {RESET, UNDEF, SWI, PREFETCH_ABORT, DATA_ABORT, ADDRESS_EXCEED, FIQ, IRQ};
        enum class Interrupt {VBLANK, HBLANK, VCOUNT, TIMERx, COM, DMAx, KEYPAD, GAMEPAK, DUMMY};

        void raiseException(Exception ex, Interrupt inter=Interrupt::DUMMY);
        void handleException();

    private:
        bool isMasterInterruptEnabled();
        bool isSpecificInterruptEnabled(Interrupt inter);
        void setInterruptIF(Interrupt inter, bool raise);

        Interrupt inter;
        Exception ex;
        bool mustHandleException;
        
        ARM7TDMI &cpu;
        const static uint32_t REG_IE_ADDR = 0x04000200;
        const static uint32_t REG_IE_VBLANK_OFFSET = 0x0;

        const static uint32_t REG_IF_ADDR = 0x04000202;
        const static uint32_t REG_IME_ADDR = 0x04000208;

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
};

#endif


