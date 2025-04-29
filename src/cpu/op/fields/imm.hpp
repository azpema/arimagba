#ifndef _OPCODE_FIELD_IMM_
#define _OPCODE_FIELD_IMM_

#include "../../../utils/utils.hpp"
#include "../opcode.hpp"
#include "operand.hpp"
#include <iostream>

class Imm : public Operand {
  public:
    Imm(uint16_t val);
    ~Imm() override;
    void init(uint16_t val);
    std::string toString();
    uint32_t getMnemonicVal();

    uint32_t getOperandVal(ARM7TDMI& cpu) override;
    bool getC() override;

  private:
    bool c;
    uint16_t immVal;
};

#endif