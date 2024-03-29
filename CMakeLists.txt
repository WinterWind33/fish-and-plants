# Copyright (C) 2023 Andrea Ballestrazzi
cmake_minimum_required(VERSION 3.19)

if(COMPILE_AARCH64_ON_LINUX)
    # Note: if aarch64 compilation is triggered then a valid aarch64 toolchain
    # should be passed. See the "toolchains" folder for options.
    message(STATUS "Compiling for aarch64 on Linux.")

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

if(NOT MSVC)
    message(STATUS "Compiling with -g option")

    # With clang or gcc we need this option in order to debug the
    # project.
    add_compile_options(-g)
endif()

# === External Dependencies ===
find_package(Microsoft.GSL CONFIG REQUIRED)

option(RPI_GC_BUILD_TESTS "Build the tests project." ON)

if(RPI_GC_BUILD_TESTS)
    option(USE_CATCH2_AS_TESTING_FRAMEWORK "Use Catch2 as testing framework." ON)
    option(USE_GMOCK "Use gmock submodule as the main mock framework." ON)

    find_package(Catch2 CONFIG REQUIRED)
    find_package(GTest CONFIG REQUIRED)
endif()

# = Output folders =
set(PRODUCTION_EXE_COMPILATION_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/bin")
set(PRODUCTION_LIB_COMPILATION_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/lib")

# === Options ===
set(SPDLOG_VERSION "1.12.0")
set(LIBGPIOD_VERSION "2.0.1")

option(USE_SPDLOG "Use SPDLOG ${SPDLOG_VERSION} submodule as logging framework." ON)
option(USE_LIBGPIOD "Use libgpiod ${LIBGPIOD_VERSION} module as the main I/O ports communication module." ON)
option(USE_POPL20 "Use popl20 header-only library as command line parser." ON)
option(USE_SEMVER "Use Neargye/semver third-library software as semantic versioning library." ON)
option(GC_USE_NLOHMANN_JSON "Use nlohmann/json as json engine" ON)
option(GC_USE_SAGO_PLATFORM_FOLDERS "Use sago/platform_folders as platform folders library" ON)

# === Project dependencies ===
include(FetchContent)

if(USE_SEMVER)
    message(STATUS "Using semver.")

    FetchContent_Declare(semver
        GIT_REPOSITORY "https://github.com/Neargye/semver.git"
        GIT_TAG origin/master
    )

    FetchContent_MakeAvailable(semver)
endif()

if(GC_USE_NLOHMANN_JSON)
    message(STATUS "Using NLohmann/json as json engine")

    FetchContent_Declare(nlohmann_json
        GIT_REPOSITORY "https://github.com/nlohmann/json.git"
        GIT_TAG v3.11.2
    )

    FetchContent_MakeAvailable(nlohmann_json)
endif()

if(USE_SPDLOG)
    message(STATUS "Using spdlog v${SPDLOG_VERSION} as logger framework.")

    FetchContent_Declare(spdlog
        GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
        GIT_TAG "v${SPDLOG_VERSION}"
    )

    FetchContent_MakeAvailable(spdlog)

    set_target_properties(spdlog
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )
endif()

if(GC_USE_SAGO_PLATFORM_FOLDERS)
    message(STATUS "Using sago/PlatformFolders third-party library")

    FetchContent_Declare(platform_folders
        GIT_REPOSITORY "https://github.com/sago007/PlatformFolders.git"
        GIT_TAG "4.2.0"
    )

    FetchContent_MakeAvailable(platform_folders)

    set_target_properties(platform_folders
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )
endif()

# === SUB-PROJECTS ===

# Add third party libraries.
add_subdirectory("src/third-party")

# Add wrapper libraries.
add_subdirectory("src/wrappers/gh_cmd")
add_subdirectory("src/wrappers/gh_hal")
add_subdirectory("src/wrappers/gh_log")

# Applications Modules
add_subdirectory("src/modules/project-management")
add_subdirectory("src/modules/folder-provider")
add_subdirectory("src/modules/workflows")

# Add the greenhouse controller application to the build system.
add_subdirectory("src/rpi_gc")

# Tests project
add_subdirectory("test")
