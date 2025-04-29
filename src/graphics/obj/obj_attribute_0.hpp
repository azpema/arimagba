#ifndef _OBJ_ATTRIBUTE_0_
#define _OBJ_ATTRIBUTE_0_

#include <cstdint>

class ObjAttribute0 {
  public:
    ObjAttribute0(uint16_t rawVal);
    ~ObjAttribute0() = default;

    uint8_t getYCoord() const;
    uint8_t getObjMode() const;
    uint8_t getGfxMode() const;
    bool getMosaicEnabled() const;
    uint8_t getColorMode() const;
    uint8_t getSpriteShape() const;

  private:
    uint16_t rawVal;
    const static uint16_t Y_COORD_MASK = 0b0000000011111111;
    const static uint16_t Y_COORD_SHIFT = 0x0;

    const static uint16_t OBJ_MODE_MASK = 0b0000001100000000;
    const static uint16_t OBJ_MODE_SHIFT = 0x8;

    const static uint16_t GFX_MODE_MASK = 0b0000110000000000;
    const static uint16_t GFX_MODE_SHIFT = 0xA;

    const static uint16_t MOSAIC_MASK = 0b0001000000000000;
    const static uint16_t MOSAIC_SHIFT = 0xC;

    const static uint16_t COLOR_MODE_MASK = 0b0010000000000000;
    const static uint16_t COLOR_MODE_SHIFT = 0xD;

    const static uint16_t SPRITE_SHAPE_MASK = 0b1100000000000000;
    const static uint16_t SPRITE_SHAPE_SHIFT = 0xE;
};

#endif