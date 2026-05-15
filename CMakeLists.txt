cmake_minimum_required(VERSION 3.16)

# Platform selection: PC (default) or DS
set(PLATFORM "PC" CACHE STRING "Target platform: PC or DS")
set_property(CACHE PLATFORM PROPERTY STRINGS "PC" "DS")

# Set up toolchain for cross-compilation BEFORE project declaration
if(PLATFORM STREQUAL "DS")
    if(NOT DEFINED DEVKITPRO)
        set(DEVKITPRO "/opt/devkitpro" CACHE PATH "devkitPRO installation path")
    endif()
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/devkitarm-toolchain.cmake" CACHE PATH "Toolchain file")
endif()

project(BLOCK-BLAST-DS C)

set(CMAKE_C_STANDARD 11)

# Platform selection: PC (default) or DS
set(PLATFORM "PC" CACHE STRING "Target platform: PC or DS")
set_property(CACHE PLATFORM PROPERTY STRINGS "PC" "DS")

# Core game source files (platform-independent)
set(GAME_SOURCES
    src/core/main.c
    src/core/game.c
    src/core/piece.c
    src/input/input.c
)

# Platform-specific implementation
if(PLATFORM STREQUAL "PC")
    message(STATUS "Building for PC with SDL2")
    find_package(SDL2 REQUIRED)
    
    list(APPEND GAME_SOURCES 
        src/rendering/render.c
        src/platform/platform_sdl2.c
    )
    
    add_executable(BLOCK-BLAST-DS ${GAME_SOURCES})
    
    target_include_directories(BLOCK-BLAST-DS PRIVATE 
        src/core 
        src/rendering 
        src/input 
        src/platform
    )
    
    target_link_libraries(BLOCK-BLAST-DS PRIVATE SDL2::SDL2)
    
elseif(PLATFORM STREQUAL "DS")
    message(STATUS "Building for Nintendo DS with devkitARM")
    
    if(NOT DEFINED DEVKITPRO)
        set(DEVKITPRO "/opt/devkitpro")
    endif()
    
    set(LIBNDS "${DEVKITPRO}/libnds")
    
    list(APPEND GAME_SOURCES 
        src/rendering/render_ds.c
        src/platform/platform_ds.c
    )
    
    add_executable(BLOCK-BLAST-DS.elf ${GAME_SOURCES})
    
    target_include_directories(BLOCK-BLAST-DS.elf PRIVATE 
        src/core 
        src/rendering 
        src/input 
        src/platform
        ${LIBNDS}/include
    )
    
    target_link_directories(BLOCK-BLAST-DS.elf PRIVATE ${LIBNDS}/lib)
    target_link_libraries(BLOCK-BLAST-DS.elf PRIVATE nds9 c m)
    
    # Set ARM7 CPU flags
    target_compile_options(BLOCK-BLAST-DS.elf PRIVATE -mcpu=arm7tdmi -mtune=arm7tdmi -mthumb -mthumb-interwork)
    target_link_options(BLOCK-BLAST-DS.elf PRIVATE -mcpu=arm7tdmi -mtune=arm7tdmi -mthumb -mthumb-interwork -Wl,--gc-sections)
    
    # Add custom command to generate .nds file
    add_custom_command(TARGET BLOCK-BLAST-DS.elf POST_BUILD
        COMMAND ${DEVKITPRO}/tools/bin/ndstool -c ${CMAKE_BINARY_DIR}/BLOCK-BLAST-DS.nds -9 BLOCK-BLAST-DS.elf
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating NDS file..."
    )
    
else()
    message(FATAL_ERROR "Invalid PLATFORM: ${PLATFORM}. Must be 'PC' or 'DS'")
endif()
