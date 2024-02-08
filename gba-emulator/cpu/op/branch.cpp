#include "branch.hpp"
#include <bitset>

Branch::Branch(uint32_t op): OpCode::OpCode(op) {
    L = Utils::getRegBits(op, LINK_MASK, LINK_SHIFT);
    offsetField = Utils::getRegBits(op, OFFSET_MASK, OFFSET_SHIFT);
    realOffset = static_cast<int32_t>(Utils::twosComplementExtendSignTo(offsetField, 24, 32) << 2) + 8;
}   

std::string Branch::getLinkFlagMnemonic(){
    return linkFlag2Mnemonic[L];
}

std::string Branch::toString(){
    return "b" + getLinkFlagMnemonic() + getCondFieldMnemonic() + " pc+" + Utils::toHexString(realOffset);
}