#ifndef _ARM7TDMI_
#define _ARM7TDMI_

class OpCode;
class BarrelShifter;
class ExceptionHandler;
class MemoryManager;

#include "registers/cpsr.hpp"
#include "../memory/memory_manager.hpp"
#include "components/alu.hpp"
#include "components/barrel_shifter.hpp"
#include "op/arm_pool.hpp"
#include "op/thumb_pool.hpp"
#include <functional>
#include <iostream>
#include <memory>

class ARM7TDMI {
  public:
    ARM7TDMI(MemoryManager* memoryManager);
    ~ARM7TDMI();

    OpCode* decodeInstruction(uint32_t op);

    PSR& getCPSR();
    PSR& getSPSR();

    void setCPSR(uint32_t val);
    void setSPSR(uint32_t val);

    ALU& getALU();
    BarrelShifter& getBarrelShifter();
    MemoryManager& getMemManager();
    ExceptionHandler& getExceptionHandler();

    int64_t fetchInstructionThumb(uint32_t offset);
    int64_t fetchInstructionArm(uint32_t offset);
    uint32_t fetchNextInstruction();
    uint32_t executeNextInstruction();
    bool getMustFlushPipeline() const;
    void setMustFlushPipeline(bool val);
    void printStatus();
    void printRegisterValues();

    uint32_t getReg(uint16_t n, bool userBank = false);
    void setReg(uint16_t n, uint32_t val, bool userBank = false);

    uint32_t getPC() const;
    void setPC(uint32_t pc);
    void setLR(uint32_t lr);

    ArmPool& getArmPool();
    bool isPcInBios() const;

    // Cycle counting. INACCURATE
    // "if you want to, you can count each one of S/N/I cycles as 1 cycle too"
    const static uint32_t CPU_CYCLES_PER_S_CYCLE = 1;
    const static uint32_t CPU_CYCLES_PER_N_CYCLE = 1;
    const static uint32_t CPU_CYCLES_PER_I_CYCLE = 1;
    constexpr static int REG_CNT = 16;

  private:
    void generateArmDecodingLookup();
    void generateThumbDecodingLookup();
    PSR& getCorrespondingSPSR();
    uint32_t getSPSRval();

    OpCode* decodeInstructionARM(uint32_t op);
    OpCode* decodeInstructionThumb(uint16_t op);

    ArmPool armPool;
    ThumbPool thumbPool;
    /*
    The ARM7TDMI processor has a total of 37 registers:
            - 31 general-purpose 32-bit registers:
                    * r0-r15
                    * r8_fiq-r14_fiq
                    * r13_svc-r14_svc
                    * r13_abt-r14_abt
                    * r13_irq-r14_irq
                    * r13_und-r14_und
            - 6 status registers:
                    * CPSR
                    * SPSR_fiq
                    * SPSR_svc
                    * SPSR_abt
                    * SPSR_irq
                    * SPSR_und

    Registers r0 to r13 are general-purpose registers used to hold either data or address values.
    Registers r14 and r15 have the following special functions:

    Register 14 is used as the subroutine Link Register (LR)

    Register 15 holds the PC
    In ARM state, bits [1:0] of r15 are undefined and must be ignored.
    Bits [31:2] contain the PC.
    In Thumb state, bit [0] is undefined and must be ignored. Bits
    [31:1] contain the PC.

    By convention, r13 is used as the Stack Pointer (SP).
    */

    constexpr static int REG_SP = 13;
    constexpr static int REG_LR = 14;
    constexpr static int REG_PC = 15;
    uint32_t reg[REG_CNT];
    uint32_t r8_fiq[7];
    uint32_t r13_svc[2];
    uint32_t r13_abt[2];
    uint32_t r13_irq[2];
    uint32_t r13_und[2];
    PSR cpsr;
    // // ARM test 254
    // https://github.com/jsmolka/gba-tests/blob/master/arm/psr_transfer.asm
    PSR spsr_sys_usr;
    PSR spsr_fiq;
    PSR spsr_svc;
    PSR spsr_abt;
    PSR spsr_irq;
    PSR spsr_und;

    ALU alu;
    MemoryManager* mem;
    std::unique_ptr<BarrelShifter> barrelShifter;
    std::unique_ptr<ExceptionHandler> exceptionHandler;

    // Pipeline
    uint32_t insFetch, insDecode;
    OpCode* opExecute;
    bool insFetchSet = false;
    bool insDecodeSet = false;
    bool insExecuteSet = false;
    bool mustFlushPipeline = false;
    uint32_t fetchPC;
};

#endif