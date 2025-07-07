#include "generic_memory.hpp"
#include <cstring>

GenericMemory::GenericMemory(size_t size, bool fillWith1s) : n(size), mem(std::make_unique<uint8_t[]>(n)) { 
    if (fillWith1s) {
        std::fill(mem.get(), mem.get() + n, 0xFF);
    }
}
    
uint64_t GenericMemory::read(uint32_t addr, uint8_t bytes) {
    if (addr + bytes > n) {
        // Out of bounds, return Openbus val?
        return 0;
    }

    if ((bytes == 2 && addr % 2 != 0) || (bytes == 4 && addr % 4 != 0)) {
        throw std::runtime_error("TODO: Unaligned access in GenericMemory::read bytes=" + std::to_string(bytes));
    }

    uint64_t val = 0;
    std::memcpy(&val, &mem[addr], bytes);

    return val;
}

void GenericMemory::store(uint32_t addr, uint64_t val, uint8_t bytes) {
    if (addr + bytes > n) {
        // Out of bounds, do nothing
        return;
    }

    if ((bytes == 2 && addr % 2 != 0) || (bytes == 4 && addr % 4 != 0)) {
        throw std::runtime_error("TODO: Unaligned access in GenericMemory::store bytes=" + std::to_string(bytes));
    }

    std::memcpy(&mem[addr], &val, bytes);
}

uint8_t* GenericMemory::getRawMemory() { return mem.get(); }