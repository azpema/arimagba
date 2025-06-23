#ifndef _GENERIC_MEMORY_
#define _GENERIC_MEMORY_

#include <iostream>
#include <memory>

const static uint32_t BIOS_SIZE = 0x4000;
const static uint32_t EWRAM_SIZE = 0x40000;
const static uint32_t ROM_SIZE = 0x2000000;
const static uint32_t IOREGISTERS_SIZE = 0x800;
const static uint32_t IWRAM_SIZE = 0x8000;
const static uint32_t OAM_SIZE = 0x400;
const static uint32_t PALETTERAM_SIZE = 0x400;
const static uint32_t SRAM_SIZE = 0x10000;
const static uint32_t VRAM_SIZE = 0x18000;

class GenericMemory {
  public:
    GenericMemory(size_t size, bool fillWith1s = false);
    ~GenericMemory() = default;
    uint64_t read(uint32_t addr, uint8_t bytes);
    void store(uint32_t addr, uint64_t val, uint8_t bytes);
    uint8_t* getRawMemory();

  protected:
    size_t n;
    std::unique_ptr<uint8_t[]> mem;
};

#endif
