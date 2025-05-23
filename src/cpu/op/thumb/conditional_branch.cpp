#include "conditional_branch.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"
#include "../arm/branch.hpp"

using namespace Thumb;

const std::string ConditionalBranch::cond2Mnemonic[16] = {
    "eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", "ERR", "ERR"};

ConditionalBranch::ConditionalBranch(uint16_t op, ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(op, cpu) { init(op); }

ConditionalBranch::ConditionalBranch(ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(cpu) {}

void ConditionalBranch::init(uint32_t op) {
    ThumbOpCode::init(op);
    cond = Utils::getRegBits(op, COND_MASK, COND_SHIFT);
    sOffset8 = Utils::getRegBits(op, SOFFSET8_MASK, SOFFSET8_SHIFT);
    oldPC = cpu.getPC() - 2;
    int16_t signExtended = Utils::twosComplementExtendSignTo(sOffset8 << 1, 9, 16);
    offsetVal = oldPC + signExtended + 4;
}

std::string ConditionalBranch::getCondMnemonic() { return cond2Mnemonic[cond]; }

std::string ConditionalBranch::toString() { return "b" + getCondMnemonic() + " #" + Utils::toHexString(offsetVal); }

void ConditionalBranch::doDecode() {}

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
        //std::cout << "DEBUG: Instructions skipped; condition not met" << '\n';
        return false;
    }

}



*/

void ConditionalBranch::doExecute() {
    bool execute = false;
    switch (static_cast<ArmOpcode::Condition>(cond)) {
    case ArmOpcode::Condition::EQ:
        execute = cpu.getCPSR().getZFlag();
        break;
    case ArmOpcode::Condition::NE:
        execute = !cpu.getCPSR().getZFlag();
        break;
    case ArmOpcode::Condition::CS:
        execute = cpu.getCPSR().getCFlag();
        break;
    case ArmOpcode::Condition::CC:
        execute = !cpu.getCPSR().getCFlag();
        break;
    case ArmOpcode::Condition::MI:
        execute = cpu.getCPSR().getNFlag();
        break;
    case ArmOpcode::Condition::PL:
        execute = !cpu.getCPSR().getNFlag();
        break;
    case ArmOpcode::Condition::VS:
        execute = cpu.getCPSR().getVFlag();
        break;
    case ArmOpcode::Condition::VC:
        execute = !cpu.getCPSR().getVFlag();
        break;
    case ArmOpcode::Condition::HI:
        execute = (cpu.getCPSR().getCFlag()) && (!cpu.getCPSR().getZFlag());
        break;
    case ArmOpcode::Condition::LS:
        execute = (!cpu.getCPSR().getCFlag()) || (cpu.getCPSR().getZFlag());
        break;
    case ArmOpcode::Condition::GE:
        execute = (cpu.getCPSR().getNFlag() == cpu.getCPSR().getVFlag());
        break;
    case ArmOpcode::Condition::LT:
        execute = (cpu.getCPSR().getNFlag() != cpu.getCPSR().getVFlag());
        break;
    case ArmOpcode::Condition::GT:
        execute = (!cpu.getCPSR().getZFlag()) && (cpu.getCPSR().getNFlag() == cpu.getCPSR().getVFlag());
        break;
    case ArmOpcode::Condition::LE:
        execute = (cpu.getCPSR().getZFlag()) || (cpu.getCPSR().getNFlag() != cpu.getCPSR().getVFlag());
        break;
    case ArmOpcode::Condition::AL:
        execute = true;
        break;
    }

    if (execute) {
        // ARM::Branch opArm = ARM::Branch(sOffset8, offsetVal, cond, cpu);
        ARM::Branch* opArm = static_cast<ARM::Branch*>(cpu.getArmPool().getArmInstance(ArmOpcode::OpCodeEnum::BRANCH));
        opArm->init(sOffset8, offsetVal, cond);
        opArm->doExecute();
        // std::cout << "<< ARM >> " << opArm.toString() << '\n';
    } else {
        // std::cout << "Condition not met; skipping instruction" << '\n';
    }
}

uint32_t ConditionalBranch::cyclesUsed() const { return 1; }