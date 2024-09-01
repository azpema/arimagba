#include "bios.hpp"
#include <fstream>

BIOS::BIOS(std::string filePath) {
    std::cerr << "TODO Set proper initial values at EWRAM" << std::endl;
    for(size_t i=0; i<BIOS_SIZE; i++){
        mem[i] = 0x00;
    }

    std::ifstream fileStream = std::ifstream(filePath, std::ios::binary | std::ifstream::ate);
    if (!fileStream) {
        throw std::runtime_error("ERROR: BIOS Failed to open the file.");
    }

    uint32_t fileSize = fileStream.tellg();
    fileStream.seekg(0, std::ios::beg);

    if (!fileStream.is_open()) {
        throw std::runtime_error("ERROR: BIOS Failed to open the file.");
    }else {
        fileStream.read(reinterpret_cast<char *>(mem), fileSize);
        if (fileStream) {
            std::cout << "DEBUG: BIOS READ OK" << std::endl;
        } else{
            throw std::runtime_error("ERROR: GamePak reading");
        }
    }
}
