/*
    PC/SDL input — polls keyboard each frame.
*/

#include <SDL2/SDL.h>

#include "../../core/game.h"
#include "../platform.h"
#include "pc_internal.h"

static void CyclePiecePrev(GameState* game)
{
    int prevSelected = game->selectedPiece;
    game->selectedPiece--;

    if (game->selectedPiece < 0)
    {
        game->selectedPiece = 2;
    }

    if (game->pieceUsed[game->selectedPiece])
    {
        game->selectedPiece = prevSelected;
    }
}

static void CyclePieceNext(GameState* game)
{
    int prevSelected = game->selectedPiece;
    game->selectedPiece++;

    if (game->selectedPiece > 2)
    {
        game->selectedPiece = 0;
    }

    if (game->pieceUsed[game->selectedPiece])
    {
        game->selectedPiece = prevSelected;
    }
}

void Input_Update(GameState* game)
{
    if (game->phase != GAME_PHASE_PLAYING)
    {
        return;
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    static bool leftWasDown;
    static bool rightWasDown;
    static bool upWasDown;
    static bool downWasDown;
    static bool qWasDown;
    static bool eWasDown;
    static bool spaceWasDown;

    bool leftDown = keys[SDL_SCANCODE_LEFT];
    bool rightDown = keys[SDL_SCANCODE_RIGHT];
    bool upDown = keys[SDL_SCANCODE_UP];
    bool downDown = keys[SDL_SCANCODE_DOWN];
    bool qDown = keys[SDL_SCANCODE_Q];
    bool eDown = keys[SDL_SCANCODE_E];
    bool spaceDown = keys[SDL_SCANCODE_SPACE];

    if (leftDown && !leftWasDown && game->cursorX > 0)
    {
        game->cursorX--;
    }

    if (rightDown && !rightWasDown && game->cursorX < GRID_SIZE - 1)
    {
        game->cursorX++;
    }

    if (upDown && !upWasDown && game->cursorY > 0)
    {
        game->cursorY--;
    }

    if (downDown && !downWasDown && game->cursorY < GRID_SIZE - 1)
    {
        game->cursorY++;
    }

    if (qDown && !qWasDown)
    {
        CyclePiecePrev(game);
    }

    if (eDown && !eWasDown)
    {
        CyclePieceNext(game);
    }

    if (spaceDown && !spaceWasDown)
    {
        Game_PlacePiece(game);
    }

    leftWasDown = leftDown;
    rightWasDown = rightDown;
    upWasDown = upDown;
    downWasDown = downDown;
    qWasDown = qDown;
    eWasDown = eDown;
    spaceWasDown = spaceDown;
}
