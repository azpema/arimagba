#include "multiply_accumulate.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"

const std::string MultiplyAccumulate::sFlag2Mnemonic[2] = {"", "s"};
const std::string MultiplyAccumulate::op2Mnemonic[2] = {"mul", "mla"};

MultiplyAccumulate::MultiplyAccumulate(uint32_t op, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(op, cpu) {
    init(op);
}

MultiplyAccumulate::MultiplyAccumulate(uint32_t a, uint32_t s, uint32_t rd, uint32_t rn, uint32_t rs, uint32_t rm, ARM7TDMI &cpu): ArmOpcode::ArmOpcode(cpu) {
    init(a, s, rd, rn, rs, rm);
}

MultiplyAccumulate::MultiplyAccumulate(ARM7TDMI &cpu): ArmOpcode::ArmOpcode(cpu) {}

void MultiplyAccumulate::init(uint32_t op){
    ArmOpcode::init(op);
    a = Utils::getRegBits(op, A_FLAG_MASK, A_FLAG_SHIFT);
    s = Utils::getRegBits(op, S_FLAG_MASK, S_FLAG_SHIFT);
    rd = Utils::getRegBits(op, RD_FLAG_MASK, RD_FLAG_SHIFT);
    rn = Utils::getRegBits(op, RN_FLAG_MASK, RN_FLAG_SHIFT);
    rs = Utils::getRegBits(op, RS_FLAG_MASK, RS_FLAG_SHIFT);
    rm = Utils::getRegBits(op, RM_FLAG_MASK, RM_FLAG_SHIFT);
}

void MultiplyAccumulate::init(uint32_t a, uint32_t s, uint32_t rd, uint32_t rn, uint32_t rs, uint32_t rm){
    this->a = a;
    this->s = s;
    this->rd = rd;
    this->rn = rn;
    this->rs = rs;
    this->rm = rm;
}

std::string MultiplyAccumulate::getSFlagMnemonic(){
    return sFlag2Mnemonic[s];
}

std::string MultiplyAccumulate::getOpMnemonic(){
    return op2Mnemonic[a];
}

std::string MultiplyAccumulate::toString(){
    std::string mnemonic = getOpMnemonic() + getCondFieldMnemonic() + getSFlagMnemonic() + " " + \
                           OpCode::getRegMnemonic(rd) + "," + OpCode::getRegMnemonic(rm) + "," + OpCode::getRegMnemonic(rs);
    if(a == 1)
        mnemonic += " " + OpCode::getRegMnemonic(rn);
    return mnemonic;
}

void MultiplyAccumulate::doDecode(){

}

void MultiplyAccumulate::doExecute(){
    if(rd == rm){
        //throw std::runtime_error("MultiplyAccumulate::doExecute: The destination register Rd must not be the same as the operand register Rm");
        std::cout << "MultiplyAccumulate::doExecute: The destination register Rd must not be the same as the operand register Rm" << '\n';
    }
    if(rd==15 || rn==15 || rs==15 || rm==15){
        throw std::runtime_error("MultiplyAccumulate::doExecute: R15 must not be used as an operand or as the destination register");
    }

    uint32_t res = 0xDEAD;
    if(a == 0){
        res = cpu.getALU().mul(cpu.getReg(rm), cpu.getReg(rs));
    }else if(a == 1){
        res = cpu.getALU().mla(cpu.getReg(rm), cpu.getReg(rs), cpu.getReg(rn));
    }

    cpu.setReg(rd, res);

    if(s == 1){
        cpu.getCPSR().setNFlag(cpu.getALU().getN());
        cpu.getCPSR().setZFlag(cpu.getALU().getZ());
    }
}

// MUL              1S+ml
// MLA              1S+(m+1)I
uint32_t MultiplyAccumulate::cyclesUsed() const {
    //std::cerr << "TODO: MultiplyAccumulate::cyclesUsed" << '\n';
    return 1 * ARM7TDMI::CPU_CYCLES_PER_S_CYCLE + (0 + 1) * ARM7TDMI::CPU_CYCLES_PER_I_CYCLE;
}