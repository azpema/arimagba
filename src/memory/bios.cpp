#include "bios.hpp"
#include "../utils/utils.hpp"
#include <fstream>

BIOS::BIOS(const std::string& filePath) : GenericMemory(BIOS_SIZE) {
    std::ifstream fileStream = std::ifstream(filePath, std::ios::binary | std::ifstream::ate);
    if (!fileStream) {
        throw std::runtime_error("ERROR: BIOS Failed to open the file.");
    }

    uint32_t fileSize = fileStream.tellg();
    fileStream.seekg(0, std::ios::beg);

    if (!fileStream.is_open()) {
        throw std::runtime_error("ERROR: BIOS Failed to open the file.");
    } else {
        fileStream.read(reinterpret_cast<char*>(mem.get()), fileSize);
        if (!fileStream) {
            throw std::runtime_error("ERROR: GamePak reading");
        }
    }
}

uint32_t BIOS::readWrapper(uint32_t addr, uint8_t bytes, bool isPcInBios) {
    uint32_t val = 0;
    if (isPcInBios) {
        val = read(addr, bytes);
        lastValidRead = val;
    } else {
        if (bytes == 4) {
            val = lastValidRead;
        } else if (bytes == 2) {
            if (addr % 4 == 0) {
                val = lastValidRead & 0x0000FFFF;
            } else if (addr % 4 == 2) {
                val = (lastValidRead & 0xFFFF0000) >> 16;
            }
        } else {
            if (addr % 4 == 0) {
                val = lastValidRead & 0x000000FF;
            } else if (addr % 4 == 1) {
                val = (lastValidRead & 0x0000FF00) >> 8;
            } else if (addr % 4 == 2) {
                val = (lastValidRead & 0x00FF0000) >> 16;
            } else if (addr % 4 == 3) {
                val = (lastValidRead & 0xFF000000) >> 24;
            }
        }
    }

    return val;
}
