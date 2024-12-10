#include "renderer.hpp"
#include "../memory/memory_manager.hpp"
#include "screen_entry.hpp"
// TODO delete later, for std::memcpy
#include <cstring>

Renderer::Renderer(const PPU &ppu, const std::string &title) : ppu(ppu) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, PPU::SCREEN_WIDTH, PPU::SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if(window == nullptr){
        throw std::runtime_error("SDL: Window could not be created: " + std::string(SDL_GetError()));
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if(renderer == nullptr){
        throw std::runtime_error("SDL: Renderer could not be created: " + std::string(SDL_GetError()));
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, PPU::SCREEN_WIDTH, 1);
}

Renderer::~Renderer(){
    if(renderer != nullptr)
        SDL_DestroyRenderer(renderer);

    if(window != nullptr)
        SDL_DestroyWindow(window);

    if(texture != nullptr)
        SDL_DestroyTexture(texture);
}

void Renderer::getBackgroundScanline(const uint8_t bg, int32_t *toPaint){
    const uint8_t BG_NUM = bg;
    if(ppu.getBgColorMode(BG_NUM) != 0){
        throw std::runtime_error("Unimplemented 256 color mode");
    }

    uint8_t tiles[PPU::SCREEN_WIDTH / 2];

    uint8_t screenPixelX = 0;
    uint8_t screenPixelY = ppu.getVcount();

    uint8_t toPaintIndex = 0;
    uint32_t tileSetStartOffset = ppu.getTileSetVramOffset(BG_NUM);
    uint32_t tileMapStartOffset = ppu.getTileMapVramOffset(BG_NUM);
    uint32_t pixelScrollX = ppu.getBgOffsetH(BG_NUM);
    uint32_t pixelScrollY = ppu.getBgOffsetV(BG_NUM);
    
    while(screenPixelX <= PPU::SCREEN_WIDTH){
        
        uint32_t bgPixelX = screenPixelX + pixelScrollX;
        uint32_t bgPixelY = screenPixelY + pixelScrollY;

        uint32_t bgRelativeTileX = getBgRelativeTileX(BG_NUM, (bgPixelX) / BG_TILE_WIDTH_HEIGHT);
        uint32_t bgRelativeTileY = getBgRelativeTileY(BG_NUM, (bgPixelY) / BG_TILE_WIDTH_HEIGHT);
        //std::cout << "bgRelativeTileX" << Utils::toHexString(bgRelativeTileX) << std::endl;
        //std::cout << "bgRelativeTileY" << Utils::toHexString(bgRelativeTileY) << std::endl;

        uint32_t seIndex = ((bgRelativeTileY % 32)*32 + (bgRelativeTileX % 32)) * 2;
        uint32_t sbcOffset = getSbcOffset(BG_NUM, bgRelativeTileX*8, bgRelativeTileY*8);
        uint32_t seOffset = tileMapStartOffset + sbcOffset + seIndex;

        
        uint16_t tileMapRaw = *reinterpret_cast<uint16_t *>(ppu.getVRAM() + seOffset);

        ScreenEntry screenEntry(tileMapRaw);

        //Search the given tile with the tile Index:
        // 0x20: each tile takes 8*8*4bpp = 32 = 0x20 bytes
        //std::cout << "seOffset" << Utils::toHexString(seOffset) << std::endl;
        //std::cout << "tileSetRaw" << Utils::toHexString(tileSetStartOffset + screenEntry.getTileIndex() * 0x20) << std::endl;
        uint16_t tileIndex = screenEntry.getTileIndex();
        uint32_t debug1 = tileSetStartOffset + tileIndex * 0x20;

        uint8_t debugTileData[32];
        uint8_t* tileSetRaw = reinterpret_cast<uint8_t *>(ppu.getVRAM() + debug1);
        std::memcpy(debugTileData, reinterpret_cast<const uint8_t*>(ppu.getVRAM() + debug1), 32);

        // check BG_8BPP to determine palette
        uint32_t paletteOffset = 0x20 * screenEntry.getPaletteBank();
        uint16_t* paletteRAM = reinterpret_cast<uint16_t *>(ppu.getPaletteRAM() + paletteOffset);
        
        uint8_t pixelsLeftInTile = BG_TILE_WIDTH_HEIGHT;
        if(screenPixelX == 0)
            pixelsLeftInTile = BG_TILE_WIDTH_HEIGHT - (pixelScrollX % 8);
        else if(PPU::SCREEN_WIDTH - toPaintIndex < 8)
            pixelsLeftInTile = PPU::SCREEN_WIDTH - toPaintIndex;
        
       for(int i=0; i < pixelsLeftInTile; i++){
            uint16_t pixelIndex = screenPixelX + (ppu.getVcount()%8)*8 +i/2;
            // Take the pixel from the given palette!!!!
            uint32_t tileOffset = (ppu.getVcount()%8)*4 + i/2;
            uint8_t tile = *(tileSetRaw + tileOffset);
            if(tile != 0){
                uint16_t pixel;
                if(i % 2 == 0) 
                    pixel = paletteRAM[tile & 0b1111];
                else
                    pixel = paletteRAM[(tile & 0b11110000) >> 4];
                
                toPaint[toPaintIndex++] = pixel;
            }else{
                // Transparency
                toPaint[toPaintIndex++] = -1;
            }

        }
        screenPixelX += 8;
    }
}

void Renderer::renderScanlineMode0(){
    int32_t toPaint[4][PPU::SCREEN_WIDTH * 1];
    getBackgroundScanline(0, toPaint[0]);
    getBackgroundScanline(1, toPaint[1]);
    getBackgroundScanline(2, toPaint[2]);
    getBackgroundScanline(3, toPaint[3]);

    uint16_t toPaintEnd[PPU::SCREEN_WIDTH * 1];
    
    auto bgBlendOrder = ppu.getBgBlendOrder();
    for(size_t i=0; i<PPU::SCREEN_WIDTH; i++){
        bool pixelChosen = false;
        for(auto it = bgBlendOrder.begin(); it != bgBlendOrder.end(); ++it){
            const auto &bgNum = *it;
            // -1 corresponds to transparency
            if(toPaint[bgNum][i] != -1){
                toPaintEnd[i] = static_cast<uint16_t>(toPaint[bgNum][i]);
                break;
            }
            
            // If I am looking at the last bg and this pixel was -1, set backdrop color
            if(std::next(it) == bgBlendOrder.end()){
                toPaintEnd[i] = *reinterpret_cast<uint16_t *>(ppu.getPaletteRAM());
            }
        }
    }


    void* pixels;
    int pitch;

    SDL_UpdateTexture(texture, NULL, toPaintEnd, PPU::SCREEN_WIDTH * sizeof(uint16_t));
    SDL_Rect rect;

    rect.x = 0;
    rect.y = ppu.getVcount();
    rect.w = PPU::SCREEN_WIDTH;
    rect.h = 1;
    
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
    
    //std::cout << "" << std::endl;
}

void Renderer::renderScanlineMode3(){
    // Create a texture
    //SDL_Texture *texture = SDL_CreateTexture(renderer,
    //    SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, PPU::SCREEN_WIDTH, 1);

    void* pixels;
    int pitch;

    // Lock the texture, which allows direct pixel access
    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    // Copy your pixel data into the texture's pixel data
    memcpy(pixels, ppu.getVRAM() + 2*PPU::SCREEN_WIDTH*(ppu.getVcount()), PPU::SCREEN_WIDTH * sizeof(uint8_t)*2);

    // Unlock the texture to update the changes
    SDL_UnlockTexture(texture);

    // Copy the texture to the renderer
    SDL_Rect rect;
    rect.x = 0;
    rect.y = ppu.getVcount();
    rect.w = PPU::SCREEN_WIDTH;
    rect.h = 1;
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    // Present the renderer
    SDL_RenderPresent(renderer);

    // Clean up
    //SDL_DestroyTexture(texture);

}

void Renderer::renderScanlineMode4(){
    // Create a texture
    //SDL_Texture *texture = SDL_CreateTexture(renderer,
    //    SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, PPU::SCREEN_WIDTH, 1);

    void* pixels;
    int pitch;

    // Lock the texture, which allows direct pixel access
    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    // Get full palette
    uint16_t* paletteRAM = reinterpret_cast<uint16_t *>(ppu.getPaletteRAM());
    // Get VRAM corresponding to scanline
    uint8_t* VRAM = ppu.getVRAM() + ppu.getPageFlipOffset() + PPU::SCREEN_WIDTH*(ppu.getVcount());
    // Copy your pixel data into the texture's pixel data

    // TODO: Improve mode4 rendering; too slow!"
    uint16_t toPaint[240];

    for(int i=0; i<240; i++){
        toPaint[i] = paletteRAM[VRAM[i]];
    }
    memcpy(pixels, &toPaint, PPU::SCREEN_WIDTH * 1 * sizeof(Uint16));

    // Unlock the texture to update the changes
    SDL_UnlockTexture(texture);

    // Copy the texture to the renderer
    SDL_Rect rect;
    rect.x = 0;
    rect.y = ppu.getVcount();
    rect.w = PPU::SCREEN_WIDTH;
    rect.h = 1;
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    // Present the renderer
    SDL_RenderPresent(renderer);

    // Clean up
    //SDL_DestroyTexture(texture);
}

uint32_t Renderer::getBgRelativeTileX(uint8_t bg,uint32_t tileX) const{ 
    uint8_t bgSize = ppu.getBgSize(bg);
    if(bgSize == 0 || bgSize == 2){
        return tileX % 32;
    }else{
        return tileX % 64;
    }
}

uint32_t Renderer::getBgRelativeTileY(uint8_t bg, uint32_t tileY) const{
    uint8_t bgSize = ppu.getBgSize(bg);
    if(bgSize == 0 || bgSize == 1){
        return tileY % 32;
    }else{
        return tileY % 64;
    }
}

uint32_t Renderer::getSbcOffset(uint8_t bg, uint32_t pixelX, uint32_t pixelY) const{
    uint32_t tileSize = ppu.getBgSize(bg);
    switch(tileSize){
        case 0:
            return 0;
            break;
        case 1:
            return (pixelX >= 32 * BG_TILE_WIDTH_HEIGHT) ? 0x800 : 0;
            break;
        case 2:
            return (pixelY >= 32 * BG_TILE_WIDTH_HEIGHT) ? 0x800 : 0;
            break;
        case 3:
            if(pixelX >= 32 * BG_TILE_WIDTH_HEIGHT){
                if(pixelY >= 32 * BG_TILE_WIDTH_HEIGHT){
                    return 3 * 0x800;
                }else{
                    return 0x800;
                }
            }else{
                if(pixelY >= 32 * BG_TILE_WIDTH_HEIGHT){
                    return 2 * 0x800;
                }else{
                    return 0;
                }
            }
    }

}

