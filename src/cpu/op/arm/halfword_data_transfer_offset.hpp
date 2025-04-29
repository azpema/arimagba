#ifndef _OPCODE_HALFWORD_DATA_TRANSFER_OFFSET_
#define _OPCODE_HALFWORD_DATA_TRANSFER_OFFSET_

#include "../../../utils/utils.hpp"
#include "halfword_data_transfer.hpp"
#include <iostream>

class HalfwordDataTransferOffset : public HalfwordDataTransfer {
  public:
    HalfwordDataTransferOffset(uint32_t op, ARM7TDMI& cpu);
    HalfwordDataTransferOffset(ARM7TDMI& cpu);
    HalfwordDataTransferOffset(uint16_t p,
                               uint16_t u,
                               uint16_t w,
                               uint16_t l,
                               uint16_t rn,
                               uint16_t rd,
                               uint16_t s,
                               uint16_t h,
                               uint16_t offsetHi,
                               uint16_t offsetLo,
                               ARM7TDMI& cpu);
    void init(uint32_t op) override;
    void init(uint16_t offsetHi, uint16_t offsetLo);
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    uint16_t offsetHi, offsetLo, offsetVal;

    const static uint32_t OFFSETHI_FLAG_MASK = 0b00000000000000000000111100000000;
    const static uint32_t OFFSETHI_FLAG_SHIFT = 8;

    const static uint32_t OFFSETLO_FLAG_MASK = 0b00000000000000000000000000001111;
    const static uint32_t OFFSETLO_FLAG_SHIFT = 0;

    uint16_t getOffsetVal();
};

#endif