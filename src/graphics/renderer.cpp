#include "renderer.hpp"
#include "../memory/memory_manager.hpp"
#include "obj/obj_attributes.hpp"
#include "screen_entry.hpp"
#include <algorithm>

// TODO delete later, for std::memcpy
#include <cstring>

Renderer::Renderer(const PPU& ppu, const std::string& title) : ppu(ppu) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              PPU::SCREEN_WIDTH * 4,
                              PPU::SCREEN_HEIGHT * 4,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

    if (window == nullptr) {
        throw std::runtime_error("SDL: Window could not be created: " + std::string(SDL_GetError()));
    }

    SDL_SetWindowMinimumSize(window, PPU::SCREEN_WIDTH, PPU::SCREEN_HEIGHT);

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == nullptr) {
        throw std::runtime_error("SDL: Renderer could not be created: " + std::string(SDL_GetError()));
    }

    textureFrame = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, PPU::SCREEN_WIDTH, PPU::SCREEN_HEIGHT);

    ui = std::make_unique<UI>(renderer, window);
}

Renderer::~Renderer() {
    if (textureFrame != nullptr)
        SDL_DestroyTexture(textureFrame);

    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);

    if (window != nullptr)
        SDL_DestroyWindow(window);
}

void Renderer::getBackgroundScanline(const uint8_t bg, int32_t* toPaint) {
    const uint8_t BG_NUM = bg;

    uint8_t tileSize = 0x20;
    bool colorMode256 = ppu.getBgColorMode(BG_NUM) == 1;
    if (colorMode256) {
        tileSize = 0x40;
    }

    uint8_t screenPixelX = 0;
    uint8_t screenPixelY = ppu.getVcount();

    uint8_t toPaintIndex = 0;
    uint32_t tileSetStartOffset = ppu.getTileSetVramOffset(BG_NUM);
    uint32_t tileMapStartOffset = ppu.getTileMapVramOffset(BG_NUM);
    uint32_t pixelScrollX = ppu.getBgOffsetH(BG_NUM);
    uint32_t pixelScrollY = ppu.getBgOffsetV(BG_NUM);

    while (screenPixelX <= PPU::SCREEN_WIDTH) {

        uint32_t bgPixelX = screenPixelX + pixelScrollX;
        uint32_t bgPixelY = screenPixelY + pixelScrollY;

        uint32_t bgRelativeTileX = getBgRelativeTileX(BG_NUM, (bgPixelX) / PPU::TILE_WIDTH_HEIGHT);
        uint32_t bgRelativeTileY = getBgRelativeTileY(BG_NUM, (bgPixelY) / PPU::TILE_WIDTH_HEIGHT);
        // std::cout << "bgRelativeTileX" << Utils::toHexString(bgRelativeTileX) << '\n';
        // std::cout << "bgRelativeTileY" << Utils::toHexString(bgRelativeTileY) << '\n';

        uint32_t seIndex = ((bgRelativeTileY % 32) * 32 + (bgRelativeTileX % 32)) * 2;
        uint32_t sbcOffset = getSbcOffset(BG_NUM, bgRelativeTileX * 8, bgRelativeTileY * 8);
        uint32_t seOffset = tileMapStartOffset + sbcOffset + seIndex;

        uint16_t tileMapRaw = *reinterpret_cast<uint16_t*>(ppu.getVRAM() + seOffset);

        ScreenEntry screenEntry(tileMapRaw);

        // Search the given tile with the tile Index:
        //  0x20: each tile takes 8*8*4bpp = 32 = 0x20 bytes
        // std::cout << "seOffset" << Utils::toHexString(seOffset) << '\n;
        // std::cout << "tileSetRaw" << Utils::toHexString(tileSetStartOffset + screenEntry.getTileIndex() * 0x20) <<
        // '\n';
        uint16_t tileIndex = screenEntry.getTileIndex();
        uint32_t debug1 = tileSetStartOffset + tileIndex * tileSize;

        uint8_t* tileSetRaw = reinterpret_cast<uint8_t*>(ppu.getVRAM() + debug1);

        // check BG_8BPP to determine palette
        uint32_t paletteOffset = colorMode256 ? 0 : tileSize * screenEntry.getPaletteBank();
        uint16_t* paletteRAM = reinterpret_cast<uint16_t*>(ppu.getBgPaletteRAM() + paletteOffset);

        uint8_t pixelsLeftInTile = PPU::TILE_WIDTH_HEIGHT;
        uint8_t tileStartOffset = 0;
        if (screenPixelX == 0) {
            pixelsLeftInTile = PPU::TILE_WIDTH_HEIGHT - (pixelScrollX % 8);
            tileStartOffset = pixelScrollX % 8;
        } else if (PPU::SCREEN_WIDTH - toPaintIndex < 8) {
            pixelsLeftInTile = PPU::SCREEN_WIDTH - toPaintIndex;
        }

        for (uint8_t i = tileStartOffset; i < tileStartOffset + pixelsLeftInTile; i++) {
            // Take the pixel from the given palette!!!!
            uint8_t pixelIndex = i;
            if (screenEntry.getHorizontalFlip()) {
                pixelIndex = PPU::TILE_WIDTH_HEIGHT - 1 - i;
            }

            uint32_t tileOffset = ((ppu.getVcount() + pixelScrollY) % PPU::TILE_WIDTH_HEIGHT);
            if (colorMode256) {
                tileOffset = tileOffset * 8 + pixelIndex;
            } else {
                tileOffset = tileOffset * 4 + pixelIndex / 2;
            }
            uint8_t tile = *(tileSetRaw + tileOffset);

            uint8_t paletteIndex;
            if (colorMode256) {
                paletteIndex = tile;
            } else {
                if (pixelIndex % 2 == 0) {
                    paletteIndex = tile & 0b1111;
                } else {
                    paletteIndex = (tile & 0b11110000) >> 4;
                }
            }

            if (paletteIndex != 0) {
                uint16_t pixel = paletteRAM[paletteIndex];

                toPaint[toPaintIndex++] = pixel;
            } else {
                toPaint[toPaintIndex++] = TRANSPARENT_PIXEL;
            }
        }
        screenPixelX += 8;
    }
}

bool Renderer::getObjScanline(const uint8_t objNum, int32_t* toPaint) {
    auto obj = ppu.getObj(objNum);

    if (!obj.has_value()) {
        return false;
    }

    auto height = obj->getHeight();
    auto width = obj->getWidth();

    auto spriteX = obj->getXCoord();
    auto spriteY = obj->getYCoord();

    uint8_t screenPixelY = ppu.getVcount();
    bool colorMode256 = obj->getColorMode();

    const uint16_t* paletteRAM = reinterpret_cast<const uint16_t*>(
        ppu.getObjPaletteRAM() + (colorMode256 ? 0 : obj->getPaletteBank() * PPU::PALETTE_BANK_SIZE));

    // Sprite should be drawn in this scanline
    for (size_t i = 0; i < PPU::SCREEN_WIDTH; i++) {
        uint8_t screenPixelX = i;
        if ((spriteY <= screenPixelY || screenPixelY < spriteY + height) || (spriteX <= screenPixelX ||
            screenPixelX < spriteX + width)) {

            // Determine tile
            // Check REG_DISPCNT mapping: 2d or 1d
            auto objRelativeX = screenPixelX - spriteX;
            auto objRelativeY = screenPixelY - spriteY;

            if (obj->getHorizontalFlip()) {
                objRelativeX = obj->getWidth() - 1 - objRelativeX;
            }

            auto pixelOffset = obj->getPaletteIndex(objRelativeX, objRelativeY, ppu.getObjMapping1D());
            auto tile = *(ppu.getOVRAM() + pixelOffset);

            bool reverseTile = false;
            if (obj->getHorizontalFlip()) {
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

void Renderer::renderScanlineMode0() {
    static bool bgxEnabled[4] = {true, true, true, true};

    int32_t toPaint[PPU::BG_NUM][PPU::SCREEN_WIDTH * 1];
    // Get enabled backgrounds' data
    auto bgBlendOrder = ppu.getBgBlendOrder();

    for (const auto& bgNum : bgBlendOrder) {
        if (bgxEnabled[bgNum]) {
            getBackgroundScanline(bgNum, toPaint[bgNum]);
        }
    }

    uint16_t* toPaintEnd = pixelsFrame[ppu.getVcount()];
    uint16_t backdropPixel = *reinterpret_cast<uint16_t*>(ppu.getBgPaletteRAM());

    if (!bgBlendOrder.empty()) {
        for (size_t i = 0; i < PPU::SCREEN_WIDTH; i++) {
            for (auto it = bgBlendOrder.begin(); it != bgBlendOrder.end(); ++it) {
                const auto& bgNum = *it;
                if (toPaint[bgNum][i] != TRANSPARENT_PIXEL) {
                    toPaintEnd[i] = static_cast<uint16_t>(toPaint[bgNum][i]);
                    break;
                }

                // If I am looking at the last bg and this pixel was TRANSPARENT_PIXEL, set backdrop color
                if (std::next(it) == bgBlendOrder.end()) {
                    toPaintEnd[i] = backdropPixel;
                }
            }
        }
    } else {
        memset(toPaintEnd, backdropPixel, PPU::SCREEN_WIDTH * sizeof(uint16_t));
    }

    // TODO interleave backgrounds and sprites
    // For now just draw the sprites over the backgrounds...
    if (ppu.getObjEnabled()) {
        int32_t toPaintSprites[PPU::SCREEN_WIDTH];
        std::fill(std::begin(toPaintSprites), std::end(toPaintSprites), TRANSPARENT_PIXEL);

        auto objList = ppu.getObjList();
        for (const auto& obj : objList) {
            obj.getScanline(ppu, toPaintSprites);
        }

        for (size_t i = 0; i < PPU::SCREEN_WIDTH; i++) {
            if (toPaintSprites[i] != TRANSPARENT_PIXEL) {
                toPaintEnd[i] = static_cast<uint16_t>(toPaintSprites[i]);
            }
        }
    }

    if (ppu.getVcount() == PPU::SCREEN_HEIGHT - 1) {
        SDL_UpdateTexture(textureFrame, NULL, pixelsFrame, PPU::SCREEN_WIDTH * sizeof(uint16_t));
        ui->render(textureFrame);
    }
}

// test this
void Renderer::renderScanlineMode3() {
    auto vCount = ppu.getVcount();
    std::memcpy(pixelsFrame[vCount],
                ppu.getVRAM() + 2 * PPU::SCREEN_WIDTH * (ppu.getVcount()),
                PPU::SCREEN_WIDTH * sizeof(uint16_t));
    if (vCount == PPU::SCREEN_HEIGHT - 1) {
        SDL_UpdateTexture(textureFrame, NULL, pixelsFrame, 240 * sizeof(uint16_t));
        ui->render(textureFrame);
    }
}

void Renderer::renderScanlineMode4() {

    // Get full palette
    uint16_t* paletteRAM = reinterpret_cast<uint16_t*>(ppu.getBgPaletteRAM());
    // Get VRAM corresponding to scanline
    uint8_t* VRAM = ppu.getVRAM() + ppu.getPageFlipOffset() + PPU::SCREEN_WIDTH * (ppu.getVcount());
    // Copy your pixel data into the texture's pixel data

    // TODO: Improve mode4 rendering; too slow!"
    uint16_t* toPaint = pixelsFrame[ppu.getVcount()];

    std::transform(VRAM, VRAM + PPU::SCREEN_WIDTH, toPaint, [&](uint8_t val) { return paletteRAM[val]; });

    if (ppu.getVcount() == PPU::SCREEN_HEIGHT - 1) {
        SDL_UpdateTexture(textureFrame, NULL, pixelsFrame, 240 * sizeof(uint16_t));
        ui->render(textureFrame);
    }
}

uint32_t Renderer::getBgRelativeTileX(uint8_t bg, uint32_t tileX) const {
    uint8_t bgSize = ppu.getBgSize(bg);
    if (bgSize == 0 || bgSize == 2) {
        return tileX % 32;
    } else {
        return tileX % 64;
    }
}

uint32_t Renderer::getBgRelativeTileY(uint8_t bg, uint32_t tileY) const {
    uint8_t bgSize = ppu.getBgSize(bg);
    if (bgSize == 0 || bgSize == 1) {
        return tileY % 32;
    } else {
        return tileY % 64;
    }
}

uint32_t Renderer::getSbcOffset(uint8_t bg, uint32_t pixelX, uint32_t pixelY) const {
    uint32_t tileSize = ppu.getBgSize(bg);
    switch (tileSize) {
    case 0:
        return 0;
        break;
    case 1:
        return (pixelX >= 32 * PPU::TILE_WIDTH_HEIGHT) ? 0x800 : 0;
        break;
    case 2:
        return (pixelY >= 32 * PPU::TILE_WIDTH_HEIGHT) ? 0x800 : 0;
        break;
    case 3:
        if (pixelX >= 32 * PPU::TILE_WIDTH_HEIGHT) {
            if (pixelY >= 32 * PPU::TILE_WIDTH_HEIGHT) {
                return 3 * 0x800;
            } else {
                return 0x800;
            }
        } else {
            if (pixelY >= 32 * PPU::TILE_WIDTH_HEIGHT) {
                return 2 * 0x800;
            } else {
                return 0;
            }
        }
        break;
    default:
        throw std::runtime_error("Unknown background size in Renderer::getSbcOffset");
        break;
    }
    return 0;
}
