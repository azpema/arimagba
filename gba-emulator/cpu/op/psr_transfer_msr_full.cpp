#include "psr_transfer_msr_full.hpp"

PSRTransferMSRFull::PSRTransferMSRFull(uint32_t op): PSRTransferMSR::PSRTransferMSR(op) {
    Rm = Utils::getRegBits(op, RM_MASK, RM_SHIFT);
}

std::string PSRTransferMSRFull::toString() {
    return PSRTransferMSR::toString() + ",r" + std::to_string(Rm);
}