#ifndef _OPCODE_FIELD_RM_
#define _OPCODE_FIELD_RM_

#include "operand.hpp"

class Rm : public Operand {
  public:
    Rm(uint16_t val);
    ~Rm() override;
    void init(uint16_t val);
    std::string toString();
    uint16_t getRmVal();
    bool getC() override;
    uint32_t getOperandVal(ARM7TDMI& cpu) override;

  private:
    bool c;
    uint16_t rmVal;
    const static uint16_t RM_MASK = 000000001111;
    const static uint16_t RM_SHIFT = 0;
};

#endif