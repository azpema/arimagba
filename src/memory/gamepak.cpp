#include "gamepak.hpp"
#include <iostream>

GamePak::GamePak(std::string filePath) {
    fileStream = std::ifstream(filePath, std::ios::binary | std::ifstream::ate);
    if (!fileStream) {
        std::cerr << "ERROR: GamePak Failed to open the file." << std::endl;
    }

    fileSize = fileStream.tellg();
    fileStream.seekg(0, std::ios::beg);
    gameMem = new uint16_t[fileSize / sizeof(uint16_t)];

    if (!fileStream.is_open()) {
        std::cerr << "ERROR: GamePak Failed to open the file." << std::endl;
    }else {
        this->fileStream.read(reinterpret_cast<char *>(gameMem), fileSize);
        if (this->fileStream) {
            std::cout << "DEBUG: GamePak READ OK" << std::endl;
        } else{
            std::cerr << "ERROR: GamePak reading" << std::endl;
        }
    }
}

GamePak::~GamePak(){
    delete[] gameMem;
}


uint32_t GamePak::read(uint32_t addr, uint8_t bytes) {
    uint32_t val=0;

    if(addr > fileSize)
        val = 0;
    else{
        if(bytes == 2){
            if(addr % 2 != 0){
                std::cerr << "TODO: Unaligned memory access in VRAM::store" << std::endl;
            }else {
                val = gameMem[addr / 2];
            }
        }else if(bytes == 4){
            if(addr % 4 != 0){
                std::cerr << "TODO: Unaligned memory access in VRAM::store" << std::endl;
            }else{
                val = gameMem[addr/2];
                val |= gameMem[addr/2 + 1] << 16;
            }

        }
    }

    return val;
}

int GamePak::readEntryPoint() {
    this->fileStream.seekg(GamePak::ENTRY_POINT_OFF, std::ios::beg);
    this->fileStream.read(reinterpret_cast<char*>(&this->entryPoint), GamePak::ENTRY_POINT_SIZE);
    if (this->fileStream)
        return 0;
    else
        return -1;
}

int GamePak::readNintendoLogo() {
    this->fileStream.seekg(GamePak::NINTENDO_LOGO_OFF, std::ios::beg);
    this->fileStream.read(reinterpret_cast<char*>(this->nintendoLogo), GamePak::NINTENDO_LOGO_SIZE);
    if (this->fileStream)
        return 0;
    else
        return -1;
}

int GamePak::readGameTitle() {
    this->fileStream.seekg(GamePak::GAME_TITLE_OFF, std::ios::beg);
    this->fileStream.read(reinterpret_cast<char *>(this->gameTitle), GamePak::GAME_TITLE_SIZE);
    if (this->fileStream) {
        // Null terminator
        this->gameTitle[GamePak::GAME_TITLE_SIZE] = 0;
        return 0;
    }
    else
        return -1;
}

int GamePak::readGameCode() {
    this->fileStream.seekg(GamePak::GAME_CODE_OFF, std::ios::beg);
    this->fileStream.read(reinterpret_cast<char*>(this->gameCode), GamePak::GAME_CODE_SIZE);
    if (this->fileStream) {
        this->gameCode[GamePak::GAME_CODE_SIZE] = 0;
        return 0;
    }
    else
        return -1;
}

int GamePak::readMakerCode() {
    this->fileStream.seekg(GamePak::MAKER_CODE_OFF, std::ios::beg);
    this->fileStream.read(reinterpret_cast<char*>(this->makerCode), GamePak::MAKER_CODE_SIZE);
    if (this->fileStream) {
        this->makerCode[GamePak::MAKER_CODE_SIZE] = 0;
        return 0;
    }
    else
        return -1;
}

int GamePak::readFixedValue() {
    this->fileStream.seekg(GamePak::FIXED_VALUE_OFF, std::ios::beg);
    this->fileStream.read(reinterpret_cast<char*>(&this->fixedValue), GamePak::FIXED_VALUE_SIZE);
    if (this->fileStream) {
        return 0;
    }
    else {
        return -1;
    }
}

int GamePak::readMainUnitCode() {
    this->fileStream.seekg(GamePak::MAIN_UNIT_CODE_OFF, std::ios::beg);
    this->fileStream.read(reinterpret_cast<char*>(&this->mainUnitCode), GamePak::MAIN_UNIT_CODE_SIZE);
    if (this->fileStream) {
        return 0;
    }
    else {
        return -1;
    }
}

int GamePak::readDeviceType() {
    this->fileStream.seekg(GamePak::DEVICE_TYPE_OFF, std::ios::beg);
    this->fileStream.read(reinterpret_cast<char*>(&this->deviceType), GamePak::DEVICE_TYPE_SIZE);
    if (this->fileStream) {
        return 0;
    }
    else {
        return -1;
    }
}

int GamePak::readReservedArea1() {
    this->fileStream.seekg(GamePak::RESERVED_AREA_1_OFF, std::ios::beg);
    this->fileStream.read(reinterpret_cast<char*>(this->reservedArea1), GamePak::RESERVED_AREA_1_SIZE);
    if (this->fileStream) {
        return 0;
    }
    else {
        return -1;
    }
}

int GamePak::readSoftwareVersion() {
    this->fileStream.seekg(GamePak::SOFTWARE_VERSION_OFF, std::ios::beg);
    this->fileStream.read(reinterpret_cast<char*>(&this->softwareVersion), GamePak::SOFTWARE_VERSION_SIZE);
    if (this->fileStream) {
        return 0;
    }
    else {
        return -1;
    }
}

int GamePak::readComplementCheck() {
    this->fileStream.seekg(GamePak::COMPLEMENT_CHECK_OFF, std::ios::beg);
    this->fileStream.read(reinterpret_cast<char*>(&this->complementCheck), GamePak::COMPLEMENT_CHECK_SIZE);
    if (this->fileStream) {
        return 0;
    }
    else {
        return -1;
    }
}

int GamePak::readReservedArea2() {
    this->fileStream.seekg(GamePak::RESERVED_AREA_2_OFF, std::ios::beg);
    this->fileStream.read(reinterpret_cast<char*>(this->reservedArea2), GamePak::RESERVED_AREA_2_SIZE);
    if (this->fileStream) {
        return 0;
    }
    else {
        return -1;
    }
}

int GamePak::calcComplementCheck() {
    int chk = 0;
    for (int i = 0x0A0; i < 0x0BC; i++) {
        int val;
        this->fileStream.seekg(i, std::ios::beg);
        this->fileStream.read(reinterpret_cast<char*>(&val), 1);
        chk = chk - val;
    }
    
    chk = (chk - 0x19) & 0x0FF;
    return chk;
}

int GamePak::readHeader() {
    readEntryPoint();
    readNintendoLogo();
    readGameTitle();
    readGameCode();
    readMakerCode();
    readFixedValue();
    readMainUnitCode();
    readDeviceType();
    readReservedArea1();
    readSoftwareVersion();
    readComplementCheck();
    readReservedArea2();

    return 0;
}

void GamePak::printHeader() {
    std::cout << "Entry point: 0x" << std::hex << this->entryPoint << std::dec << std::endl;
    //std::cout << "Nintendo Logo: " << this->nintendoLogo << std::endl;
    std::cout << "Game Title: " << this->gameTitle << std::endl;
    std::cout << "Game Code: " << this->gameCode << std::endl;
    std::cout << "Maker Code: " << this->makerCode << std::endl;
    std::cout << "Fixed Value: 0x" << std::hex << static_cast<int>(this->fixedValue) << std::dec << std::endl;
    std::cout << "Main Unit Code: 0x" << std::hex << static_cast<int>(this->mainUnitCode) << std::dec << std::endl;
    std::cout << "Device Type: 0x" << std::hex << static_cast<int>(this->deviceType) << std::dec << std::endl;
    std::cout << "Reserved Area 1: 0x" << static_cast<int>(this->reservedArea1[0]) << std::dec << std::endl;
    std::cout << "Software Version: 0x" << std::hex << static_cast<int>(this->softwareVersion) << std::dec << std::endl;
    std::cout << "Complement Check: 0x" << std::hex << static_cast<int>(this->complementCheck) << std::dec << " ";
    int chk = this->calcComplementCheck();
    if (chk == this->complementCheck)
        std::cout << "(CORRECT)" << std::endl;
    else
        std::cout << "(INCORRECT)" << std::endl;
    std::cout << "Reserved Area 2: 0x" << std::hex << static_cast<int>(this->reservedArea2[0]) << std::dec << std::endl;
}