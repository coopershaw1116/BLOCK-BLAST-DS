/*
    =====================================
    PLATFORM ABSTRACTION LAYER
    =====================================
    
    Platform-agnostic interface for rendering and input.
    Allows compilation for both PC (SDL2) and DS (libnds).
*/

#ifndef PLATFORM_H
#define PLATFORM_H

#include "../core/game.h"

/*
    Platform Initialization
    ======================
    Initializes the platform-specific rendering/input systems.
    Must be called before any rendering or input operations.
    
    Returns: 0 on success, non-zero on failure
*/
int Platform_Init(void);

/*
    Platform Shutdown
    =================
    Cleans up platform resources.
    Should be called before program exit.
*/
void Platform_Shutdown(void);

/*
    Platform Render
    ===============
    Renders a single frame of the game.
    Platform-specific implementation handles all drawing.
*/
void Platform_Render(GameState* game);

/*
    Platform Update Input
    ====================
    Processes input events and updates game state.
    Platform-specific implementation handles input method.
*/
void Platform_UpdateInput(GameState* game);

/*
    Platform Frame Timing
    ====================
    Handles frame rate limiting and timing.
    Platform-specific implementation manages timing.
*/
void Platform_FrameTiming(void);

/*
    Platform Should Quit
    ====================
    Checks if the application should exit.
    Returns: 1 if should quit, 0 if should continue
*/
int Platform_ShouldQuit(void);

#endif
