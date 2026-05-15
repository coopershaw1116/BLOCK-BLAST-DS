/*
    =====================================
    PLATFORM IMPLEMENTATION - LIBNDS (DS)
    =====================================
    
    libnds-based rendering and input for real DS hardware.
*/

#ifdef PLATFORM_DS

#include <nds.h>
#include <stdio.h>
#include "platform.h"
#include "../input/input.h"

/*
    Global state
*/
static bool g_should_quit = false;

/*
    Platform_Init
    =============
    Initializes libnds video and input systems.
*/
int Platform_Init(void)
{
    /* Initialize NDS */
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);

    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);

    /* Initialize backgrounds */
    REG_BG0CNT = BG_MAP_BASE(31) | BG_TILE_BASE(0) | BG_PRIORITY(3);
    REG_BG0CNT_SUB = BG_MAP_BASE(31) | BG_TILE_BASE(0) | BG_PRIORITY(3);

    /* Initialize console for debug output */
    consoleInit(NULL, 3, BgType_Text4bpp, BgFormat_Asm, SCREEN_BASE_BLOCK_SUB(31), CHAR_BASE_BLOCK_SUB(0), false, true);

    g_should_quit = false;
    return 0;
}

/*
    Platform_Shutdown
    =================
    Cleans up libnds resources.
*/
void Platform_Shutdown(void)
{
    /* Nothing specific needed for DS */
}

/*
    Platform_Render
    ===============
    Renders game frame using libnds.
    
    TODO: Implement DS-specific rendering
    This is a placeholder that should be filled with actual libnds drawing code.
*/
void Platform_Render(GameState* game)
{
    /* TODO: Implement DS rendering */
    /* For now, just clear screens and sync */
    swiWaitForVBlank();
}

/*
    Platform_UpdateInput
    ====================
    Processes DS button input using libnds.
*/
void Platform_UpdateInput(GameState* game)
{
    /* Scan keys */
    scanKeys();
    u16 keys = keysDown();

    /* Check for quit (START button) */
    if (keys & KEY_START)
    {
        g_should_quit = true;
    }

    /* D-Pad for cursor movement */
    if (keys & KEY_UP)
        Input_HandleCursorMove(game, 0);
    if (keys & KEY_DOWN)
        Input_HandleCursorMove(game, 1);
    if (keys & KEY_LEFT)
        Input_HandleCursorMove(game, 2);
    if (keys & KEY_RIGHT)
        Input_HandleCursorMove(game, 3);

    /* Shoulder buttons for piece selection */
    if (keys & KEY_L)
        Input_HandlePieceSelect(game, 0);  /* Previous */
    if (keys & KEY_R)
        Input_HandlePieceSelect(game, 1);  /* Next */

    /* A button for piece placement */
    if (keys & KEY_A)
        Input_HandlePlacePiece(game);
}

/*
    Platform_FrameTiming
    ====================
    DS runs at ~60 FPS by default with VBlank sync.
*/
void Platform_FrameTiming(void)
{
    swiWaitForVBlank();
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

#endif /* PLATFORM_DS */
