#include "cpsr.hpp"
#include "../../utils/utils.hpp"
/* https://problemkaputt.de/gbatek.htm#armcpuexceptions
   Reset
   Forces PC=VVVV0000h, and forces control bits of CPSR to T=0 (ARM state),
   F=1 and I=1 (disable FIQ and IRQ), and M4-0=10011b (Supervisor mode).


*/ 

PSR::PSR() {
    reset();
}

void PSR::reset(){
    this->value = 0;
    this->setTFlag(false);
    this->setFFlag(true);
    this->setIFlag(true);
    std::cout << "TODO: Set proper initial CPU mode" << '\n';
    this->setMode(System);
}

void PSR::setCPSRFlags(PSR::Flag flag, bool val){
    switch (flag)
    {
    case PSR::Flag::N:
        Utils::setRegBits(value, N_FLAG_MASK, val << N_FLAG_SHIFT);
        break;

    case PSR::Flag::Z:
        Utils::setRegBits(value, Z_FLAG_MASK, val << Z_FLAG_SHIFT);
        break;

    case PSR::Flag::C:
        Utils::setRegBits(value, C_FLAG_MASK, val << C_FLAG_SHIFT);
        break;

    case PSR::Flag::V:
        Utils::setRegBits(value, V_FLAG_MASK, val << V_FLAG_SHIFT);
        break;

    case PSR::Flag::I:
        Utils::setRegBits(value, I_FLAG_MASK, val << I_FLAG_SHIFT);
        break;

    case PSR::Flag::F:
        Utils::setRegBits(value, F_FLAG_MASK, val << F_FLAG_SHIFT);
        break;

    case PSR::Flag::T:
        Utils::setRegBits(value, T_FLAG_MASK, val << T_FLAG_SHIFT);
        break;
    
    default:
        std::cout << "ERROR: invalid CPSR flag" << '\n';
        break;
    }
}

uint32_t PSR::getValue(){
   return value;
}

void PSR::setValue(uint32_t val){
    value = val;
}

PSR::Mode PSR::getMode(){
    switch(getModeUInt()) {
        case MODE_FLAG_USR_VAL: return Mode::User;
        case MODE_FLAG_FIQ_VAL: return Mode::FIQ;
        case MODE_FLAG_IRQ_VAL: return Mode::IRQ;
        case MODE_FLAG_SVC_VAL: return Mode::Supervisor;
        case MODE_FLAG_ABT_VAL: return Mode::Abort;
        case MODE_FLAG_UND_VAL: return Mode::Undefined;
        case MODE_FLAG_SYS_VAL: return Mode::System;
        default: throw std::invalid_argument("Invalid value");
    }
}

uint32_t PSR::getModeUInt(){
    return Utils::getRegBits(value, MODE_FLAG_MASK, MODE_FLAG_SHIFT);
}

std::string PSR::getModeString(){
    uint32_t mode = getModeUInt();
    return mode2String[mode];
}

bool PSR::getNFlag(){
    return Utils::getRegBits(value, N_FLAG_MASK, N_FLAG_SHIFT);
}

bool PSR::getZFlag(){
    return Utils::getRegBits(value, Z_FLAG_MASK, Z_FLAG_SHIFT);
}

bool PSR::getCFlag(){
    return Utils::getRegBits(value, C_FLAG_MASK, C_FLAG_SHIFT);
}

bool PSR::getVFlag(){
    return Utils::getRegBits(value, V_FLAG_MASK, V_FLAG_SHIFT);
}

bool PSR::getIFlag(){
    return Utils::getRegBits(value, I_FLAG_MASK, I_FLAG_SHIFT);
}

bool PSR::getFFlag(){
    return Utils::getRegBits(value, F_FLAG_MASK, F_FLAG_SHIFT);
}

bool PSR::getTFlag(){
    return Utils::getRegBits(value, T_FLAG_MASK, T_FLAG_SHIFT);
}


void PSR::setNFlag(bool val){
    setCPSRFlags(N, val);
}

void PSR::setZFlag(bool val){
    setCPSRFlags(Z, val);
}

void PSR::setCFlag(bool val){
    setCPSRFlags(C, val);
}

void PSR::setVFlag(bool val){
    setCPSRFlags(V, val);
}

void PSR::setIFlag(bool val){
    setCPSRFlags(I, val);
}

void PSR::setFFlag(bool val){
    setCPSRFlags(F, val);
}

void PSR::setTFlag(bool val){
    setCPSRFlags(T, val);
}

void PSR::setMode(Mode mode){
    Utils::setRegBits(value, MODE_FLAG_MASK, mode);
}

bool PSR::isThumbMode(){
    return Utils::getRegBits(value, T_FLAG_MASK, T_FLAG_SHIFT) == 1;
}