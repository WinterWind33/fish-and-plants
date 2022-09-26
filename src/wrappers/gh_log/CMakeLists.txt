# Copyright (c) 2022 Andrea Ballestrazzi

set(GH_LOG_SOURCE_FILES
    "spl-logger.cpp"
)
set(GH_LOG_HEADER_FILES
    "gh-log-lib-base.hpp"
    "logger.hpp"
    "spl-logger.hpp"
)

# We create a static library
add_library(gh_log STATIC ${GH_LOG_HEADER_FILES} ${GH_LOG_SOURCE_FILES})

# Compilation configurations.
set_target_properties(gh_log
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
)

set(GH_LOG_INCLUDE_DIR ".")

target_include_directories(gh_log PUBLIC ${GH_LOG_INCLUDE_DIR})
target_include_directories(rpi_gc PUBLIC ${GH_LOG_INCLUDE_DIR})
target_include_directories(test_app PUBLIC ${GH_LOG_INCLUDE_DIR})

# Now we link the static library to the production and test targets
target_link_libraries(rpi_gc PUBLIC gh_log)
target_link_libraries(test_app PUBLIC gh_log)
