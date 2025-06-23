#ifndef _EEPROM_
#define _EEPROM_

#include "../generic_memory.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <unordered_map>
/*

https://rust-console.github.io/gbatek-gbaonly/#addressing-and-waitstates-1
The eeprom is connected to Bit0 of the data bus, and to the upper 1 bit (or upper 17 bits in case of large 32MB ROM)
of the cartridge ROM address bus, communication with the chip takes place serially.

The eeprom must be used with 8 waitstates (set WAITCNT=X3XXh; 8,8 clks in WS2 area), 
the eeprom can be then addressed at DFFFF00h..DFFFFFFh.

Respectively, with eeprom, ROM is restricted to 8000000h-9FFFeFFh (max. 1FFFF00h bytes = 32MB minus 256 bytes).
On carts with 16MB or smaller ROM, eeprom can be alternately accessed anywhere at D000000h-DFFFFFFh.

*/

class EEPROM : public GenericMemory {
  public:
    enum class Size { _512_BYTES, _8_KBYTES};
    EEPROM(Size eepromSize, const std::filesystem::path& filePath);
    void runCycle();
    void serialWrite(bool val);
    bool serialRead();

    const static std::unordered_map<Size, size_t> sizeToBytes;

  private:
    uint64_t readDataBlock(uint16_t addr) const;
    void writeDataBlock(uint16_t addr, uint64_t data);
    void saveToFile();
    void readFromFile();


    enum class Cmd { READ = 0b11, WRITE = 0b10 };
    enum class State { IDLE, RECEIVING_CMD, RECEIVING_ADDR, RECEIVING_END_BIT, RECEIVING_DATA, SENDING_PADDING, SENDING_END_BIT, SENDING_DATA };

    
    const static std::unordered_map<Size, size_t> sizeToAddrBits;
    const static std::unordered_map<Cmd, std::string> cmdToStr;
    
    State status = State::IDLE;
    Size size;
    std::filesystem::path savePath;
    size_t bytes;
    const uint16_t maxAddr;
    const size_t sizeAddr;
    uint16_t addr = 0;
    std::ifstream outFile;
    
};

#endif
