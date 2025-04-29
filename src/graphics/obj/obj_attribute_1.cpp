#include "obj_attribute_1.hpp"
#include "../../utils/utils.hpp"

ObjAttribute1::ObjAttribute1(uint16_t rawVal) : rawVal(rawVal) {}

uint16_t ObjAttribute1::getXCoord() const { return Utils::getRegBits(rawVal, X_COORD_MASK, X_COORD_SHIFT); }

uint8_t ObjAttribute1::getAffineIndex() const {
    return Utils::getRegBits(rawVal, AFFINE_INDEX_MASK, AFFINE_INDEX_SHIFT);
}

bool ObjAttribute1::getHorizontalFlip() const {
    return Utils::getRegBits(rawVal, HORIZONTAL_FLIP_MASK, HORIZONTAL_FLIP_SHIFT);
}

bool ObjAttribute1::getVerticalFlip() const {
    return Utils::getRegBits(rawVal, VERTICAL_FLIP_MASK, VERTICAL_FLIP_SHIFT);
}

uint8_t ObjAttribute1::getSpriteSize() const { return Utils::getRegBits(rawVal, SPRITE_SIZE_MASK, SPRITE_SIZE_SHIFT); }