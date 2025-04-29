#ifndef _OPCODE_BLOCK_DATA_TRANSFER_
#define _OPCODE_BLOCK_DATA_TRANSFER_

#include "arm_opcode.hpp"
#include <iostream>
#include <vector>

class BlockDataTransfer : public ArmOpcode {
  public:
    BlockDataTransfer(uint32_t op, ARM7TDMI& cpu);
    BlockDataTransfer(ARM7TDMI& cpu);
    BlockDataTransfer(uint16_t _p,
                      uint16_t _u,
                      uint16_t _s,
                      uint16_t _w,
                      uint16_t _l,
                      uint16_t _rn,
                      uint16_t _registerList,
                      ARM7TDMI& cpu);
    void init(uint32_t op) override;
    void init(uint16_t _p, uint16_t _u, uint16_t _s, uint16_t _w, uint16_t _l, uint16_t _rn, uint16_t _registerList);
    std::string toString() override;
    void doExecute() override;
    void doDecode() override;
    uint32_t cyclesUsed() const override;

  private:
    std::string getSFlagMnemonic();
    std::string getWFlagMnemonic();
    std::string getRegisterListMnemonic();
    std::string getOpAddressingModeMnemonic();

    uint16_t p, u, s, w, l, rn, registerList;
    std::vector<uint32_t> registerListVec;

    const static uint32_t P_MASK = 0b00000001000000000000000000000000;
    const static uint32_t P_SHIFT = 24;

    const static uint32_t U_MASK = 0b00000000100000000000000000000000;
    const static uint32_t U_SHIFT = 23;

    const static uint32_t S_MASK = 0b00000000010000000000000000000000;
    const static uint32_t S_SHIFT = 22;
    const static std::string SFlag2Mnemonic[2];

    const static uint32_t W_MASK = 0b00000000001000000000000000000000;
    const static uint32_t W_SHIFT = 21;
    const static std::string WFlag2Mnemonic[2];

    const static uint32_t L_MASK = 0b00000000000100000000000000000000;
    const static uint32_t L_SHIFT = 20;

    const static uint32_t RN_MASK = 0b00000000000011110000000000000000;
    const static uint32_t RN_SHIFT = 16;

    const static uint32_t REGISTER_LIST_MASK = 0b00000000000000001111111111111111;
    const static uint32_t REGISTER_LIST_SHIFT = 0;

    // L, P, U
    // TODO "Other" column: LDMIB, LDMIA, LDMDB...
    const static std::string opAddressingMode2Mnemonic[2][2][2];
};

#endif