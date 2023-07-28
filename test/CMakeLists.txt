# Copyright (C) 2023 Andrea Ballestrazzi

# We use FetchContent to clone the submodules.
include(FetchContent)

# === Test App target configuration ===
set(TEST_APP_HEADER_FILES
    "testing-core.hpp"
    "catch2-gmock-listener.hpp"

    # Test doubles
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
    "rpi_gc/test-doubles/diagnostics/diagnostic-status-probeable.mock.hpp"
    "rpi_gc/test-doubles/gc-project/project-component.mock.hpp"

    "modules/folder-provider/folder-provider.mock.hpp"
)

set(TEST_APP_SOURCE_FILES
    "test-app-entry-point.cpp"

    # Modules
    "modules/project-management/version-integrity-checker.tests.cpp"
    "modules/project-management/project-io/json-project-reader.tests.cpp"
    "modules/project-management/project-io/json-project-writer.tests.cpp"
    "modules/project-management/project-io/project-reader.tests.cpp"
    "modules/project-management/title-integrity-checker.tests.cpp"
    "modules/project-management/project.tests.cpp"
    "gh_hal/hardware-access/board-chip.tests.cpp"
    "gh_cmd/switch.tests.cpp"
    "gh_cmd/value.tests.cpp"
    "gh_cmd/default-option-parser.tests.cpp"

    # rpi_gc
    "rpi_gc/greenhouse-controller-application.tests.cpp"
    "rpi_gc/commands/application-command.tests.cpp"
    "rpi_gc/commands/automatic-watering-command.tests.cpp"
    "rpi_gc/commands/abort-command.tests.cpp"
    "rpi_gc/commands/status-command.tests.cpp"
    "rpi_gc/commands/version-command.tests.cpp"
    "rpi_gc/commands/project-command.tests.cpp"
    "rpi_gc/automatic-watering/daily-cycle-automatic-watering-system.tests.cpp"
    "rpi_gc/automatic-watering/hardware-controllers/daily-cycle-aws-hardware-controller.tests.cpp"
    "rpi_gc/hardware-management/hardware-initializer.tests.cpp"
    "rpi_gc/functional/aws-hardware-controller-interactions.tests.cpp"
    "rpi_gc/gc-project/project-controller.tests.cpp"

    # integration tests
    "integration/rpi_gc/application-command-integration.tests.cpp"
    "integration/automatic-watering-system-project-saving.tests.cpp"
    "integration/automatic-watering-system-project-loading.tests.cpp"
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

target_include_directories(test_app PRIVATE "${PROJECT_SOURCE_DIR}/test")
target_include_directories(test_app PRIVATE "${PROJECT_SOURCE_DIR}/src/rpi_gc")
target_include_directories(test_app PRIVATE "${PROJECT_SOURCE_DIR}/src/wrappers")
target_include_directories(test_app PRIVATE "${PROJECT_SOURCE_DIR}/src/modules/project-management")

target_link_libraries(test_app PRIVATE rpi_gc_lib nlohmann_json::nlohmann_json)

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
    message(STATUS "Using Catch2 v${CATCH2_VERSION} as testing framework.")

    set(CATCH2_DIR "${PROJECT_SOURCE_DIR}/test/third-party/Catch2-${CATCH2_VERSION}")
    set(AMALGAMATED_HPP_FILE "${CATCH2_DIR}/include/catch2/catch_amalgamated.hpp")
    set(AMALGAMATED_CPP_FILE "${CATCH2_DIR}/include/catch2/catch_amalgamated.cpp")

    FetchContent_Declare(Catch2
        GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
        GIT_TAG v3.4.0
    )

    FetchContent_MakeAvailable(Catch2)

    target_link_libraries(test_app PRIVATE Catch2)
    target_compile_definitions(test_app PRIVATE "USE_CATCH2_AS_TESTING_FRAMEWORK")
endif()

if(USE_GMOCK)
    message(STATUS "Using GMock as test doubles framework.")

    FetchContent_Declare(GoogleTest
        GIT_REPOSITORY "https://github.com/google/googletest.git"
        GIT_TAG "v1.13.0"
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # We only want GMock.
    set(BUILD_GTEST OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(GoogleTest)

    target_compile_definitions(test_app PRIVATE USE_GMOCK)

    # We ony want the google mock subdirectory.
    target_link_libraries(test_app PRIVATE GTest::gmock)
endif()
