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

void Renderer::renderScanlineMode0(){
    if(ppu.getBgColorMode(0) != 0){
        throw std::runtime_error("Unimplemented 256 color mode");
    }
    uint8_t screenPixelX = 0;
    uint8_t screenPixelY = ppu.getVcount();

    uint8_t toPaintIndex = 0;
    uint32_t tileSetStartOffset = ppu.getTileSetVramOffset(0);
    uint32_t tileMapStartOffset = ppu.getTileMapVramOffset(0);
    uint32_t pixelScrollX = ppu.getBgOffsetH(0);
    uint32_t pixelScrollY = ppu.getBgOffsetV(0);

    uint16_t toPaint[PPU::SCREEN_WIDTH * 1];
    
    while(screenPixelX <= PPU::SCREEN_WIDTH){
        
        uint32_t bgPixelX = screenPixelX + pixelScrollX;
        uint32_t bgPixelY = screenPixelY + pixelScrollY;

        uint32_t bgRelativeTileX = getBgRelativeTileX((bgPixelX) / BG_TILE_WIDTH_HEIGHT);
        uint32_t bgRelativeTileY = getBgRelativeTileY((bgPixelY) / BG_TILE_WIDTH_HEIGHT);
        //std::cout << "bgRelativeTileX" << Utils::toHexString(bgRelativeTileX) << std::endl;
        //std::cout << "bgRelativeTileY" << Utils::toHexString(bgRelativeTileY) << std::endl;

        uint32_t seIndex = ((bgRelativeTileY % 32)*32 + (bgRelativeTileX % 32)) * 2;
        uint32_t sbcOffset = getSbcOffset(bgRelativeTileX*8, 0);
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
                uint16_t backdrop = *reinterpret_cast<uint16_t *>(ppu.getPaletteRAM());
                toPaint[toPaintIndex++] = backdrop;
            }

        }
        screenPixelX += 8;
    }

    void* pixels;
    int pitch;

    // Lock the texture, which allows direct pixel access
    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    memcpy(pixels, &toPaint, PPU::SCREEN_WIDTH * 1 * sizeof(Uint16));

    SDL_UnlockTexture(texture);

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

uint32_t Renderer::getBgRelativeTileX(uint32_t tileX) const{ 
    uint8_t bgSize = ppu.getBgSize(0);
    if(bgSize == 0 || bgSize == 2){
        return tileX % 32;
    }else{
        return tileX % 64;
    }
}

uint32_t Renderer::getBgRelativeTileY(uint32_t tileY) const{
    uint8_t bgSize = ppu.getBgSize(0);
    if(bgSize == 0 || bgSize == 1){
        return tileY % 32;
    }else{
        throw std::runtime_error("TODO getBgRelativeTileY");
    }
}

uint32_t Renderer::getSbcOffset(uint32_t pixelX, uint32_t pixelY) const{
    uint32_t tileSize = ppu.getBgSize(0);
    if(tileSize == 0){
        return 0;
    }
    if(tileSize == 1){
        if(pixelX >= 32 * BG_TILE_WIDTH_HEIGHT){
            return 0x800;
        }else{
            return 0;
        }
    }
    throw std::runtime_error("TODO getSbcOffset");
}

