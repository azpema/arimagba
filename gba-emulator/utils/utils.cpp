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

int32_t Utils::twosComplementExtendSignTo(uint32_t val, uint32_t fromBits, uint32_t toBits){
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
