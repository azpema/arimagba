#include "screen_entry.hpp"
#include "../utils/utils.hpp"

ScreenEntry::ScreenEntry(uint16_t rawVal){
    tileIndex = Utils::getRegBits(rawVal, TILE_INDEX_MASK, TILE_INDEX_SHIFT);
    horizontalFlip = Utils::getRegBits(rawVal, HF_MASK, HF_SHIFT);
    verticalFlip = Utils::getRegBits(rawVal, VF_MASK, VF_SHIFT);
    paletteBank = Utils::getRegBits(rawVal, PALETTE_BANK_MASK, PALETTE_BANK_SHIFT);
}

uint16_t ScreenEntry::getTileIndex() const{
    return tileIndex;
}
        
bool ScreenEntry::getHorizontalFlip() const{
    return horizontalFlip;
}

bool ScreenEntry::getVerticalFlip() const{
    return horizontalFlip;
}

uint8_t ScreenEntry::getPaletteBank() const{
    return paletteBank;
}
