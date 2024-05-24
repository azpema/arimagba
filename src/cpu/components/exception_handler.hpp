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
        enum Exception {RESET, UNDEF, SWI, PREFETCH_ABORT, DATA_ABORT, ADDRESS_EXCEED, FIQ, IRQ};

        void raiseException(ARM7TDMI &cpu, Exception ex);

    private:
        std::unordered_map<Exception, PSR::Mode> except2Mode = {
            {RESET, PSR::Supervisor},
            {UNDEF, PSR::Undefined},
            {SWI, PSR::Supervisor},
            {PREFETCH_ABORT, PSR::Abort},
            {DATA_ABORT, PSR::Abort},
            {ADDRESS_EXCEED, PSR::Supervisor},
            {FIQ, PSR::IRQ},
            {IRQ, PSR::FIQ},
        };

        std::unordered_map<Exception, uint32_t> exceptionVector = {
            {RESET,             0x00000000},
            {UNDEF,             0x00000004},
            {SWI,               0x00000008},
            {PREFETCH_ABORT,    0x0000000C},
            {DATA_ABORT,        0x00000010},
            {ADDRESS_EXCEED,    0x00000014},
            {FIQ,               0x00000018},
            {IRQ,               0x0000001C},
        };
};

#endif
