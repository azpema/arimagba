#ifndef _KEYS_ 
#define _KEYS_ 

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <map>

#include "../memory/memory_manager.hpp"

class Keys {
    public:
        Keys(MemoryManager *memManager);
        void handleKey(SDL_Keycode key, bool press);
         
    private:
        MemoryManager *mem;
        
        uint8_t* io;
        uint16_t* KEYINPUT;
        uint16_t* KEYCNT;

        constexpr static uint16_t KEYINPUT_BUTTON_A_MASK = 0x1;
        constexpr static uint16_t KEYINPUT_BUTTON_A_SHIFT = 0x0;

        constexpr static uint16_t KEYINPUT_BUTTON_B_MASK = 0x2;
        constexpr static uint16_t KEYINPUT_BUTTON_B_SHIFT = 0x1;

        constexpr static uint16_t KEYINPUT_BUTTON_SELECT_MASK = 0x4;
        constexpr static uint16_t KEYINPUT_BUTTON_SELECT_SHIFT = 0x2;

        constexpr static uint16_t KEYINPUT_BUTTON_START_MASK = 0x8;
        constexpr static uint16_t KEYINPUT_BUTTON_START_SHIFT = 0x3;

        constexpr static uint16_t KEYINPUT_BUTTON_RIGHT_MASK = 0x10;
        constexpr static uint16_t KEYINPUT_BUTTON_RIGHT_SHIFT = 0x4;

        constexpr static uint16_t KEYINPUT_BUTTON_LEFT_MASK = 0x20;
        constexpr static uint16_t KEYINPUT_BUTTON_LEFT_SHIFT = 0x5;

        constexpr static uint16_t KEYINPUT_BUTTON_UP_MASK = 0x40;
        constexpr static uint16_t KEYINPUT_BUTTON_UP_SHIFT = 0x6;

        constexpr static uint16_t KEYINPUT_BUTTON_DOWN_MASK = 0x80;
        constexpr static uint16_t KEYINPUT_BUTTON_DOWN_SHIFT = 0x7;

        constexpr static uint16_t KEYINPUT_BUTTON_R_MASK = 0x100;
        constexpr static uint16_t KEYINPUT_BUTTON_R_SHIFT = 0x8;

        constexpr static uint16_t KEYINPUT_BUTTON_L_MASK = 0x200;
        constexpr static uint16_t KEYINPUT_BUTTON_L_SHIFT = 0x9;

        const static std::map<SDL_Keycode, uint16_t> SdlKeyToGbaMask;
        const static std::map<SDL_Keycode, uint16_t> SdlKeyToGbaShift;
};

#endif