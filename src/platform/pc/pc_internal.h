#ifndef PC_INTERNAL_H
#define PC_INTERNAL_H

#include <stdbool.h>
#include <SDL2/SDL.h>

SDL_Renderer* PC_GetRenderer(void);
bool PC_IsRunning(void);
void PC_SetRunning(bool running);

#endif
