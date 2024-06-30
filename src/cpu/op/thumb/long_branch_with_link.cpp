#include "long_branch_with_link.hpp"

using namespace Thumb;

LongBranchWithLink::LongBranchWithLink(uint16_t op, uint32_t pc, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    _type = LONG_BRANCH_WITH_LINK;
    h = Utils::getRegBits(op, H_MASK, H_SHIFT);
    offset = Utils::getRegBits(op, OFFSET_MASK, OFFSET_SHIFT);
    // TODO fix possible bug
    if(h == 0){
        offsetVal = offset << 12;
    }else if(h == 1){
        offsetVal = offset << 1;
    }else{
        std::runtime_error("ERROR: Invalid h value in LongBranchWithLink::LongBranchWithLink");
    }
}   


std::string LongBranchWithLink::toString(){
    // TODO set proper decoded value
    return "bl h=" + std::to_string(h) + " " + std::to_string(offsetVal);
}

void LongBranchWithLink::doDecode(){

}

void LongBranchWithLink::doExecute(){
    std::cerr << "CHECK LONGBRANCHWITHLINK" << std::endl;
    if(h == 0){
        cpu.setLR(cpu.getPC() + offsetVal);
    }else if(h == 1){
        // PC is 4 steps ahead (THUMB)
        uint32_t temp = cpu.getPC() - 2;
        cpu.setPC(cpu.getReg(14) + offsetVal);
        cpu.setLR(temp | 1);
    }else{
        std::runtime_error("ERROR: Invalid h value in LongBranchWithLink::doExecute");
    }
}

uint32_t LongBranchWithLink::cyclesUsed() const {
    return 1;
}