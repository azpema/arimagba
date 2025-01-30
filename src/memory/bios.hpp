#ifndef _BIOS_ 
#define _BIOS_ 

#include <iostream>
#include "generic_memory.hpp"

class BIOS : public GenericMemory<BIOS_SIZE>{
    public:
        BIOS(const std::string &filePath);
        uint32_t readWrapper(uint32_t addr, uint8_t bytes, bool isPcInBios);

        struct OPENBUS_ADDR {
            const static uint32_t SOFTRESET_EXIT    = 0x000000DC;
            const static uint32_t IRQ_ENTRY         = 0x00000134;
            const static uint32_t IRQ_EXIT          = 0x0000013C;
            const static uint32_t SWI_EXIT          = 0x00000188;
        };

    private:
        uint32_t lastValidRead;
};

#endif
