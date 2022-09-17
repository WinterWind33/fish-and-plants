# Copyright (c) 2022 Andrea Ballestrazzi

include(FetchContent)

if(USE_SPDLOG)
    message("[INFO] => Use spdlog as logger framework.")

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

    target_include_directories(rpi_gc PRIVATE "spdlog/include")
    target_link_libraries(rpi_gc PRIVATE spdlog)

    # Add USE_SPDLOG macro to the compilations.
    target_compile_definitions(rpi_gc PUBLIC USE_SPDLOG)
endif()
