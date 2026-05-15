/*
    =====================================
    GAME IMPLEMENTATION
    =====================================
    
    Initializes game state and randomly selects starting pieces.
*/

#include <stdlib.h>

#include "game.h"

static int Game_DetectClears(GameState* game, uint8_t* rows, uint8_t* cols)
{
    uint8_t rowMask = 0;
    uint8_t colMask = 0;

    for (int y = 0; y < GRID_SIZE; y++)
    {
        uint8_t occ = 0;
        for (int x = 0; x < GRID_SIZE; x++)
        {
            if (game->board.cells[y][x])
            {
                occ |= (uint8_t)(1u << x);
            }
        }
        if (occ == 0xFF)
        {
            rowMask |= (uint8_t)(1u << y);
        }
    }

    for (int x = 0; x < GRID_SIZE; x++)
    {
        uint8_t occ = 0;
        for (int y = 0; y < GRID_SIZE; y++)
        {
            if (game->board.cells[y][x])
            {
                occ |= (uint8_t)(1u << y);
            }
        }
        if (occ == 0xFF)
        {
            colMask |= (uint8_t)(1u << x);
        }
    }

    *rows = rowMask;
    *cols = colMask;
    return rowMask != 0 || colMask != 0;
}

static void Game_ApplyClears(GameState* game)
{
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            if ((game->clearingRows & (uint8_t)(1u << y)) ||
                (game->clearingCols & (uint8_t)(1u << x)))
            {
                game->board.cells[y][x] = 0;
            }
        }
    }
}

static void Game_StartClearing(GameState* game)
{
    uint8_t rows = 0;
    uint8_t cols = 0;

    if (!Game_DetectClears(game, &rows, &cols))
    {
        return;
    }

    game->clearingRows = rows;
    game->clearingCols = cols;
    game->clearAnimFrame = 0;
    game->phase = GAME_PHASE_CLEARING;
}

void Game_InitSeeded(GameState* game, unsigned int seed)
{
    game->cursorX = 0;
    game->cursorY = 0;
    game->selectedPiece = 0;
    game->usedCount = 0;

    for (int i = 0; i < 3; i++)
    {
        game->pieceUsed[i] = 0;
    }

    srand((int)seed);

    Game_GenerateNewPieces(game);

    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            game->board.cells[y][x] = 0;
        }
    }

    game->phase = GAME_PHASE_PLAYING;
    game->clearingRows = 0;
    game->clearingCols = 0;
    game->clearAnimFrame = 0;
}

void Game_Init(GameState* game)
{
    Game_InitSeeded(game, 1u);
}

/*
    Game_GenerateNewPieces
    ======================
    Randomly generates 3 new pieces and resets usage tracking.
*/
void Game_GenerateNewPieces(GameState* game)
{
    /* Generate 3 random pieces */
    for (int i = 0; i < 3; i++)
    {
        int randomIndex = rand() % pieceDefinitionCount;
        game->activePieces[i] = pieceDefinitions[randomIndex];
        game->pieceUsed[i] = 0; /* Mark as available */
    }
    
    /* Reset usage counter */
    game->usedCount = 0;
    
    /* Select first available piece */
    game->selectedPiece = 0;
}

/*
    Game_PlacePiece
    ===============
    Attempts to place the selected piece at the cursor position.
    
    Steps:
    1. Check if the piece has already been used (skip if already placed)
    2. Get the selected piece
    3. Check if placement is valid (no overlaps, within bounds)
    4. If valid, place the piece on the board
    5. Mark the piece as used
    6. If all 3 pieces are used, generate new ones
    
    Returns: 1 if successful, 0 if invalid placement
*/
int Game_PlacePiece(GameState* game)
{
    /* Check if piece is already used */
    if (game->pieceUsed[game->selectedPiece])
    {
        return 0; /* Cannot place already-used piece */
    }

    Piece* piece = &game->activePieces[game->selectedPiece];
    int startX = game->cursorX;
    int startY = game->cursorY;

    /* Check bounds and collisions */
    for (int py = 0; py < piece->height; py++)
    {
        for (int px = 0; px < piece->width; px++)
        {
            /* Skip empty cells in piece */
            if (!piece->cells[py][px])
                continue;

            int boardX = startX + px;
            int boardY = startY + py;

            /* Check if out of bounds */
            if (boardX < 0 || boardX >= GRID_SIZE ||
                boardY < 0 || boardY >= GRID_SIZE)
            {
                return 0; /* Invalid placement */
            }

            /* Check if cell is already occupied */
            if (game->board.cells[boardY][boardX] != 0)
            {
                return 0; /* Invalid placement */
            }
        }
    }

    /* Placement is valid, place the piece */
    for (int py = 0; py < piece->height; py++)
    {
        for (int px = 0; px < piece->width; px++)
        {
            if (!piece->cells[py][px])
                continue;

            int boardX = startX + px;
            int boardY = startY + py;
            game->board.cells[boardY][boardX] = 1;
        }
    }

    /* Mark piece as used */
    game->pieceUsed[game->selectedPiece] = 1;
    game->usedCount++;

    /* If all pieces are used, generate new ones */
    if (game->usedCount == 3)
    {
        Game_GenerateNewPieces(game);
    }

    Game_StartClearing(game);

    return 1; /* Success */
}

void Game_Update(GameState* game)
{
    if (game->phase != GAME_PHASE_CLEARING)
    {
        return;
    }

    game->clearAnimFrame++;

    if (game->clearAnimFrame >= CLEAR_ANIM_FRAMES)
    {
        Game_ApplyClears(game);
        game->clearingRows = 0;
        game->clearingCols = 0;
        game->clearAnimFrame = 0;
        game->phase = GAME_PHASE_PLAYING;
    }
}
