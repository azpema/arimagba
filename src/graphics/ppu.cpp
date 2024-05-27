#include "ppu.hpp"

PPU::PPU(std::string title, MemoryManager *memManager){
    mem = memManager;
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if(window == nullptr){
        throw std::runtime_error("SDL: Window could not be created: " + std::string(SDL_GetError()));
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if(renderer == nullptr){
        throw std::runtime_error("SDL: Renderer could not be created: " + std::string(SDL_GetError()));
    }

    io = mem->getIOregisters();
    DISPCNT = io + (0x04000000 - 0x04000000) / 2;
    GREEN_SWAP = io + (0x04000002 - 0x04000000) / 2;
    DISPSTAT = io + (0x04000004 - 0x04000000) / 2;
    VCOUNT = io + (0x04000006 - 0x04000000) / 2;
}

PPU::~PPU(){
    if(renderer != nullptr)
        SDL_DestroyRenderer(renderer);

    if(window != nullptr)
        SDL_DestroyWindow(window);    
}

void PPU::renderScanline(){
    // Check BG Mode in DISPCNT
    // Panda will stop working!!!
    if(*VCOUNT <= 160){
        switch(getDCNT_MODE()){
            case 3:
                renderScanlineMode3();
                break;
            case 4:
                renderScanlineMode4();
                break;
            default:
                throw std::runtime_error("ERROR: Unsupported PPU Mode");
        }
    }

    // Update VCOUNT
    *VCOUNT += 1; 
    if(*VCOUNT == VCOUNT_START_VBLANK)
        setVBlankFlag(true);
    else if(*VCOUNT == VCOUNT_END_VBLANK){
        setVBlankFlag(false);
    }
    else if(*VCOUNT == 228)
        *VCOUNT = 0;

    //TODO REMOVE
    //setHBlankFlag(true);
}

void PPU::setVBlankFlag(bool val){
    uint32_t bitVal = val << DSTAT_IN_VBL_SHIFT;
    uint32_t regVal = *DISPSTAT;
    *DISPSTAT = Utils::setRegBits(regVal, DSTAT_IN_VBL_MASK, bitVal);
}

void PPU::setHBlankFlag(bool val){
    uint32_t bitVal = val << DSTAT_IN_HBL_SHIFT;
    uint32_t regVal = *DISPSTAT;
    *DISPSTAT = Utils::setRegBits(regVal, DSTAT_IN_HBL_MASK, bitVal);
}

void PPU::renderScanlineMode3(){
    // Create a texture
    SDL_Texture *texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, 1);

    void* pixels;
    int pitch;

    // Lock the texture, which allows direct pixel access
    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    // Copy your pixel data into the texture's pixel data
    memcpy(pixels, mem->getRawVRAM() + SCREEN_WIDTH*(*VCOUNT), SCREEN_WIDTH * sizeof(Uint16));

    // Unlock the texture to update the changes
    SDL_UnlockTexture(texture);

    // Copy the texture to the renderer
    SDL_Rect rect;
    rect.x = 0;
    rect.y = *VCOUNT;
    rect.w = SCREEN_WIDTH;
    rect.h = 1;
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    // Present the renderer
    SDL_RenderPresent(renderer);

    // Clean up
    SDL_DestroyTexture(texture);

}

void PPU::renderScanlineMode4(){
    // Create a texture
    SDL_Texture *texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, 1);

    void* pixels;
    int pitch;

    // Lock the texture, which allows direct pixel access
    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    // Get full palette
    uint16_t* paletteRAM = mem->getPaletteRAM();
    // Get VRAM corresponding to scanline
    uint16_t* VRAM = mem->getRawVRAM() + SCREEN_WIDTH/2*(*VCOUNT);
    // Copy your pixel data into the texture's pixel data

    // TODO: Improve mode4 rendering; too slow!"
    uint16_t toPaint[240];
    for(int i=0; i<240/ 2; i++){
        toPaint[i*2] = paletteRAM[VRAM[i] & 0xFF];
        toPaint[i*2+1] = paletteRAM[(VRAM[i] & 0xFF00) > 8 ];
    }
    memcpy(pixels, &toPaint, SCREEN_WIDTH * 1 * sizeof(Uint16));

    // Unlock the texture to update the changes
    SDL_UnlockTexture(texture);

    // Copy the texture to the renderer
    SDL_Rect rect;
    rect.x = 0;
    rect.y = *VCOUNT;
    rect.w = SCREEN_WIDTH;
    rect.h = 1;
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    // Present the renderer
    SDL_RenderPresent(renderer);

    // Clean up
    SDL_DestroyTexture(texture);
}

uint8_t PPU::getDCNT_MODE(){
    return Utils::getRegBits(*DISPCNT, DCNT_MODE_MASK, DCNT_MODE_SHIFT);
}

