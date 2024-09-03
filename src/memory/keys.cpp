#include "keys.hpp"
#include "../memory/memory_manager.hpp"

const std::map<SDL_Keycode, uint16_t> Keys::SdlKeyToGbaMask = { {SDLK_x, KEYINPUT_BUTTON_A_MASK},
                                                                {SDLK_z, KEYINPUT_BUTTON_B_MASK},
                                                                {SDLK_BACKSPACE, KEYINPUT_BUTTON_SELECT_MASK},
                                                                {SDLK_RETURN, KEYINPUT_BUTTON_START_MASK},
                                                                {SDLK_RIGHT, KEYINPUT_BUTTON_RIGHT_MASK},
                                                                {SDLK_LEFT, KEYINPUT_BUTTON_LEFT_MASK},
                                                                {SDLK_UP, KEYINPUT_BUTTON_UP_MASK},
                                                                {SDLK_DOWN, KEYINPUT_BUTTON_DOWN_MASK},
                                                                {SDLK_s, KEYINPUT_BUTTON_R_MASK},
                                                                {SDLK_a, KEYINPUT_BUTTON_L_MASK}
                                                              };

const std::map<SDL_Keycode, uint16_t> Keys::SdlKeyToGbaShift = { {SDLK_x, KEYINPUT_BUTTON_A_SHIFT},
                                                                 {SDLK_z, KEYINPUT_BUTTON_B_SHIFT},
                                                                 {SDLK_BACKSPACE, KEYINPUT_BUTTON_SELECT_SHIFT},
                                                                 {SDLK_RETURN, KEYINPUT_BUTTON_START_SHIFT},
                                                                 {SDLK_RIGHT, KEYINPUT_BUTTON_RIGHT_SHIFT},
                                                                 {SDLK_LEFT, KEYINPUT_BUTTON_LEFT_SHIFT},
                                                                 {SDLK_UP, KEYINPUT_BUTTON_UP_SHIFT},
                                                                 {SDLK_DOWN, KEYINPUT_BUTTON_DOWN_SHIFT},
                                                                 {SDLK_s, KEYINPUT_BUTTON_R_SHIFT},
                                                                 {SDLK_a, KEYINPUT_BUTTON_L_SHIFT}
                                                               };


Keys::Keys(MemoryManager *memManager){
    mem = memManager;

    io = mem->getIOregisters();
    KEYINPUT = reinterpret_cast<uint16_t*>(io + (0x04000130 - MemoryManager::IO_REGISTERS_OFFSET_START));
    KEYCNT = reinterpret_cast<uint16_t*>(io + (0x04000132- MemoryManager::IO_REGISTERS_OFFSET_START));

    // Start with keys released
    *KEYINPUT = 0x03FF;
}

void Keys::handleKey(SDL_Keycode key, bool press){
    if(SdlKeyToGbaMask.contains(key)){
        static uint32_t* reg = reinterpret_cast<uint32_t*>(KEYINPUT);
        if(press){
            Utils::clearRegBits(*reg, SdlKeyToGbaMask.at(key));
        }else{
            Utils::setRegBits(*reg, SdlKeyToGbaMask.at(key), 1 << SdlKeyToGbaShift.at(key));
        }
    }
}