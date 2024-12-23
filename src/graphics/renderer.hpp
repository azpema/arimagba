#ifndef _RENDERER_ 
#define _RENDERER_ 

#include <iostream>
#include <SDL2/SDL.h>

#include "ppu.hpp"

//#define RENDER_SCANLINE
class PPU;

class Renderer {
    public:
        Renderer(const PPU &ppu, const std::string &title);
        ~Renderer();
        
        void renderScanlineMode0();
        void renderScanlineMode3();
        void renderScanlineMode4();
        
        

    private:
        void getBackgroundScanline(const uint8_t bg, int32_t *toPaint);
        void getSpriteScanline(const uint8_t bg, int32_t *toPaint);

        bool getObjScanline(const uint8_t objNum, int32_t *toPaint);

        uint32_t getBgRelativeTileX(uint8_t bg, uint32_t tileX) const;
        uint32_t getBgRelativeTileY(uint8_t bg, uint32_t tileY) const;
        uint32_t getSbcOffset(uint8_t bg, uint32_t pixelX, uint32_t pixelY) const;

        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        SDL_Texture *textureScanline = nullptr;
        SDL_Texture *textureFrame = nullptr;
        uint16_t pixelsFrame[160][240] = {{0}};
        const PPU &ppu;
        
        const static uint32_t SCREENBLOCK_SIZE = 0x0800;
        const static uint32_t PAGE_FLIP_SECOND_OFFSET = 0xA000;
};

#endif