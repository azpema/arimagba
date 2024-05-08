#include "undefined.hpp"

Undefined::Undefined(uint32_t op, ARM7TDMI &cpu): OpCode::OpCode(op, cpu) {}   

std::string Undefined::toString(){
    return "Undefined";
}

void Undefined::doDecode(){

}

void Undefined::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: Undefined");
}

uint32_t Undefined::cyclesUsed() const {
    return 1;
}