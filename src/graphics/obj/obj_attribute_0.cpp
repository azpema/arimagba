#include "obj_attribute_0.hpp"
#include "../../utils/utils.hpp"

ObjAttribute0::ObjAttribute0(uint16_t rawVal) : rawVal(rawVal) {}

uint8_t ObjAttribute0::getYCoord() const{
    return Utils::getRegBits(rawVal, Y_COORD_MASK, Y_COORD_SHIFT);
}

uint8_t ObjAttribute0::getObjMode() const{
    return Utils::getRegBits(rawVal, OBJ_MODE_MASK, OBJ_MODE_SHIFT);
}

uint8_t ObjAttribute0::getGfxMode() const{
    return Utils::getRegBits(rawVal, GFX_MODE_MASK, GFX_MODE_SHIFT);
}

bool ObjAttribute0::getMosaicEnabled() const{
    return Utils::getRegBits(rawVal, MOSAIC_MASK, MOSAIC_SHIFT);
}

uint8_t ObjAttribute0::getColorMode() const{
    return Utils::getRegBits(rawVal, COLOR_MODE_MASK, COLOR_MODE_SHIFT);
}

uint8_t ObjAttribute0::getSpriteShape() const{
    return Utils::getRegBits(rawVal, SPRITE_SHAPE_MASK, SPRITE_SHAPE_SHIFT);
}
