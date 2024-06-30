#include "pc_relative_load.hpp"
#include "../arm/single_data_transfer.hpp"

PCRelativeLoad::PCRelativeLoad(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    rd = Utils::getRegBits(op, RD_MASK, RD_SHIFT);
    word8 = Utils::getRegBits(op, WORD8_MASK, WORD8_SHIFT);
    offset = word8 << 2;
}   



std::string PCRelativeLoad::toString(){
    return "ldr " + OpCode::getRegMnemonic(rd) + ",[pc,#" + Utils::toHexString(offset) + "]";
}

void PCRelativeLoad::doDecode(){

}

void PCRelativeLoad::doExecute(){
    uint8_t i = 0;
    uint8_t p = 1;
    uint8_t u = 1;
    uint8_t b = 0;
    uint8_t w = 0;
    uint8_t l = 1;
    SingleDataTransfer opArm = SingleDataTransfer(i, p, u, b, w, l, 15, rd, offset, cpu);
    std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    opArm.doExecute();  
}

uint32_t PCRelativeLoad::cyclesUsed() const {
    return 1;
}