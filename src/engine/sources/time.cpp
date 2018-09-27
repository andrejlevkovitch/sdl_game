// time.cpp

#include "time.hpp"
#include <SDL2/SDL.h>

uint32_t levi::get_time() { return ::SDL_GetTicks(); }

void levi::delay(uint32_t time) { ::SDL_Delay(time); }
