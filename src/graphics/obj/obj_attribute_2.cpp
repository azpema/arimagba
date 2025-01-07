#include "obj_attribute_2.hpp"
#include "../../utils/utils.hpp"

ObjAttribute2::ObjAttribute2(uint16_t rawVal) : rawVal(rawVal) {}

uint16_t ObjAttribute2::getTileIndex() const{
    return Utils::getRegBits(rawVal, TILE_INDEX_MASK, TILE_INDEX_SHIFT);
}

uint8_t ObjAttribute2::getPriority() const{
    return Utils::getRegBits(rawVal, PRIORITY_MASK, PRIORITY_SHIFT);
}

uint8_t ObjAttribute2::getPaletteBank() const{
    return Utils::getRegBits(rawVal, PALETTE_BANK_MASK, PALETTE_BANK_SHIFT);
}