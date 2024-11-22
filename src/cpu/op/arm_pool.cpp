#include "arm_pool.hpp"
#include <memory>

#include "../arm7tdmi.hpp"

// ARM
#include "arm/data_processing.hpp"
#include "arm/branch.hpp"
#include "arm/branch_and_exchange.hpp"
#include "arm/single_data_transfer.hpp"
#include "arm/block_data_transfer.hpp"
#include "arm/psr_transfer_mrs.hpp"
#include "arm/psr_transfer_msr.hpp"
#include "arm/undefined.hpp"
#include "arm/software_interrupt.hpp"
#include "arm/multiply_accumulate.hpp"
#include "arm/multiply_accumulate_long.hpp"
#include "arm/single_data_swap.hpp"
#include "arm/halfword_data_transfer_register.hpp"
#include "arm/halfword_data_transfer_offset.hpp"

ArmPool::ArmPool(ARM7TDMI &cpu){
    armPool.insert({ArmOpcode::OpCodeEnum::BRANCH_AND_EXCHANGE, std::make_unique<BranchAndExchange>(cpu)});
    armPool.insert({ArmOpcode::OpCodeEnum::BLOCK_DATA_TRANSFER, std::make_unique<BlockDataTransfer>(cpu)});
    armPool.insert({ArmOpcode::OpCodeEnum::BRANCH, std::make_unique<ARM::Branch>(cpu)});
    armPool.insert({ArmOpcode::OpCodeEnum::SOFTWARE_INTERRUPT, std::make_unique<ARM::SoftwareInterrupt>(cpu)});
    armPool.insert({ArmOpcode::OpCodeEnum::UNDEFINED, std::make_unique<Undefined>(cpu)});
    armPool.insert({ArmOpcode::OpCodeEnum::SINGLE_DATA_TRANSFER, std::make_unique<SingleDataTransfer>(cpu)});
    armPool.insert({ArmOpcode::OpCodeEnum::SINGLE_DATA_SWAP, std::make_unique<SingleDataSwap>(cpu)});
    armPool.insert({ArmOpcode::OpCodeEnum::MULTIPLY, std::make_unique<MultiplyAccumulate>(cpu)});
    armPool.insert({ArmOpcode::OpCodeEnum::MULTIPLY_LONG, std::make_unique<MultiplyAccumulateLong>(cpu)});
    armPool.insert({ArmOpcode::OpCodeEnum::HALFWORD_DATA_TRANSFER_REGISTER, std::make_unique<HalfwordDataTransferRegister>(cpu)});
    armPool.insert({ArmOpcode::OpCodeEnum::HALFWORD_DATA_TRANSFER_OFFSET, std::make_unique<HalfwordDataTransferOffset>(cpu)});
    armPool.insert({ArmOpcode::OpCodeEnum::PSR_TRANSFER_MRS, std::make_unique<PSRTransferMRS>(cpu)});
    armPool.insert({ArmOpcode::OpCodeEnum::PSR_TRANSFER_MSR, std::make_unique<PSRTransferMSR>(cpu)});
    armPool.insert({ArmOpcode::OpCodeEnum::DATA_PROCESSING, std::make_unique<DataProcessing>(cpu)});
}
ArmOpcode* ArmPool::getArmInstance(ArmOpcode::OpCodeEnum key) const{
    return armPool.at(key).get();
}