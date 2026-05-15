/*
    PC/SDL rendering (desktop test build only).
*/

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "../../core/game.h"
#include "../platform.h"
#include "pc_internal.h"

static bool CellIsClearing(const GameState* game, int x, int y)
{
    return (game->clearingRows & (uint8_t)(1u << y)) != 0 ||
           (game->clearingCols & (uint8_t)(1u << x)) != 0;
}

static void DrawPieceUI(
    SDL_Renderer* renderer,
    Piece* piece,
    int startX,
    int startY,
    int alpha,
    float scale,
    bool drawOutline
)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int y = 0; y < piece->height; y++)
    {
        for (int x = 0; x < piece->width; x++)
        {
            if (!piece->cells[y][x])
            {
                continue;
            }

            SDL_Rect block =
            {
                startX + (int)(x * TILE_PIXEL_SIZE * scale),
                startY + (int)(y * TILE_PIXEL_SIZE * scale),
                (int)(TILE_PIXEL_SIZE * scale),
                (int)(TILE_PIXEL_SIZE * scale)
            };

            SDL_SetRenderDrawColor(renderer, 0, 200, 255, alpha);
            SDL_RenderFillRect(renderer, &block);

            if (drawOutline)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                SDL_RenderDrawRect(renderer, &block);
            }
        }
    }
}

static float GetPieceScale(Piece* piece)
{
    int maxDim = piece->width > piece->height ? piece->width : piece->height;

    if (maxDim <= 2) return 0.75f;
    if (maxDim == 3) return 0.65f;
    if (maxDim == 4) return 0.55f;
    return 0.5f;
}

void Render_Draw(GameState* game)
{
    SDL_Renderer* renderer = PC_GetRenderer();

    if (!renderer)
    {
        return;
    }

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    SDL_Rect topScreen = { 0, 0, DS_WIDTH, DS_SCREEN_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &topScreen);

    int centerX = DS_WIDTH / 2;
    int baseY = 32;
    int slotWidth = TILE_PIXEL_SIZE * 3;

    for (int i = 0; i < 3; i++)
    {
        if (game->pieceUsed[i])
        {
            continue;
        }

        Piece* p = &game->activePieces[i];
        float scale = GetPieceScale(p);
        int pieceWidth = (int)(p->width * TILE_PIXEL_SIZE * scale);
        int pieceHeight = (int)(p->height * TILE_PIXEL_SIZE * scale);
        int slotCenterX = centerX + (i - 1) * slotWidth;
        int startX = slotCenterX - pieceWidth / 2;
        int startY = baseY;

        if (i == game->selectedPiece)
        {
            SDL_Rect highlight =
            {
                startX - 8,
                startY - 8,
                pieceWidth + 16,
                pieceHeight + 16
            };

            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderDrawRect(renderer, &highlight);
        }

        DrawPieceUI(renderer, p, startX, startY, 255, scale, false);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    SDL_Rect hinge = { 0, DS_SCREEN_HEIGHT, DS_WIDTH, 4 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &hinge);

    SDL_Rect bottomScreen =
    {
        0,
        BOARD_OFFSET_Y,
        DS_WIDTH,
        DS_SCREEN_HEIGHT
    };

    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderFillRect(renderer, &bottomScreen);

    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            SDL_Rect cell =
            {
                BOARD_OFFSET_X + x * TILE_PIXEL_SIZE,
                BOARD_OFFSET_Y + y * TILE_PIXEL_SIZE,
                TILE_PIXEL_SIZE,
                TILE_PIXEL_SIZE
            };

            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            if (game->board.cells[y][x] == 0)
            {
                continue;
            }

            int px = BOARD_OFFSET_X + x * TILE_PIXEL_SIZE;
            int py = BOARD_OFFSET_Y + y * TILE_PIXEL_SIZE;
            int size = TILE_PIXEL_SIZE;
            int inset = 0;
            uint8_t r = 100;
            uint8_t g = 150;
            uint8_t b = 255;
            uint8_t alpha = 255;

            if (game->phase == GAME_PHASE_CLEARING && CellIsClearing(game, x, y))
            {
                int frame = game->clearAnimFrame;

                if (frame < 6)
                {
                    r = 255;
                    g = 255;
                    b = 200;
                }
                else
                {
                    inset = (frame - 6) * 2;
                    if (inset > size / 2)
                    {
                        inset = size / 2;
                    }
                    alpha = (uint8_t)(255 - ((frame - 6) * 255 / 5));
                }
            }

            SDL_Rect block =
            {
                px + inset,
                py + inset,
                size - inset * 2,
                size - inset * 2
            };

            if (block.w > 0 && block.h > 0)
            {
                SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
                SDL_RenderFillRect(renderer, &block);

                SDL_SetRenderDrawColor(renderer, 60, 100, 200, alpha);
                SDL_RenderDrawRect(renderer, &block);
            }
        }
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    if (!game->pieceUsed[game->selectedPiece])
    {
        Piece* selected = &game->activePieces[game->selectedPiece];

        DrawPieceUI(
            renderer,
            selected,
            BOARD_OFFSET_X + game->cursorX * TILE_PIXEL_SIZE,
            BOARD_OFFSET_Y + game->cursorY * TILE_PIXEL_SIZE,
            120,
            1.0f,
            true
        );
    }

    SDL_Rect cursor =
    {
        BOARD_OFFSET_X + game->cursorX * TILE_PIXEL_SIZE,
        BOARD_OFFSET_Y + game->cursorY * TILE_PIXEL_SIZE,
        TILE_PIXEL_SIZE,
        TILE_PIXEL_SIZE
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &cursor);

    SDL_RenderPresent(renderer);
}
