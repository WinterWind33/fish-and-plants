# Copyright (C) 2022 Andrea Ballestrazzi

# We use FetchContent to clone the submodules.
include(FetchContent)

# === Test App target configuration ===
set(TEST_APP_HEADER_FILES
    "testing-core.hpp"
    "catch2-gmock-listener.hpp"

    # Test doubles
    "gh_hal/test-doubles/hal-context.mock.hpp"
    "gh_cmd/test-doubles/option-visitor.mock.hpp"
    "gh_cmd/test-doubles/option-parser.mock.hpp"
    "gh_cmd/test-doubles/command-option.mock.hpp"
    "rpi_gc/test-doubles/commands/terminal-command.mock.hpp"
    "rpi_gc/test-doubles/commands/bivalent-command.mock.hpp"
)

set(TEST_APP_SOURCE_FILES
    "test-app-entry-point.cpp"

    # Modules
    "gh_hal/hal-board-pin.tests.cpp"
    "gh_cmd/switch.tests.cpp"
    "gh_cmd/default-option-parser.tests.cpp"

    # rpi_gc
    "rpi_gc/greenhouse-controller-application.tests.cpp"
    "rpi_gc/commands/application-command.tests.cpp"

    # integration tests
    "integration/rpi_gc/application-command-integration.tests.cpp"
)

if(USE_CATCH2_AS_TESTING_FRAMEWORK AND USE_GMOCK)
    # If we can use both catch2 and googletest we need to add
    # the Catch2 listener on GMock in order to fail the test whenever
    # a GMock assertion fails.
    list(APPEND TEST_APP_SOURCE_FILES
        "catch2-gmock-listener.cpp"
    )

    # If we use catch2 as the main test kernel with GMock, we have
    # conflicts on some MACROS. Defining the following two macros
    # prevents this clash.
    add_compile_definitions(GTEST_DONT_DEFINE_FAIL)
    add_compile_definitions(GTEST_DONT_DEFINE_SUCCEED)
endif()

add_executable(test_app ${TEST_APP_HEADER_FILES} ${TEST_APP_SOURCE_FILES})

set_target_properties(test_app
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
)

target_include_directories(test_app PUBLIC "${PROJECT_SOURCE_DIR}/test")
target_include_directories(test_app PUBLIC "${PROJECT_SOURCE_DIR}/src/rpi_gc")
target_include_directories(test_app PUBLIC "${PROJECT_SOURCE_DIR}/src/wrappers")

target_link_libraries(test_app PRIVATE rpi_gc_lib)
target_link_libraries(test_app PRIVATE gh_log)
target_link_libraries(test_app PRIVATE gh_hal)
target_link_libraries(test_app PRIVATE gh_cmd)

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
    message("[INFO] => Using amalgamated Catch2 v3.1.1 as testing framework.")

    set(CATCH2_3_1_1_DIR "third-party/Catch2-3.1.1")

    set(CATCH2_COMPILATION_FILES
        "${CATCH2_3_1_1_DIR}/include/catch2/catch_amalgamated.cpp"
        "${CATCH2_3_1_1_DIR}/include/catch2/catch_amalgamated.hpp"
    )

    add_library(Catch2 STATIC ${CATCH2_COMPILATION_FILES})

    # We want our custom main so we need to define this MACRO so
    # the compilation won't generate its custom main.
    target_compile_definitions(Catch2 PUBLIC "CATCH_AMALGAMATED_CUSTOM_MAIN")
    set_target_properties(Catch2
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )

    target_link_libraries(test_app PRIVATE Catch2)
    target_include_directories(test_app PRIVATE "${CATCH2_3_1_1_DIR}/include")
    target_compile_definitions(test_app PRIVATE "USE_CATCH2_AS_TESTING_FRAMEWORK")
endif()

if(USE_GMOCK)
    message("[INFO] => Using GMock as test doubles framework.")

    FetchContent_Declare(
        googletest
        URL "https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip"
        SOURCE_DIR "${PROJECT_SOURCE_DIR}/test/third-party/GoogleTest-1.2.1"
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # We only want GMock.
    set(BUILD_GTEST OFF CACHE BOOL "" FORCE)

    set_target_properties(gtest
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )

    set_target_properties(gmock
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )

    set_target_properties(gtest_main
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )

    set_target_properties(gmock_main
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )

    target_compile_definitions(test_app PUBLIC USE_GMOCK)

    # We ony want the google mock subdirectory.
	target_include_directories(test_app PUBLIC "third-party/GoogleTest-1.2.1/googlemock/include")
	target_link_libraries(test_app PRIVATE GTest::gmock)
endif()
