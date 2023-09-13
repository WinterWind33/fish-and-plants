# Copyright (c) 2023 Andrea Ballestrazzi

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

set(GH_LOG_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src/wrappers")
target_include_directories(gh_log PRIVATE ${GH_LOG_INCLUDE_DIR})

if(USE_SPDLOG)
    target_link_libraries(gh_log PUBLIC spdlog::spdlog_header_only)
    target_compile_definitions(gh_log PUBLIC USE_SPDLOG)
endif()
