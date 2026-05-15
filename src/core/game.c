/*
    =====================================
    GAME IMPLEMENTATION
    =====================================
    
    Initializes game state and randomly selects starting pieces.
*/

#include <stdlib.h>
#include <time.h>

#include "game.h"

/*
    Game_Init
    =========
    Initializes the game state with:
    - Cursor at origin (0, 0)
    - Random piece selection for the player's hand
    - Random seed set for procedural generation
*/
void Game_Init(GameState* game)
{
    /* Initialize cursor position */
    game->cursorX = 0;
    game->cursorY = 0;

    /* Select first piece as default */
    game->selectedPiece = 0;
    
    /* Initialize piece usage tracking */
    game->usedCount = 0;
    for (int i = 0; i < 3; i++)
    {
        game->pieceUsed[i] = 0;
    }

    /* Seed random number generator */
    srand(time(NULL));

    /* Populate player's hand with 3 random pieces */
    Game_GenerateNewPieces(game);

    /* Initialize board (all cells empty) */
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            game->board.cells[y][x] = 0;
            game->animation.cells[y][x] = 0;
        }
    }
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

    /* Check for and clear complete rows */
    Game_ClearRows(game);

    /* If all pieces are used, generate new ones */
    if (game->usedCount == 3)
    {
        Game_GenerateNewPieces(game);
    }

    return 1; /* Success */
}

/*
    Game_ClearRows
    ===============
    Scans the board for complete rows and columns, then marks them for animation.
    Complete rows/columns must be filled edge-to-edge of the grid.
    Cells are marked for animation and cleared after animation completes.
    
    Returns: Total number of rows and columns marked for clearing
*/
int Game_ClearRows(GameState* game)
{
    int cellsToClear[GRID_SIZE][GRID_SIZE]; /* Track which cells to clear */
    int rowsToClear[GRID_SIZE];             /* List of complete row indices */
    int columnsToClear[GRID_SIZE];          /* List of complete column indices */
    int rowClearCount = 0;                  /* Number of complete rows */
    int colClearCount = 0;                  /* Number of complete columns */
    int x, y, i;
    
    /* Initialize cells to clear grid */
    for (y = 0; y < GRID_SIZE; y++)
    {
        for (x = 0; x < GRID_SIZE; x++)
        {
            cellsToClear[y][x] = 0;
        }
    }
    
    /* Scan each row to find complete ones */
    for (y = 0; y < GRID_SIZE; y++)
    {
        int rowComplete = 1; /* Assume row is complete */
        
        /* Check if all cells in this row are filled */
        for (x = 0; x < GRID_SIZE; x++)
        {
            if (game->board.cells[y][x] == 0)
            {
                rowComplete = 0;
                break;
            }
        }
        
        /* If row is complete, mark it for clearing */
        if (rowComplete)
        {
            rowsToClear[rowClearCount++] = y;
            for (x = 0; x < GRID_SIZE; x++)
            {
                cellsToClear[y][x] = 1;
            }
        }
    }
    
    /* Scan each column to find complete ones */
    for (x = 0; x < GRID_SIZE; x++)
    {
        int colComplete = 1; /* Assume column is complete */
        
        /* Check if all cells in this column are filled */
        for (y = 0; y < GRID_SIZE; y++)
        {
            if (game->board.cells[y][x] == 0)
            {
                colComplete = 0;
                break;
            }
        }
        
        /* If column is complete, mark it for clearing */
        if (colComplete)
        {
            columnsToClear[colClearCount++] = x;
            for (y = 0; y < GRID_SIZE; y++)
            {
                cellsToClear[y][x] = 1;
            }
        }
    }
    
    /* Mark all cells for animation (store frame counter) */
    for (y = 0; y < GRID_SIZE; y++)
    {
        for (x = 0; x < GRID_SIZE; x++)
        {
            if (cellsToClear[y][x])
            {
                game->animation.cells[y][x] = CLEAR_ANIMATION_FRAMES;
            }
        }
    }
    
    return rowClearCount + colClearCount;
}

/*
    Game_UpdateAnimation
    ====================
    Updates animation state for clearing effects.
    Decrements animation frame counters and clears cells when animation ends.
    Should be called once per frame during gameplay.
*/
void Game_UpdateAnimation(GameState* game)
{
    int x, y;
    
    /* Update all animating cells */
    for (y = 0; y < GRID_SIZE; y++)
    {
        for (x = 0; x < GRID_SIZE; x++)
        {
            if (game->animation.cells[y][x] > 0)
            {
                game->animation.cells[y][x]--;
                
                /* When animation ends, clear the cell */
                if (game->animation.cells[y][x] == 0)
                {
                    game->board.cells[y][x] = 0;
                }
            }
        }
    }
}

/*
    Game_ApplyGravity
    =================
    Applies gravity to the board, shifting all blocks downward
    to fill empty spaces below them.
    
    Algorithm:
    - For each column, collect all filled cells
    - Rebuild the column with blocks at the bottom
    - Leaves gaps at the top
*/
void Game_ApplyGravity(GameState* game)
{
    /* Process each column */
    for (int x = 0; x < GRID_SIZE; x++)
    {
        int blocks[GRID_SIZE];    /* Temporary array to store filled blocks */
        int blockCount = 0;       /* Count of filled blocks in column */
        
        /* Collect all filled cells in this column */
        for (int y = 0; y < GRID_SIZE; y++)
        {
            if (game->board.cells[y][x] != 0)
            {
                blocks[blockCount++] = 1;
            }
        }
        
        /* Clear the column */
        for (int y = 0; y < GRID_SIZE; y++)
        {
            game->board.cells[y][x] = 0;
        }
        
        /* Rebuild column with blocks at the bottom */
        int startY = GRID_SIZE - blockCount;
        for (int i = 0; i < blockCount; i++)
        {
            game->board.cells[startY + i][x] = blocks[i];
        }
    }
}
