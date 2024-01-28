#include "cpsr.hpp"
#include "../../utils/utils.hpp"

CPSR::CPSR() {
    this->value = 0;
    this->setMode(Supervisor);
}

void CPSR::setCPSRFlags(CPSR::Flag flag, bool val){
    switch (flag)
    {
    case CPSR::Flag::N:
        Utils::setRegBits(value, N_FLAG_MASK, val << N_FLAG_SHIFT);
        break;

    case CPSR::Flag::Z:
        Utils::setRegBits(value, Z_FLAG_MASK, val << Z_FLAG_SHIFT);
        break;

    case CPSR::Flag::C:
        Utils::setRegBits(value, C_FLAG_MASK, val << C_FLAG_SHIFT);
        break;

    case CPSR::Flag::V:
        Utils::setRegBits(value, V_FLAG_MASK, val << V_FLAG_SHIFT);
        break;
    
    default:
        std::cout << "ERROR: invalid CPSR flag" << std::endl;
        break;
    }
}

uint32_t CPSR::getValue(){
   return value;
}


uint32_t CPSR::getModeUInt(){
    return Utils::getRegBits(value, MODE_FLAG_MASK, MODE_FLAG_SHIFT);
}

std::string CPSR::getModeString(){
    uint32_t mode = getModeUInt();
    return mode2String[mode];
}

void CPSR::setNFlag(bool val){
    setCPSRFlags(N, val);
}

void CPSR::setZFlag(bool val){
    setCPSRFlags(Z, val);
}

void CPSR::setCFlag(bool val){
    setCPSRFlags(C, val);
}

void CPSR::setVFlag(bool val){
    setCPSRFlags(V, val);
}

void CPSR::setIFlag(bool val){
    setCPSRFlags(I, val);
}

void CPSR::setFFlag(bool val){
    setCPSRFlags(F, val);
}

void CPSR::setTFlag(bool val){
    setCPSRFlags(T, val);
}

void CPSR::setMode(Mode mode){
    Utils::setRegBits(value, MODE_FLAG_MASK, mode);
}