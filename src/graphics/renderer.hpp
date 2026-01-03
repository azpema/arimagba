#ifndef _RENDERER_
#define _RENDERER_

#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include "ui.hpp"

class Renderer {
  public:
    Renderer(const std::string& title, const uint16_t* frameBuffer, int width, int height);
    ~Renderer();
    void drawFrame();

  private:
    const uint16_t* frameBuffer;
    const int frameWidth;
    const int frameHeight;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* textureFrame = nullptr;
    
    std::unique_ptr<UI> ui;

    constexpr static int WINDOW_WIDTH = 1200;
    constexpr static int WINDOW_HEIGHT = 800;
};

#endif