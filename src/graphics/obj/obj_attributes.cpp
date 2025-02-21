#include "obj_attributes.hpp"
#include "../ppu.hpp"
#include "../../utils/utils.hpp"

ObjAttributes::ObjAttributes(uint64_t rawVal) : 
    objAttr0(Utils::getRegBits(rawVal & 0xFFFFFFFF, LO_ATTRIBUTES_0_MASK, LO_ATTRIBUTES_0_SHIFT)),
    objAttr1(Utils::getRegBits(rawVal & 0xFFFFFFFF, LO_ATTRIBUTES_1_MASK, LO_ATTRIBUTES_1_SHIFT)),
    objAttr2(Utils::getRegBits(rawVal >> 32, HI_ATTRIBUTES_2_MASK, HI_ATTRIBUTES_2_SHIFT)) {}

uint8_t ObjAttributes::getYCoord() const{
    return objAttr0.getYCoord();
}

ObjMode ObjAttributes::getObjMode() const{
    return static_cast<ObjMode>(objAttr0.getObjMode());
}

uint8_t ObjAttributes::getGfxMode() const{
    return objAttr0.getGfxMode();
}

bool ObjAttributes::getMosaicEnabled() const{
    return objAttr0.getMosaicEnabled();
}

uint8_t ObjAttributes::getColorMode() const{
    return objAttr0.getColorMode();
}

uint8_t ObjAttributes::getSpriteShape() const{
    return objAttr0.getSpriteShape();
}

uint16_t ObjAttributes::getXCoord() const{
    return objAttr1.getXCoord();
}

uint8_t ObjAttributes::getAffineIndex() const{
    return objAttr1.getAffineIndex();
}

bool ObjAttributes::getHorizontalFlip() const{
    return objAttr1.getHorizontalFlip();
}

bool ObjAttributes::getVerticalFlip() const{
    return objAttr1.getVerticalFlip();
}

uint8_t ObjAttributes::getSpriteSize() const{
    return objAttr1.getSpriteSize();
}

uint16_t ObjAttributes::getTileIndex() const{
    return objAttr2.getTileIndex();
}

uint8_t ObjAttributes::getPriority() const{
    return objAttr2.getPriority();
}

uint8_t ObjAttributes::getPaletteBank() const{
    return objAttr2.getPaletteBank();
}

uint8_t ObjAttributes::getWidth() const{
    return widthHeight[getSpriteShape()][getSpriteSize()].width;
}

uint8_t ObjAttributes::getHeight() const{
    return widthHeight[getSpriteShape()][getSpriteSize()].height;
}

uint32_t ObjAttributes::getPaletteIndex(const uint8_t coordX, const uint8_t coordY, const bool mapping1D) const{
    const bool colorMode256 = getColorMode();
    const uint8_t tileMultiplier = mapping1D ? (getWidth() / PPU::TILE_WIDTH_HEIGHT) : (0x400 / PPU::TILE_SIZE);

    auto tileBlockY = (coordY / PPU::TILE_WIDTH_HEIGHT) * tileMultiplier;
    auto tileBlockX = (coordX / PPU::TILE_WIDTH_HEIGHT);

    uint32_t tileIndex = getTileIndex() + (colorMode256 ? 2 : 1) * (tileBlockY + tileBlockX);

    auto tileRelativeX = (coordX % PPU::TILE_WIDTH_HEIGHT);
    auto tileRelativeY = (coordY % PPU::TILE_WIDTH_HEIGHT);

    uint32_t pixelOffset;
    
    if(colorMode256){
        pixelOffset = (tileIndex * PPU::TILE_SIZE) + tileRelativeX + (tileRelativeY * PPU::TILE_WIDTH_HEIGHT);
    }else{
        pixelOffset = (tileIndex * PPU::TILE_SIZE) + tileRelativeX/2 + (tileRelativeY * PPU::TILE_WIDTH_HEIGHT)/2;
    }
    
    return pixelOffset;
}
