/*
    NDS libnds rendering — MODE_5 16-bit bitmap on both screens.
*/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <nds.h>

#include "../../core/game.h"
#include "../platform.h"

#define SCREEN_W 256
#define SCREEN_H 192

static int s_bgMain;
static int s_bgSub;
static u16* s_mainBuf;
static u16* s_subBuf;

/* Bmp16 pixels are aBBBBBGGGGGRRRRR — bit 15 must be set or the pixel is invisible */
static u16 ColorFrom8(u8 r, u8 g, u8 b)
{
    return ARGB16(1, r >> 3, g >> 3, b >> 3);
}

static void FillRect(u16* buf, int x, int y, int w, int h, u16 color)
{
    if (x < 0)
    {
        w += x;
        x = 0;
    }

    if (y < 0)
    {
        h += y;
        y = 0;
    }

    if (x + w > SCREEN_W)
    {
        w = SCREEN_W - x;
    }

    if (y + h > SCREEN_H)
    {
        h = SCREEN_H - y;
    }

    if (w <= 0 || h <= 0)
    {
        return;
    }

    for (int row = y; row < y + h; row++)
    {
        for (int col = x; col < x + w; col++)
        {
            buf[row * SCREEN_W + col] = color;
        }
    }
}

static void DrawRectOutline(u16* buf, int x, int y, int w, int h, u16 color)
{
    FillRect(buf, x, y, w, 1, color);
    FillRect(buf, x, y + h - 1, w, 1, color);
    FillRect(buf, x, y, 1, h, color);
    FillRect(buf, x + w - 1, y, 1, h, color);
}

static u16 Blend15(u16 fg, u16 bg, int alpha)
{
    int fr = fg & 31;
    int fg_g = (fg >> 5) & 31;
    int fb = (fg >> 10) & 31;
    int br = bg & 31;
    int bg_g = (bg >> 5) & 31;
    int bb = (bg >> 10) & 31;
    int inv = 31 - (alpha >> 3);

    int r = (fr * alpha + br * inv) / 31;
    int g = (fg_g * alpha + bg_g * inv) / 31;
    int b = (fb * alpha + bb * inv) / 31;

    return ARGB16(1, (u8)r, (u8)g, (u8)b);
}

static bool CellIsClearing(const GameState* game, int x, int y)
{
    return (game->clearingRows & (uint8_t)(1u << y)) != 0 ||
           (game->clearingCols & (uint8_t)(1u << x)) != 0;
}

/* Scale tray pieces as integer num/den (no soft-float on ARM9) */
static void GetPieceScale(Piece* piece, int* num, int* den)
{
    int maxDim = piece->width > piece->height ? piece->width : piece->height;

    if (maxDim <= 2)
    {
        *num = 3;
        *den = 4;
    }
    else if (maxDim == 3)
    {
        *num = 13;
        *den = 20;
    }
    else if (maxDim == 4)
    {
        *num = 11;
        *den = 20;
    }
    else
    {
        *num = 1;
        *den = 2;
    }
}

static void DrawPieceOnBuffer(
    u16* buf,
    Piece* piece,
    int startX,
    int startY,
    int alpha,
    int scaleNum,
    int scaleDen,
    bool drawOutline,
    u16 fillColor,
    u16 outlineColor
)
{
    for (int y = 0; y < piece->height; y++)
    {
        for (int x = 0; x < piece->width; x++)
        {
            if (!piece->cells[y][x])
            {
                continue;
            }

            int bx = startX + (x * TILE_PIXEL_SIZE * scaleNum) / scaleDen;
            int by = startY + (y * TILE_PIXEL_SIZE * scaleNum) / scaleDen;
            int bw = (TILE_PIXEL_SIZE * scaleNum) / scaleDen;
            int bh = (TILE_PIXEL_SIZE * scaleNum) / scaleDen;

            if (alpha < 255)
            {
                for (int row = by; row < by + bh; row++)
                {
                    for (int col = bx; col < bx + bw; col++)
                    {
                        if (col >= 0 && col < SCREEN_W && row >= 0 && row < SCREEN_H)
                        {
                            u16* px = &buf[row * SCREEN_W + col];
                            *px = Blend15(fillColor, *px, alpha >> 3);
                        }
                    }
                }
            }
            else
            {
                FillRect(buf, bx, by, bw, bh, fillColor);
            }

            if (drawOutline)
            {
                DrawRectOutline(buf, bx, by, bw, bh, outlineColor);
            }
        }
    }

}

void Platform_Init(void)
{
    videoSetMode(MODE_5_2D);
    videoSetModeSub(MODE_5_2D);
    lcdMainOnTop();

    /* Map VRAM banks for main/sub 16-bit bitmap backgrounds (devkitPro double-buffer layout) */
    vramSetPrimaryBanks(
        VRAM_A_MAIN_BG_0x06000000,
        VRAM_B_MAIN_BG_0x06020000,
        VRAM_C_SUB_BG,
        VRAM_D_LCD
    );

    /* BG3 is the standard 256x256 16-bit bitmap layer in MODE_5 (see devkitPro examples) */
    s_bgMain = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    s_bgSub = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

    s_mainBuf = (u16*)bgGetGfxPtr(s_bgMain);
    s_subBuf = (u16*)bgGetGfxPtr(s_bgSub);
}

void Platform_Shutdown(void)
{
}

void Platform_FrameWait(void)
{
    swiWaitForVBlank();
}

void Render_Draw(GameState* game)
{
    u16 colBgTop = ColorFrom8(30, 30, 30);
    u16 colBgSub = ColorFrom8(25, 25, 25);
    u16 colGrid = ColorFrom8(60, 60, 60);
    u16 colBlock = ColorFrom8(100, 150, 255);
    u16 colBlockEdge = ColorFrom8(60, 100, 200);
    u16 colPiece = ColorFrom8(0, 200, 255);
    u16 colPieceEdge = ColorFrom8(31, 31, 31);
    u16 colHighlight = ColorFrom8(31, 31, 0);
    u16 colFlash = ColorFrom8(31, 31, 25);

    dmaFillHalfWords(colBgTop, s_mainBuf, SCREEN_W * SCREEN_H * sizeof(u16));
    dmaFillHalfWords(colBgSub, s_subBuf, SCREEN_W * SCREEN_H * sizeof(u16));

    int centerX = SCREEN_W / 2;
    int baseY = 32;
    int slotWidth = TILE_PIXEL_SIZE * 3;

    for (int i = 0; i < 3; i++)
    {
        if (game->pieceUsed[i])
        {
            continue;
        }

        Piece* p = &game->activePieces[i];
        int scaleNum;
        int scaleDen;
        GetPieceScale(p, &scaleNum, &scaleDen);
        int pieceWidth = (p->width * TILE_PIXEL_SIZE * scaleNum) / scaleDen;
        int pieceHeight = (p->height * TILE_PIXEL_SIZE * scaleNum) / scaleDen;
        int slotCenterX = centerX + (i - 1) * slotWidth;
        int startX = slotCenterX - pieceWidth / 2;
        int startY = baseY;

        if (i == game->selectedPiece)
        {
            DrawRectOutline(
                s_mainBuf,
                startX - 8,
                startY - 8,
                pieceWidth + 16,
                pieceHeight + 16,
                colHighlight
            );
        }

        DrawPieceOnBuffer(
            s_mainBuf,
            p,
            startX,
            startY,
            255,
            scaleNum,
            scaleDen,
            false,
            colPiece,
            colPieceEdge
        );
    }

    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            int px = BOARD_OFFSET_X + x * TILE_PIXEL_SIZE;
            int py = y * TILE_PIXEL_SIZE;

            DrawRectOutline(s_subBuf, px, py, TILE_PIXEL_SIZE, TILE_PIXEL_SIZE, colGrid);
        }
    }

    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            if (game->board.cells[y][x] == 0)
            {
                continue;
            }

            int px = BOARD_OFFSET_X + x * TILE_PIXEL_SIZE;
            int py = y * TILE_PIXEL_SIZE;
            int size = TILE_PIXEL_SIZE;
            int inset = 0;
            u16 fill = colBlock;
            int alpha = 31;

            if (game->phase == GAME_PHASE_CLEARING && CellIsClearing(game, x, y))
            {
                int frame = game->clearAnimFrame;

                if (frame < 6)
                {
                    fill = colFlash;
                }
                else
                {
                    inset = (frame - 6) * 2;
                    if (inset > size / 2)
                    {
                        inset = size / 2;
                    }
                    alpha = 31 - ((frame - 6) * 31 / 5);
                    if (alpha < 0)
                    {
                        alpha = 0;
                    }
                }
            }

            int bw = size - inset * 2;
            int bh = size - inset * 2;

            if (bw > 0 && bh > 0)
            {
                if (alpha >= 31)
                {
                    FillRect(s_subBuf, px + inset, py + inset, bw, bh, fill);
                    DrawRectOutline(s_subBuf, px + inset, py + inset, bw, bh, colBlockEdge);
                }
                else
                {
                    for (int row = py + inset; row < py + inset + bh; row++)
                    {
                        for (int col = px + inset; col < px + inset + bw; col++)
                        {
                            if (col >= 0 && col < SCREEN_W && row >= 0 && row < SCREEN_H)
                            {
                                u16* pixel = &s_subBuf[row * SCREEN_W + col];
                                *pixel = Blend15(fill, *pixel, alpha);
                            }
                        }
                    }
                }
            }
        }
    }

    if (!game->pieceUsed[game->selectedPiece])
    {
        Piece* selected = &game->activePieces[game->selectedPiece];

        DrawPieceOnBuffer(
            s_subBuf,
            selected,
            BOARD_OFFSET_X + game->cursorX * TILE_PIXEL_SIZE,
            game->cursorY * TILE_PIXEL_SIZE,
            120,
            1,
            1,
            true,
            colPiece,
            colPieceEdge
        );
    }

    DrawRectOutline(
        s_subBuf,
        BOARD_OFFSET_X + game->cursorX * TILE_PIXEL_SIZE,
        game->cursorY * TILE_PIXEL_SIZE,
        TILE_PIXEL_SIZE,
        TILE_PIXEL_SIZE,
        colHighlight
    );
}
