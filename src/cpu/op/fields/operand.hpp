#ifndef _OPCODE_FIELD_OPERAND_
#define _OPCODE_FIELD_OPERAND_

#include "../../../utils/utils.hpp"
#include "../opcode.hpp"
#include <iostream>

class Operand {
  protected:
    uint16_t value;

  public:
    enum OperandType { SHIFT_RM, ROTATE_IMM, IMM, RM };
    OperandType _type;
    Operand(uint16_t val, OperandType _type);
    void init(uint16_t val, OperandType _type);
    virtual ~Operand(){};
    virtual uint32_t getOperandVal(ARM7TDMI& cpu) = 0;
    virtual bool getC() = 0;

    uint16_t getRawVal();
    void setRawVal(uint16_t val);
};

#endif