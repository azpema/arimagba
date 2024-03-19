#ifndef _BIOS_ 
#define _BIOS_ 

#include <iostream>
#include <fstream>
#include <string>


class BIOS {
    public:
        BIOS();
        BIOS(std::string path);
        int64_t readWord(uint32_t offset);
        int64_t readHalfWord(uint32_t offset);
        
    private:
        int64_t readBytes(uint32_t offset, uint32_t nBytes);
        
        std::string filePath;
        std::ifstream fileStream;
        
};

#endif