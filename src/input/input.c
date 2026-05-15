/*
    =====================================
    INPUT IMPLEMENTATION
    =====================================
    
    Platform-independent input handling.
    Translates input commands to game actions.
*/

#include "input.h"

/*
    Input_HandleCursorMove
    ======================
    Moves cursor in specified direction.
    
    Direction: 0=up, 1=down, 2=left, 3=right
*/
void Input_HandleCursorMove(GameState* game, int direction)
{
    switch (direction)
    {
        case 0:  /* Up */
            if (game->cursorY > 0)
                game->cursorY--;
            break;
        case 1:  /* Down */
            if (game->cursorY < GRID_SIZE - 1)
                game->cursorY++;
            break;
        case 2:  /* Left */
            if (game->cursorX > 0)
                game->cursorX--;
            break;
        case 3:  /* Right */
            if (game->cursorX < GRID_SIZE - 1)
                game->cursorX++;
            break;
    }
}

/*
    Input_HandlePieceSelect
    =======================
    Cycles piece selection forward or backward.
    
    Direction: 0=previous, 1=next
*/
void Input_HandlePieceSelect(GameState* game, int direction)
{
    int prevSelected = game->selectedPiece;
    
    if (direction == 0)  /* Previous */
    {
        game->selectedPiece--;
        if (game->selectedPiece < 0)
            game->selectedPiece = 2;
    }
    else  /* Next */
    {
        game->selectedPiece++;
        if (game->selectedPiece > 2)
            game->selectedPiece = 0;
    }
    
    /* Skip if piece is already used */
    if (game->pieceUsed[game->selectedPiece])
    {
        game->selectedPiece = prevSelected;
    }
}

/*
    Input_HandlePlacePiece
    ======================
    Attempts to place the currently selected piece.
*/
void Input_HandlePlacePiece(GameState* game)
{
    Game_PlacePiece(game);
}
