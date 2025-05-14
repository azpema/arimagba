#ifndef _UTILS_
#define _UTILS_

#include <iomanip>
#include <iostream>
#include <cstdint>
#include <sstream>

class Utils {
  public:
    static uint32_t setRegBits(uint32_t& reg, const uint32_t mask, const uint32_t val);
    static uint32_t clearRegBits(uint32_t& reg, const uint32_t mask);
    static uint32_t getRegBits(uint32_t reg, const uint32_t mask, const uint32_t shift);
    static bool getRegSingleBit(uint32_t reg, uint8_t pos);

    static uint32_t rotateRight(uint32_t reg, uint32_t nTimes);
    static int32_t twosComplementExtendSignTo(uint32_t val, uint32_t fromBits, uint32_t toBits);
    static std::string toHexString(uint32_t val, uint32_t paddingNum = 0);

    static constexpr uint32_t byteSwap32(uint32_t val);
    static uint32_t readUint32(uint8_t* data);

    template <bool inclusiveMin = true, bool inclusiveMax = true>
    static bool inRange(uint32_t val, uint32_t from, uint32_t to) {
        if (inclusiveMin && inclusiveMax) {
            return (val >= from) && (val <= to);
        } else if (inclusiveMin && !inclusiveMax) {
            return (val >= from) && (val < to);
        } else if (!inclusiveMin && inclusiveMax) {
            return (val > from) && (val <= to);
        } else {
            return (val > from) && (val < to);
        }
    }
};

#endif