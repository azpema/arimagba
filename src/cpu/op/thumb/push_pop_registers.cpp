#include "push_pop_registers.hpp"
#include "../../../utils/utils.hpp"
#include "../../arm7tdmi.hpp"
#include "../arm/block_data_transfer.hpp"

const std::string PushPopRegisters::op2Mnemonic[2] = {"push", "pop"};

PushPopRegisters::PushPopRegisters(uint16_t op, ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(op, cpu) { init(op); }

PushPopRegisters::PushPopRegisters(ARM7TDMI& cpu) : ThumbOpCode::ThumbOpCode(cpu) {}

void PushPopRegisters::init(uint32_t op) {
    ThumbOpCode::init(op);
    l = Utils::getRegBits(op, L_MASK, L_SHIFT);
    r = Utils::getRegBits(op, R_MASK, R_SHIFT);
    rList = Utils::getRegBits(op, RLIST_MASK, RLIST_SHIFT);

    for (size_t i = 0; i < 8; i++) {
        registerListVec[i] = ((rList >> i) & 0x1);
    }
}

std::string PushPopRegisters::getOpMnemonic() { return op2Mnemonic[l]; }

std::string PushPopRegisters::toString() {
    std::string mnemonic = getOpMnemonic() + " {";

    bool insertComma = false;
    for (size_t i = 0; i < 8; i++) {
        if (i == 1) {
            if (insertComma) {
                mnemonic += ",";
            }
            mnemonic += OpCode::getRegMnemonic(i);
            insertComma = true;
        }
    }

    if (r == 1) {
        if (l == 0) {
            mnemonic += "," + OpCode::getRegMnemonic(14);
        } else if (l == 1) {
            mnemonic += "," + OpCode::getRegMnemonic(15);
        }
    }

    mnemonic += "}";

    return mnemonic;
}

void PushPopRegisters::doDecode() {}

void PushPopRegisters::doExecute() {
    BlockDataTransfer* opArm = static_cast<BlockDataTransfer*>(
            cpu.getArmPool().getArmInstance(ArmOpcode::OpCodeEnum::BLOCK_DATA_TRANSFER));
    if (l == 0 && r == 0) {
        opArm->init(1, 0, 0, 1, 0, 13, rList);
    } else if (l == 1 && r == 0) {
        opArm->init(0, 1, 0, 1, 1, 13, rList);
    } else if (l == 0 && r == 1) {
        uint16_t rListWithLr = rList | (0x1 << 14);
        opArm->init(1, 0, 0, 1, 0, 13, rListWithLr);
    } else if (l == 1 && r == 1) {
        uint16_t rListWithPc = rList | (0x1 << 15);
        opArm->init(0, 1, 0, 1, 1, 13, rListWithPc);
    } else {
        throw std::runtime_error("Error: Unimplemented instruction: PushPopRegisters");
    }

    opArm->doExecute();
    // std::cout << "<< ARM >> " << opArm.toString() << '\n';
}

uint32_t PushPopRegisters::cyclesUsed() const { return 1; }