#ifndef _THUMB_OPCODE_ALU_OPERATIONS_
#define _THUMB_OPCODE_ALU_OPERATIONS_

#include "thumb_opcode.hpp"

class ALUOperations : public ThumbOpCode {
  public:
    ALUOperations(uint16_t op, ARM7TDMI& cpu);
    ALUOperations(ARM7TDMI& cpu);
    void init(uint32_t op) override;
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    uint16_t opField, rs, rd;

    const static uint16_t OP_MASK = 0b0000001111000000;
    const static uint16_t OP_SHIFT = 6;

    const static uint16_t RS_MASK = 0b0000000000111000;
    const static uint16_t RS_SHIFT = 3;

    const static uint16_t RD_MASK = 0b0000000000000111;
    const static uint16_t RD_SHIFT = 0;

    enum Opcode {
        AND = 0b0000,
        EOR = 0b0001,
        LSL = 0b0010,
        LSR = 0b0011,
        ASR = 0b0100,
        ADC = 0b0101,
        SBC = 0b0110,
        ROR = 0b0111,
        TST = 0b1000,
        NEG = 0b1001,
        CMP = 0b1010,
        CMN = 0b1011,
        ORR = 0b1100,
        MUL = 0b1101,
        BIC = 0b1110,
        MVN = 0b1111
    };

    const static std::string op2Mnemonic[16];
    std::string getOpMnemonic();
};

#endif