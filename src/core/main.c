/*
    =====================================
    BLOCK BLAST DS - MAIN ENTRY POINT
    =====================================
    
    Platform-agnostic main loop.
    Delegates rendering/input to platform abstraction layer.
    Compiles for both PC (SDL2) and DS (libnds) targets.
*/

#include <stdbool.h>

#include "game.h"
#include "../platform/platform.h"

/*
    Main Game Loop
    ==============
    - Initializes platform
    - Runs event loop at target frame rate
    - Coordinates game updates with platform
    - Cleans up resources on exit
*/
int main(int argc, char* argv[])
{
    /*
        Initialize Platform
        ===================
        Sets up rendering and input system (SDL2 or libnds)
    */
    if (Platform_Init() != 0)
    {
        return 1;
    }

    /*
        Initialize Game State
    */
    GameState game;
    Game_Init(&game);

    /*
        Main Game Loop
        ==============
        Runs until platform signals exit
        Target: 60 FPS
    */
    while (!Platform_ShouldQuit())
    {
        /*
            Update Game State
            =================
            Process animations and frame updates
        */
        Game_UpdateAnimation(&game);

        /*
            Process Input
        */
        Platform_UpdateInput(&game);

        /*
            Render Frame
        */
        Platform_Render(&game);

        /*
            Frame Timing
        */
        Platform_FrameTiming();
    }

    /*
        Cleanup
    */
    Platform_Shutdown();

    return 0;
}
