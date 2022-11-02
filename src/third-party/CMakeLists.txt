# Copyright (c) 2022 Andrea Ballestrazzi

include(FetchContent)

if(USE_SPDLOG)
    message("[INFO] => Using spdlog v1.10.0 as logger framework.")

    FetchContent_Declare(spdlog
        URL "https://github.com/gabime/spdlog/archive/refs/tags/v1.10.0.zip"
        SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/third-party/spdlog-1.10.0"
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
