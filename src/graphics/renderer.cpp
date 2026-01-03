#include "renderer.hpp"
#include <thread>

// TODO delete later, for std::memcpy
#include <cstring>

Renderer::Renderer(const std::string& title, const uint16_t* frameBuffer, int width, int height) :
    frameBuffer(frameBuffer), frameWidth(width), frameHeight(height) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              frameWidth * 4,
                              frameHeight * 4,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

    if (window == nullptr) {
        throw std::runtime_error("SDL: Window could not be created: " + std::string(SDL_GetError()));
    }

    SDL_SetWindowMinimumSize(window, frameWidth, frameHeight);

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == nullptr) {
        throw std::runtime_error("SDL: Renderer could not be created: " + std::string(SDL_GetError()));
    }

    textureFrame = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, frameWidth, frameHeight);

    ui = std::make_unique<UI>(renderer, window);
}

Renderer::~Renderer() {
    if (textureFrame != nullptr)
        SDL_DestroyTexture(textureFrame);

    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);

    if (window != nullptr)
        SDL_DestroyWindow(window);
}

void Renderer::drawFrame() {
    SDL_UpdateTexture(textureFrame, NULL, frameBuffer, frameWidth * sizeof(uint16_t));
    ui->render(textureFrame);
} 
