#include "conditional_branch.hpp"
#include "../arm/branch.hpp"
#include "../arm/arm_opcode.hpp"

using namespace Thumb;

const std::string ConditionalBranch::cond2Mnemonic[16] = {"eq", "ne", "cs", "cc",
                                                          "mi", "pl", "vs", "vc",
                                                          "hi", "ls", "ge", "lt",
                                                          "gt", "le", "ERR", "ERR"};

ConditionalBranch::ConditionalBranch(uint16_t op, uint32_t pc, ARM7TDMI &cpu): ThumbOpCode::ThumbOpCode(op, cpu) {
    cond = Utils::getRegBits(op, COND_MASK, COND_SHIFT);
    sOffset8 = Utils::getRegBits(op, SOFFSET8_MASK, SOFFSET8_SHIFT);
    oldPC = pc;
    int16_t signExtended = Utils::twosComplementExtendSignTo(sOffset8 << 1, 9, 16);
    offsetVal = oldPC + signExtended + 4;
}

std::string ConditionalBranch::getCondMnemonic(){
    return cond2Mnemonic[cond];
}

std::string ConditionalBranch::toString(){
    return "b" + getCondMnemonic() +   " #" + Utils::toHexString(offsetVal);
}

void ConditionalBranch::doDecode(){

}

/*

bool ArmOpcode::execute(){
    // Execute only if conditions are met
    bool execute = false;
    switch(cond){
        case EQ:
            execute = cpu.getCPSR().getZFlag();
            break;
        case NE:
            execute = !cpu.getCPSR().getZFlag();
            break;
        case CS:
            execute = cpu.getCPSR().getCFlag();
            break;
        case CC:
            execute = !cpu.getCPSR().getCFlag();
            break;
        case MI:
            execute = cpu.getCPSR().getNFlag();
            break;
        case PL:
            execute = !cpu.getCPSR().getNFlag();
            break;
        case VS:
            execute = cpu.getCPSR().getVFlag();
            break;
        case VC:
            execute = !cpu.getCPSR().getVFlag();
            break;
        case HI:
            execute = (cpu.getCPSR().getCFlag()) && (!cpu.getCPSR().getZFlag());
            break;
        case LS:
            execute = (!cpu.getCPSR().getCFlag()) || (cpu.getCPSR().getZFlag());
            break;
        case GE:
            execute = (cpu.getCPSR().getNFlag() == cpu.getCPSR().getVFlag());
            break;
        case LT:
            execute = (cpu.getCPSR().getNFlag() != cpu.getCPSR().getVFlag());
            break;
        case GT:
            execute = (!cpu.getCPSR().getZFlag()) && (cpu.getCPSR().getNFlag() == cpu.getCPSR().getVFlag());
            break;
        case LE:
            execute = (cpu.getCPSR().getZFlag()) || (cpu.getCPSR().getNFlag() != cpu.getCPSR().getVFlag());
            break;
        case AL:
            execute = true;
            break;
    }
        
    if(execute){
        doExecute();
        return true;
    }else {
        //std::cout << "DEBUG: Instructions skipped; condition not met" << std::endl;
        return false;
    }

}



*/

void ConditionalBranch::doExecute(){
    bool execute = false;
    switch(cond){
        case ArmOpcode::EQ:
            execute = cpu.getCPSR().getZFlag();
            break;
        case ArmOpcode::NE:
            execute = !cpu.getCPSR().getZFlag();
            break;
        case ArmOpcode::CS:
            execute = cpu.getCPSR().getCFlag();
            break;
        case ArmOpcode::CC:
            execute = !cpu.getCPSR().getCFlag();
            break;
        case ArmOpcode::MI:
            execute = cpu.getCPSR().getNFlag();
            break;
        case ArmOpcode::PL:
            execute = !cpu.getCPSR().getNFlag();
            break;
        case ArmOpcode::VS:
            execute = cpu.getCPSR().getVFlag();
            break;
        case ArmOpcode::VC:
            execute = !cpu.getCPSR().getVFlag();
            break;
        case ArmOpcode::HI:
            execute = (cpu.getCPSR().getCFlag()) && (!cpu.getCPSR().getZFlag());
            break;
        case ArmOpcode::LS:
            execute = (!cpu.getCPSR().getCFlag()) || (cpu.getCPSR().getZFlag());
            break;
        case ArmOpcode::GE:
            execute = (cpu.getCPSR().getNFlag() == cpu.getCPSR().getVFlag());
            break;
        case ArmOpcode::LT:
            execute = (cpu.getCPSR().getNFlag() != cpu.getCPSR().getVFlag());
            break;
        case ArmOpcode::GT:
            execute = (!cpu.getCPSR().getZFlag()) && (cpu.getCPSR().getNFlag() == cpu.getCPSR().getVFlag());
            break;
        case ArmOpcode::LE:
            execute = (cpu.getCPSR().getZFlag()) || (cpu.getCPSR().getNFlag() != cpu.getCPSR().getVFlag());
            break;
        case ArmOpcode::AL:
            execute = true;
            break;
    }
        
    if(execute){
        ARM::Branch opArm = ARM::Branch(oldPC, sOffset8, offsetVal, cond, cpu);
        opArm.doExecute();
        //std::cout << "<< ARM >> " << opArm.toString() << std::endl;
    }else{
        //std::cout << "Condition not met; skipping instruction" << std::endl;
    }
}

uint32_t ConditionalBranch::cyclesUsed() const {
    return 1;
}