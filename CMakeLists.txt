# Copyright (C) 2022 Andrea Ballestrazzi
cmake_minimum_required(VERSION 3.18)

project(FishAndPlants)

# === C++ Configuration ===

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# If we are compiling with gcc or clang we need to pass the -fconcpets
# option in order to compile with concepts.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("[INFO] => Compiling with: -fconcepts.")
    add_compile_options(-fconcepts)
endif()

if(NOT MSVC)
    message("[INFO] => Compiling with -g option")

    # With clang or gcc we need this option in order to debug the
    # project.
    add_compile_options(-g)
endif()