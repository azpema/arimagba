#include "rotate_imm.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

// ROR by 32 has result equal to Rm, carry out equal to bit 31 of Rm.
RotateImm::RotateImm(uint32_t val, ConstructorType constructorType) :
    Operand::Operand(0xDEAD, OperandType::ROTATE_IMM) {
    init(val, constructorType);
}

RotateImm::RotateImm(uint8_t rot, uint8_t imm) : Operand::Operand(0xDEAD, OperandType::ROTATE_IMM) { init(rot, imm); }

void RotateImm::init(uint32_t val, ConstructorType constructorType) {
    switch (constructorType) {
    case ConstructorType::FINAL_IMM_VAL: {
        uint8_t rotateVal = 0;
        uint16_t tmpOffsetVal = val;
        if (val > 0xFF) {
            while (tmpOffsetVal > 0xFF) {
                tmpOffsetVal = tmpOffsetVal >> 2;
                rotateVal += 2;
            }
            rotateVal = (32 - rotateVal) / 2;
        }
        init(rotateVal, tmpOffsetVal);
        break;
    }
    case ConstructorType::RAW_ROTATE_IMM: {
        rotate = Utils::getRegBits(val, ROTATE_MASK, ROTATE_SHIFT);
        imm = Utils::getRegBits(val, IMM_MASK, IMM_SHIFT);
        // TODO What if ROR#0 is specified
        operandVal = Utils::rotateRight(imm, 2 * rotate);
        c = ((operandVal & 0x80000000) >> 31) == 1;
        setRawVal(val);
        break;
    }
    default:
        throw std::runtime_error("Invalid ConstructorType in RotateImm constructor");
        break;
    }
}

void RotateImm::init(uint8_t rot, uint8_t imm) {
    Operand::init(0xDEAD, OperandType::ROTATE_IMM);
    this->rotate = rot;
    this->imm = imm;

    uint32_t raw = 0;
    Utils::setRegBits(raw, ROTATE_MASK, rot << ROTATE_SHIFT);
    Utils::setRegBits(raw, IMM_MASK, imm << IMM_SHIFT);
    setRawVal(raw);

    operandVal = Utils::rotateRight(imm, 2 * rot);
    c = ((operandVal & 0x80000000) >> 31) == 1;
}

RotateImm::~RotateImm() {}

uint32_t RotateImm::getMnemonicVal() { return operandVal; }

uint32_t RotateImm::getOperandVal(ARM7TDMI& /*cpu*/) { return getMnemonicVal(); }

uint32_t RotateImm::getRorShiftAmount() { return rotate; }

bool RotateImm::getC() { return c; }