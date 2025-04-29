#ifndef _THUMB_OPCODE_LOAD_ADDRESS_
#define _THUMB_OPCODE_LOAD_ADDRESS_

#include "thumb_opcode.hpp"

class LoadAddress : public ThumbOpCode {
  public:
    LoadAddress(uint16_t op, ARM7TDMI& cpu);
    LoadAddress(ARM7TDMI& cpu);
    void init(uint32_t op) override;
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    uint8_t sp, rd, word8;
    uint16_t offsetVal;

    const static uint16_t SP_MASK = 0b0000100000000000;
    const static uint16_t SP_SHIFT = 11;

    const static uint16_t RD_MASK = 0b0000011100000000;
    const static uint16_t RD_SHIFT = 8;

    const static uint16_t WORD8_MASK = 0b0000000011111111;
    const static uint16_t WORD8_SHIFT = 0;

    const static uint8_t sp2RegVal[2];
    uint8_t getSPRegVal();
};

#endif