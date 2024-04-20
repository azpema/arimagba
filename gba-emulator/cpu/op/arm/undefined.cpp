#include "undefined.hpp"

Undefined::Undefined(uint32_t op): OpCode::OpCode(op) {}   

std::string Undefined::toString(){
    return "Undefined";
}

void Undefined::doExecute(ARM7TDMI &cpu){

}

bool Undefined::mustFlushPipeline() const {
    return false;
}

uint32_t Undefined::cyclesUsed() const {
    return 1;
}