#include "screen_entry.hpp"
#include "../memory/memory_manager.hpp"

ScreenEntry::ScreenEntry(uint16_t rawVal){
    tileIndex = Utils::getRegBits(rawVal, TILE_INDEX_MASK, TILE_INDEX_SHIFT);
    horizontalFlip = Utils::getRegBits(rawVal, HF_MASK, HF_SHIFT);
    verticalFlip = Utils::getRegBits(rawVal, VF_MASK, VF_SHIFT);
    paletteBank =Utils::getRegBits(rawVal, PALETTE_BANK_MASK, PALETTE_BANK_SHIFT);
}

uint8_t ScreenEntry::getTileIndex(){
    return tileIndex;
}
        
bool ScreenEntry::getHorizontalFlip(){
    return horizontalFlip;
}

bool ScreenEntry::getVerticalFlip(){
    return horizontalFlip;
}

uint8_t ScreenEntry::getPaletteBank(){
    return paletteBank;
}
