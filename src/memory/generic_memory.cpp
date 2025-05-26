#include "generic_memory.hpp"

GenericMemory::GenericMemory(size_t size) : n(size), mem(std::make_unique<uint8_t[]>(n)) { }
    
uint32_t GenericMemory::read(uint32_t addr, uint8_t bytes) {
    if (addr + bytes > n) {
        // Out of bounds, return Openbus val?
        return 0;
    }
    uint32_t val = 0;
    if (bytes == 1) {
        val = mem[addr];
    } else if (bytes == 2) {
        if (addr % 2 != 0) {
            throw std::runtime_error("TODO: Unaligned memory access in GenericMemory::read bytes==2");
        } else {
            val = mem[addr];
            val |= mem[addr + 1] << 8;
        }
    } else if (bytes == 4) {
        if (addr % 4 != 0) {
            throw std::runtime_error("TODO: Unaligned memory access in GenericMemory::read bytes==4");
        } else {
            val = mem[addr];
            val |= mem[addr + 1] << 8;
            val |= mem[addr + 2] << 16;
            val |= mem[addr + 3] << 24;
        }
    } else {
        throw std::runtime_error("ERROR: Invalid bytes = " + std::to_string(bytes) +
                                    " value in GenericMemory::read");
    }

    return val;
}

void GenericMemory::store(uint32_t addr, uint32_t val, uint8_t bytes) {
    if (addr + bytes > n) {
        // Out of bounds, do nothing
        return;
    }
    if (bytes == 1) {
        mem[addr] = val;
    } else if (bytes == 2) {
        if (addr % 2 != 0) {
            throw std::runtime_error("TODO: Unaligned memory access in GenericMemory::store bytes==2");
        }
        mem[addr] = val & 0xFF;
        mem[addr + 1] = (val & 0xFF00) >> 8;
    } else if (bytes == 4) {
        if (addr % 4 != 0) {
            throw std::runtime_error("TODO: Unaligned memory access in GenericMemory::store bytes==4");
        }
        mem[addr] = val & 0xFF;
        mem[addr + 1] = (val & 0xFF00) >> 8;
        mem[addr + 2] = (val & 0xFF0000) >> 16;
        mem[addr + 3] = (val & 0xFF000000) >> 24;
    } else {
        throw std::runtime_error("ERROR: Invalid bytes = " + std::to_string(bytes) +
                                    " value in GenericMemory::store");
    }
}

uint8_t* GenericMemory::getRawMemory() { return mem.get(); }