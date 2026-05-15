/*
    =====================================
    PLATFORM IMPLEMENTATION - SDL2 (PC)
    =====================================
    
    SDL2-based rendering and input for desktop/testing.
*/

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "platform.h"
#include "../rendering/render.h"
#include "../input/input.h"

/*
    Global state
*/
static SDL_Window* g_window = NULL;
static SDL_Renderer* g_renderer = NULL;
static bool g_should_quit = false;

/*
    Platform_Init
    =============
    Initializes SDL2 window and renderer for PC testing.
*/
int Platform_Init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return 1;
    }

    g_window = SDL_CreateWindow(
        "BLOCK BLAST DS",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DS_WIDTH,
        DS_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!g_window)
    {
        SDL_Quit();
        return 1;
    }

    g_renderer = SDL_CreateRenderer(
        g_window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!g_renderer)
    {
        SDL_DestroyWindow(g_window);
        SDL_Quit();
        return 1;
    }

    g_should_quit = false;
    return 0;
}

/*
    Platform_Shutdown
    =================
    Cleans up SDL2 resources.
*/
void Platform_Shutdown(void)
{
    if (g_renderer)
    {
        SDL_DestroyRenderer(g_renderer);
    }

    if (g_window)
    {
        SDL_DestroyWindow(g_window);
    }

    SDL_Quit();
}

/*
    Platform_Render
    ===============
    Renders game frame using SDL2.
*/
void Platform_Render(GameState* game)
{
    Render_Draw(g_renderer, game);
    SDL_RenderPresent(g_renderer);
}

/*
    Platform_UpdateInput
    ====================
    Processes SDL2 input events and translates them to game input.
*/
void Platform_UpdateInput(GameState* game)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            g_should_quit = true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_LEFT:
                    Input_HandleCursorMove(game, 2);  /* Left */
                    break;
                case SDLK_RIGHT:
                    Input_HandleCursorMove(game, 3);  /* Right */
                    break;
                case SDLK_UP:
                    Input_HandleCursorMove(game, 0);  /* Up */
                    break;
                case SDLK_DOWN:
                    Input_HandleCursorMove(game, 1);  /* Down */
                    break;
                case SDLK_q:
                    Input_HandlePieceSelect(game, 0);  /* Previous */
                    break;
                case SDLK_e:
                    Input_HandlePieceSelect(game, 1);  /* Next */
                    break;
                case SDLK_SPACE:
                    Input_HandlePlacePiece(game);
                    break;
                default:
                    break;
            }
        }
    }
}

/*
    Platform_FrameTiming
    ====================
    Limits frame rate to ~60 FPS on PC.
*/
void Platform_FrameTiming(void)
{
    SDL_Delay(16);  /* 16ms ≈ 60 FPS */
}

/*
    Platform_ShouldQuit
    ===================
    Returns quit flag.
*/
int Platform_ShouldQuit(void)
{
    return g_should_quit;
}
