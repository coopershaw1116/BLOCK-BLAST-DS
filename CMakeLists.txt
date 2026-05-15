# Desktop/SDL test build only — use `make` at repo root for .nds (devkitPro).
cmake_minimum_required(VERSION 3.16)

project(BLOCK-BLAST-DS-PC C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)

add_executable(BLOCK-BLAST-DS-PC
    src/core/game.c
    src/core/piece.c
    src/platform/pc/main_pc.c
    src/platform/pc/input_pc.c
    src/platform/pc/render_pc.c
)

target_include_directories(BLOCK-BLAST-DS-PC PRIVATE
    src/core
    src/platform
)

target_link_libraries(BLOCK-BLAST-DS-PC PRIVATE SDL2::SDL2)
