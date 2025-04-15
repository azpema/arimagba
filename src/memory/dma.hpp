#ifndef _DMA_ 
#define _DMA_

#include <cstdint>
#include "../memory/memory_manager.hpp"
#include "../cpu/arm7tdmi.hpp"

// Make it static so it is shared among all DMA channels
static uint32_t lastValidVal = 0;

template <uint8_t N>
class DMA {
    public:
        DMA(ARM7TDMI &cpu, MemoryManager &mem);
        ~DMA() = default;

        void runCycle(const bool vblankNow, const bool hblankNow) const;
        static bool isSourceAddrValid(int dma, uint32_t sourceAddr);
        static bool isDestAddrValid(int dma, uint32_t sourceAddr);

    private:
        enum class DstAdj {DMA_DST_INC = 0b00, DMA_DST_DEC = 0b01, DMA_DST_FIXED = 0b10, DMA_DST_RELOAD = 0b11};
        enum class SrcAdj {DMA_SRC_INC = 0b00, DMA_SRC_DEC = 0b01, DMA_SRC_FIXED = 0b10, DMA_SRC_RELOAD = 0b11};
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

        ARM7TDMI &cpu;
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
DMA<N>::DMA(ARM7TDMI &cpu, MemoryManager &mem) : cpu(cpu), mem(mem), dmaNum(N), io(mem.getIOregisters()),
                                                 DMAxSAD(reinterpret_cast<uint32_t*>(io + (REG_ADDR::DMAxSAD[N] - MemoryManager::IO_REGISTERS_OFFSET_START))),
                                                 DMAxDAD(reinterpret_cast<uint32_t*>(io + (REG_ADDR::DMAxDAD[N] - MemoryManager::IO_REGISTERS_OFFSET_START))),
                                                 DMAxCNT_L(reinterpret_cast<uint16_t*>(io + (REG_ADDR::DMAxCNT_L[N] - MemoryManager::IO_REGISTERS_OFFSET_START))),
                                                 DMAxCNT_H(reinterpret_cast<uint16_t*>(io + (REG_ADDR::DMAxCNT_H[N] - MemoryManager::IO_REGISTERS_OFFSET_START)))    
{
}

template <uint8_t N> 
void DMA<N>::runCycle(const bool vblankNow, const bool hblankNow) const {
    static bool prevDmaEnabled = false;

    if(isDmaEnabled()){
        static uint32_t readAddr;
        static uint32_t writeAddr;
        static uint32_t numTransfers;

        SrcAdj sourceAdjustment = getSourceAdjustment();
        DstAdj destAdjustment = getDestAdjustment();

        auto transferSizeBytes = getTransferSizeBytes();
        if(!prevDmaEnabled){
            readAddr = *DMAxSAD;
            writeAddr = *DMAxDAD;
            numTransfers = getNumTransfers();
        }

        if(isRepeatEnabled()){
            numTransfers = getNumTransfers();
            if(destAdjustment == DstAdj::DMA_DST_RELOAD){
                writeAddr = *DMAxDAD;
            }
        }

        uint32_t bytes = transferSizeBytes * numTransfers;
        std::cout << "DMA" << std::to_string(dmaNum) << " bytes " << bytes << std::endl;
        std::cout << "src: " << Utils::toHexString(readAddr) << " dst: " << Utils::toHexString(writeAddr) << " " << Utils::toHexString(*DMAxCNT_H) << ":" << Utils::toHexString(*DMAxCNT_L) << "\n"; 

        TimingMode timingMode = getTimingMode();
        bool doTransferNow = false;
        switch(timingMode){
            case TimingMode::DMA_NOW:
                // In reality there is a 2 cpu cycle delay
                doTransferNow = true;
                break;
            case TimingMode::DMA_AT_VBLANK:
                //throw std::runtime_error("Unimplemented DMA timing mode DMA_AT_VBLANK");
                if(vblankNow){
                    doTransferNow = true;
                }
                break;
            case TimingMode::DMA_AT_HBLANK:
                //throw std::runtime_error("Unimplemented DMA timing mode DMA_AT_HBLANK");
                if(hblankNow){
                    doTransferNow = true;
                }
                break;
            case TimingMode::DMA_AT_REFRESH:
                //throw std::runtime_error("Unimplemented DMA timing mode DMA_AT_REFRESH");
                doTransferNow = true;
                break;
            default:
                throw std::runtime_error("Unknown DMA timing mode");
        }

        if(doTransferNow){
            
            if(dmaNum != 3 && *DMAxDAD >= 0x08000000){
                throw std::runtime_error("DMA dest address must not be in ROM address space");
            }
            

            
            uint32_t iterReadAddr = readAddr;
            uint32_t iterWriteAddr = writeAddr;
            for(size_t i=0; i<numTransfers; i++)
            {
                // Force alignment
                uint32_t alignedReadAddress;
                uint32_t alignedWriteAddress;
                if(transferSizeBytes == 2){
                    alignedReadAddress = iterReadAddr & 0xFFFFFFFE;
                    alignedWriteAddress = iterWriteAddr & 0xFFFFFFFE;
                }else{
                    alignedReadAddress = iterReadAddr & 0xFFFFFFFC;
                    alignedWriteAddress = iterWriteAddr & 0xFFFFFFFC;
                }
                
                // Set static to return last valid value in case of openbus
                if(alignedReadAddress >= 0x01FFFFFF){
                    lastValidVal = mem.read(alignedReadAddress, transferSizeBytes);
                }else{
                    // Open bus
                }

                // Only dma3 can access gamepak
                /*if(N != 3 && alignedReadAddress >= 0x08000000 && alignedReadAddress <= 0x08FFFFFF){
                    val = 0;
                }*/

                //if(alignedWriteAddress != DMAxSAD[N] && alignedWriteAddress != DMAxDAD[N] &&
                //   alignedWriteAddress != DMAxCNT_L[N] && alignedWriteAddress != DMAxCNT_H[N]){
                    mem.store(alignedWriteAddress, lastValidVal, transferSizeBytes);
                //}
                

                switch(destAdjustment){
                    case DstAdj::DMA_DST_INC:
                        writeAddr += transferSizeBytes;
                        iterWriteAddr += transferSizeBytes;
                        break;
                    case DstAdj::DMA_DST_DEC:
                        writeAddr -= transferSizeBytes;
                        iterWriteAddr -= transferSizeBytes;
                        break;
                    case DstAdj::DMA_DST_FIXED:
                        // Useful for Direct Sound FIFO
                        break;
                    case DstAdj::DMA_DST_RELOAD:
                        writeAddr += transferSizeBytes;
                        iterWriteAddr += transferSizeBytes;
                        break;
                    default:
                        throw std::runtime_error("Unknown DMA destination adjustment value");
                        break;
                }

                switch(sourceAdjustment){
                    case SrcAdj::DMA_SRC_INC:
                        readAddr += transferSizeBytes;
                        iterReadAddr += transferSizeBytes;
                        break;
                    case SrcAdj::DMA_SRC_DEC:
                        readAddr -= transferSizeBytes;
                        iterReadAddr -= transferSizeBytes;
                        break;
                    case SrcAdj::DMA_SRC_FIXED:
                        break;
                    case SrcAdj::DMA_SRC_RELOAD:
                        readAddr += transferSizeBytes;
                        iterReadAddr += transferSizeBytes;
                        //throw std::runtime_error("Forbidden DMA source adjustment value DMA_SRC_RELOAD");
                        break;
                    default:
                        throw std::runtime_error("Unknown DMA SOURCE adjustment value");
                        break;
                }
            }

            /*if(destAdjustment == DstAdj::DMA_DST_RELOAD){
                *DMAxDAD = *DMAxDAD - transferSizeBytes * numTransfers;
            }*/
            //mem.blockTransfer(*DMAxDAD, *DMAxSAD, bytes);
            if(isRepeatEnabled() && (timingMode == TimingMode::DMA_AT_VBLANK || timingMode == TimingMode::DMA_AT_VBLANK)){

            }else{
                disableDma();
            }
        }

        if(isIrqEnabled()){
            const static ExceptionHandler::Interrupt interrupt[4] = { ExceptionHandler::Interrupt::DMA0,
                                                                      ExceptionHandler::Interrupt::DMA1,
                                                                      ExceptionHandler::Interrupt::DMA2,
                                                                      ExceptionHandler::Interrupt::DMA3 };

            cpu.getExceptionHandler().raiseException(ExceptionHandler::Exception::IRQ, interrupt[N]);
        }
        prevDmaEnabled = true;
    }else{
        prevDmaEnabled = false;
    }
}

template <uint8_t N> 
bool DMA<N>::isDmaEnabled() const {
    return Utils::getRegBits(*DMAxCNT_H, REG_DMAxCNT::DMA_ENABLE_MASK, REG_DMAxCNT::DMA_ENABLE_SHIFT);
}

template <uint8_t N> 
bool DMA<N>::isRepeatEnabled() const {
    return Utils::getRegBits(*DMAxCNT_H, REG_DMAxCNT::REPEAT_MASK, REG_DMAxCNT::REPEAT_SHIFT);
}

template <uint8_t N> 
typename DMA<N>::DstAdj DMA<N>::getDestAdjustment() const {
    uint8_t destAdj = Utils::getRegBits(*DMAxCNT_H, REG_DMAxCNT::DA_MASK, REG_DMAxCNT::DA_SHIFT);
    return static_cast<DstAdj>(destAdj);
}

template <uint8_t N> 
typename DMA<N>::SrcAdj DMA<N>::getSourceAdjustment() const {
    uint8_t srcAdj = Utils::getRegBits(*DMAxCNT_H, REG_DMAxCNT::SRC_ADJ_MASK, REG_DMAxCNT::SRC_ADJ_SHIFT);
    return static_cast<SrcAdj>(srcAdj);
}

template <uint8_t N>
typename DMA<N>::ChunkSize DMA<N>::getChunkSize() const {
    uint8_t chunkSize = Utils::getRegBits(*DMAxCNT_H, REG_DMAxCNT::CHUNK_SIZE_MASK, REG_DMAxCNT::CHUNK_SIZE_SHIFT);
    return static_cast<ChunkSize>(chunkSize);
}

template <uint8_t N>
typename DMA<N>::TimingMode DMA<N>::getTimingMode() const {
    uint8_t timingMode = Utils::getRegBits(*DMAxCNT_H, REG_DMAxCNT::START_TIMING_MASK, REG_DMAxCNT::START_TIMING_SHIFT);
    return static_cast<TimingMode>(timingMode);
}

template <uint8_t N>
bool DMA<N>::isIrqEnabled() const {
    return Utils::getRegBits(*DMAxCNT_H, REG_DMAxCNT::IRQ_ENABLE_MASK, REG_DMAxCNT::IRQ_ENABLE_SHIFT);
}

/*
DMAxCNT_L

Specifies the number of data units to be transferred, each unit is 16bit or 32bit depending on the transfer type,
 a value of zero is treated as max length (ie. 4000h, or 10000h for DMA3).
*/
template <uint8_t N>
uint32_t DMA<N>::getNumTransfers() const {
    if(*DMAxCNT_L == 0){
        if(N == 3){
            return 0x10000;
        }else{
            return 0x4000;
        }
    }
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

template <uint8_t N>
bool DMA<N>::isSourceAddrValid(int dma, uint32_t sourceAddr){
	if(dma == 0 && sourceAddr >= MemoryManager::GAMEPAK_WAIT_0_OFFSET_START && sourceAddr < MemoryManager::GAMEPAK_SRAM_OFFSET_START){
		return false;
	}
	return sourceAddr >= MemoryManager::EWRAM_OFFSET_START;
}

template <uint8_t N>
bool DMA<N>::isDestAddrValid(int dma, uint32_t address) {
	return dma == 3 || address < MemoryManager::GAMEPAK_WAIT_0_OFFSET_START;
}

#endif