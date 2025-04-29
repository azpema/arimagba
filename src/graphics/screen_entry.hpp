#ifndef _SCREEN_ENTRY_
#define _SCREEN_ENTRY_

#include <iostream>

class ScreenEntry {
  public:
    ScreenEntry(uint16_t rawVal);

    uint16_t getTileIndex() const;
    bool getHorizontalFlip() const;
    bool getVerticalFlip() const;
    uint8_t getPaletteBank() const;

  private:
    uint16_t tileIndex;
    uint8_t horizontalFlip;
    uint8_t verticalFlip;
    uint8_t paletteBank;

    const static uint16_t TILE_INDEX_MASK = 0b0000001111111111;
    const static uint16_t TILE_INDEX_SHIFT = 0x0;

    const static uint16_t HF_MASK = 0b0000010000000000;
    const static uint16_t HF_SHIFT = 0xA;

    const static uint16_t VF_MASK = 0b0000100000000000;
    const static uint16_t VF_SHIFT = 0xB;

    const static uint16_t PALETTE_BANK_MASK = 0b1111000000000000;
    const static uint16_t PALETTE_BANK_SHIFT = 0xC;
};

#endif