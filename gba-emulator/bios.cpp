#include "bios.hpp"

BIOS::BIOS(std::string path){
    this->fileStream = std::ifstream(path, std::ios::binary);

    if (!this->fileStream.is_open()) {
        std::cerr << "ERROR: Failed to open the file." << std::endl;
    }

}

uint32_t BIOS::readBytes(uint32_t offset, uint32_t nBytes){
    if(nBytes <= 0 || nBytes > 4){
        std::cerr << "ERROR: Invalid number of bytes to read" << std::endl;
        return -1;
    }

    uint32_t word;
    this->fileStream.seekg(offset, std::ios::beg);
    this->fileStream.read(reinterpret_cast<char*>(&word), nBytes);
    if (this->fileStream)
        return word;
    else{
        std::cerr << "ERROR: BIOS readBytes offset=" << offset << " nBytes=" << nBytes << std::endl;
        return -1;
    }
        return -1;
}

uint32_t BIOS::readWord(uint32_t offset){
    if (offset % 4 != 0){
        std::cerr << "ERROR: Offset " << std::hex << offset << std::dec << " is not aligned to word size" << std::endl;
        return -1;
    }

    return readBytes(offset, 4);
}

uint16_t BIOS::readHalfWord(uint32_t offset){
    if (offset % 2 != 0){
        std::cerr << "ERROR: Offset " << std::hex << offset << std::dec << " is not aligned to half word size" << std::endl;
        return -1;
    }

    return readBytes(offset, 2);
}
