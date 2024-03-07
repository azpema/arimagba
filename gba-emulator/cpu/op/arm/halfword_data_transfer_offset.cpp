#include "halfword_data_transfer_offset.hpp"

HalfwordDataTransferOffset::HalfwordDataTransferOffset(uint32_t op): HalfwordDataTransfer::HalfwordDataTransfer(op) {
    offsetHi = Utils::getRegBits(op, OFFSETHI_FLAG_MASK, OFFSETHI_FLAG_SHIFT);
    offsetLo = Utils::getRegBits(op, OFFSETLO_FLAG_MASK, OFFSETLO_FLAG_SHIFT);

    offsetVal = (offsetHi << 4) | offsetLo;
}

uint16_t HalfwordDataTransferOffset::getOffsetVal() {
    return offsetVal;
}

std::string HalfwordDataTransferOffset::toString(){
    std::string base = getOpMnemonic() + getCondFieldMnemonic() + getSFlagMnemonic() + getHFlagMnemonic() +  + " " + \
                        getRegMnemonic(rd) + ",";
    std::string address = "[" + getRegMnemonic(rn);

    if(p == 1){
        address += "," + Utils::toHexString(getOffsetVal()) + "]" + getWFlagMnemonic();
    }else if(p == 0){
        address += "]," + getUFlagMnemonic() + Utils::toHexString(getOffsetVal());
    }else{
        std::cerr << "ERROR: Invalid p flag value HalfwordDataTransferOffset" << std::endl;
    }

    return base + address;
}