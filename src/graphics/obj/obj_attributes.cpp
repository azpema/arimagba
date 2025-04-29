#include "obj_attributes.hpp"
#include "../../utils/utils.hpp"
#include "../ppu.hpp"

ObjAttributes::ObjAttributes(uint64_t rawVal) :
    rawVal(rawVal), objAttr0(Utils::getRegBits(rawVal & 0xFFFFFFFF, LO_ATTRIBUTES_0_MASK, LO_ATTRIBUTES_0_SHIFT)),
    objAttr1(Utils::getRegBits(rawVal & 0xFFFFFFFF, LO_ATTRIBUTES_1_MASK, LO_ATTRIBUTES_1_SHIFT)),
    objAttr2(Utils::getRegBits(rawVal >> 32, HI_ATTRIBUTES_2_MASK, HI_ATTRIBUTES_2_SHIFT)) {}

uint64_t ObjAttributes::getRawVal() const { return rawVal; }

uint8_t ObjAttributes::getYCoord() const { return objAttr0.getYCoord(); }

ObjMode ObjAttributes::getObjMode() const { return static_cast<ObjMode>(objAttr0.getObjMode()); }

uint8_t ObjAttributes::getGfxMode() const { return objAttr0.getGfxMode(); }

bool ObjAttributes::getMosaicEnabled() const { return objAttr0.getMosaicEnabled(); }

uint8_t ObjAttributes::getColorMode() const { return objAttr0.getColorMode(); }

uint8_t ObjAttributes::getSpriteShape() const { return objAttr0.getSpriteShape(); }

uint16_t ObjAttributes::getXCoord() const { return objAttr1.getXCoord(); }

uint8_t ObjAttributes::getAffineIndex() const { return objAttr1.getAffineIndex(); }

bool ObjAttributes::getHorizontalFlip() const { return objAttr1.getHorizontalFlip(); }

bool ObjAttributes::getVerticalFlip() const { return objAttr1.getVerticalFlip(); }

uint8_t ObjAttributes::getSpriteSize() const { return objAttr1.getSpriteSize(); }

uint16_t ObjAttributes::getTileIndex() const { return objAttr2.getTileIndex(); }

uint8_t ObjAttributes::getPriority() const { return objAttr2.getPriority(); }

uint8_t ObjAttributes::getPaletteBank() const { return objAttr2.getPaletteBank(); }

uint8_t ObjAttributes::getWidth() const { return widthHeight[getSpriteShape()][getSpriteSize()].width; }

uint8_t ObjAttributes::getHeight() const { return widthHeight[getSpriteShape()][getSpriteSize()].height; }

uint32_t ObjAttributes::getPaletteIndex(const uint8_t coordX, const uint8_t coordY, const bool mapping1D) const {
    const bool colorMode256 = getColorMode();
    const uint8_t tileMultiplier = mapping1D ? (getWidth() / PPU::TILE_WIDTH_HEIGHT) : (0x400 / PPU::TILE_SIZE);

    auto tileBlockY = (coordY / PPU::TILE_WIDTH_HEIGHT) * tileMultiplier;
    auto tileBlockX = (coordX / PPU::TILE_WIDTH_HEIGHT);

    uint32_t tileIndex = getTileIndex() + (colorMode256 ? 2 : 1) * (tileBlockY + tileBlockX);

    auto tileRelativeX = (coordX % PPU::TILE_WIDTH_HEIGHT);
    auto tileRelativeY = (coordY % PPU::TILE_WIDTH_HEIGHT);

    uint32_t pixelOffset;

    if (colorMode256) {
        pixelOffset = (tileIndex * PPU::TILE_SIZE) + tileRelativeX + (tileRelativeY * PPU::TILE_WIDTH_HEIGHT);
    } else {
        pixelOffset = (tileIndex * PPU::TILE_SIZE) + tileRelativeX / 2 + (tileRelativeY * PPU::TILE_WIDTH_HEIGHT) / 2;
    }

    return pixelOffset;
}

bool ObjAttributes::getScanline(const PPU& ppu, int32_t* toPaint) const {
    auto height = getHeight();
    auto width = getWidth();

    auto spriteX = getXCoord();
    auto spriteY = getYCoord();

    uint8_t screenPixelY = ppu.getVcount();
    bool colorMode256 = getColorMode();

    const uint16_t* paletteRAM = reinterpret_cast<const uint16_t*>(
        ppu.getObjPaletteRAM() + (colorMode256 ? 0 : getPaletteBank() * PPU::PALETTE_BANK_SIZE));

    // Sprite should be drawn in this scanline
    for (size_t i = 0; i < PPU::SCREEN_WIDTH; i++) {
        uint8_t screenPixelX = i;
        if (spriteY <= screenPixelY && screenPixelY < spriteY + height && spriteX <= screenPixelX &&
            screenPixelX < spriteX + width) {

            // Determine tile
            // Check REG_DISPCNT mapping: 2d or 1d
            auto objRelativeX = screenPixelX - spriteX;
            auto objRelativeY = screenPixelY - spriteY;

            if (getHorizontalFlip()) {
                objRelativeX = width - 1 - objRelativeX;
            }

            auto pixelOffset = getPaletteIndex(objRelativeX, objRelativeY, ppu.getObjMapping1D());
            auto tile = *(ppu.getOVRAM() + pixelOffset);

            bool reverseTile = false;
            if (getHorizontalFlip()) {
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
