# Copyright (C) 2023 Andrea Ballestrazzi
cmake_minimum_required(VERSION 3.19)

if(COMPILE_AARCH64_ON_LINUX)
    # Note: if aarch64 compilation is triggered then a valid aarch64 toolchain
    # should be passed. See the "toolchains" folder for options.
    message("[INFO] => Compiling for aarch64 on Linux.")

    # Target operating system name.
    set(CMAKE_SYSTEM_NAME Linux)
    set(CMAKE_SYSTEM_PROCESSOR aarch64)

    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
endif()

project(FishAndPlants CXX)
enable_language(CXX)

# === C++ and Compilation Configuration ===

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# If we are compiling with gcc or clang we need to pass the -fconcpets
# option in order to compile with concepts.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_EXPORT_COMPILE_COMMANDS)
    message("[INFO] => Compiling with: -fconcepts.")
    add_compile_options(-fconcepts)
endif()

if(NOT MSVC)
    message("[INFO] => Compiling with -g option")

    # With clang or gcc we need this option in order to debug the
    # project.
    add_compile_options(-g)
endif()

# = Output folders =
set(PRODUCTION_EXE_COMPILATION_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/bin")
set(PRODUCTION_LIB_COMPILATION_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/lib")

# === Options ===

option(USE_CATCH2_AS_TESTING_FRAMEWORK "Use Catch2 as testing framework." ON)
option(USE_SPDLOG "Use SPDLOG submodule as logging framework." ON)
option(USE_CPPGPIO "Use CppGPIO submodule as the main I/O ports communication module." ON)
option(USE_GMOCK "Use gmock submodule as the main mock framework." ON)
option(USE_POPL20 "Use popl20 header-only library as command line parser." ON)

# === SUB-PROJECTS ===

# Add third party libraries.
add_subdirectory("src/third-party")

# Add wrapper libraries.
add_subdirectory("src/wrappers")

# Add the greenhouse controller application to the build system.
add_subdirectory("src/rpi_gc")

# Tests project
add_subdirectory("test")

add_custom_target(
    clang-tidy-check clang-tidy -p ${CMAKE_BINARY_DIR}/compile_commands.json -checks=cppcoreguidelines-* ${RPI_GC_SOURCE_FILES}
    DEPENDS ${RPI_GC_SOURCE_FILES})

add_custom_target(check DEPENDS clang-tidy-check)
