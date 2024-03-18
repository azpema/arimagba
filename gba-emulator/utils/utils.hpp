#ifndef _UTILS_ 
#define _UTILS_ 

#include <iostream>
#include <iomanip>
#include <sstream>

class Utils {
public:
    static uint32_t setRegBits(uint32_t &reg, const uint32_t mask, const uint32_t val);
    static uint32_t clearRegBits(uint32_t &reg, const uint32_t mask);
    static uint32_t getRegBits(uint32_t reg, const uint32_t mask, const uint32_t shift);
    static uint32_t rotateRight(uint32_t reg, uint32_t nTimes);
    static int32_t twosComplementExtendSignTo(uint32_t val, uint32_t fromBits, uint32_t toBits);
    static std::string toHexString(uint32_t val, uint32_t paddingNum = 0);
};

#endif