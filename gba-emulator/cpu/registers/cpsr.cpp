#include "cpsr.hpp"
#include "../../utils/utils.hpp"
/* https://problemkaputt.de/gbatek.htm#armcpuexceptions
   Reset
   Forces PC=VVVV0000h, and forces control bits of CPSR to T=0 (ARM state),
   F=1 and I=1 (disable FIQ and IRQ), and M4-0=10011b (Supervisor mode).


*/ 

CPSR::CPSR() {
    reset();
}

void CPSR::reset(){
    this->value = 0;
    this->setTFlag(false);
    this->setFFlag(true);
    this->setIFlag(true);
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

    case CPSR::Flag::I:
        Utils::setRegBits(value, I_FLAG_MASK, val << I_FLAG_SHIFT);
        break;

    case CPSR::Flag::F:
        Utils::setRegBits(value, F_FLAG_MASK, val << F_FLAG_SHIFT);
        break;

    case CPSR::Flag::T:
        Utils::setRegBits(value, T_FLAG_MASK, val << T_FLAG_SHIFT);
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

bool CPSR::getNFlag(){
    return Utils::getRegBits(value, N_FLAG_MASK, N_FLAG_SHIFT);
}

bool CPSR::getZFlag(){
    return Utils::getRegBits(value, Z_FLAG_MASK, Z_FLAG_SHIFT);
}

bool CPSR::getCFlag(){
    return Utils::getRegBits(value, C_FLAG_MASK, C_FLAG_SHIFT);
}

bool CPSR::getVFlag(){
    return Utils::getRegBits(value, V_FLAG_MASK, V_FLAG_SHIFT);
}

bool CPSR::getIFlag(){
    return Utils::getRegBits(value, I_FLAG_MASK, I_FLAG_SHIFT);
}

bool CPSR::getFFlag(){
    return Utils::getRegBits(value, F_FLAG_MASK, F_FLAG_SHIFT);
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

bool CPSR::isThumbMode(){
    uint8_t mode = Utils::getRegBits(value, T_FLAG_MASK, T_FLAG_SHIFT);
    if(mode == 1)
        return true;
    else 
        return false;
}