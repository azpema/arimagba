#include "ppu.hpp"
#include "../memory/memory_manager.hpp"
#include "../memory/vram.hpp"
#include "../memory/palette_ram.hpp"
#include "../memory/io_registers.hpp"
#include "renderer.hpp"
#include "screen_entry.hpp"
#include "imgui.h"
#include <cstring>

PPU::PPU(ARM7TDMI& cpu, MemoryManager* memManager) :
    cpu(cpu), mem(memManager), io(mem->getIOregisters()),
    DISPCNT(reinterpret_cast<uint16_t*>(io + (REG_ADDR::DISPCNT - MemoryManager::IO_REGISTERS_OFFSET_START))),
    GREEN_SWAP(reinterpret_cast<uint16_t*>(io + (REG_ADDR::GREEN_SWAP - MemoryManager::IO_REGISTERS_OFFSET_START))),
    DISPSTAT(reinterpret_cast<uint16_t*>(io + (REG_ADDR::DISPSTAT - MemoryManager::IO_REGISTERS_OFFSET_START))),
    VCOUNT(reinterpret_cast<uint16_t*>(io + (REG_ADDR::VCOUNT - MemoryManager::IO_REGISTERS_OFFSET_START)))

{
    for (size_t n = 0; n < PPU::BG_NUM; n++) {
        BGxCNT[n] = reinterpret_cast<uint16_t*>(io + (REG_ADDR::BGxCNT[n] - MemoryManager::IO_REGISTERS_OFFSET_START));
        BGxHOFS[n] =
            reinterpret_cast<uint16_t*>(io + (REG_ADDR::BGxHOFS[n] - MemoryManager::IO_REGISTERS_OFFSET_START));
        BGxVOFS[n] =
            reinterpret_cast<uint16_t*>(io + (REG_ADDR::BGxVOFS[n] - MemoryManager::IO_REGISTERS_OFFSET_START));
    }

    setDCNT_MODE(3);
    *VCOUNT = VCOUNT_INITIAL_VALUE;
}

PPU::~PPU() {}

uint8_t* PPU::getBgPaletteRAM() const { return mem->getPaletteRAM() + PaletteRAM::BG_OFFSET; }

uint8_t* PPU::getObjPaletteRAM() const { return mem->getPaletteRAM() + PaletteRAM::OBJ_OFFSET; }

uint8_t* PPU::getOAM() const { return mem->getOAM(); }

uint8_t* PPU::getVRAM() const { return mem->getRawVRAM(); }

uint8_t* PPU::getOVRAM() const { return mem->getRawVRAM() + VRAM::OBJECT_VRAM_OFFSET; }

void PPU::renderScanline(bool& vblankNow, bool& hblankNow) {
    if (vCountIrqEnabled() && getVcountTrigger() == *VCOUNT) {
        setVcountFlag(true);
        cpu.getExceptionHandler().raiseException(ExceptionHandler::Exception::IRQ, ExceptionHandler::Interrupt::VCOUNT);
    } else {
        setVcountFlag(false);
    }

    // Update VCOUNT
    *VCOUNT += 1;
    if (*VCOUNT == VCOUNT_START_VBLANK) {
        setVBlankFlag(true);
        vblankNow = true;
        if (vBlankIrqEnabled()) {
            cpu.getExceptionHandler().raiseException(ExceptionHandler::Exception::IRQ,
                                                     ExceptionHandler::Interrupt::VBLANK);
        }
    } else if (*VCOUNT == VCOUNT_END_VBLANK) {
        setVBlankFlag(false);
        vblankNow = false;
    } else if (*VCOUNT == 228) {
        *VCOUNT = 0;
    }

    // Check BG Mode in DISPCNT
    // Panda will stop working!!!
    if (*VCOUNT < PPU::SCREEN_HEIGHT) {
        uint8_t dcntMode = getDCNT_MODE();
        switch (dcntMode) {
        case 0:
            // BG0 regular - BG1 regular - BG2 regular - BG3 regular
            renderScanlineMode0();
            break;
        case 1:
            // BG0 regular - BG1 regular - BG2 affine - BG3 disabled
            renderScanlineMode0();
            //throw std::runtime_error("ERROR: Unsupported PPU Mode: 1");
            break;
        case 2:
            // BG0 disabled - BG1 disabled - BG2 affine - BG3 affine
            throw std::runtime_error("ERROR: Unsupported PPU Mode: 2");
            break;
        case 3:
            renderScanlineMode3();
            break;
        case 4:
            renderScanlineMode4();
            break;
        case 7:
            throw std::runtime_error("ERROR: Unsupported PPU Mode: 7");
            break;
        default:
            // renderScanlineMode3();
            throw std::runtime_error("ERROR: Unsupported PPU Mode: " + std::to_string(dcntMode));
        }
    }

    if (*VCOUNT % 2 == 0) {
        setHBlankFlag(true);
        hblankNow = true;
        if (hBlankIrqEnabled()) {
            cpu.getExceptionHandler().raiseException(ExceptionHandler::Exception::IRQ,
                                                     ExceptionHandler::Interrupt::HBLANK);
        }
    } else {
        setHBlankFlag(false);
        hblankNow = false;
    }
}

uint32_t PPU::getVcount() const { return *VCOUNT; }

uint32_t PPU::getBgOffsetH(const uint8_t bg) const { return *(BGxHOFS[bg]); }

uint32_t PPU::getBgOffsetV(const uint8_t bg) const { return *(BGxVOFS[bg]); }

void PPU::setVBlankFlag(bool val) {
    uint32_t bitVal = val << REG_DISPSTAT::DSTAT_IN_VBL_SHIFT;
    uint32_t regVal = *DISPSTAT;
    *DISPSTAT = Utils::setRegBits(regVal, REG_DISPSTAT::DSTAT_IN_VBL_MASK, bitVal);
}

void PPU::setHBlankFlag(bool val) {
    uint32_t bitVal = val << REG_DISPSTAT::DSTAT_IN_HBL_SHIFT;
    uint32_t regVal = *DISPSTAT;
    *DISPSTAT = Utils::setRegBits(regVal, REG_DISPSTAT::DSTAT_IN_HBL_MASK, bitVal);
}

void PPU::setVcountFlag(bool val) {
    uint32_t bitVal = val << REG_DISPSTAT::DSTAT_IN_VCT_SHIFT;
    uint32_t regVal = *DISPSTAT;
    *DISPSTAT = Utils::setRegBits(regVal, REG_DISPSTAT::DSTAT_IN_VCT_MASK, bitVal);
}

void PPU::setDCNT_MODE(uint8_t mode) {
    uint32_t bitVal = mode << REG_DISPCNT::DCNT_MODE_SHIFT;
    uint32_t regVal = *DISPCNT;
    *DISPCNT = Utils::setRegBits(regVal, REG_DISPCNT::DCNT_MODE_MASK, bitVal);
}

uint8_t PPU::getVcountTrigger() const {
    return Utils::getRegBits(*DISPSTAT, REG_DISPSTAT::DSTAT_VCT_NUM_MASK, REG_DISPSTAT::DSTAT_VCT_NUM_SHIFT);
}

uint8_t PPU::getDCNT_MODE() const {
    return Utils::getRegBits(*DISPCNT, REG_DISPCNT::DCNT_MODE_MASK, REG_DISPCNT::DCNT_MODE_SHIFT);
}

uint8_t PPU::getDCNT_PAGE() const {
    return Utils::getRegBits(*DISPCNT, REG_DISPCNT::DCNT_PAGE_MASK, REG_DISPCNT::DCNT_PAGE_SHIFT);
}

bool PPU::getBgEnabled(const uint8_t bgNum) const {
    const static uint16_t mask[4] = {
        REG_DISPCNT::DCNT_BG0_MASK, REG_DISPCNT::DCNT_BG1_MASK, REG_DISPCNT::DCNT_BG2_MASK, REG_DISPCNT::DCNT_BG3_MASK};
    const static uint16_t shift[4] = {REG_DISPCNT::DCNT_BG0_SHIFT,
                                      REG_DISPCNT::DCNT_BG1_SHIFT,
                                      REG_DISPCNT::DCNT_BG2_SHIFT,
                                      REG_DISPCNT::DCNT_BG3_SHIFT};

    return Utils::getRegBits(*DISPCNT, mask[bgNum], shift[bgNum]) == 1;
}

bool PPU::getObjMapping1D() const {
    return Utils::getRegBits(*DISPCNT, REG_DISPCNT::DCNT_OBJ_1D_MASK, REG_DISPCNT::DCNT_OBJ_1D_SHIFT);
}

bool PPU::getObjEnabled() const {
    return Utils::getRegBits(*DISPCNT, REG_DISPCNT::DCNT_OBJ_MASK, REG_DISPCNT::DCNT_OBJ_SHIFT);
}

std::optional<ObjAttributes> PPU::getObj(int objNum) const {
    uint64_t rawObjAttributes = *reinterpret_cast<uint64_t*>(getOAM() + objNum * OBJ_ATTRIBUTES_SIZE);
    ObjAttributes obj(rawObjAttributes);

    switch (obj.getObjMode()) {
    case ObjMode::DISABLED:
        return std::nullopt;
    case ObjMode::NORMAL:
        break;
    case ObjMode::AFFINE:
        //throw std::runtime_error("Unimplemented OBJ Mode AFFINE");
        break;
    case ObjMode::AFFINE_DOUBLE:
        //throw std::runtime_error("Unimplemented OBJ Mode AFFINE_DOUBLE");
        break;
    default:
        //throw std::runtime_error("Unrecognized OBJ Mode");
        break;
    }

    return obj;
}

std::vector<ObjAttributes> PPU::getObjList() const {
    std::vector<ObjAttributes> vec;
    vec.reserve(PPU::MAX_SPRITE_NUM);

    for (size_t i = 0; i < PPU::MAX_SPRITE_NUM; i++) {
        auto obj = getObj(i);
        if (!obj.has_value()) {
            break;
        }

        vec.push_back(*obj);
    }

    std::sort(vec.begin(), vec.end(), [](const ObjAttributes& a, const ObjAttributes& b) {
        if (a.getPriority() != b.getPriority()) {
            return a.getPriority() < b.getPriority();
        } else {
            return a.getRawVal() > b.getRawVal();
        }
    });

    return vec;
}

uint32_t PPU::getPageFlipOffset() const { return getDCNT_PAGE() ? PAGE_FLIP_SECOND_OFFSET : 0; }

bool PPU::vBlankIrqEnabled() const {
    return Utils::getRegBits(*DISPSTAT, REG_DISPSTAT::DSTAT_VBL_IRQ_MASK, REG_DISPSTAT::DSTAT_VBL_IRQ_SHIFT);
}

bool PPU::hBlankIrqEnabled() const {
    return Utils::getRegBits(*DISPSTAT, REG_DISPSTAT::DSTAT_HBL_IRQ_MASK, REG_DISPSTAT::DSTAT_HBL_IRQ_SHIFT);
}

bool PPU::vCountIrqEnabled() const {
    return Utils::getRegBits(*DISPSTAT, REG_DISPSTAT::DSTAT_VCT_IRQ_MASK, REG_DISPSTAT::DSTAT_VCT_IRQ_SHIFT);
}

uint8_t PPU::getBgCharacterBaseBlock(const uint8_t bgNum) const {
    return Utils::getRegBits(*(BGxCNT[bgNum]), REG_BGxCNT::BG_CBB_NUM_MASK, REG_BGxCNT::BG_CBB_NUM_SHIFT);
}

uint8_t PPU::getBgColorMode(const uint8_t bgNum) const {
    return Utils::getRegBits(*(BGxCNT[bgNum]), REG_BGxCNT::BG_8BPP_MASK, REG_BGxCNT::BG_8BPP_SHIFT);
}

uint8_t PPU::getBgScreenBaseBlock(const uint8_t bgNum) const {
    return Utils::getRegBits(*(BGxCNT[bgNum]), REG_BGxCNT::BG_SBB_NUM_MASK, REG_BGxCNT::BG_SBB_NUM_SHIFT);
}

uint32_t PPU::getTileSetVramOffset(const uint8_t bgNum) const { return getBgCharacterBaseBlock(bgNum) * 0x4000; }

uint32_t PPU::getTileMapVramOffset(const uint8_t bgNum) const { return getBgScreenBaseBlock(bgNum) * 0x800; }

uint8_t PPU::getBgSize(const uint8_t bgNum) const {
    return Utils::getRegBits(*(BGxCNT[bgNum]), REG_BGxCNT::BG_SIZE_NUM_MASK, REG_BGxCNT::BG_SIZE_NUM_SHIFT);
}

uint8_t PPU::getBgPriority(const uint8_t bgNum) const {
    return Utils::getRegBits(*(BGxCNT[bgNum]), REG_BGxCNT::BG_PRIO_NUM_MASK, REG_BGxCNT::BG_PRIO_NUM_SHIFT);
}

// Will only return enabled backgrounds!
std::vector<uint8_t> PPU::getBgsWithPriorityX(const uint8_t prio) const {
    std::vector<uint8_t> res = {};
    res.reserve(PPU::BG_NUM);
    for (size_t bgNum = 0; bgNum < BG_NUM; bgNum++) {
        if (getBgPriority(bgNum) == prio && getBgEnabled(bgNum)) {
            res.push_back(bgNum);
        }
    }

    return res;
}

// Descending order (from highest to lowest priority).
std::vector<uint8_t> PPU::getBgBlendOrder() const {
    std::vector<uint8_t> res;
    res.reserve(PPU::BG_NUM);
    int index = 0;
    for (size_t bgNum = 0; bgNum < BG_NUM; bgNum++) {
        auto bgs = getBgsWithPriorityX(bgNum);

        res.insert(res.end(), bgs.begin(), bgs.end());
        index += bgs.size();
    }

    return res;
}

void PPU::getBackgroundScanline(const uint8_t bg, int32_t* toPaint) {
    const uint8_t BG_NUM = bg;

    uint8_t tileSize = 0x20;
    bool colorMode256 = getBgColorMode(BG_NUM) == 1;
    if (colorMode256) {
        tileSize = 0x40;
    }

    uint8_t screenPixelX = 0;
    uint8_t screenPixelY = getVcount();

    uint8_t toPaintIndex = 0;
    uint32_t tileSetStartOffset = getTileSetVramOffset(BG_NUM);
    uint32_t tileMapStartOffset = getTileMapVramOffset(BG_NUM);
    uint32_t pixelScrollX = getBgOffsetH(BG_NUM);
    uint32_t pixelScrollY = getBgOffsetV(BG_NUM);

    while (screenPixelX <= SCREEN_WIDTH) {

        uint32_t bgPixelX = screenPixelX + pixelScrollX;
        uint32_t bgPixelY = screenPixelY + pixelScrollY;

        uint32_t bgRelativeTileX = getBgRelativeTileX(BG_NUM, (bgPixelX) / TILE_WIDTH_HEIGHT);
        uint32_t bgRelativeTileY = getBgRelativeTileY(BG_NUM, (bgPixelY) / TILE_WIDTH_HEIGHT);
        // std::cout << "bgRelativeTileX" << Utils::toHexString(bgRelativeTileX) << '\n';
        // std::cout << "bgRelativeTileY" << Utils::toHexString(bgRelativeTileY) << '\n';

        uint32_t seIndex = ((bgRelativeTileY % 32) * 32 + (bgRelativeTileX % 32)) * 2;
        uint32_t sbcOffset = getSbcOffset(BG_NUM, bgRelativeTileX * 8, bgRelativeTileY * 8);
        uint32_t seOffset = tileMapStartOffset + sbcOffset + seIndex;

        uint16_t tileMapRaw = *reinterpret_cast<uint16_t*>(getVRAM() + seOffset);

        ScreenEntry screenEntry(tileMapRaw);

        // Search the given tile with the tile Index:
        //  0x20: each tile takes 8*8*4bpp = 32 = 0x20 bytes
        // std::cout << "seOffset" << Utils::toHexString(seOffset) << '\n;
        // std::cout << "tileSetRaw" << Utils::toHexString(tileSetStartOffset + screenEntry.getTileIndex() * 0x20) <<
        // '\n';
        uint16_t tileIndex = screenEntry.getTileIndex();
        uint32_t debug1 = tileSetStartOffset + tileIndex * tileSize;

        uint8_t* tileSetRaw = reinterpret_cast<uint8_t*>(getVRAM() + debug1);

        // check BG_8BPP to determine palette
        uint32_t paletteOffset = colorMode256 ? 0 : tileSize * screenEntry.getPaletteBank();
        uint16_t* paletteRAM = reinterpret_cast<uint16_t*>(getBgPaletteRAM() + paletteOffset);

        uint8_t pixelsLeftInTile = TILE_WIDTH_HEIGHT;
        uint8_t tileStartOffset = 0;
        if (screenPixelX == 0) {
            pixelsLeftInTile = TILE_WIDTH_HEIGHT - (pixelScrollX % 8);
            tileStartOffset = pixelScrollX % 8;
        } else if (SCREEN_WIDTH - toPaintIndex < 8) {
            pixelsLeftInTile = SCREEN_WIDTH - toPaintIndex;
        }

        for (uint8_t i = tileStartOffset; i < tileStartOffset + pixelsLeftInTile; i++) {
            // Take the pixel from the given palette!!!!
            uint8_t pixelIndex = i;
            if (screenEntry.getHorizontalFlip()) {
                pixelIndex = TILE_WIDTH_HEIGHT - 1 - i;
            }

            uint32_t tileOffset = ((getVcount() + pixelScrollY) % TILE_WIDTH_HEIGHT);
            if (colorMode256) {
                tileOffset = tileOffset * 8 + pixelIndex;
            } else {
                tileOffset = tileOffset * 4 + pixelIndex / 2;
            }
            uint8_t tile = *(tileSetRaw + tileOffset);

            uint8_t paletteIndex;
            if (colorMode256) {
                paletteIndex = tile;
            } else {
                if (pixelIndex % 2 == 0) {
                    paletteIndex = tile & 0b1111;
                } else {
                    paletteIndex = (tile & 0b11110000) >> 4;
                }
            }

            if (paletteIndex != 0) {
                uint16_t pixel = paletteRAM[paletteIndex];

                toPaint[toPaintIndex++] = pixel;
            } else {
                toPaint[toPaintIndex++] = TRANSPARENT_PIXEL;
            }
        }
        screenPixelX += 8;
    }
}

uint32_t PPU::getBgRelativeTileX(uint8_t bg, uint32_t tileX) const {
    uint8_t bgSize = getBgSize(bg);
    if (bgSize == 0 || bgSize == 2) {
        return tileX % 32;
    } else {
        return tileX % 64;
    }
}

uint32_t PPU::getBgRelativeTileY(uint8_t bg, uint32_t tileY) const {
    uint8_t bgSize = getBgSize(bg);
    if (bgSize == 0 || bgSize == 1) {
        return tileY % 32;
    } else {
        return tileY % 64;
    }
}

uint32_t PPU::getSbcOffset(uint8_t bg, uint32_t pixelX, uint32_t pixelY) const {
    uint32_t tileSize = getBgSize(bg);
    switch (tileSize) {
    case 0:
        return 0;
        break;
    case 1:
        return (pixelX >= 32 * TILE_WIDTH_HEIGHT) ? 0x800 : 0;
        break;
    case 2:
        return (pixelY >= 32 * TILE_WIDTH_HEIGHT) ? 0x800 : 0;
        break;
    case 3:
        if (pixelX >= 32 * TILE_WIDTH_HEIGHT) {
            if (pixelY >= 32 * TILE_WIDTH_HEIGHT) {
                return 3 * 0x800;
            } else {
                return 0x800;
            }
        } else {
            if (pixelY >= 32 * TILE_WIDTH_HEIGHT) {
                return 2 * 0x800;
            } else {
                return 0;
            }
        }
        break;
    default:
        throw std::runtime_error("Unknown background size in PPU::getSbcOffset");
        break;
    }
    return 0;
}

bool PPU::getObjScanline(const uint8_t objNum, int32_t* toPaint) {
    auto obj = getObj(objNum);

    if (!obj.has_value()) {
        return false;
    }

    auto height = obj->getHeight();
    auto width = obj->getWidth();

    auto spriteX = obj->getXCoord();
    auto spriteY = obj->getYCoord();

    uint8_t screenPixelY = getVcount();
    bool colorMode256 = obj->getColorMode();

    const uint16_t* paletteRAM = reinterpret_cast<const uint16_t*>(
        getObjPaletteRAM() + (colorMode256 ? 0 : obj->getPaletteBank() * PALETTE_BANK_SIZE));

    // Sprite should be drawn in this scanline
    for (size_t i = 0; i < SCREEN_WIDTH; i++) {
        uint8_t screenPixelX = i;
        if ((spriteY <= screenPixelY || screenPixelY < spriteY + height) || (spriteX <= screenPixelX ||
            screenPixelX < spriteX + width)) {

            // Determine tile
            // Check REG_DISPCNT mapping: 2d or 1d
            auto objRelativeX = screenPixelX - spriteX;
            auto objRelativeY = screenPixelY - spriteY;

            if (obj->getHorizontalFlip()) {
                objRelativeX = obj->getWidth() - 1 - objRelativeX;
            }

            auto pixelOffset = obj->getPaletteIndex(objRelativeX, objRelativeY, getObjMapping1D());
            auto tile = *(getOVRAM() + pixelOffset);

            bool reverseTile = false;
            if (obj->getHorizontalFlip()) {
                reverseTile = !reverseTile;
            }
            if (spriteX % 2 != 0) {
                reverseTile = !reverseTile;
            }
            if (i % 2 != 0) {
                reverseTile = !reverseTile;
            }

            uint8_t paletteIndex;
            if (colorMode256) {
                paletteIndex = tile;
            } else {
                paletteIndex = reverseTile ? (tile >> 4) : (tile & 0b1111);
            }

            if (paletteIndex != 0) {
                uint16_t pixel = paletteRAM[paletteIndex];

                toPaint[i] = pixel;
            }
        }
    }
    return true;
}

void PPU::renderScanlineMode0() {
    static bool bgxEnabled[4] = {true, true, true, true};

    int32_t toPaint[BG_NUM][SCREEN_WIDTH * 1];
    // Get enabled backgrounds' data
    auto bgBlendOrder = getBgBlendOrder();

    for (const auto& bgNum : bgBlendOrder) {
        if (bgxEnabled[bgNum]) {
            getBackgroundScanline(bgNum, toPaint[bgNum]);
        }
    }

    uint16_t* toPaintEnd = pixelsFrame[getVcount()];
    uint16_t backdropPixel = *reinterpret_cast<uint16_t*>(getBgPaletteRAM());

    if (!bgBlendOrder.empty()) {
        for (size_t i = 0; i < SCREEN_WIDTH; i++) {
            for (auto it = bgBlendOrder.begin(); it != bgBlendOrder.end(); ++it) {
                const auto& bgNum = *it;
                if (toPaint[bgNum][i] != TRANSPARENT_PIXEL) {
                    toPaintEnd[i] = static_cast<uint16_t>(toPaint[bgNum][i]);
                    break;
                }

                // If I am looking at the last bg and this pixel was TRANSPARENT_PIXEL, set backdrop color
                if (std::next(it) == bgBlendOrder.end()) {
                    toPaintEnd[i] = backdropPixel;
                }
            }
        }
    } else {
        memset(toPaintEnd, backdropPixel, SCREEN_WIDTH * sizeof(uint16_t));
    }

    // TODO interleave backgrounds and sprites
    // For now just draw the sprites over the backgrounds...
    if (getObjEnabled()) {
        int32_t toPaintSprites[SCREEN_WIDTH];
        std::fill(std::begin(toPaintSprites), std::end(toPaintSprites), TRANSPARENT_PIXEL);

        auto objList = getObjList();
        for (const auto& obj : objList) {
            obj.getScanline(*this, toPaintSprites);
        }

        for (size_t i = 0; i < SCREEN_WIDTH; i++) {
            if (toPaintSprites[i] != TRANSPARENT_PIXEL) {
                toPaintEnd[i] = static_cast<uint16_t>(toPaintSprites[i]);
            }
        }
    }
}

// test this
void PPU::renderScanlineMode3() {
    auto vCount = getVcount();
    std::memcpy(pixelsFrame[vCount],
                getVRAM() + 2 * SCREEN_WIDTH * (getVcount()),
                SCREEN_WIDTH * sizeof(uint16_t));
}

void PPU::renderScanlineMode4() {

    // Get full palette
    uint16_t* paletteRAM = reinterpret_cast<uint16_t*>(getBgPaletteRAM());
    // Get VRAM corresponding to scanline
    uint8_t* VRAM = getVRAM() + getPageFlipOffset() + SCREEN_WIDTH * (getVcount());
    // Copy your pixel data into the texture's pixel data

    // TODO: Improve mode4 rendering; too slow!"
    uint16_t* toPaint = pixelsFrame[getVcount()];

    std::transform(VRAM, VRAM + SCREEN_WIDTH, toPaint, [&](uint8_t val) { return paletteRAM[val]; });
}

bool PPU::isVblank() const {
    return getVcount() == SCREEN_HEIGHT - 1;
}

const uint16_t* PPU::getFrameBuffer() const {
    return &pixelsFrame[0][0];
}

int PPU::getScreenWidth() const {
    return SCREEN_WIDTH;
}
int PPU::getScreenHeight() const {
    return SCREEN_HEIGHT;
}