#include "halfword_data_transfer_offset.hpp"
#include <bitset>


HalfwordDataTransferOffset::HalfwordDataTransferOffset(uint32_t op, ARM7TDMI &cpu): HalfwordDataTransfer::HalfwordDataTransfer(op, cpu) {
    init(op);
}

HalfwordDataTransferOffset::HalfwordDataTransferOffset(uint16_t p, uint16_t u, uint16_t w, uint16_t l, uint16_t rn,
 uint16_t rd, uint16_t s, uint16_t h, uint16_t offsetHi, uint16_t offsetLo, ARM7TDMI &cpu): HalfwordDataTransfer::HalfwordDataTransfer(p, u, w, l, rn, rd, s, h, cpu){
    init(offsetHi, offsetLo);
}

HalfwordDataTransferOffset::HalfwordDataTransferOffset(ARM7TDMI &cpu): HalfwordDataTransfer::HalfwordDataTransfer(cpu) {}

void HalfwordDataTransferOffset::init(uint32_t op){
    HalfwordDataTransfer::init(op);
    offsetHi = Utils::getRegBits(op, OFFSETHI_FLAG_MASK, OFFSETHI_FLAG_SHIFT);
    offsetLo = Utils::getRegBits(op, OFFSETLO_FLAG_MASK, OFFSETLO_FLAG_SHIFT);

    offsetVal = (offsetHi << 4) | offsetLo;
}

void HalfwordDataTransferOffset::init(uint16_t offsetHi, uint16_t offsetLo){
    this->offsetHi = offsetHi;
    this->offsetLo = offsetLo;

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
        throw std::runtime_error("ERROR: Invalid p flag value HalfwordDataTransferOffset");
    }

    return base + address;
}

// Depends on load or store
uint32_t HalfwordDataTransferOffset::cyclesUsed() const {
    //std::cerr << "TODO: HalfwordDataTransferOffset::cyclesUsed" << '\n';
    return 1;
}

void HalfwordDataTransferOffset::doDecode(){

}

void HalfwordDataTransferOffset::doExecute(){
    _doExecute(offsetVal);
}
