#include "renderer.hpp"
#include "../memory/memory_manager.hpp"
#include "screen_entry.hpp"
#include "obj/obj_attributes.hpp"

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

    textureScanline = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, PPU::SCREEN_WIDTH, 1);
    textureFrame = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, PPU::SCREEN_WIDTH, PPU::SCREEN_HEIGHT);
}

Renderer::~Renderer(){
    if(renderer != nullptr)
        SDL_DestroyRenderer(renderer);

    if(window != nullptr)
        SDL_DestroyWindow(window);

    if(textureScanline != nullptr)
        SDL_DestroyTexture(textureScanline);
    
    if(textureFrame != nullptr)
        SDL_DestroyTexture(textureFrame);
}

void Renderer::getBackgroundScanline(const uint8_t bg, int32_t *toPaint){
    const uint8_t BG_NUM = bg;
    if(ppu.getBgColorMode(BG_NUM) != 0){
        throw std::runtime_error("Unimplemented 256 color mode");
    }

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

        uint32_t bgRelativeTileX = getBgRelativeTileX(BG_NUM, (bgPixelX) / PPU::TILE_WIDTH_HEIGHT);
        uint32_t bgRelativeTileY = getBgRelativeTileY(BG_NUM, (bgPixelY) / PPU::TILE_WIDTH_HEIGHT);
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
        uint16_t* paletteRAM = reinterpret_cast<uint16_t *>(ppu.getBgPaletteRAM() + paletteOffset);
        
        uint8_t pixelsLeftInTile = PPU::TILE_WIDTH_HEIGHT;
        uint8_t tileStartOffset = 0;
        if(screenPixelX == 0){
            pixelsLeftInTile = PPU::TILE_WIDTH_HEIGHT - (pixelScrollX % 8);
            tileStartOffset = pixelScrollX % 8;
        }else if(PPU::SCREEN_WIDTH - toPaintIndex < 8){
            pixelsLeftInTile = PPU::SCREEN_WIDTH - toPaintIndex;
        }

        for(uint8_t i=tileStartOffset; i < tileStartOffset + pixelsLeftInTile; i++){
            // Take the pixel from the given palette!!!!
            uint32_t tileOffset = ((ppu.getVcount() + pixelScrollY) % PPU::TILE_WIDTH_HEIGHT)*4 + i/2;
            uint8_t tile = *(tileSetRaw + tileOffset);

            uint8_t paletteIndex;
            if(i % 2 == 0){
                paletteIndex = tile & 0b1111;
            }else{
                paletteIndex = (tile & 0b11110000) >> 4;
            }

            if(paletteIndex != 0){
                uint16_t pixel = paletteRAM[paletteIndex];
                
                toPaint[toPaintIndex++] = pixel;
            }else{
                // Transparency
                toPaint[toPaintIndex++] = -1;
            }

        }
        screenPixelX += 8;
    }
}

bool Renderer::getObjScanline(const uint8_t objNum, int32_t *toPaint){
    uint64_t rawObjAttributes = *reinterpret_cast<uint64_t *>(ppu.getOAM() + objNum*0x8);
    ObjAttributes obj(rawObjAttributes);

    switch(obj.getObjMode()){
        case ObjMode::DISABLED:
            return false;
        case ObjMode::NORMAL:
            break;
        default:
        throw std::runtime_error("Unimplemented OBJ Mode");
    }

    auto height = obj.getHeight();
    auto width = obj.getWidth();

    auto spriteX = obj.getXCoord();
    auto spriteY = obj.getYCoord();
    
    uint8_t screenPixelY = ppu.getVcount();
    auto ovram = ppu.getOVRAM();
    uint16_t* paletteRAM = reinterpret_cast<uint16_t *>(ppu.getObjPaletteRAM() + obj.getPaletteBank() * PPU::PALETTE_BANK_SIZE);
    // Sprite should be drawn in this scanline
    for(size_t i=0; i<PPU::SCREEN_WIDTH; i++){
        uint8_t screenPixelX = i;
        if(spriteY <= screenPixelY && screenPixelY < spriteY + height && 
           spriteX <= screenPixelX && screenPixelX < spriteX + width ){
            // Determine tile
            // Check REG_DISPCNT mapping: 2d or 1d
            auto objRelativeX = screenPixelX - spriteX;
            auto objRelativeY = screenPixelY - spriteY;

            auto pixelOffset = obj.getPaletteIndex(objRelativeX, objRelativeY, ppu.getObjMapping1D());
            auto tile = *(ovram + pixelOffset);

            uint8_t paletteIndex;
            if(i % 2 == 0){
                if(spriteX % 2 == 0){
                    paletteIndex = tile & 0b1111;
                }else{
                    paletteIndex = tile >> 4;
                }
            }else{
                if(spriteX % 2 == 0){
                    paletteIndex = tile >> 4;
                }else{
                    paletteIndex = tile & 0b1111;
                }
            }


            if(paletteIndex != 0){
                uint16_t pixel = paletteRAM[paletteIndex];
                
                toPaint[i] = pixel;
            }else{
                if(toPaint[i] == -2){
                    toPaint[i] = -1;
                }
                
            }
        }else{
            if(toPaint[i] == -2){
                toPaint[i] = -1;
            }
                
        }
    }
    return true;
}

void Renderer::renderScanlineMode0(){
    int32_t toPaint[PPU::BG_NUM][PPU::SCREEN_WIDTH * 1];
    // Get enabled backgrounds' data
    auto bgBlendOrder = ppu.getBgBlendOrder();
    for(const auto &bgNum : bgBlendOrder){
        getBackgroundScanline(bgNum, toPaint[bgNum]);
    }

    uint16_t *toPaintEnd = pixelsFrame[ppu.getVcount()];
    uint16_t backdropPixel = *reinterpret_cast<uint16_t *>(ppu.getBgPaletteRAM());
    
    if(!bgBlendOrder.empty()){
        for(size_t i=0; i<PPU::SCREEN_WIDTH; i++){
            for(auto it = bgBlendOrder.begin(); it != bgBlendOrder.end(); ++it){
                const auto &bgNum = *it;
                // -1 corresponds to transparency
                if(toPaint[bgNum][i] != -1){
                    toPaintEnd[i] = static_cast<uint16_t>(toPaint[bgNum][i]);
                    break;
                }
                
                // If I am looking at the last bg and this pixel was -1, set backdrop color
                if(std::next(it) == bgBlendOrder.end()){
                    toPaintEnd[i] = backdropPixel;
                }
            }
        }
    }else{
        memset(toPaintEnd, backdropPixel, PPU::SCREEN_WIDTH * sizeof(uint16_t));
    }

    // For now just draw the sprites over the backgrounds...
    int32_t toPaintSprites[PPU::SCREEN_WIDTH];
    std::fill(std::begin(toPaintSprites), std::end(toPaintSprites)+10, -2);

    for(size_t i=0; i<128; i++){
        bool ret = getObjScanline(i, toPaintSprites);
        if(!ret){
            break;
        }
    }
    

    for(size_t i=0; i<PPU::SCREEN_WIDTH; i++){
        if(toPaintSprites[i] != -1){
            toPaintEnd[i] = static_cast<uint16_t>(toPaintSprites[i]);
        }
    }

#ifdef RENDER_SCANLINE
    SDL_UpdateTexture(textureScanline, NULL, toPaintEnd, PPU::SCREEN_WIDTH * sizeof(uint16_t));
    SDL_Rect rect;

    rect.x = 0;
    rect.y = ppu.getVcount();
    rect.w = PPU::SCREEN_WIDTH;
    rect.h = 1;
    
    SDL_RenderCopy(renderer, textureScanline, NULL, &rect);
    
#else
    if(ppu.getVcount() == PPU::SCREEN_HEIGHT - 1){
        SDL_UpdateTexture(textureFrame, NULL, pixelsFrame, 240 * sizeof(uint16_t));
        SDL_Rect rect;

        rect.x = 0;
        rect.y = 0;
        rect.w = PPU::SCREEN_WIDTH;
        rect.h = PPU::SCREEN_HEIGHT;

        SDL_RenderCopy(renderer, textureFrame, NULL, &rect);
        SDL_RenderPresent(renderer);
    }
#endif
    
}

// test this
void Renderer::renderScanlineMode3(){
#ifdef RENDER_SCANLINE
    SDL_UpdateTexture(textureScanline, NULL, ppu.getVRAM() + 2*PPU::SCREEN_WIDTH*(ppu.getVcount()), PPU::SCREEN_WIDTH * sizeof(uint16_t));

    SDL_Rect rect;
    rect.x = 0;
    rect.y = ppu.getVcount();
    rect.w = PPU::SCREEN_WIDTH;
    rect.h = 1;
    SDL_RenderCopy(renderer, textureScanline, NULL, &rect);
    SDL_RenderPresent(renderer);
#else
    auto vCount = ppu.getVcount();
    std::memcpy(pixelsFrame[vCount], ppu.getVRAM() + 2*PPU::SCREEN_WIDTH*(ppu.getVcount()), PPU::SCREEN_WIDTH * sizeof(uint16_t));
    if(vCount == PPU::SCREEN_HEIGHT - 1){
        SDL_UpdateTexture(textureFrame, NULL, pixelsFrame, 240 * sizeof(uint16_t));
        SDL_Rect rect;

        rect.x = 0;
        rect.y = 0;
        rect.w = PPU::SCREEN_WIDTH;
        rect.h = PPU::SCREEN_HEIGHT;
        
        SDL_RenderCopy(renderer, textureFrame, NULL, &rect);
        SDL_RenderPresent(renderer);
    }
#endif

}

void Renderer::renderScanlineMode4(){

    // Get full palette
    uint16_t* paletteRAM = reinterpret_cast<uint16_t *>(ppu.getBgPaletteRAM());
    // Get VRAM corresponding to scanline
    uint8_t* VRAM = ppu.getVRAM() + ppu.getPageFlipOffset() + PPU::SCREEN_WIDTH*(ppu.getVcount());
    // Copy your pixel data into the texture's pixel data

    // TODO: Improve mode4 rendering; too slow!"
    uint16_t *toPaint = pixelsFrame[ppu.getVcount()];

    std::transform(VRAM, VRAM + PPU::SCREEN_WIDTH, toPaint, [&](uint8_t val) { return paletteRAM[val]; });

#ifdef RENDER_SCANLINE
    SDL_UpdateTexture(textureScanline, NULL, toPaint, PPU::SCREEN_WIDTH * sizeof(uint16_t));

    SDL_Rect rect;
    rect.x = 0;
    rect.y = ppu.getVcount();
    rect.w = PPU::SCREEN_WIDTH;
    rect.h = 1;
    SDL_RenderCopy(renderer, textureScanline, NULL, &rect);
    SDL_RenderPresent(renderer);
#else
    if(ppu.getVcount() == PPU::SCREEN_HEIGHT - 1){
        SDL_UpdateTexture(textureFrame, NULL, pixelsFrame, 240 * sizeof(uint16_t));
        SDL_Rect rect;

        rect.x = 0;
        rect.y = 0;
        rect.w = PPU::SCREEN_WIDTH;
        rect.h = PPU::SCREEN_HEIGHT;
        
        SDL_RenderCopy(renderer, textureFrame, NULL, &rect);
        SDL_RenderPresent(renderer);
    }
#endif
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
            return (pixelX >= 32 * PPU::TILE_WIDTH_HEIGHT) ? 0x800 : 0;
            break;
        case 2:
            return (pixelY >= 32 * PPU::TILE_WIDTH_HEIGHT) ? 0x800 : 0;
            break;
        case 3:
            if(pixelX >= 32 * PPU::TILE_WIDTH_HEIGHT){
                if(pixelY >= 32 * PPU::TILE_WIDTH_HEIGHT){
                    return 3 * 0x800;
                }else{
                    return 0x800;
                }
            }else{
                if(pixelY >= 32 * PPU::TILE_WIDTH_HEIGHT){
                    return 2 * 0x800;
                }else{
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

