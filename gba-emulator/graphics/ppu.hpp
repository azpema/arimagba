#ifndef _PPU_ 
#define _PPU_ 

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

#include "../memory/memory_manager.hpp"

class PPU {
    public:
        PPU(std::string title, MemoryManager *memManager);
        ~PPU();
        void renderScanline();
        
    private:
        MemoryManager *mem;
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;

        const uint32_t SCREEN_WIDTH = 240;
        const uint32_t SCREEN_HEIGHT = 160;
        
};

#endif