#include "gamepak.hpp"
#include "../utils/utils.hpp"
#include <iostream>
#include <cstring>

GamePak::GamePak(const std::string &filePath) {
    std::ifstream fileStream = std::ifstream(filePath, std::ios::binary | std::ifstream::ate);
    if (!fileStream) {
        throw std::runtime_error("ERROR: GamePak Failed to open the file.");
    }

    uint32_t fileSize = fileStream.tellg();
    fileStream.seekg(0, std::ios::beg);

    if (!fileStream.is_open()) {
        throw std::runtime_error("ERROR: GamePak Failed to open the file.");
    }else {
        fileStream.read(reinterpret_cast<char *>(mem.get()), fileSize);
        if (fileStream) {
            std::cout << "DEBUG: BIOS READ OK" << std::endl;
        } else{
            throw std::runtime_error("ERROR: GamePak reading");
        }
    }

    /* Precalculate Out-of-bounds ROM reads */
    for(size_t addr = fileSize; addr < ROM_SIZE; addr += 2){
        mem[addr]     = addr >> 1;          // lower nibble
        mem[addr + 1] = addr >> (8 + 1);    // upper nibble
    }

    entryPoint = Utils::readUint32(&mem[GamePak::ENTRY_POINT_OFF]);
    std::memcpy(nintendoLogo, &mem[GamePak::NINTENDO_LOGO_OFF], GamePak::NINTENDO_LOGO_SIZE);
    std::memcpy(gameTitle, &mem[GamePak::GAME_TITLE_OFF], GamePak::GAME_TITLE_SIZE);
    std::memcpy(gameCode, &mem[GamePak::GAME_CODE_OFF], GamePak::GAME_CODE_SIZE);

    std::memcpy(makerCode, &mem[GamePak::MAKER_CODE_OFF], GamePak::MAKER_CODE_SIZE);

    fixedValue = mem[GamePak::FIXED_VALUE_OFF];
    mainUnitCode = mem[GamePak::MAIN_UNIT_CODE_OFF];
    deviceType = mem[GamePak::DEVICE_TYPE_OFF];

    std::memcpy(reservedArea1, &mem[GamePak::RESERVED_AREA_1_OFF], GamePak::RESERVED_AREA_1_SIZE);
    softwareVersion = mem[GamePak::SOFTWARE_VERSION_OFF];
    complementCheck = mem[GamePak::COMPLEMENT_CHECK_OFF];
    std::memcpy(reservedArea2, &mem[GamePak::RESERVED_AREA_2_OFF], GamePak::RESERVED_AREA_2_SIZE);

}

int GamePak::calcComplementCheck() {
    int chk = 0;
    for (int i = 0x0A0; i < 0x0BC; i++) {
        int val;
        val = mem[i];
        chk = chk - val;
    }
    
    chk = (chk - 0x19) & 0x0FF;
    return chk;
}

void GamePak::printHeader() {
    std::cout << "Entry point: 0x" << std::hex << this->entryPoint << std::dec << std::endl;
    //std::cout << "Nintendo Logo: " << this->nintendoLogo << std::endl;
    std::cout << "Game Title: " << gameTitle << std::endl;
    std::cout << "Game Code: " << gameCode << std::endl;
    std::cout << "Maker Code: " << makerCode << std::endl;
    std::cout << "Fixed Value: 0x" << std::hex << static_cast<int>(fixedValue) << std::dec << std::endl;
    std::cout << "Main Unit Code: 0x" << std::hex << static_cast<int>(mainUnitCode) << std::dec << std::endl;
    std::cout << "Device Type: 0x" << std::hex << static_cast<int>(deviceType) << std::dec << std::endl;
    std::cout << "Reserved Area 1: 0x" << static_cast<int>(reservedArea1[0]) << std::dec << std::endl;
    std::cout << "Software Version: 0x" << std::hex << static_cast<int>(softwareVersion) << std::dec << std::endl;
    std::cout << "Complement Check: 0x" << std::hex << static_cast<int>(complementCheck) << std::dec << " ";
    int chk = calcComplementCheck();
    if (chk == complementCheck)
        std::cout << "(CORRECT)" << std::endl;
    else
        std::cout << "(INCORRECT)" << std::endl;
    std::cout << "Reserved Area 2: 0x" << std::hex << static_cast<int>(this->reservedArea2[0]) << std::dec << std::endl;
}
