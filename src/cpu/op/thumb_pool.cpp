#include "thumb_pool.hpp"
#include <memory>

// Thumb
#include "thumb/move_comp_add_sub_imm.hpp"
#include "thumb/move_shifted_register.hpp"
#include "thumb/add_subtract.hpp"
#include "thumb/alu_operations.hpp"
#include "thumb/hi_register_branch_exchange.hpp"
#include "thumb/pc_relative_load.hpp"
#include "thumb/load_store_register_offset.hpp"
#include "thumb/push_pop_registers.hpp"
#include "thumb/add_offset_sp.hpp"
#include "thumb/sp_load_store.hpp"
#include "thumb/load_address.hpp"
#include "thumb/load_store_sign_extended.hpp"
#include "thumb/load_store_halfword.hpp"
#include "thumb/load_store_imm_offset.hpp"
#include "thumb/multiple_load_store.hpp"
#include "thumb/software_interrupt.hpp"
#include "thumb/unconditional_branch.hpp"
#include "thumb/conditional_branch.hpp"
#include "thumb/long_branch_with_link.hpp"

ThumbPool::ThumbPool(ARM7TDMI &cpu){
    thumbPool.insert({ThumbOpCode::OpCodeEnum::SOFTWARE_INTERRUPT, std::make_unique<Thumb::SoftwareInterrupt>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::ADD_OFFSET_TO_SP, std::make_unique<AddOffsetSP>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::PUSH_POP_REGISTERS, std::make_unique<PushPopRegisters>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::ALU_OPERATIONS, std::make_unique<ALUOperations>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::HI_REGISTER_BRANCH_EXCHANGE, std::make_unique<HiRegisterBranchExchange>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::PC_RELATIVE_LOAD, std::make_unique<PCRelativeLoad>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::LOAD_STORE_REGISTER_OFFSET, std::make_unique<LoadStoreRegisterOffset>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::LOAD_STORE_SIGN_EXTENDED, std::make_unique<LoadStoreSignExtended>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::UNCONDITIONAL_BRANCH, std::make_unique<Thumb::UnconditionalBranch>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::ADD_SUBTRACT, std::make_unique<AddSubtract>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::LOAD_STORE_HALFWORD, std::make_unique<LoadStoreHalfword>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::SP_LOAD_STORE, std::make_unique<SPLoadStore>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::LOAD_ADDRESS, std::make_unique<LoadAddress>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::MULTIPLE_LOAD_STORE, std::make_unique<MultipleLoadStore>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::CONDITIONAL_BRANCH, std::make_unique<Thumb::ConditionalBranch>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::LONG_BRANCH_WITH_LINK, std::make_unique<Thumb::LongBranchWithLink>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::MOVE_SHIFTED_REGISTER, std::make_unique<MoveShiftedRegister>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::MOVE_COMP_ADD_SUB_IMM, std::make_unique<MoveCompAddSubImm>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::LOAD_STORE_IMM_OFFSET, std::make_unique<LoadStoreImmOffset>(cpu)});
    thumbPool.insert({ThumbOpCode::OpCodeEnum::UNKNOWN, nullptr});
}

ThumbOpCode* ThumbPool::getThumbInstance(ThumbOpCode::OpCodeEnum key) const{
    return thumbPool.at(key).get();
}