/*
    =====================================
    INPUT HANDLING INTERFACE
    =====================================
    
    Platform-agnostic input interface.
    Actual implementation varies by platform (SDL2 or DS).
*/

#ifndef INPUT_H
#define INPUT_H

#include "../core/game.h"

/*
    Input_HandleCursorMove
    ======================
    Moves cursor in specified direction.
    
    Direction: 0=up, 1=down, 2=left, 3=right
*/
void Input_HandleCursorMove(GameState* game, int direction);

/*
    Input_HandlePieceSelect
    =======================
    Cycles piece selection forward or backward.
    
    Direction: 0=previous, 1=next
*/
void Input_HandlePieceSelect(GameState* game, int direction);

/*
    Input_HandlePlacePiece
    ======================
    Attempts to place the currently selected piece.
*/
void Input_HandlePlacePiece(GameState* game);

#endif
