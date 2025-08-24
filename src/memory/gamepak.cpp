#include "gamepak.hpp"
#include "../utils/utils.hpp"
#include <cstring>
#include <iostream>
#include <regex>

GamePak::GamePak(const std::string& filePath) : GenericMemory(ROM_SIZE) {
    std::ifstream fileStream = std::ifstream(filePath, std::ios::binary | std::ifstream::ate);
    if (!fileStream) {
        throw std::runtime_error("ERROR: GamePak Failed to open the file.");
    }

    uint32_t fileSize = fileStream.tellg();
    size = fileSize;

    fileStream.seekg(0, std::ios::beg);

    if (!fileStream.is_open()) {
        throw std::runtime_error("ERROR: GamePak Failed to open the file.");
    } else {
        fileStream.read(reinterpret_cast<char*>(mem.get()), fileSize);
        if (!fileStream) {
            throw std::runtime_error("ERROR: GamePak reading");
        }
    }

    /* Precalculate Out-of-bounds ROM reads */
    for (size_t addr = fileSize; addr < ROM_SIZE; addr += 2) {
        mem[addr] = addr >> 1;           // lower nibble
        mem[addr + 1] = addr >> (8 + 1); // upper nibble
    }

    entryPoint = Utils::readUint32(&mem[GamePak::ENTRY_POINT_OFF]);
    std::memcpy(nintendoLogo, &mem[GamePak::NINTENDO_LOGO_OFF], GamePak::NINTENDO_LOGO_SIZE);
    std::memcpy(gameTitle, &mem[GamePak::GAME_TITLE_OFF], GamePak::GAME_TITLE_SIZE);
    gameTitleStr = std::string(reinterpret_cast<char*>(gameTitle), GamePak::GAME_TITLE_SIZE);
    std::memcpy(gameCode, &mem[GamePak::GAME_CODE_OFF], GamePak::GAME_CODE_SIZE);
    gameCodeStr = std::string(reinterpret_cast<char*>(gameCode), GamePak::GAME_CODE_SIZE);

    std::memcpy(makerCode, &mem[GamePak::MAKER_CODE_OFF], GamePak::MAKER_CODE_SIZE);

    fixedValue = mem[GamePak::FIXED_VALUE_OFF];
    mainUnitCode = mem[GamePak::MAIN_UNIT_CODE_OFF];
    deviceType = mem[GamePak::DEVICE_TYPE_OFF];

    std::memcpy(reservedArea1, &mem[GamePak::RESERVED_AREA_1_OFF], GamePak::RESERVED_AREA_1_SIZE);
    softwareVersion = mem[GamePak::SOFTWARE_VERSION_OFF];
    complementCheck = mem[GamePak::COMPLEMENT_CHECK_OFF];
    std::memcpy(reservedArea2, &mem[GamePak::RESERVED_AREA_2_OFF], GamePak::RESERVED_AREA_2_SIZE);

    saveType = detectSaveType();
}

GamePak::SaveType GamePak::detectSaveType() const {
    std::string memStr(reinterpret_cast<const char*>(mem.get()), size);

    for(const auto& pair : regexToSaveType) {
        std::regex pattern(pair.first);
        std::smatch match;

        if (std::regex_search(memStr, match, pattern)) {
            return regexToSaveType.at(pair.first);
        } 
    }

    return SaveType::UNKNOWN;
}

size_t GamePak::getSize() const { return size; }

int GamePak::calcComplementCheck() const {
    int chk = 0;
    for (int i = 0x0A0; i < 0x0BC; i++) {
        int val;
        val = mem[i];
        chk = chk - val;
    }

    chk = (chk - 0x19) & 0x0FF;
    return chk;
}

void GamePak::printInfo() const {
    std::cout << "Entry point: 0x" << std::hex << this->entryPoint << std::dec << '\n';
    // std::cout << "Nintendo Logo: " << this->nintendoLogo << '\n';
    std::cout << "Game Title: " << gameTitleStr << '\n';
    std::cout << "Game Code: " << gameCodeStr << '\n';
    std::cout << "Maker Code: " << makerCode << '\n';
    std::cout << "Fixed Value: 0x" << std::hex << static_cast<int>(fixedValue) << std::dec << '\n';
    std::cout << "Main Unit Code: 0x" << std::hex << static_cast<int>(mainUnitCode) << std::dec << '\n';
    std::cout << "Device Type: 0x" << std::hex << static_cast<int>(deviceType) << std::dec << '\n';
    std::cout << "Reserved Area 1: 0x" << static_cast<int>(reservedArea1[0]) << std::dec << '\n';
    std::cout << "Software Version: 0x" << std::hex << static_cast<int>(softwareVersion) << std::dec << '\n';
    std::cout << "Complement Check: 0x" << std::hex << static_cast<int>(complementCheck) << std::dec << " ";
    int chk = calcComplementCheck();
    if (chk == complementCheck)
        std::cout << "(CORRECT)" << '\n';
    else
        std::cout << "(INCORRECT)" << '\n';
    std::cout << "Reserved Area 2: 0x" << std::hex << static_cast<int>(this->reservedArea2[0]) << std::dec << '\n';
    std::cout << "Detected save data type: " << saveTypeToStr.at(saveType) << '\n';
}
