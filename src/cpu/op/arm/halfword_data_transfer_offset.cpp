#include "halfword_data_transfer_offset.hpp"
#include <bitset>

HalfwordDataTransferOffset::HalfwordDataTransferOffset(uint32_t op, ARM7TDMI &cpu): HalfwordDataTransfer::HalfwordDataTransfer(op, cpu) {
    offsetHi = Utils::getRegBits(op, OFFSETHI_FLAG_MASK, OFFSETHI_FLAG_SHIFT);
    offsetLo = Utils::getRegBits(op, OFFSETLO_FLAG_MASK, OFFSETLO_FLAG_SHIFT);

    offsetVal = (offsetHi << 4) | offsetLo;
}

uint16_t HalfwordDataTransferOffset::getOffsetVal() {
    return offsetVal;
}

std::string HalfwordDataTransferOffset::toString(){
    std::string base = getOpMnemonic() + getCondFieldMnemonic() + getSFlagMnemonic() + getHFlagMnemonic() +  + " " + \
                        OpCode::getRegMnemonic(rd) + ",";
    std::string address = "[" + OpCode::getRegMnemonic(rn);

    bool showOffset = getOffsetVal() !=0;

    if(p == 1){
        if(showOffset)
            address += "," + getUFlagMnemonic() + Utils::toHexString(getOffsetVal()) + "]" + getWFlagMnemonic();
        else
            address += "]" + getWFlagMnemonic();
    }else if(p == 0){
        address += "]";
        if(showOffset) 
            address += "," + getUFlagMnemonic() + Utils::toHexString(getOffsetVal());
    }else{
        std::cerr << "ERROR: Invalid p flag value HalfwordDataTransferOffset" << std::endl;
    }

    return base + address;
}

// Depends on load or store
uint32_t HalfwordDataTransferOffset::cyclesUsed() const {
    std::cerr << "TODO: HalfwordDataTransferOffset::cyclesUsed" << std::endl;
    return 1;
}

void HalfwordDataTransferOffset::doDecode(){

}

void HalfwordDataTransferOffset::doExecute(){
    _doExecute(offsetVal);
}
