#ifndef _OPCODE_FIELD_SHIFT_RM_
#define _OPCODE_FIELD_SHIFT_RM_

#include <iostream>
#include "operand.hpp"
#include "../opcode.hpp"
#include "../../../utils/utils.hpp"

class ShiftRm : public Operand {
    public:
		ShiftRm(uint16_t val);
        ShiftRm(uint8_t rm, bool shiftAmount, uint8_t amount, uint8_t type);
        uint16_t getShiftType();
        std::string getShiftTypeMnemonic();
        bool getC() override;
        uint16_t getShiftAmount();
        uint16_t getRm();
        uint32_t getOperandVal(ARM7TDMI &cpu) override;
        enum Type {AMOUNT, REGISTER};
        Type getType();
        uint16_t getShiftReg();
    private:
        bool c;
        uint16_t _shift, _rm, _shiftType, _shiftAmount, _shiftReg;
        uint32_t shiftAmount, operand2;

        
        enum Type type;

        enum ShiftType {LSL = 0b00, LSR = 0b01, ASR = 0b10, ROR = 0b11};

        const static uint32_t SHIFT_MASK = 0b111111110000;
        const static uint32_t SHIFT_SHIFT = 4;

        const static uint32_t RM_MASK = 0b000000001111;
        const static uint32_t RM_SHIFT = 0;

        // If shift ends with 0b0
        const static uint16_t SHIFT_AMOUNT_MASK = 0b11111000;
        const static uint16_t SHIFT_AMOUNT_SHIFT = 3;

        const static uint16_t SHIFT_TYPE_MASK = 0b00000110;
        const static uint16_t SHIFT_TYPE_SHIFT = 1;

        // If shift ends with 0b1
        const static uint16_t SHIFT_RS_MASK = 0b11110000;
        const static uint16_t SHIFT_RS_SHIFT = 4;

        const std::string shiftType2Mnemonic[4] = {"lsl", "lsr", "asr", "ror"};

};

#endif