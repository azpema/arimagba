#include "undefined.hpp"

Undefined::Undefined(uint32_t op): OpCode::OpCode(op) {}   

std::string Undefined::toString(){
    return "Undefined";
}

void Undefined::doExecute(ARM7TDMI &cpu){

}