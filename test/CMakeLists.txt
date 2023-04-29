# Copyright (C) 2023 Andrea Ballestrazzi

# We use FetchContent to clone the submodules.
include(FetchContent)

# === Test App target configuration ===
set(TEST_APP_HEADER_FILES
    "testing-core.hpp"
    "catch2-gmock-listener.hpp"

    # Test doubles
    "gh_hal/test-doubles/hal-context.mock.hpp"
    "gh_hal/test-doubles/hal-digital-output.mock.hpp"
    "gh_hal/test-doubles/hardware-access/board-chip.mock.hpp"
    "gh_hal/test-doubles/hardware-access/board-chip-factory.mock.hpp"
    "gh_hal/test-doubles/hardware-access/board-digital-pin.mock.hpp"
    "gh_cmd/test-doubles/option-visitor.mock.hpp"
    "gh_cmd/test-doubles/option-parser.mock.hpp"
    "gh_cmd/test-doubles/command-option.mock.hpp"
    "gh_log/test-doubles/logger.mock.hpp"
    "rpi_gc/test-doubles/commands/terminal-command.mock.hpp"
    "rpi_gc/test-doubles/commands/bivalent-command.mock.hpp"
    "rpi_gc/test-doubles/abort-system/terminable-system.mock.hpp"
    "rpi_gc/test-doubles/abort-system/emergency-stoppable-system.mock.hpp"
    "rpi_gc/test-doubles/automatic-watering/hardware-controllers/watering-system-hardware-controller.mock.hpp"
    "rpi_gc/test-doubles/automatic-watering/time-providers/watering-system-time-provider.mock.hpp"
)

set(TEST_APP_SOURCE_FILES
    "test-app-entry-point.cpp"

    # Modules
    "gh_hal/hal-board-pin.tests.cpp"
    "gh_hal/hardware-access/board-chip.tests.cpp"
    "gh_cmd/switch.tests.cpp"
    "gh_cmd/value.tests.cpp"
    "gh_cmd/default-option-parser.tests.cpp"

    # rpi_gc
    "rpi_gc/greenhouse-controller-application.tests.cpp"
    "rpi_gc/commands/application-command.tests.cpp"
    "rpi_gc/commands/automatic-watering-command.tests.cpp"
    "rpi_gc/commands/abort-command.tests.cpp"
    "rpi_gc/automatic-watering/daily-cycle-automatic-watering-system.tests.cpp"
    "rpi_gc/automatic-watering/hardware-controllers/daily-cycle-aws-hardware-controller.tests.cpp"
    "rpi_gc/hardware-management/hardware-initializer.tests.cpp"
    "rpi_gc/functional/aws-hardware-controller-interactions.tests.cpp"

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
    set(CATCH2_VERSION "3.3.1")

    message("[INFO] => Using amalgamated Catch2 v${CATCH2_VERSION} as testing framework.")

    set(CATCH2_DIR "${PROJECT_SOURCE_DIR}/test/third-party/Catch2-${CATCH2_VERSION}")
    set(AMALGAMATED_HPP_FILE "${CATCH2_DIR}/include/catch2/catch_amalgamated.hpp")
    set(AMALGAMATED_CPP_FILE "${CATCH2_DIR}/include/catch2/catch_amalgamated.cpp")

    message("[INFO] => Downloading Catch2 amalgamated HPP file.")
    file(DOWNLOAD "https://github.com/catchorg/Catch2/releases/download/v${CATCH2_VERSION}/catch_amalgamated.hpp"
        ${AMALGAMATED_HPP_FILE}
    )

    message("[INFO] => Downloading Catch2 amalgamated CPP file.")
    file(DOWNLOAD "https://github.com/catchorg/Catch2/releases/download/v${CATCH2_VERSION}/catch_amalgamated.cpp"
        ${AMALGAMATED_CPP_FILE}
    )

    set(CATCH2_COMPILATION_FILES
        ${AMALGAMATED_HPP_FILE}
        ${AMALGAMATED_CPP_FILE}
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
    target_include_directories(test_app PRIVATE "${CATCH2_DIR}/include")
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
