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
    renderScanlineMode4();

    // Update VCOUNT
    mem->getIOregisters()[0x03] += 1; 
    if(mem->getIOregisters()[0x03] == 228)
        mem->getIOregisters()[0x03] = 0;
}


void PPU::renderScanlineMode3(){
    // Create a texture
    SDL_Texture *texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    void* pixels;
    int pitch;

    // Lock the texture, which allows direct pixel access
    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    // Copy your pixel data into the texture's pixel data
    memcpy(pixels, mem->getRawVRAM(), SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint16));

    // Unlock the texture to update the changes
    SDL_UnlockTexture(texture);

    // Copy the texture to the renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

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

    uint16_t vcount = mem->getIOregisters()[0x03];

    // Get full palette
    uint16_t* paletteRAM = mem->getPaletteRAM();
    // Get VRAM corresponding to scanline
    uint16_t* VRAM = mem->getRawVRAM() + vcount;
    // Copy your pixel data into the texture's pixel data

    std::cout << "TODO: Improve mode4 rendering; too slow!" << std::endl;
    uint16_t toPaint[240];
    for(int i=0; i<240/ 2; i++){
        toPaint[i*2] = paletteRAM[VRAM[i]];
        toPaint[i*2+1] = paletteRAM[VRAM[i]];
    }
    memcpy(pixels, &toPaint, SCREEN_WIDTH * 1 * sizeof(Uint16));

    // Unlock the texture to update the changes
    SDL_UnlockTexture(texture);

    // Copy the texture to the renderer
    SDL_Rect rect;
    rect.x = 0; // x position where the line will be drawn
    rect.y = vcount;   // y position where the line will be drawn
    rect.w = SCREEN_WIDTH;   // width of the line (1 pixel)
    rect.h = 1; // height of the line (600 pixels)
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    // Present the renderer
    SDL_RenderPresent(renderer);

    // Clean up
    SDL_DestroyTexture(texture);
}


