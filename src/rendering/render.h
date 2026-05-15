/*
    =====================================
    RENDERING INTERFACE
    =====================================
    
    Handles all visual output to the screen including:
    - Top screen piece selection UI
    - Bottom screen game board
    - Grid rendering
    - Cursor and preview visualization
    
    Platform-agnostic interface - implementation varies by platform.
*/

#ifndef RENDER_H
#define RENDER_H

#include "../core/game.h"

/* Opaque renderer pointer - platform-specific */
typedef void* Renderer;

/*
    Render_Draw
    ===========
    Main rendering function called once per frame.
    Draws both DS screens with appropriate layout.
    
    Parameters:
    - renderer: Platform-specific renderer object (SDL_Renderer* on PC, NULL on DS)
    - game: Current game state
*/
void Render_Draw(Renderer renderer, GameState* game);

#endif
