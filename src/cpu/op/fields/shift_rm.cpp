#include "shift_rm.hpp"
#include <iostream>

ShiftRm::ShiftRm(uint16_t val) : Operand::Operand(val, OperandType::SHIFT_RM){
    c = false;
    _shift = Utils::getRegBits(val, SHIFT_MASK, SHIFT_SHIFT);
    _rm = Utils::getRegBits(val, RM_MASK, RM_SHIFT);
    _shiftType = Utils::getRegBits(_shift, SHIFT_TYPE_MASK, SHIFT_TYPE_SHIFT);

    shiftAmount = 0xDEAD;

    // Shift amount
    if((_shift & 0b00000001) == 0b00000000){
        _shiftAmount = Utils::getRegBits(_shift, SHIFT_AMOUNT_MASK, SHIFT_AMOUNT_SHIFT);
        type = AMOUNT;
    // SHift register
    }else if((_shift & 0b00001001) == 0b00000001){
        _shiftReg = Utils::getRegBits(_shift, SHIFT_RS_MASK, SHIFT_RS_SHIFT);
        type = REGISTER;
    }else{
        std::cerr << "ERROR: Unknown ShiftRm" << std::endl;
    }
}

ShiftRm::ShiftRm(uint8_t rm, bool typeShiftAmount, uint8_t amount, uint8_t type, uint8_t rs)
 : Operand::Operand(0xDEAD, OperandType::SHIFT_RM){
    c = false;
    _rm = rm;
    _shiftType = type;

    shiftAmount = 0xDEAD;

    // Shift amount
    if(typeShiftAmount){
        shiftAmount = amount;
        _shiftAmount = amount;
        type = AMOUNT;
        _shift = (((amount << 3) | (_shiftType << 1)) << 4) | (rm);
    // Shift register
    }else{
        _shiftReg = rs;
        type = REGISTER;
        uint32_t a = rs << 4;
        uint32_t b = _shiftType << 1;
        uint32_t c = ((a) | (b) | 1);
        _shift = (c << 4) | (rm);
    }

    setRawVal(_shift);
}

std::string ShiftRm::getShiftTypeMnemonic(){
    return shiftType2Mnemonic[_shiftType];
}

uint16_t ShiftRm::getShiftType(){
    return _shiftType;
}

uint16_t ShiftRm::getRm(){
    return _rm;
}

uint16_t ShiftRm::getShiftAmount(){
    return shiftAmount;
}

ShiftRm::Type ShiftRm::getType(){
    return type;
}

uint16_t ShiftRm::getShiftReg(){
    return _shiftReg;
}

bool ShiftRm::getC(){
    return c;
}

uint32_t ShiftRm::getOperandVal(ARM7TDMI &cpu){
    uint32_t rmVal = cpu.getReg(_rm);
    uint32_t res = 0;
    if(type == AMOUNT){
        shiftAmount = _shiftAmount;
    } else if(type == REGISTER){
        // The amount by which the register should be shifted may be contained in an immediate field
        // in the instruction, or in the bottom byte of another register (other than R15)
        // ...
        // Only the least significant byte of the contents of Rs is used to determine the shift
        // amount. Rs can be any general register other than R15.
        shiftAmount = cpu.getReg(_shiftReg) & 0xFF;
    }
        // If this byte is zero, the unchanged contents of Rm will be used as the second operand,
        // and the old value of the CPSR C flag will be passed on as the shifter carry output.
    if(type == AMOUNT && shiftAmount == 0){
        switch (static_cast<ShiftType>(_shiftType)){
            case LSL:
                // LSL#0: No shift performed, ie. directly Op2=Rm, the C flag is NOT affected.
                return rmVal;
                break;
            case LSR:
                // LSR#0: Interpreted as LSR#32, ie. Op2 becomes zero, C becomes Bit 31 of Rm.
                res = 0;
                c = Utils::getRegSingleBit(rmVal, 31);
                return res;
                break;
            case ASR:
                // LSR#0: Interpreted as LSR#32, ie. Op2 becomes zero, C becomes Bit 31 of Rm.
                c = Utils::getRegSingleBit(rmVal, 31);
                res = c * 0xFFFFFFFF;
                return res;
                break;
            case ROR:
                // ROR#0: Interpreted as RRX#1 (RCR), like ROR#1, but Op2 Bit 31 set to old C.
                res = cpu.getBarrelShifter().ror(cpu, rmVal, shiftAmount);
                c = cpu.getBarrelShifter().getC();
                return res;
                break;
            default:
                std::cerr << "ERROR: Invalid _shiftType value!" << std::endl;
                break;
        }
    }
    if(type == REGISTER){
        if(shiftAmount == 0){
            // If this byte is zero, the unchanged contents of Rm will be used as the second operand,
            // and the old value of the CPSR C flag will be passed on as the shifter carry output
            c = cpu.getCPSR().getCFlag();
            return cpu.getReg(_rm);
        }else if(shiftAmount == 32){
            switch (static_cast<ShiftType>(_shiftType)){
                case LSL:
                    // LSL by 32 has result zero, carry out equal to bit 0 of Rm
                    c = Utils::getRegSingleBit(rmVal, 0);
                    return 0;
                case LSR:
                    // LSR by 32 has result zero, carry out equal to bit 31 of Rm
                    c = Utils::getRegSingleBit(rmVal, 31);
                    return 0;
                case ASR:
                    // ASR by 32 or more has result filled with and carry out equal to bit 31 of Rm 
                    c = Utils::getRegSingleBit(rmVal, 31);
                    return c * 0xFFFFFFFF;
                    break;
                case ROR:
                    // ROR by 32 has result equal to Rm, carry out equal to bit 31 of Rm.
                    c = Utils::getRegSingleBit(rmVal, 31);
                    return rmVal;
                default:
                    std::cerr << "ERROR: Invalid _shiftType value!" << std::endl;
                    break;
            }
        }else if (shiftAmount > 32){
            switch (static_cast<ShiftType>(_shiftType)){
                case LSL:
                case LSR:
                    // LSL by more than 32 has result zero, carry out zero
                    // LSR by more than 32 has result zero, carry out zero
                    c = 0;
                    return 0;
                case ASR:
                    // ASR by 32 or more has result filled with and carry out equal to bit 31 of Rm 
                    c = Utils::getRegSingleBit(rmVal, 31);
                    return c * 0xFFFFFFFF;
                    break;
                case ROR:
                    // ROR by n where n is greater than 32 will give the same result and carry out
                    // as ROR by n-32; therefore repeatedly subtract 32 from n until the amount is
                    // in the range 1 to 32 and see above.
                    res = cpu.getBarrelShifter().ror(cpu, rmVal, shiftAmount);
                    c = cpu.getBarrelShifter().getC();
                    return res;
                default:
                    std::cerr << "ERROR: Invalid _shiftType value!" << std::endl;
                    break;
            }
        }
    }
    

    switch (static_cast<ShiftType>(_shiftType))
    {
    case LSL:
        res = cpu.getBarrelShifter().lsl(cpu, rmVal, shiftAmount);
        break;
    case LSR:
        res = cpu.getBarrelShifter().lsr(rmVal, shiftAmount);
        break;
    case ASR:
        res = cpu.getBarrelShifter().asr(rmVal, shiftAmount);
        break;
    case ROR:
        res = cpu.getBarrelShifter().ror(cpu, rmVal, shiftAmount);
        break;
    default:
        std::cerr << "ERROR: Invalid _shiftType value!" << std::endl;
        break;
    }

    c = cpu.getBarrelShifter().getC();
    return res;

}