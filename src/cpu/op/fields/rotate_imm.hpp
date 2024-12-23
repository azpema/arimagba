#ifndef _OPCODE_FIELD_ROTATE_IMM_
#define _OPCODE_FIELD_ROTATE_IMM_

#include "operand.hpp"

class RotateImm : public Operand {
	public:
        enum class ConstructorType {RAW_ROTATE_IMM, FINAL_IMM_VAL};
		RotateImm(uint32_t val, ConstructorType constructorType=ConstructorType::RAW_ROTATE_IMM);
        RotateImm(uint8_t rot, uint8_t imm);
        ~RotateImm() override;
        void init(uint32_t val, ConstructorType constructorType=ConstructorType::RAW_ROTATE_IMM);
        void init(uint8_t rot, uint8_t imm);
        uint32_t getMnemonicVal();

        uint32_t getOperandVal(ARM7TDMI &cpu) override;
        uint32_t getRorShiftAmount();
        bool getC() override;

        

    private:
        bool c;
        uint16_t rotate, imm;
        uint32_t operandVal;

        const static uint32_t ROTATE_MASK = 0b111100000000;
        const static uint32_t ROTATE_SHIFT = 8;

        const static uint32_t IMM_MASK = 0b000011111111;
        const static uint32_t IMM_SHIFT = 0;
};

#endif