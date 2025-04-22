#include "hi_register_branch_exchange.hpp"
#include "../arm/data_processing.hpp"
#include "../../../utils/utils.hpp"
#include "../fields/shift_rm.hpp"
#include "../../arm7tdmi.hpp"

const std::string HiRegisterBranchExchange::op2Mnemonic[4] = {"add", "cmp", "mov", "bx"};

HiRegisterBranchExchange::HiRegisterBranchExchange(uint16_t op, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    init(op);
}

HiRegisterBranchExchange::HiRegisterBranchExchange(ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(cpu) {}

void HiRegisterBranchExchange::init(uint32_t op){
    ThumbOpCode::init(op);
    opField = Utils::getRegBits(op, OP_MASK, OP_SHIFT);
    h1 = Utils::getRegBits(op, H1_MASK, H1_SHIFT);
    h2 = Utils::getRegBits(op, H2_MASK, H2_SHIFT);
    rsHs = Utils::getRegBits(op, RS_HS_MASK, RS_HS_SHIFT);
    rdHd = Utils::getRegBits(op, RD_HD_MASK, RD_HD_SHIFT);
    rs = rsHs + (h2 * 8);
    rd = rdHd + (h1 * 8);
}

std::string HiRegisterBranchExchange::getOpMnemonic(){
    return op2Mnemonic[opField];
}

std::string HiRegisterBranchExchange::toString(){
    std::string mnemonic = getOpMnemonic() + " ";
    // bx
    if(opField != 0b11){
        mnemonic += OpCode::getRegMnemonic(rd) + ",";
    }
    mnemonic += OpCode::getRegMnemonic(rs);
    return mnemonic;
}

void HiRegisterBranchExchange::doDecode(){

}

void HiRegisterBranchExchange::doExecute(){
    uint8_t op = static_cast<HiRegisterBranchExchange::OP>(opField);
    uint32_t rsVal = cpu.getReg(rs);
    switch(op){
        case ADD:
        {
            ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
            DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_ADD_VAL, 0, rd, rd, shiftRm.getRawVal(), cpu);
            opArm.doExecute();
            //std::cout << "<< ARM >> " << opArm.toString() << '\n';
        }
        break;
        case CMP:
        {
            ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
            DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_CMP_VAL, 1, rd, rd, shiftRm.getRawVal(), cpu);
            opArm.doExecute();
            //std::cout << "<< ARM >> " << opArm.toString() << '\n';
        }
        break;
        case MOV:
        {
            //cpu.setReg(rd, rsVal);
            ShiftRm shiftRm = ShiftRm(rs, true, 0, 0);
            DataProcessing opArm = DataProcessing(0, DataProcessing::OPCODE_MOV_VAL, 0, rs, rd, shiftRm.getRawVal(), cpu);
            opArm.doExecute();
            //std::cout << "<< ARM >> " << opArm.toString() << '\n';
        }
        break;
        case BX:
            if(h1 == 1){
                throw std::runtime_error("Undefined instruction HiRegisterBranchExchange::doExecute BX, H1=1");
            }
            cpu.getCPSR().setTFlag((rsVal & 0x1) == 1);
            cpu.setPC(rsVal);
            cpu.getCPSR().setTFlag((rsVal & 0x1) == 1);
            cpu.setMustFlushPipeline(true);
        break;
    }
}

uint32_t HiRegisterBranchExchange::cyclesUsed() const {
    return 1;
}