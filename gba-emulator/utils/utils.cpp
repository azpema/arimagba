#include "utils.hpp"

uint32_t Utils::clearRegBits(uint32_t &reg, const uint32_t mask){
    reg &= ~mask;
    return reg;
}

uint32_t Utils::setRegBits(uint32_t &reg, const uint32_t mask, const uint32_t val){
    if((mask | val) != mask){
        std::cout << "ERROR: value exceeds mask!" << std::endl;
        return -1;
    }

    clearRegBits(reg, mask);
    reg |= val;
    return reg;
}

uint32_t Utils::getRegBits(uint32_t reg, const uint32_t mask, const uint32_t shift){
    return (reg & mask) >> shift;
}

// NO FUNCIONA BIEN
uint32_t Utils::rotateRight(uint32_t reg, uint32_t nTimes){
    for (size_t i = 0; i < nTimes; i++) {
        uint32_t zerothBit = reg & 0b00000000000000000000000000000001;
        reg >>= 1;
        reg |= zerothBit << 31;
    }
    
    return reg;
}

std::string Utils::toHexString(uint32_t val){
    std::stringstream stream;
    stream << "0x" << std::uppercase << std::hex << val;
    return stream.str();
}

uint32_t Utils::twosComplementExtendSignTo(uint32_t val, uint32_t fromBits, uint32_t toBits){
    if(fromBits >= toBits){
        std::cout << "ERROR: extend sign" << std::endl;
    }

    uint32_t sign = (val >> (fromBits - 1)) & 0x1;
    if(sign == 0)
        return val;
    else{
        uint32_t orVal = 0x1;
        for(size_t i=0; i<toBits-fromBits-1; i++){
            orVal = (orVal << 1) | 1;
        }
        orVal <<= fromBits;
        return val | orVal;
    }

}

