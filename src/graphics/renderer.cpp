#include "renderer.hpp"
#include "../memory/memory_manager.hpp"
#include "screen_entry.hpp"

// TEST: PAINT WHOLE FRAME!! TODO: PAINT ONLY CURRENT SCANLINE

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
}

Renderer::~Renderer(){
    if(renderer != nullptr)
        SDL_DestroyRenderer(renderer);

    if(window != nullptr)
        SDL_DestroyWindow(window);  
}

void Renderer::renderScanlineMode0(){
    //std::cout << " " << std::endl;
    //std::cout << "Tile Set VRAM offset " << Utils::toHexString(0x06000000 + getTileSetVramOffset(0)) << std::endl;
    //std::cout << "Tile map VRAM offset " << Utils::toHexString(0x06000000 + getTileMapVramOffset(0)) << std::endl;
    //std::cout << "Tile size " << Utils::toHexString(getTileSize(0)) << std::endl;

    uint32_t tileSetStartOffset = ppu.getTileSetVramOffset(0);
    uint32_t tileMapStartOffset = ppu.getTileMapVramOffset(0);
    uint32_t tileScrollX = ppu.getBgOffsetH(0);
    uint32_t tileScrollY = ppu.getBgOffsetV(0);

    // Create a texture
    SDL_Texture *texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, BG_TILE_SIZE, BG_TILE_SIZE);

    for(size_t tileX=0; tileX < PPU::SCREEN_WIDTH/BG_TILE_SIZE; tileX++){
        uint32_t tileY = ppu.getVcount() / BG_TILE_SIZE;

        uint32_t bgRelativeTileX = getBgRelativeTileX(tileX + tileScrollX);
        uint32_t bgRelativeTileY = getBgRelativeTileY(tileY + tileScrollY);

        uint32_t seIndex = ((bgRelativeTileY % 32)*32 + (bgRelativeTileX % 32)) * 2;
        uint32_t seOffset = tileMapStartOffset + getSbcOffset(bgRelativeTileX*8, 0) + seIndex;

        uint16_t tileMapRaw = *reinterpret_cast<uint16_t *>(ppu.getVRAM() + seOffset);
        ScreenEntry screenEntry(tileMapRaw);

        //std::cout << "Tile index: " << Utils::toHexString(screenEntry.getTileIndex()) << std::endl;
        //std::cout << "Palette bank: " << Utils::toHexString(screenEntry.getPaletteBank()) << std::endl;

        //Search the given tile with the tile Index:
        // 0x20: each tile takes 8*8*4bpp = 32 = 0x20 bytes
        uint8_t* tileSetRaw = reinterpret_cast<uint8_t *>(ppu.getVRAM() + tileSetStartOffset + screenEntry.getTileIndex() * 0x20);

        // check BG_8BPP to determine palette
        uint16_t* paletteRAM = reinterpret_cast<uint16_t *>(ppu.getPaletteRAM() + 0x20 * screenEntry.getPaletteBank());


        uint16_t toPaint[BG_TILE_SIZE * BG_TILE_SIZE];

        for(int i=0; i<0x20; i++){
            // Take the pixel from the given palette!!!!
            uint8_t tile = *(tileSetRaw + i);
    
            uint16_t pixel1 = paletteRAM[(tile & 0b11110000) >> 4];
            uint16_t pixel2 = paletteRAM[tile & 0b1111];


            toPaint[i*2] = pixel2;
            toPaint[i*2 + 1] = pixel1;
        }

        


        void* pixels;
        int pitch;

        // Lock the texture, which allows direct pixel access
        SDL_LockTexture(texture, NULL, &pixels, &pitch);


        memcpy(pixels, &toPaint, BG_TILE_SIZE * BG_TILE_SIZE * sizeof(uint16_t));

        SDL_UnlockTexture(texture);

        SDL_Rect rect;

        rect.x = tileX * BG_TILE_SIZE;
        rect.y = tileY * BG_TILE_SIZE;
        rect.w = BG_TILE_SIZE;
        rect.h = BG_TILE_SIZE;
        
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);
        

    }
    SDL_DestroyTexture(texture);
}

void Renderer::renderScanlineMode3(){
    // Create a texture
    SDL_Texture *texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, PPU::SCREEN_WIDTH, 1);

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
    SDL_DestroyTexture(texture);

}

void Renderer::renderScanlineMode4(){
    // Create a texture
    SDL_Texture *texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, PPU::SCREEN_WIDTH, 1);

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
    SDL_DestroyTexture(texture);
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
        throw new std::runtime_error("TODO getBgRelativeTileY");
    }
}

uint32_t Renderer::getSbcOffset(uint32_t pixelX, uint32_t pixelY) const{
    uint32_t tileSize = ppu.getBgSize(0);
    if(tileSize == 0){
        return 0;
    }
    if(tileSize == 1){
        if(pixelX >= 32 * BG_TILE_SIZE){
            return 0x800;
        }else{
            return 0;
        }
    }
    throw new std::runtime_error("TODO getSbcOffset");
}

