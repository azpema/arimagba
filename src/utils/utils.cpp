#include "utils.hpp"
#include <iostream>
#include <cstring>
#include <bit>

uint32_t Utils::clearRegBits(uint32_t &reg, const uint32_t mask){
    reg &= ~mask;
    return reg;
}

uint32_t Utils::setRegBits(uint32_t &reg, const uint32_t mask, const uint32_t val){
    reg = (reg & ~mask) | (val & mask);
    return reg;
}

uint32_t Utils::getRegBits(uint32_t reg, const uint32_t mask, const uint32_t shift){
    return (reg & mask) >> shift;
}

bool Utils::getRegSingleBit(uint32_t reg, uint8_t pos){
    return (reg >> pos) & 0x1;
}


uint32_t Utils::rotateRight(uint32_t reg, uint32_t nTimes){
    uint32_t res = reg;
    for (size_t i = 0; i < nTimes; i++) {
        uint32_t zerothBit = res & 0x1;
        res >>= 1;
        res |= (zerothBit << 31);
    }
    return res;
}

std::string Utils::toHexString(uint32_t val, uint32_t paddingNum){
    std::stringstream stream;
    if(paddingNum == 0){
        stream << "0x" << std::uppercase << std::hex << val;
    }else{
        stream << "0x" << std::uppercase << std::setfill('0') << std::setw(paddingNum) << std::hex << val;
    }
    
    return stream.str();
}

int32_t Utils::twosComplementExtendSignTo(uint32_t val, uint32_t fromBits, uint32_t toBits) {
    if(fromBits >= toBits){
        throw std::runtime_error("ERROR: extend sign");
    }

    uint32_t sign = (val >> (fromBits - 1)) & 0x1;
    if(sign == 0){
        return val;
    }else{
        uint32_t mask = ((1u << (toBits - fromBits)) - 1) << fromBits;
        return val | mask;
    }
}

constexpr uint32_t Utils::byteSwap32(uint32_t val) {
    return ((val >> 24) & 0x000000FF) |
           ((val >> 8)  & 0x0000FF00) |
           ((val << 8)  & 0x00FF0000) |
           ((val << 24) & 0xFF000000);
}

uint32_t Utils::readUint32(uint8_t* data){
    uint32_t val;
    std::memcpy(&val, data, sizeof(uint32_t));
    return std::endian::native == std::endian::little ? val : byteSwap32(val);
}