# Copyright (c) 2023 Andrea Ballestrazzi

include(FetchContent)

if(USE_SPDLOG)
    set(SPDLOG_VERSION "1.11.0")

    message("[INFO] => Using spdlog v${SPDLOG_VERSION} as logger framework.")

    FetchContent_Declare(spdlog
        URL "https://github.com/gabime/spdlog/archive/refs/tags/v${SPDLOG_VERSION}.zip"
        SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/third-party/spdlog-${SPDLOG_VERSION}"
    )

    FetchContent_MakeAvailable(spdlog)

    set_target_properties(spdlog
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )
endif()

if(USE_CPPGPIO AND UNIX)
    message("[INFO] => Using CppGPIO as the main I/O communication module.")

    FetchContent_Declare(cppgpio
        GIT_REPOSITORY "https://github.com/JoachimSchurig/CppGPIO.git"
        GIT_TAG "master"
        SOURCE_DIR "CppGPIO"
    )

    FetchContent_MakeAvailable(cppgpio)

    # Compilation configurations.
    set_target_properties(cppgpio
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )
endif()

if(USE_POPL20)
    message("[INFO] => Using popl20 header only library as command-line interpreter.")

    add_subdirectory("popl20")
endif()

if(USE_LIBGPIOD AND UNIX)
    message("[INFO] => Using libgpiod as as the main I/O communication module.")

    set(LIBGPIOD_NAME "libgpiod-2.0")

    set(LIBGPIOD_FILENAME "${LIBGPIOD_NAME}.tar.gz")
    set(LIBGPIOD_FOLDER_PATH "${PROJECT_SOURCE_DIR}/src/third-party")
    set(LIBGPIOD_FILEPATH "${LIBGPIOD_FOLDER_PATH}/${LIBGPIOD_FILENAME}")

    # We need to download the libgpiod v2.0 tar.gz file and extract it inside the
    # third-party folder
    file(DOWNLOAD
        "https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/snapshot/${LIBGPIOD_FILENAME}"
        ${LIBGPIOD_FILEPATH})

    # Now we need to extract the file.
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${LIBGPIOD_FILENAME}
        WORKING_DIRECTORY ${LIBGPIOD_FOLDER_PATH}
    )

    # Cleanup: we don't need the tar.gz file anymore
    file(REMOVE ${LIBGPIOD_FILEPATH})
endif()
