#ifndef _OBJ_ATTRIBUTE_2_
#define _OBJ_ATTRIBUTE_2_

#include <cstdint>

class ObjAttribute2 {
  public:
    ObjAttribute2(uint16_t rawVal);
    ~ObjAttribute2() = default;

    uint16_t getTileIndex() const;
    uint8_t getPriority() const;
    uint8_t getPaletteBank() const;

  private:
    uint16_t rawVal;
    const static uint16_t TILE_INDEX_MASK = 0b0000001111111111;
    const static uint16_t TILE_INDEX_SHIFT = 0x0;

    const static uint16_t PRIORITY_MASK = 0b0000110000000000;
    const static uint16_t PRIORITY_SHIFT = 0xA;

    const static uint16_t PALETTE_BANK_MASK = 0b1111000000000000;
    const static uint16_t PALETTE_BANK_SHIFT = 0xC;
};

#endif