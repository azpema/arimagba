#ifndef _DMA_ 
#define _DMA_

#include <cstdint>
#include "../memory/memory_manager.hpp"

template <uint8_t N>
class DMA {
    public:
        DMA(MemoryManager &mem);
        ~DMA() = default;

        void runCycle() const;
        
    private:
        enum class DstAdj {DMA_DST_INC = 0b00, DMA_DST_DEC = 0b01, DMA_DST_FIXED = 0b10, DMA_DST_RELOAD = 0b11};
        enum class SrcAdj {DMA_SRC_INC = 0b00, DMA_SRC_DEC = 0b01, DMA_SRC_FIXED = 0b10};
        enum class ChunkSize {DMA_16 = 0b0, DMA_32 = 0b1};
        enum class TimingMode {DMA_NOW = 0b00, DMA_AT_VBLANK = 0b01, DMA_AT_HBLANK = 0b10, DMA_AT_REFRESH = 0b11};
        
        bool isDmaEnabled() const;
        bool isRepeatEnabled() const;
        DstAdj getDestAdjustment() const;
        SrcAdj getSourceAdjustment() const;
        ChunkSize getChunkSize() const;
        TimingMode getTimingMode() const;
        bool isIrqEnabled() const;

        uint32_t getNumTransfers() const;

        uint8_t getTransferSizeBytes() const;
        void disableDma() const;

        MemoryManager &mem;
        uint8_t dmaNum;
        uint8_t* io;
        uint32_t* DMAxSAD;
        uint32_t* DMAxDAD;
        uint16_t* DMAxCNT_L;
        uint16_t* DMAxCNT_H;
        
        struct REG_DMAxCNT {
            const static uint16_t DA_MASK =                   0b0000000001100000;
            const static uint16_t DA_SHIFT =                  5;

            const static uint16_t SRC_ADJ_MASK =              0b0000000110000000;
            const static uint16_t SRC_ADJ_SHIFT =             7;

            const static uint16_t REPEAT_MASK =               0b0000001000000000;
            const static uint16_t REPEAT_SHIFT =              9;

            const static uint16_t CHUNK_SIZE_MASK =           0b0000010000000000;
            const static uint16_t CHUNK_SIZE_SHIFT =          10;

            const static uint16_t GAMEPAK_DRQ_MASK =          0b0000100000000000;
            const static uint16_t GAMEPAK_DRQ_SIZE_SHIFT =    11;

            const static uint16_t START_TIMING_MASK =         0b0011000000000000;
            const static uint16_t START_TIMING_SHIFT =        12;

            const static uint16_t IRQ_ENABLE_MASK =           0b0100000000000000;
            const static uint16_t IRQ_ENABLE_SHIFT =          14;

            const static uint16_t DMA_ENABLE_MASK =           0b1000000000000000;
            const static uint16_t DMA_ENABLE_SHIFT =          15;
        };

};

template <uint8_t N> 
DMA<N>::DMA(MemoryManager &mem) : mem(mem), dmaNum(N), io(mem.getIOregisters()),
                                  DMAxSAD(reinterpret_cast<uint32_t*>(io + (REG_ADDR::DMAxSAD[N] - MemoryManager::IO_REGISTERS_OFFSET_START))),
                                  DMAxDAD(reinterpret_cast<uint32_t*>(io + (REG_ADDR::DMAxDAD[N] - MemoryManager::IO_REGISTERS_OFFSET_START))),
                                  DMAxCNT_L(reinterpret_cast<uint16_t*>(io + (REG_ADDR::DMAxCNT_L[N] - MemoryManager::IO_REGISTERS_OFFSET_START))),
                                  DMAxCNT_H(reinterpret_cast<uint16_t*>(io + (REG_ADDR::DMAxCNT_H[N] - MemoryManager::IO_REGISTERS_OFFSET_START)))    
{
}

template <uint8_t N> 
void DMA<N>::runCycle() const {
    if(isDmaEnabled()){        
        auto transferSizeBytes = getTransferSizeBytes();
        auto numTransfers = getNumTransfers();

        uint32_t bytes = transferSizeBytes * numTransfers;
        std::cout << "DMA" << std::to_string(dmaNum) << " bytes " << bytes << std::endl;
        std::cout << "src: " << Utils::toHexString(*DMAxSAD) << " dst: " << Utils::toHexString(*DMAxDAD) << std::endl;

        TimingMode timingMode = getTimingMode();
        bool doTransferNow = false;
        switch(timingMode){
            case TimingMode::DMA_NOW:
                // In reality there is a 2 cpu cycle delay
                doTransferNow = true;
                break;
            case TimingMode::DMA_AT_VBLANK:
                throw std::runtime_error("Unimplemented DMA timing mode DMA_AT_VBLANK");
                break;
            case TimingMode::DMA_AT_HBLANK:
                throw std::runtime_error("Unimplemented DMA timing mode DMA_AT_HBLANK");
                break;
            case TimingMode::DMA_AT_REFRESH:
                throw std::runtime_error("Unimplemented DMA timing mode DMA_AT_REFRESH");
                break;
            default:
                throw std::runtime_error("Unknown DMA timing mode");
        }

        if(doTransferNow){
            /*
            if(dmaNum != 3 && *DMAxDAD >= 0x08000000){
                throw std::runtime_error("DMA dest address must not be in ROM address space");
            }
            */
            mem.blockTransfer(*DMAxDAD, *DMAxSAD, bytes);
            disableDma();
        }
    }
}

template <uint8_t N> 
bool DMA<N>::isDmaEnabled() const {
    return Utils::getRegBits(*DMAxCNT_H, REG_DMAxCNT::DMA_ENABLE_MASK, REG_DMAxCNT::DMA_ENABLE_SHIFT);
}

template <uint8_t N> 
DMA<N>::DstAdj DMA<N>::getDestAdjustment() const {
    uint8_t destAdj = Utils::getRegBits(*DMAxCNT_H, REG_DMAxCNT::DA_MASK, REG_DMAxCNT::DA_SHIFT);
    return static_cast<DstAdj>(destAdj);
}

template <uint8_t N> 
DMA<N>::SrcAdj DMA<N>::getSourceAdjustment() const {
    uint8_t srcAdj = Utils::getRegBits(*DMAxCNT_H, REG_DMAxCNT::SRC_ADJ_MASK, REG_DMAxCNT::SRC_ADJ_SHIFT);
    return static_cast<SrcAdj>(srcAdj);
}

template <uint8_t N>
DMA<N>::ChunkSize DMA<N>::getChunkSize() const {
    uint8_t chunkSize = Utils::getRegBits(*DMAxCNT_H, REG_DMAxCNT::CHUNK_SIZE_MASK, REG_DMAxCNT::CHUNK_SIZE_SHIFT);
    return static_cast<ChunkSize>(chunkSize);
}

template <uint8_t N>
DMA<N>::TimingMode DMA<N>::getTimingMode() const {
    uint8_t timingMode = Utils::getRegBits(*DMAxCNT_H, REG_DMAxCNT::START_TIMING_MASK, REG_DMAxCNT::START_TIMING_SHIFT);
    return static_cast<TimingMode>(timingMode);
}

template <uint8_t N>
bool DMA<N>::isIrqEnabled() const {
    return Utils::getRegBits(*DMAxCNT_H, REG_DMAxCNT::IRQ_ENABLE_MASK, REG_DMAxCNT::IRQ_ENABLE_SHIFT);
}

template <uint8_t N>
uint32_t DMA<N>::getNumTransfers() const {
    return *DMAxCNT_L;
}

template <uint8_t N>
uint8_t DMA<N>::getTransferSizeBytes() const {
    const static uint8_t transferSizeBytes[2] = {2, 4};
    return transferSizeBytes[static_cast<uint8_t>(getChunkSize())];
}

template <uint8_t N>
void DMA<N>::disableDma() const {
    *DMAxCNT_H &= ~REG_DMAxCNT::DMA_ENABLE_MASK;
}

#endif