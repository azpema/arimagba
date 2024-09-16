#include "ppu.hpp"
#include "../memory/memory_manager.hpp"

PPU::PPU(const std::string &title, MemoryManager *memManager, ExceptionHandler &ex){
    exceptionHandler = ex;
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
    DISPCNT = reinterpret_cast<uint16_t*>(io + (0x04000000 - MemoryManager::IO_REGISTERS_OFFSET_START));
    GREEN_SWAP = reinterpret_cast<uint16_t*>(io + (0x04000002 - MemoryManager::IO_REGISTERS_OFFSET_START));
    DISPSTAT = reinterpret_cast<uint16_t*>(io + (0x04000004 - MemoryManager::IO_REGISTERS_OFFSET_START));
    VCOUNT = reinterpret_cast<uint16_t*>(io + (0x04000006 - MemoryManager::IO_REGISTERS_OFFSET_START));

    setDCNT_MODE(3);
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
        uint8_t dcntMode = getDCNT_MODE();
        switch(dcntMode){
            case 3:
                renderScanlineMode3();
                break;
            case 4:
                renderScanlineMode4();
                break;
            case 7:
                renderScanlineMode3();
                break;
            default:
                throw std::runtime_error("ERROR: Unsupported PPU Mode: " + std::to_string(dcntMode));
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
    memcpy(pixels, mem->getRawVRAM() + 2*SCREEN_WIDTH*(*VCOUNT), SCREEN_WIDTH * sizeof(uint8_t)*2);

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
    uint16_t* paletteRAM = reinterpret_cast<uint16_t *>(mem->getPaletteRAM());
    // Get VRAM corresponding to scanline
    uint8_t* VRAM = mem->getRawVRAM() + getPageFlipOffset() + SCREEN_WIDTH*(*VCOUNT);
    // Copy your pixel data into the texture's pixel data

    // TODO: Improve mode4 rendering; too slow!"
    uint16_t toPaint[240];

    for(int i=0; i<240; i++){
        toPaint[i] = paletteRAM[VRAM[i]];
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

void PPU::setDCNT_MODE(uint8_t mode){
    uint32_t bitVal = mode << DCNT_MODE_SHIFT;
    uint32_t regVal = *DISPCNT;
    *DISPCNT = Utils::setRegBits(regVal, DCNT_MODE_MASK, bitVal);
}

uint8_t PPU::getDCNT_MODE(){
    return Utils::getRegBits(*DISPCNT, DCNT_MODE_MASK, DCNT_MODE_SHIFT);
}

uint8_t PPU::getDCNT_PAGE(){
    return Utils::getRegBits(*DISPCNT, DCNT_PAGE_MASK, DCNT_PAGE_SHIFT);
}

uint32_t PPU::getPageFlipOffset(){
    return getDCNT_PAGE() ? PAGE_FLIP_SECOND_OFFSET : 0;
}
