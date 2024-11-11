#include "ppu.hpp"
#include "../memory/memory_manager.hpp"
#include "screen_entry.hpp"

PPU::PPU(const std::string &title, ARM7TDMI &cpu, MemoryManager *memManager): cpu(cpu), mem(memManager) {

    rend = std::make_unique<Renderer>(*this, title);
    io = mem->getIOregisters();
    DISPCNT = reinterpret_cast<uint16_t*>(io + (REG_ADDR::DISPCNT - MemoryManager::IO_REGISTERS_OFFSET_START));
    GREEN_SWAP = reinterpret_cast<uint16_t*>(io + (REG_ADDR::GREEN_SWAP - MemoryManager::IO_REGISTERS_OFFSET_START));
    DISPSTAT = reinterpret_cast<uint16_t*>(io + (REG_ADDR::DISPSTAT - MemoryManager::IO_REGISTERS_OFFSET_START));
    VCOUNT = reinterpret_cast<uint16_t*>(io + (REG_ADDR::VCOUNT - MemoryManager::IO_REGISTERS_OFFSET_START));
    BG0CNT = reinterpret_cast<uint16_t*>(io + (REG_ADDR::BG0CNT - MemoryManager::IO_REGISTERS_OFFSET_START));
    BG0HOFS = reinterpret_cast<uint16_t*>(io + (REG_ADDR::BG0HOFS - MemoryManager::IO_REGISTERS_OFFSET_START));
    BG0VOFS = reinterpret_cast<uint16_t*>(io + (REG_ADDR::BG0VOFS - MemoryManager::IO_REGISTERS_OFFSET_START));

    setDCNT_MODE(3);
    *VCOUNT = VCOUNT_INITIAL_VALUE;
}

PPU::~PPU(){
  
}

uint8_t *PPU::getPaletteRAM() const{
    return mem->getPaletteRAM();
}

uint8_t *PPU::getVRAM() const{
    return mem->getRawVRAM();
}

void PPU::renderScanline(){
    if(vCountIrqEnabled() && getVcountTrigger() == *VCOUNT){
        setVcountFlag(true);
        cpu.getExceptionHandler().raiseException(ExceptionHandler::Exception::IRQ, ExceptionHandler::Interrupt::VCOUNT);
    }else{
        setVcountFlag(false);
    }
    // Check BG Mode in DISPCNT
    // Panda will stop working!!!
    if(*VCOUNT <= 160){
        uint8_t dcntMode = getDCNT_MODE();
        switch(dcntMode){
            case 0:
                rend->renderScanlineMode0();
                break;
            case 3:
                rend->renderScanlineMode3();
                break;
            case 4:
                rend->renderScanlineMode4();
                break;
            case 7:
                rend->renderScanlineMode3();
                break;
            default:
                //renderScanlineMode3();
                throw std::runtime_error("ERROR: Unsupported PPU Mode: " + std::to_string(dcntMode));
        }
    }

    // Update VCOUNT
    *VCOUNT += 1; 
    if(*VCOUNT == VCOUNT_START_VBLANK){
        setVBlankFlag(true);
        if(vBlankIrqEnabled()){
            cpu.getExceptionHandler().raiseException(ExceptionHandler::Exception::IRQ, ExceptionHandler::Interrupt::VBLANK);
        }
    }else if(*VCOUNT == VCOUNT_END_VBLANK){
        setVBlankFlag(false);
    }
    else if(*VCOUNT == 228){
        *VCOUNT = 0;
    }
        
    //TODO REMOVE
    setHBlankFlag(true);
    if(hBlankIrqEnabled()){
        cpu.getExceptionHandler().raiseException(ExceptionHandler::Exception::IRQ, ExceptionHandler::Interrupt::HBLANK);
    }
}

uint32_t PPU::getVcount() const{
    return *VCOUNT;
}

uint32_t PPU::getBgOffsetH(uint8_t bg) const{
    switch(bg){
        case 0:
            return *BG0HOFS;
            break;
        default:
            throw new std::runtime_error("TODO getBgOffsetH");
    }
}

uint32_t PPU::getBgOffsetV(uint8_t bg) const{
    switch(bg){
        case 0:
            return *BG0VOFS;
            break;
        default:
            throw new std::runtime_error("TODO getBgOffsetV");
    }
}

void PPU::setVBlankFlag(bool val){
    uint32_t bitVal = val << REG_DISPSTAT::DSTAT_IN_VBL_SHIFT;
    uint32_t regVal = *DISPSTAT;
    *DISPSTAT = Utils::setRegBits(regVal, REG_DISPSTAT::DSTAT_IN_VBL_MASK, bitVal);
}

void PPU::setHBlankFlag(bool val){
    uint32_t bitVal = val << REG_DISPSTAT::DSTAT_IN_HBL_SHIFT;
    uint32_t regVal = *DISPSTAT;
    *DISPSTAT = Utils::setRegBits(regVal, REG_DISPSTAT::DSTAT_IN_HBL_MASK, bitVal);
}

void PPU::setVcountFlag(bool val){
    uint32_t bitVal = val << REG_DISPSTAT::DSTAT_IN_VCT_SHIFT;
    uint32_t regVal = *DISPSTAT;
    *DISPSTAT = Utils::setRegBits(regVal, REG_DISPSTAT::DSTAT_IN_VCT_MASK, bitVal);
}

void PPU::setDCNT_MODE(uint8_t mode){
    uint32_t bitVal = mode << REG_DISPCNT::DCNT_MODE_SHIFT;
    uint32_t regVal = *DISPCNT;
    *DISPCNT = Utils::setRegBits(regVal, REG_DISPCNT::DCNT_MODE_MASK, bitVal);
}

uint8_t PPU::getVcountTrigger() const{
    return Utils::getRegBits(*DISPSTAT, REG_DISPSTAT::DSTAT_VCT_NUM_MASK, REG_DISPSTAT::DSTAT_VCT_NUM_SHIFT);
}

uint8_t PPU::getDCNT_MODE() const{
    return Utils::getRegBits(*DISPCNT, REG_DISPCNT::DCNT_MODE_MASK, REG_DISPCNT::DCNT_MODE_SHIFT);
}

uint8_t PPU::getDCNT_PAGE() const{
    return Utils::getRegBits(*DISPCNT, REG_DISPCNT::DCNT_PAGE_MASK, REG_DISPCNT::DCNT_PAGE_SHIFT);
}

uint32_t PPU::getPageFlipOffset() const{
    return getDCNT_PAGE() ? PAGE_FLIP_SECOND_OFFSET : 0;
}

bool PPU::vBlankIrqEnabled() const{
    return Utils::getRegBits(*DISPSTAT, REG_DISPSTAT::DSTAT_VBL_IRQ_MASK, REG_DISPSTAT::DSTAT_VBL_IRQ_SHIFT);
}

bool PPU::hBlankIrqEnabled() const{
    return Utils::getRegBits(*DISPSTAT, REG_DISPSTAT::DSTAT_HBL_IRQ_MASK, REG_DISPSTAT::DSTAT_HBL_IRQ_SHIFT);
}

bool PPU::vCountIrqEnabled() const{
    return Utils::getRegBits(*DISPSTAT, REG_DISPSTAT::DSTAT_VCT_IRQ_MASK, REG_DISPSTAT::DSTAT_VCT_IRQ_SHIFT);
}

uint8_t PPU::getBgCharacterBaseBlock(uint8_t bgNum) const{
    uint16_t bgCntVal;
    switch(bgNum){
        case 0:
            bgCntVal = *BG0CNT;
            break;
        default:
            throw new std::runtime_error("TODO: getBgCharacterBaseBlock");
    }
    return Utils::getRegBits(bgCntVal, REG_BGxCNT::BG_CBB_NUM_MASK, REG_BGxCNT::BG_CBB_NUM_SHIFT);
}

uint8_t PPU::getBgColorMode(uint8_t bgNum) const{
    uint16_t bgCntVal;
    switch(bgNum){
        case 0:
            bgCntVal = *BG0CNT;
            break;
        default:
            throw new std::runtime_error("TODO: getBgColorMode");
    }
    return Utils::getRegBits(bgCntVal, REG_BGxCNT::BG_8BPP_MASK, REG_BGxCNT::BG_8BPP_SHIFT);
}

uint8_t PPU::getBgScreenBaseBlock(uint8_t bgNum) const{
    uint16_t bgCntVal;
    switch(bgNum){
        case 0:
            bgCntVal = *BG0CNT;
            break;
        default:
            throw new std::runtime_error("TODO: getBgCharacterBaseBlock");
    }
    return Utils::getRegBits(bgCntVal, REG_BGxCNT::BG_SBB_NUM_MASK, REG_BGxCNT::BG_SBB_NUM_SHIFT);
}

uint32_t PPU::getTileSetVramOffset(uint8_t bgNum) const{
    return getBgCharacterBaseBlock(bgNum) * 0x4000;
}

uint32_t PPU::getTileMapVramOffset(uint8_t bgNum) const{
    return getBgScreenBaseBlock(bgNum) * 0x800;
}

uint8_t PPU::getBgSize(uint8_t bgNum) const{
    uint16_t bgCntVal;
    switch(bgNum){
        case 0:
            bgCntVal = *BG0CNT;
            break;
        default:
            throw new std::runtime_error("TODO: getBgCharacterBaseBlock");
    }
    return Utils::getRegBits(bgCntVal, REG_BGxCNT::BG_SIZE_NUM_MASK, REG_BGxCNT::BG_SIZE_NUM_SHIFT);
}