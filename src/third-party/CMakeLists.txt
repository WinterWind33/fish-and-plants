# Copyright (c) 2022 Andrea Ballestrazzi

include(FetchContent)

if(USE_SPDLOG)
    message("[INFO] => Using spdlog as logger framework.")

    FetchContent_Declare(spdlog
        GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
        GIT_TAG "v1.x"
        SOURCE_DIR "spdlog"
    )

    FetchContent_MakeAvailable(spdlog)

    # Compilation configurations.
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
