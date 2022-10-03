# Copyright (c) 2022 Andrea Ballestrazzi

include(FetchContent)

if(USE_SPDLOG)
    message("[INFO] => Using spdlog as logger framework.")

    FetchContent_Declare(spdlog
        GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
        GIT_TAG "v1.x"
    )

    FetchContent_MakeAvailable(spdlog)

    # Compilation configurations.
    set_target_properties(spdlog
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )

    set(SPDLOG_INCLUDE_DIR "spdlog/include")

    target_include_directories(gh_log PUBLIC ${SPDLOG_INCLUDE_DIR})
    target_link_libraries(gh_log PRIVATE spdlog)

    set(SPDLOG_USE_MACRO_DEF "USE_SPDLOG")

    # Add USE_SPDLOG macro to the compilations.
    target_compile_definitions(gh_log PUBLIC ${SPDLOG_USE_MACRO_DEF})
endif()

if(USE_CPPGPIO AND UNIX)
    message("[INFO] => Using CppGPIO as the main I/O communication module.")

    FetchContent_Declare(cppgpio
        GIT_REPOSITORY "https://github.com/JoachimSchurig/CppGPIO.git"
        GIT_TAG "master"
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
