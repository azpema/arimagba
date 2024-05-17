#include "bios.hpp"
#include <fstream>

BIOS::BIOS(std::string filePath) {
    std::cerr << "TODO Set proper initial values at EWRAM" << std::endl;
    for(size_t i=0; i<BIOS_SIZE_2WORDS; i++){
        mem[i] = 0x0000;
    }

    std::ifstream fileStream = std::ifstream(filePath, std::ios::binary | std::ifstream::ate);
    if (!fileStream) {
        std::cerr << "ERROR: BIOS Failed to open the file." << std::endl;
    }

    uint32_t fileSize = fileStream.tellg();
    fileStream.seekg(0, std::ios::beg);

    if (!fileStream.is_open()) {
        std::cerr << "ERROR: BIOS Failed to open the file." << std::endl;
    }else {
        fileStream.read(reinterpret_cast<char *>(mem), fileSize);
        if (fileStream) {
            std::cout << "DEBUG: BIOS READ OK" << std::endl;
        } else{
            std::cerr << "ERROR: GamePak reading" << std::endl;
        }
    }
}
