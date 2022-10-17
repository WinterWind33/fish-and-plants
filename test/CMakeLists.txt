# Copyright (C) 2022 Andrea Ballestrazzi

# We use FetchContent to clone the submodules.
include(FetchContent)

# === Test App target configuration ===
set(TEST_APP_HEADER_FILES
    "catch2-gmock-listener.hpp"

    # Test doubles
    "gh_hal/test-doubles/hal-context.mock.hpp"
)

set(TEST_APP_SOURCE_FILES
    "test-app-entry-point.cpp"
    "catch2-gmock-listener.cpp"
    "gh_hal/hal-board-pin.tests.cpp"
)

add_executable(test_app ${TEST_APP_HEADER_FILES} ${TEST_APP_SOURCE_FILES})

set_target_properties(test_app
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
)

target_include_directories(test_app PUBLIC ".")
target_include_directories(test_app PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_include_directories(test_app PUBLIC "${PROJECT_SOURCE_DIR}/wrappers")

target_link_libraries(test_app PRIVATE gh_log)
target_link_libraries(test_app PUBLIC gh_hal)

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

if(USE_CATCH2_AS_TESTING_FRAMEWORK AND USE_GMOCK)
    # If we can use both catch2 and googletest we need to add
    # the Catch2 listener on GMock in order to fail the test whenever
    # a GMock assertion falis.
    #list(APPEND CRYPASS_TESTS_CPP_FILES
    #    "framework/catch2-listener-on-gmock.cpp"
    #)

    # If we use catch2 as the main test kernel with GMock, we have
    # conflicts on some MACROS. Defining the following two macros
    # prevents this clash.
    add_compile_definitions(GTEST_DONT_DEFINE_FAIL)
    add_compile_definitions(GTEST_DONT_DEFINE_SUCCEED)
endif()

if(USE_CATCH2_AS_TESTING_FRAMEWORK)
    message("[INFO] => Using Catch2 as testing framework.")

    FetchContent_Declare(Catch2
        GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
        GIT_TAG "devel"
        SOURCE_DIR "third-party/Catch2"
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
    target_link_libraries(test_app PRIVATE Catch2::Catch2)
    target_compile_definitions(test_app PUBLIC USE_CATCH2_AS_TESTING_FRAMEWORK)
endif()

if(USE_GMOCK)
    message("[INFO] => Using GMock as test doubles framework.")

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY "https://github.com/google/googletest.git"
        GIT_TAG "main" # This is the stable working branch.
        SOURCE_DIR "third-party/googletest"
    )

    FetchContent_MakeAvailable(googletest)

    # For Windows: Prevent overriding the parent project's compiler/linker settings
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
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
	target_include_directories(test_app PUBLIC "third-party/googletest/googlemock/include")
	target_link_libraries(test_app PRIVATE GTest::gmock)

endif()
