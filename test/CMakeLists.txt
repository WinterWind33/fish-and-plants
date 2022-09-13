# Copyright (C) 2022 Andrea Ballestrazzi

# We use FetchContent to clone the submodules.
include(FetchContent)

# === Test App target configuration ===
set(TEST_APP_HEADER_FILES)
set(TEST_APP_SOURCE_FILES
    "test-app-entry-point.cpp"
)

add_executable(test_app ${TEST_APP_HEADER_FILES} ${TEST_APP_SOURCE_FILES})

set_target_properties(test_app
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
)

if(MSVC)
    # Under Windows there is also the Release folder that
    # is created during the compilation. We don't want that
    # so we can have an uniform folder configuration accross
    # platforms.
    set_target_properties(test_app
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )
endif()


# === Third party submodules configuration ===
if(USE_CATCH2_AS_TESTING_FRAMEWORK)
    message("[INFO] => Using Catch2 as testing framework.")

    FetchContent_Declare(Catch2
        GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
        GIT_TAG "devel"
    )

    FetchContent_MakeAvailable(Catch2)

    set(CATCH_DEVELOPMENT_BUILD ON)

    set_target_properties(Catch2
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )

    set_target_properties(Catch2WithMain
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )

    # We add the include directory to the test target so its files
    # can include Catch2 headers.
    target_include_directories(test_app PUBLIC "third-party/Catch2/src")

    # For now we don't need our implementation of the entry point so we
    # can use the Catch2 one.
    target_link_libraries(test_app PRIVATE Catch2::Catch2WithMain)
endif()
