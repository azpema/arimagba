#ifndef _EXCEPTION_HANDLER_ 
#define _EXCEPTION_HANDLER_ 

class ARM7TDMI;

#include <iostream>
#include <unordered_map>
#include "../../utils/utils.hpp"
#include "../arm7tdmi.hpp"

class ExceptionHandler {
    public:
        ExceptionHandler();
        enum class Exception {RESET, UNDEF, SWI, PREFETCH_ABORT, DATA_ABORT, ADDRESS_EXCEED, FIQ, IRQ};

        void raiseException(ARM7TDMI &cpu, Exception ex);

    private:
        std::unordered_map<Exception, PSR::Mode> except2Mode = {
            {Exception::RESET,              PSR::Supervisor},
            {Exception::UNDEF,              PSR::Undefined},
            {Exception::SWI,                PSR::Supervisor},
            {Exception::PREFETCH_ABORT,     PSR::Abort},
            {Exception::DATA_ABORT,         PSR::Abort},
            {Exception::ADDRESS_EXCEED,     PSR::Supervisor},
            {Exception::FIQ,                PSR::IRQ},
            {Exception::IRQ,                PSR::FIQ},
        };

        std::unordered_map<Exception, uint32_t> exceptionVector = {
            {Exception::RESET,             0x00000000},
            {Exception::UNDEF,             0x00000004},
            {Exception::SWI,               0x00000008},
            {Exception::PREFETCH_ABORT,    0x0000000C},
            {Exception::DATA_ABORT,        0x00000010},
            {Exception::ADDRESS_EXCEED,    0x00000014},
            {Exception::FIQ,               0x00000018},    // GBA does not use this
            {Exception::IRQ,               0x00000018},
        };
};

#endif
