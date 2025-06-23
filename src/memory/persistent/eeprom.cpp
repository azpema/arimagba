#include "eeprom.hpp"
#include "../../utils/utils.hpp"
#include <filesystem>

const std::unordered_map<EEPROM::Size, size_t> EEPROM::sizeToBytes = {
    { EEPROM::Size::_512_BYTES, 512 },
    { EEPROM::Size::_8_KBYTES, 8192 }
};

const std::unordered_map<EEPROM::Size, size_t> EEPROM::sizeToAddrBits = {
    { EEPROM::Size::_512_BYTES, 6 },
    { EEPROM::Size::_8_KBYTES, 14 }
};

const std::unordered_map<EEPROM::Cmd, std::string> EEPROM::cmdToStr = {
    { Cmd::READ, "READ"},
    { Cmd::WRITE, "WRITE"},
};

EEPROM::EEPROM(Size eepromSize, const std::filesystem::path& filePath) : GenericMemory(sizeToBytes.at(eepromSize), true), size(eepromSize),
  savePath(filePath), bytes(sizeToBytes.at(eepromSize)), maxAddr(sizeToBytes.at(eepromSize) / 8),
  sizeAddr(sizeToAddrBits.at(eepromSize)) { 
    std::cerr << "TODO Set proper initial values at EEPROM" << '\n';
    readFromFile();
}

uint64_t EEPROM::readDataBlock(uint16_t addr) const {
    if(addr >= maxAddr){
        throw std::runtime_error("EEPROM requested address is larger than last block address");
    }

    return Utils::readUint64(&mem[addr * 8]);
}

void EEPROM::writeDataBlock(uint16_t addr, uint64_t data) {
    if(addr >= maxAddr){
        throw std::runtime_error("EEPROM requested address is larger than last block address");
    }

    store(addr * 8, data, 8);
}

// This is the receiving end from the context of the EEPROM
void EEPROM::serialWrite(bool val) {
    static uint8_t cmd;
    static uint32_t recvdAddrBits = 0;
    static int dataCount = 64 - 1;
    static uint64_t receivedData = 0;
    uint8_t valBit = static_cast<uint8_t>(val);
    std::cout << static_cast<int>(val) << '\n';
    switch(status) 
    {
        case State::IDLE:
            status = State::RECEIVING_CMD;
            recvdAddrBits = 0;
            addr = 0;
            cmd = valBit;
            break;

        case State::RECEIVING_CMD:
            cmd = (cmd << 1) | valBit;
            if(static_cast<Cmd>(cmd) == Cmd::READ || static_cast<Cmd>(cmd) == Cmd::WRITE) {
                status = State::RECEIVING_ADDR;
            } else {
                throw std::runtime_error("Unknown EEPROM cmd");
            }
            break;
        case State::RECEIVING_ADDR:
            addr = ( addr << 1 ) | valBit;
            recvdAddrBits++;
            if (recvdAddrBits == sizeAddr) {
                if (static_cast<Cmd>(cmd) == Cmd::READ) {
                    status = State::RECEIVING_END_BIT;
                } else {
                    status = State::RECEIVING_DATA;
                }
            }
            break;
        case State::RECEIVING_DATA:
            std::cout << "EEPROM receiving data bit num "<< std::to_string(dataCount) << '\n';
            if ( dataCount == 0 ) {
                status = State::RECEIVING_END_BIT;
                dataCount = 64;
            }
            receivedData = ( receivedData << 1 ) | static_cast<uint64_t>(val);
            dataCount--;

            break;
            
        case State::RECEIVING_END_BIT:
            //if (valBit == 0) {
                std::cout << "EEPROM command OK, proceed" << '\n';
                std::cout << "CMD: " << cmdToStr.at(static_cast<Cmd>(cmd)) << '\n';
                std::cout << "Addr: 0x" << std::hex << addr << std::dec <<'\n';
                if (static_cast<Cmd>(cmd) == Cmd::READ) {
                    status = State::SENDING_PADDING;
                } else {
                    std::cout << "Data: 0x" << std::hex << receivedData << std::dec <<'\n';
                    // Actually write the data here
                    writeDataBlock(addr, receivedData);
                    saveToFile();
                    std::cout << "Saving to EEPROM file" << '\n';
                    status = State::SENDING_END_BIT;
                    receivedData = 0;
                } 
            // } else {
            //     throw std::runtime_error("Incorrect EEPROM end bit");
            //     return;
            // }
            break;

        default:
            throw std::runtime_error("EEPROM unexpected behavior");
    }
}

// Ideally the EEPROM should just write to the data bus bit, regardless of whether anyone is trying to read from it
// TODO: Move this logic to main loop

// 4 bits  - ignore these
// 64 bits - data (conventionally MSB first)
bool EEPROM::serialRead() {
    static int paddingCount = 0;
    static int dataCount = 64 - 1;
    static uint64_t dataBlock;
    switch(status) {
        case State::SENDING_PADDING:
            paddingCount++;
            if ( paddingCount == 4 ) {
                dataBlock = readDataBlock(addr);
                status = State::SENDING_DATA;
                paddingCount = 0;
            }
            std::cout << "EEPROM read padding" << '\n';
            return false;
            break;

        case State::SENDING_DATA:
        {
            std::cout << "EEPROM sending data bit num "<< std::to_string(dataCount) << '\n';
            if ( dataCount == 0 ) {
                status = State::IDLE;
                dataCount = 64;
            }
            bool val = Utils::getRegSingleBit(dataBlock, dataCount);
            dataCount--;
            return val;
            break;
        }
        case State::SENDING_END_BIT:
            status = State::IDLE;
            return true;
            break;
        default:
            status = State::IDLE;
            break;
    }

    return true;
}

void EEPROM::saveToFile() {
    std::ofstream outFile(savePath, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Failed to open file");
    }

    outFile.write(reinterpret_cast<const char*>(mem.get()), n);
    outFile.close();
}

void EEPROM::readFromFile() {
    if (std::filesystem::exists(savePath)) {
 
        std::ifstream in(savePath, std::ios::binary);
        if (!in) {
            std::cerr << "Failed to open file.\n";
            throw std::runtime_error("ERROR: Could not open EEPROM save file");
        }

        in.read(reinterpret_cast<char*>(mem.get()), n);
        in.close();
    } 
}