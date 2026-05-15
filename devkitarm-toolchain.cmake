#!/usr/bin/env cmake -P
#
# DevkitARM toolchain file for cross-compilation to Nintendo DS
#

if(NOT DEVKITPRO)
    set(DEVKITPRO "/opt/devkitpro")
endif()

if(NOT DEVKITARM)
    set(DEVKITARM "${DEVKITPRO}/devkitARM")
endif()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the cross compiler
set(CMAKE_C_COMPILER "${DEVKITARM}/bin/arm-none-eabi-gcc" CACHE PATH "C compiler")
set(CMAKE_CXX_COMPILER "${DEVKITARM}/bin/arm-none-eabi-g++" CACHE PATH "C++ compiler")
set(CMAKE_ASM_COMPILER "${DEVKITARM}/bin/arm-none-eabi-as" CACHE PATH "ASM compiler")

# Compiler flags for DS ARM7
set(CMAKE_C_FLAGS_INIT "-mcpu=arm7tdmi -mtune=arm7tdmi -fomit-frame-pointer -ffast-math -mthumb -mthumb-interwork" CACHE STRING "C flags")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_C_FLAGS_INIT}" CACHE STRING "C++ flags")

# Prevent CMake from testing the compiler
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# Set include and lib paths
set(CMAKE_FIND_ROOT_PATH "${DEVKITPRO}/libnds" "${DEVKITPRO}/portlibs/arm-none-eabi")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
