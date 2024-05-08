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
        std::cerr << "ERROR: Long branch with link invalid H value" << std::endl;
    }
}   


std::string LongBranchWithLink::toString(){
    return "KA";
}

void LongBranchWithLink::doDecode(){

}

void LongBranchWithLink::doExecute(){
    throw std::runtime_error("Error: Unimplemented instruction: LongBranchWithLink");
}

uint32_t LongBranchWithLink::cyclesUsed() const {
    return 1;
}