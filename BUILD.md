# BLOCK BLAST DS - Build Instructions

This project uses a platform abstraction layer to compile for both:
- **PC** (Linux/Windows/macOS) with SDL2 for testing
- **Nintendo DS** hardware with devkitARM/libnds

## Architecture

The project is structured with platform-independent core game logic and platform-specific implementations:

```
src/
├── core/          # Platform-independent game logic (NO platform dependencies)
├── rendering/     # Platform-specific rendering
│   ├── render.h   # Platform-agnostic interface
│   ├── render.c   # SDL2 implementation (PC)
│   └── render_ds.c # libnds stub (DS)
├── input/         # Platform-independent input interface
│   ├── input.h    # Platform-agnostic functions
│   └── input.c    # Pure game input logic
└── platform/      # Platform-specific initialization
    ├── platform.h        # Platform abstraction interface
    ├── platform_sdl2.c   # SDL2 implementation (PC)
    └── platform_ds.c     # libnds implementation (DS)
```

## Building for PC (SDL2)

### Requirements
- CMake 3.16+
- GCC or Clang
- SDL2 development libraries

### Build Steps
```bash
cd /path/to/BLOCK-BLAST-DS
mkdir build
cd build
cmake -DPLATFORM=PC ..
cmake --build .
```

### Running
```bash
./BLOCK-BLAST-DS
```

**Controls:**
- Arrow Keys: Move cursor
- Q/E: Previous/Next piece
- SPACE: Place piece
- Window close: Quit

## Building for Nintendo DS

### Requirements
- devkitARM (devkitPRO)
- libnds
- cmake (with devkitARM toolchain support)

### Setup (if not already installed)
```bash
# Install devkitPRO (follow official instructions at https://devkitpro.org/)
# On Linux, it's typically:
sudo apt install pacman-package-manager
# Then use pacman to install devkitARM and libnds
```

### Build Steps
```bash
cd /path/to/BLOCK-BLAST-DS
mkdir build_ds
cd build_ds
cmake -DPLATFORM=DS ..
cmake --build .
```

### Output
- `build_ds/BLOCK-BLAST-DS.elf` - ARM9 binary
- `build_ds/BLOCK-BLAST-DS.nds` - Final NDS ROM (when build is complete)

### Playing on DS
1. Copy the `.nds` file to your DS flashcart
2. Launch from flashcart as normal

## Platform Abstraction Details

### Core Game Logic (Platform-Independent)
- `src/core/main.c` - Calls platform abstraction functions
- `src/core/game.c` - Game rules and state
- `src/core/piece.c` - Piece definitions
- `src/input/input.c` - Platform-agnostic input handlers

### Platform Interface (`src/platform/platform.h`)
```c
int Platform_Init(void);              // Initialize rendering/input
void Platform_Shutdown(void);         // Cleanup
void Platform_Render(GameState*);     // Render one frame
void Platform_UpdateInput(GameState*); // Process input
void Platform_FrameTiming(void);      // Handle frame timing
int Platform_ShouldQuit(void);        // Check for exit signal
```

### Rendering Interface (`src/rendering/render.h`)
- Uses opaque `Renderer` typedef (void*)
- On PC: Receives `SDL_Renderer*` cast to void*
- On DS: Receives NULL (renders directly to hardware)

## Adding New Features

When adding new features:

1. **If platform-independent logic**: Add to `src/core/`
2. **If rendering**: 
   - Add interface to `src/rendering/render.h`
   - Implement in `src/rendering/render.c` (SDL2) and `src/rendering/render_ds.c` (DS)
3. **If input/initialization**: 
   - Add interface to `src/platform/platform.h`
   - Implement in `src/platform/platform_sdl2.c` and `src/platform/platform_ds.c`

## Next Steps for DS Implementation

The current DS build structure is prepared but needs:

1. **Full libnds rendering** - Implement actual graphics drawing in `render_ds.c`
2. **ARM7 binary** - DS is dual-core; need to handle ARM7 code
3. **Proper linker scripts** - devkitARM needs correct link setup
4. **Button input mapping** - Currently mapped but not rendering feedback

These can be implemented incrementally as graphics and DS-specific features are added.

## Troubleshooting

### SDL2 not found (PC build)
```bash
# Ubuntu/Debian
sudo apt-get install libsdl2-dev

# macOS
brew install sdl2

# Or manually install from https://www.libsdl.org/
```

### devkitARM toolchain issues (DS build)
```bash
# Ensure environment variables are set
export DEVKITPRO=/opt/devkitpro
export DEVKITARM=$DEVKITPRO/devkitARM
export PATH=$DEVKITARM/bin:$PATH
```

### Clean rebuild
```bash
rm -rf build* && mkdir build && cd build && cmake -DPLATFORM=PC .. && cmake --build .
```
