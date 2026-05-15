/*
    NDS button input.
*/

#include <nds.h>

#include "../../core/game.h"
#include "../platform.h"

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

    scanKeys();

    int held = keysHeld();
    int down = keysDown();

    if ((held & KEY_LEFT) && game->cursorX > 0)
    {
        game->cursorX--;
    }

    if ((held & KEY_RIGHT) && game->cursorX < GRID_SIZE - 1)
    {
        game->cursorX++;
    }

    if ((held & KEY_UP) && game->cursorY > 0)
    {
        game->cursorY--;
    }

    if ((held & KEY_DOWN) && game->cursorY < GRID_SIZE - 1)
    {
        game->cursorY++;
    }

    if (down & KEY_L)
    {
        CyclePiecePrev(game);
    }

    if (down & KEY_R)
    {
        CyclePieceNext(game);
    }

    if (down & KEY_A)
    {
        Game_PlacePiece(game);
    }
}
