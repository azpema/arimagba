#ifndef _UI_
#define _UI_

#include <SDL2/SDL.h>

class UI {
  public:
    UI(SDL_Renderer* ppuRenderer, SDL_Window* ppuWindow);
    ~UI();

    void render(SDL_Texture* texture) const;

  private:
    SDL_Renderer* renderer;
    SDL_Window* window;

};

#endif