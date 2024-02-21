#include <iostream>
#include <fstream>
#include <string>


class BIOS {
    public:
        BIOS(std::string path);
        uint32_t readWord(uint32_t offset);
        uint16_t readHalfWord(uint32_t offset);
    private:
        uint32_t readBytes(uint32_t offset, uint32_t nBytes);
        
        std::string filePath;
        std::ifstream fileStream;
        
};